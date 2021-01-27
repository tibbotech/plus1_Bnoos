#include "common_all.h"
#include "cache.h"
#include "sp_interrupt.h"
#include "ipc.h"

#define REG_GROUP_A2B		(0x9c008100) // G258
#define REG_GROUP_B2A		(0x9c008180) // G259

#define IPC_LOCAL			((ipc_t *)REG_GROUP_B2A)
#define IPC_REMOTE			((ipc_t *)REG_GROUP_A2B)


#define MAILBOX_INT  (199)

#define IPC_WRITEENABLE_MASK 	(0xFFFFF0)
#define IPC_WRITE_TIMEOUT  (500)
#define RPC_PTR_DATA_ADDR	(0x70000)
#define RPC_PTR_DATA_MAXSIZE (64*1024)

static st_ipc ipc_data;

static inline void rpc_add_seq(rpc_t *rpc)
{
	u16 len = rpc->DATA_LEN;
	u32 *addr = (u32 *)((u8 *)rpc->DATA_PTR + REG_ALIGN(len));
	rpc->SEQ_ADDR = (void *)addr;
	ipc_data.seq += 1;
	*addr = ipc_data.seq;
	rpc->SEQ = ipc_data.seq;
}

static int rpc_check_seq(rpc_t *rpc)
{
	u32 seq = rpc->SEQ;
	u32 *addr = rpc->SEQ_ADDR;
	u32 t1 = AV1_GetStc32();
	unsigned long timeout = 10; // 10ms
	printf("seq:0x%x, seq_data:%0x\n", seq, *addr);
	if(rpc->DATA_LEN < RPC_DATA_SIZE)
	{
		while(*addr != seq) {
			if((STC_get_timer(t1))>timeout) {
				printf("seq:0x%x, seq_data:%0x\n", seq, *addr);
				return IPC_FAIL_DATANOTRDY;
			}
			printf("seq:0x%x, seq_data:%0x\n", seq, *addr);
			HAL_DCACHE_INVALIDATE(rpc->DATA_PTR,CACHE_ALIGN(rpc->DATA_LEN + IPC_SEQ_LEN));
			STC_delay_us(1);
		}
	}
	return IPC_SUCCESS;
}

static void ipc_memcpy(void *dst, void *src, u32 len)
{
	u32 *d = (u32 *)dst;
	u32 *s = (u32 *)src;
	while(len) {
		*d++ = *s++;
		len -= 4;
	}
}

static void ipc_copy(rpc_t *dst, rpc_t *src)
{
	int len;
	memcpy(dst, src, RPC_HEAD_SIZE);
	len = src->DATA_LEN;
	if(len > RPC_PTR_DATA_MAXSIZE)
	{
		printf(" data size is too large \n");
		return;
	}
	if (src == &IPC_REMOTE->RPC)
	{
		if (len > RPC_DATA_SIZE) {
			dst->DATA_PTR = src->DATA_PTR;
			HAL_DCACHE_INVALIDATE(dst->DATA_PTR,CACHE_ALIGN(len + IPC_SEQ_LEN));
		} else {
			ipc_memcpy(dst->DATA, src->DATA, REG_ALIGN(len));
		}
	} 
	else if (dst == &IPC_LOCAL->RPC) 
	{
		if (len > RPC_DATA_SIZE) {
			dst->DATA_PTR = src->DATA_PTR;
			rpc_add_seq(src);
			HAL_DCACHE_FLUSH(dst->DATA_PTR,CACHE_ALIGN(len + IPC_SEQ_LEN));
		} else {
			ipc_memcpy(dst->DATA, src->DATA, REG_ALIGN(len));
		}
	}
}

static int ipc_write_enable(u32 mask)
{
	int ret = IPC_SUCCESS;
	int _i = IPC_WRITE_TIMEOUT*10;
	while (IPC_LOCAL->F_RW & (mask)) { 
		STC_delay_us(100);
		if (!_i--) {
			printf("write IPC HW timeout! writeEnableReg = 0x%x \n",IPC_LOCAL->F_RW);
			ret = IPC_FAIL_HWTIMEOUT;
			break;
		}
	}
	return ret;
}

void printf_rpc(rpc_t *rpc)
{
	int len,i=0;
	len = rpc->DATA_LEN;
	if (len > RPC_DATA_SIZE) {
		printf("\n  data start from DATA_PRT = 0x%x\n",rpc->DATA_PTR);
		while(i<len)
		{
			if(i%16 == 0)
				printf("\n");
			printf("0x%x ",((char *)(rpc->DATA_PTR))[i++]);
		}
		printf("\n0x%x ",((char *)(rpc->DATA_PTR))[i++]);
	} else {
		printf("\n  data start from DATA[4]\n");
		while(i<len)
		{
			if(i%16 == 0)
				printf("\n");
			printf("0x%x ",rpc->DATA[i++]);
		}
	}	
}
static int ipc_handle_data(void)
{
	int len,ret;
	int cmd = ipc_data.rpc.CMD;  // handle by CMD ID;
	len=ipc_data.rpc.DATA_LEN;
	if (len > RPC_DATA_SIZE) {
		printf_rpc(&ipc_data.rpc);
		printf("\n  data len =%d, used DATA_PTR !!!\n",len);
	} else {
		printf_rpc(&ipc_data.rpc);
		printf("\n  data len =%d, used DATA !!!\n",len);
	}	
	
	return IPC_SUCCESS;
}

