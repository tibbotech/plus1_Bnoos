#ifndef __SP7021_HAL_UART_H
#define __SP7021_HAL_UART_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7021_hal_def.h"
#include "sp7021_arm926.h"
#include "sp70xx.h" 	

/*
 * X = ((sclk + baud/2) / baud)
 * DIV_H = X  >> 12
 * DIV_L = (X & 0xf) << 12 | ((X >> 4) & 0xff - 1)
 */
#define UART_BAUD_DIV_H(baud, sclk)     ((((sclk) + ((baud) / 2)) / (baud)) >> 12)
#define UART_BAUD_DIV_L(baud, sclk)     ((((((sclk) + ((baud) / 2)) / (baud)) & 0xf) << 12) | \
                                         ((((((sclk) + ((baud) / 2)) / (baud)) >> 4) & 0xff) - 1))

#define UART_SRC_CLK		(27*1000*1000)

#define UART_INIT_USE_DEFAULT_VALUE		0x0
#define UART_INIT_USE_USER_VALUE		0x1

#define UATXDMA_BUF_SZ 	 			(4*1024)
#define UARXDMA_BUF_SZ 	 			UATXDMA_BUF_SZ
#define MAX_SZ_RXDMA_ISR			(1 << 9)

/************************************* uart lcr register get *****************************/
/*
0: Receive FIFO is empty.
1: Receive FIFO not empty.
*/
#define UART_LSR_RECEIVE_FIFO_EMPTY			(0x0 << 1)
#define UART_LSR_RECEIVE_FIFO_NOT_EMPTY		(0x1 << 1)

/*
0: Transmit FIFO is full.
1: Transmit FIFO is not full (and available for more input).
*/
#define UART_LSR_TRANSMIT_FIFO_FULL			(0x0)
#define UART_LSR_TRANSMIT_FIFO_NOT_FULL		(0x1)


/************************************* uart lcr register set *****************************/

/*
0x0: No major vote on uart1 RXD(default).
0x1: 3-sample vote.
0x2: 5-sample vote.
0x3: 7-sample vote.
*/
#define UART_LCR_VOTE_SAMPE_SELECT_NONE				(0x0)
#define UART_LCR_VOTE_SAMPE_SELECT_3				(0x1)
#define UART_LCR_VOTE_SAMPE_SELECT_5				(0x2)
#define UART_LCR_VOTE_SAMPE_SELECT_7				(0x3)

/*
0: No break condition(default).
1: Force serial output to spacing (low) state, break condition.
*/
#define UART_LCR_BREAK_CONDITION_DISABLE			(0x0)
#define UART_LCR_BREAK_CONDITION_ENABLE				(0x1)

/*
0: Odd parity(default).
1: Even parity.
*/
#define UART_LCR_PARITY_ODD							(0x0)
#define UART_LCR_PARITY_EVEN						(0x1)

/*
0: Disable parity bit(default).
1: Enable parity bit.
*/
#define UART_LCR_PARITY_DISABLE						(0x0)
#define UART_LCR_PARITY_ENABLE						(0x1)

/*
0: 1 stop bit(default).
1: For word length=5, 1.5 stop bits, for word length > 5, 2stop bits. 
*/
#define UART_LCR_STOP_BIT_1B						(0x0) 
#define UART_LCR_STOP_BIT_AUTO						(0x1) 

/*
0x0: 5bit.
0x1: 6bit.
0x2: 7bit.
0x3: 8bit(default).
*/
#define UART_LCR_WORD_LEN_5B						(0x0)
#define UART_LCR_WORD_LEN_6B						(0x1)
#define UART_LCR_WORD_LEN_7B						(0x2)
#define UART_LCR_WORD_LEN_8B						(0x3)


#define IS_UART_LCR_WORD_LEN(__WORD_LEN__)  				(((__WORD_LEN__) >= UART_LCR_WORD_LEN_5B) && ((__WORD_LEN__) <= UART_LCR_WORD_LEN_8B))
#define IS_UART_LCR_VOTE_SELECT(__VOTE_SELECT__)  			(((__VOTE_SELECT__) >= UART_LCR_VOTE_SAMPE_SELECT_NONE) && ((__VOTE_SELECT__) <= UART_LCR_VOTE_SAMPE_SELECT_7))


/************************************* uart mcr register set *****************************/

/*
0: No auto calibrate (or auto-calibration finished)(default).
1: Start baud-rate auto calibration. Calibration will stop after receive expected token (ASCII ?．a?． or ?．A?．).
*/
#define UART_MCR_AUTO_CALIBRATE_NO									(0x0)
#define UART_MCR_AUTO_CALIBRATE_START								(0x1)

/*
0: Disable CTS detection, TX FIFO sends out data always(default).
1: Enable CTS detection, only send data when CTS B is active (low).
*/
#define UART_MCR_AUTO_CTS_DETECTION_DISABLE							(0x0)
#define UART_MCR_AUTO_CTS_DETECTION_ENABLE							(0x1)

/*
0: UART RTS B is controlled by RTS register(default).
1: UART RTS B is controlled by receive FIFO status.
*/
#define UART_MCR_AUTO_RTS_CONTROL_BY_REGISTER						(0x0)
#define UART_MCR_AUTO_RTS_CONTROL_BY_FIFO							(0x1)

/*
0: Disable loop-back (normal function)(default).
1: Loop-back mode.
In loop-back mode:
RXD, DSR, CTS = TXD, DTR, RTS.
RI, DCD = RI OUT, DCD OUT.
External output TXD = mark (high).
External output DTR B,RTS B = inactive (high).
*/
#define UART_MCR_LOOPBACK_DISABLE									(0x0)
#define UART_MCR_LOOPBACK_ENABLE									(0x1)

