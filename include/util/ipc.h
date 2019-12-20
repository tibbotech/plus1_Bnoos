
#ifndef __IPC_H__
#define __IPC_H__

#define RPC_DATA_REGS		16
#define RPC_DATA_SIZE		(RPC_DATA_REGS * 4)
#define RPC_HEAD_SIZE		(sizeof(rpc_t) - RPC_DATA_SIZE)
#define MAILBOX_NUM			8

#define RET(r)	((r > 511) ? (r - 1024) : r)
#define REG_ALIGN(len)      (((len) + 3)&(~3))

#define CACHE_MASK			(32 - 1)
#define CACHE_ALIGNED(n)	(((u32)(n) & CACHE_MASK) == 0)
#define CACHE_ALIGN(n)		(((u32)(n) + CACHE_MASK) & (~CACHE_MASK))
#define IPC_SEQ_LEN         (4)

/* IPC Error Code */
#define IPC_SUCCESS			(0)
#define IPC_FAIL			(-1)
#define IPC_FAIL_NODEV		(-2)	// ipc device not exist
#define IPC_FAIL_NOMEM		(-3)	// out of memory
#define IPC_FAIL_INVALID	(-4)	// invalid arguments
#define IPC_FAIL_UNSUPPORT	(-5)	// rpc cmd not supported
#define IPC_FAIL_NOSERV		(-6)	// rpc server not running
#define IPC_FAIL_BUSY		(-7)	// rpc server busy
#define IPC_FAIL_TIMEOUT	(-8)	// rpc request timeout
#define IPC_FAIL_SERVNOTRDY (-9)    // rpc server  not ready
#define IPC_FAIL_DATANOTRDY (-10)   // rpc remote  data not ready
#define IPC_FAIL_HWTIMEOUT  (-11)   // rpc hw timeout


typedef struct {
	void*		REQ_H;					// Request Handler
	u16 		DATA_LEN;				// In/Out Data Length in Bytes
	u16			CMD 	: 10;			// Command ID / Return Value
	u16 		RSV 	: 3;
	u16 		F_TYPE	: 2;			// Request Type
	u16 		F_DIR	: 1;			// REQUEST / RESPONSE
	void*		SEQ_ADDR;
	u32 		SEQ;
	union {
		u32		DATA[RPC_DATA_REGS];	// if DATA_LEN <= RPC_DATA_SIZE
		struct {						// if DATA_LEN >  RPC_DATA_SIZE
			void*	DATA_PTR;			// Cache Aligned
			void*	DATA_PTR_ORG;		// Backup (INTERNAL_USE)
		};
	};
} rpc_t;

/* RPC Request Type */
enum {
	REQ_WAIT_REP = 0,		// Wait Response
	REQ_DEFER_REP,			// Deferred Response
	REQ_NO_REP,				// No Response
};

/* RPC Direction */
enum {
	RPC_REQUEST = 0,
	RPC_RESPONSE,
};

typedef struct {
	u32			TRIGGER;				// Reg00
	u32			F_RW;					// Reg01
	u32			F_OVERWRITE;			// Reg02
	u32			RSV;					// Reg03

	rpc_t		RPC;					// Reg04~23

	u32			MBOX[MAILBOX_NUM];		// Reg24~31
} ipc_t;

typedef struct {
	volatile int g_respond;
	rpc_t rpc;
	int seq;
}st_ipc;

void ipc_init();
void ipc_start();
void ipc_test(int argc, char *argv[]);

#endif // __TIMER_H__