int ipc_send_to_Achip(int cmd,char *data,int len)
{
	int ret = IPC_SUCCESS;

	if(!data)
	{
		printf("@data NULL!!\n");
		return -1;
	}	
	memset(&ipc_data.rpc,0,sizeof(rpc_t));
	ipc_data.rpc.F_DIR	 = RPC_REQUEST;
	ipc_data.rpc.F_TYPE	 = REQ_WAIT_REP;
	ipc_data.rpc.CMD 	 = cmd;
	ipc_data.rpc.DATA_LEN = len;

	if(len > RPC_PTR_DATA_MAXSIZE){
		printf(" data size is large than 0x%x \n",RPC_PTR_DATA_MAXSIZE);
		goto out;
	}
	ret = ipc_write_enable(IPC_WRITEENABLE_MASK);
	if (ret){
		goto out;
	}
	
	// fill data 
	if (len > RPC_DATA_SIZE) {
		ipc_data.rpc.DATA_PTR = (void *)RPC_PTR_DATA_ADDR;
		memset(ipc_data.rpc.DATA_PTR,0,RPC_PTR_DATA_MAXSIZE);
		ipc_memcpy(ipc_data.rpc.DATA_PTR, data, REG_ALIGN(len));
	} else {
		ipc_memcpy(ipc_data.rpc.DATA, data, REG_ALIGN(len));
	}
	// copy to mailbox
	ipc_copy(&IPC_LOCAL->RPC,&ipc_data.rpc);

	//trigger
	IPC_LOCAL->TRIGGER = 1;
	
	//wait respone
	ipc_data.g_respond = 0;
	u32 t1 = AV1_GetStc32();
	while((STC_get_timer(t1))<IPC_WRITE_TIMEOUT && !ipc_data.g_respond);
	ret = (ipc_data.g_respond)?RET(ipc_data.rpc.CMD):IPC_FAIL_TIMEOUT;
	
	if(ipc_data.rpc.CMD == IPC_SUCCESS && rpc_check_seq(&ipc_data.rpc))
	{
		printf("\n	check data error \n");
		return IPC_FAIL_DATANOTRDY;
	}

	printf(" send to Achip,ret = %d \n",ret);
out:
	return ret;
}


void ipc_response_Achip(int cmd)
{
	//send source data to A chip
	int ret = IPC_SUCCESS;
	rpc_t *src = &IPC_REMOTE->RPC;
	
	src->F_DIR	 = RPC_RESPONSE;
	src->F_TYPE	 = REQ_DEFER_REP;
	src->CMD 	 = cmd;

	ret = ipc_write_enable(IPC_WRITEENABLE_MASK);
	if (ret)
	{
		printf(" timeout F_RW = 0x%x \n",IPC_LOCAL->F_RW);
		return;
	}
	// fill data
	ipc_copy(&IPC_LOCAL->RPC,src);
	
	//  trigger int
	IPC_LOCAL->TRIGGER = 1;
	printf(" B responed to A \n");
}


void ipc_callback(void)
{
	rpc_t *src = &IPC_REMOTE->RPC;

	memset(&ipc_data.rpc,0,sizeof(rpc_t));
	ipc_copy(&ipc_data.rpc,&IPC_REMOTE->RPC);
	
	if(src->F_DIR == RPC_REQUEST){
		ipc_response_Achip(IPC_SUCCESS); //Bchip response to Achip
		ipc_handle_data();
	}
	else{ //Achip respone to Bchip
		ipc_data.g_respond=1;
	}
}
void ipc_test(int argc, char *argv[])
{
	char data[200];
	static int gg=0;
	int i=0,len;
	mon_readhex(argv[0], &len);
	if(len > 200)
	{
		printf(" test len need to small than 200 \n");
		return;
	}
	while(i<len)
	{
		data[i] = 0x22+i+gg;
		i++;
	}
	gg++;
	ipc_send_to_Achip(0,data,len);
}

void ipc_init()
{
	interrupt_register(MAILBOX_INT, "MAILBOX", ipc_callback, 0);
}

void ipc_start()
{
	memset(&ipc_data,0,sizeof(st_ipc));
	printf("ipc test start!\n");
}