/*
0: UART RI inactive (output low)(default).
1: UART RI active (output high).
*/
#define UART_MCR_RING_INDICATOR_INACTIVE							(0x0)
#define UART_MCR_RING_INDICATOR_ACTIVE								(0x1)

/*
Data carrier detect register
0: UART DCD B inactive (output high)(default).
1: UART DCD B active (output low).
*/
#define UART_MCR_DCD_INACTIVE										(0x0)
#define UART_MCR_DCD_ACTIVE											(0x1)

/*
Request to send register (not used in AUTORTS mode)
0: UART RTS B inactive (output high)(default).
1: UART RTS B active (output low).
*/
#define UART_MCR_RTS_INACTIVE										(0x0)
#define UART_MCR_RTS_ACTIVE											(0x1)

/*
Data terminal ready register
0: UART DTR B inactive (output high)(default).
1: UART DTR B active (output low).
*/
#define UART_MCR_DTS_INACTIVE										(0x0)
#define UART_MCR_DTS_ACTIVE											(0x1)


typedef struct
{
	uint32_t LCR_Init;   	/* use default value or not*/
	uint32_t breakcondition;
	uint32_t vote_select;
	uint32_t Parity;
	uint32_t Parity_en;
	uint32_t StopBits;
	uint32_t WordLength;	
}UART_LcrDef;

typedef struct
{
	uint32_t MCR_Init;   	/* use default value or not*/
	uint32_t auto_calibrate;
	uint32_t auto_cts;	
	uint32_t auto_rts;
	uint32_t loop_back;
	uint32_t RI_enable;
	uint32_t DCD_enable;
	uint32_t RTS_enable;
	uint32_t DTS_enable;
}UART_McrDef;

typedef struct
{
  uint32_t BaudRate;
  uint32_t uart_idx;
  uint32_t uart_irqno;
  UART_LcrDef LcrInit;
  UART_McrDef McrInit;
} UART_InitTypeDef;


#define  HAL_UART_STATE_RESET         0x00000000U    /*!< Peripheral is not initialized
                                                          Value is allowed for gState and RxState */
#define  HAL_UART_STATE_READY         0x00000020U    /*!< Peripheral Initialized and ready for use
                                                          Value is allowed for gState and RxState */
#define  HAL_UART_STATE_BUSY          0x00000024U    /*!< an internal process is ongoing
                                                          Value is allowed for gState only */
#define  HAL_UART_STATE_BUSY_TX       0x00000021U    /*!< Data Transmission process is ongoing
                                                          Value is allowed for gState only */
#define  HAL_UART_STATE_BUSY_RX       0x00000022U    /*!< Data Reception process is ongoing
                                                          Value is allowed for RxState only */
#define  HAL_UART_STATE_BUSY_TX_RX    0x00000023U    /*!< Data Transmission and Reception process is ongoing
                                                          Not to be used for neither gState nor RxState.
                                                          Value is result of combination (Or) between gState and RxState values */
#define  HAL_UART_STATE_TIMEOUT       0x000000A0U    /*!< Timeout state
                                                          Value is allowed for gState only */
#define  HAL_UART_STATE_ERROR         0x000000E0U    /*!< Error
                                                          Value is allowed for gState only */

#define IS_UART_ONE_OF_TWO_STATUS(__STATUS__)  				(((__STATUS__) == 0) || ((__STATUS__) == 1))


#define UART_DMA_ALIGN_SIZE		32

/* for tx dma, store data in circ buf for dma */
#define CIRC_SPACE_TO_END(head,tail,size) \
		({int end = (size) - 1 - (head); \
			int n = (end + (tail)) & ((size)-1); \
			n <= end ? n : end+1;})

#define uart_circ_empty(circ)           ((circ).head == (circ).tail) 


struct circ_buf {
        int8_t *buf;
        int head;
        int tail;
};


typedef struct __UART_HandleTypeDef
{
	UART_CTRL_Type			*Instance;                /*!< UART registers base address        */

	UART_InitTypeDef        Init;                     /*!< UART communication parameters      */

	UART_Txdma				*txdma;					  /* txdma register base address 		 */

	UART_TxGdma				*txgdma;				  /* txgdma register base address 		 */

	UART_Rxdma 				*rxdma;					  /* rxdma register base address 		 */

/* ######################## not used for user ####################### */

	HAL_LockTypeDef     	Lock;		              /*!< Locking object                    */

	uint8_t					uart_idx;				  /*  uart0:0x0  uart5:0x5				 */

	uint32_t				gState;					  /*  uart states(busy,tx,rx,ready)		 */

	uint32_t				RxState;					  /*  uart states(busy,tx,rx,ready)		 */

	int32_t					rx_size;

	int32_t					rx_index;

	int32_t					tx_size;

	int32_t					tx_index;

	uint8_t* 				pRxBuffPtr;					   /* rx buffer, */

	uint8_t* 				pTxBuffPtr;					   /* tx buffer, not set in dma mode. */

	uint8_t*				txdma_buf;					   /* tx dma buffer,set for dma config */

	uint8_t*				rxdma_buf;						/* rx dma buffer,set for dma config */

  	struct circ_buf 		xmit;							/* store txdata in dma mode  */

  	void (* TxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Tx Complete Callback             */

  	void (* RxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Rx Complete Callback             */
  	
  	void (* ErrorCallback)(struct __UART_HandleTypeDef *huart);
}UART_HandleTypeDef;


HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Config_DMA_Buf(uint32_t* dma_start_addr);
uint32_t HAL_UART_GetState(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif
#endif
