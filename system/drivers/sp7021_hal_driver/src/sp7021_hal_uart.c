
#include "sp7021_hal_uart.h"

typedef struct
{
	uint8_t* tx_buffer; // for dma transmit 
	uint8_t* tx_data_buffer; // temp buffer,for store the trransmit data;
	uint8_t* rx_buffer;  // for receive dma data;
	uint8_t  tx_used;
	uint8_t  rx_used;
}DMA_Buffer_t;

DMA_Buffer_t g_dma_buffer[2];


__weak void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

}
__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
}

__weak void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
}
static void UART_InitCallbacksToDefault(UART_HandleTypeDef *huart)
{
  huart->TxCpltCallback            = HAL_UART_TxCpltCallback;
  huart->RxCpltCallback            = HAL_UART_RxCpltCallback; 
  huart->ErrorCallback 			   = HAL_UART_ErrorCallback;
} 

static HAL_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, uint32_t Status,
                                              uint32_t Tickstart, uint32_t Timeout)
{
	while(READ_BIT(huart->Instance->lsr, Flag) != Status)
	{
	    /* Check for the Timeout */
	    if (Timeout != HAL_MAX_DELAY)
	    {
	      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
	      {
	        huart->gState = HAL_UART_STATE_READY;
	        __HAL_UNLOCK(huart);
	        return HAL_TIMEOUT;
	      }
	    }
	}
	return HAL_OK;
}   

static void __uart_lcr_config(UART_HandleTypeDef *huart)
{
	assert_param(huart);

	if(IS_UART_LCR_VOTE_SELECT(huart->Init.LcrInit.vote_select))
	{
		MODIFY_REG(huart->Instance->lcr, LCR_VOTE_BIT, huart->Init.LcrInit.vote_select);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.LcrInit.breakcondition))
	{
		MODIFY_REG(huart->Instance->lcr, LCR_BREAK_CONDITION_BIT, huart->Init.LcrInit.breakcondition);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.LcrInit.Parity))
	{
		MODIFY_REG(huart->Instance->lcr, LCR_PARITY_BIT, huart->Init.LcrInit.Parity);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.LcrInit.Parity_en))
	{
		MODIFY_REG(huart->Instance->lcr, LCR_PARITY_ENABLE_BIT, huart->Init.LcrInit.Parity_en);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.LcrInit.StopBits))
	{
		MODIFY_REG(huart->Instance->lcr, LCR_STOP_BIT_BIT, huart->Init.LcrInit.StopBits);
	}
	if(IS_UART_LCR_WORD_LEN(huart->Init.LcrInit.WordLength))
	{
		MODIFY_REG(huart->Instance->lcr, LCR_WORD_LEN_BIT, huart->Init.LcrInit.WordLength);
	}
}

static void __uart_mcr_config(UART_HandleTypeDef *huart)
{

	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.auto_calibrate))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_AUTO_CALIBRATE_BIT, huart->Init.McrInit.auto_calibrate);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.auto_cts))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_AUTO_CTS_BIT, huart->Init.McrInit.auto_cts);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.auto_rts))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_AUTO_RTS_BIT, huart->Init.McrInit.auto_rts);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.loop_back))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_LOOP_BACK_BIT, huart->Init.McrInit.loop_back);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.RI_enable))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_RING_INDICATOR_BIT, huart->Init.McrInit.RI_enable);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.DCD_enable))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_DCD_BIT, huart->Init.McrInit.DCD_enable);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.RTS_enable) && (huart->Init.McrInit.auto_rts == UART_MCR_AUTO_RTS_CONTROL_BY_REGISTER))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_RTS_BIT, huart->Init.McrInit.RTS_enable);
	}
	if(IS_UART_ONE_OF_TWO_STATUS(huart->Init.McrInit.DTS_enable))
	{
		MODIFY_REG(huart->Instance->mcr, MCR_DTS_BIT, huart->Init.McrInit.DTS_enable);
	}
}	

static inline uint32_t __get_txdma_buf_space(UART_HandleTypeDef *huart)
{
	UART_Txdma *txdma_reg = huart->txdma;
	uint32_t addr_sw, addr_hw;
	UART_TxGdma *txgdma_reg = huart->txgdma;
	if (READ_REG((txdma_reg->txdma_status)) & 0x01) {
		return 0;
	}
	addr_sw = READ_REG((txdma_reg->txdma_wr_adr));
	addr_hw = READ_REG((txdma_reg->txdma_rd_adr));

	if (addr_sw >= addr_hw)
	{
		return (UATXDMA_BUF_SZ - (addr_sw - addr_hw));
	} 
	else
	{
		return (addr_hw - addr_sw);
	}
}


static inline void __stop_tx_irq(UART_HandleTypeDef *huart)
{
	unsigned int isc;
	isc = READ_REG(huart->Instance->isc);
	isc &= ~SP_UART_ISC_TXM;
	WRITE_REG(huart->Instance->isc, isc);
}

static inline void __start_tx_irq(UART_HandleTypeDef *huart)
{
	unsigned int isc;
	isc = READ_REG(huart->Instance->isc) | SP_UART_ISC_TXM;
	WRITE_REG(huart->Instance->isc, isc);
}

static inline void __stop_rx_irq(UART_HandleTypeDef *huart)
{
	unsigned int isc;
	isc = READ_REG(huart->Instance->isc);
	isc &= ~SP_UART_ISC_RXM;
	WRITE_REG(huart->Instance->isc, isc);
}

static inline void __start_rx_irq(UART_HandleTypeDef *huart)
{
	unsigned int isc = 0;
	isc = READ_REG(huart->Instance->isc) | SP_UART_ISC_RXM;
	WRITE_REG(huart->Instance->isc, isc);
}

static void __uart_tx_irq_handler(UART_HandleTypeDef *huart)
{
	uint32_t tx_buf_available;
	uint32_t addr_sw, addr_start;
	uint32_t offset_sw;
	uint8_t *byte_ptr;

	UART_Txdma *txdma_reg;
	UART_TxGdma *txgdma_reg;
	
	if(huart->gState != HAL_UART_STATE_BUSY_TX)
	{
		return;
	}
	
	/*  transmit data by uart register */
	if(huart->pTxBuffPtr)
	{	
		while(huart->pTxBuffPtr[huart->tx_index] && (huart->tx_size > huart->tx_index))
		{
			WRITE_REG(huart->Instance->dr,huart->pTxBuffPtr[huart->tx_index]);
			huart->tx_index++;
			if(huart->tx_size = huart->tx_index)
			{
				huart->tx_index = 0;
				__stop_tx_irq(huart);
				huart->gState = HAL_UART_STATE_READY;
				huart->TxCpltCallback(huart);
				break;
			}
		}
		return;
	}

	/*  transmit data by dma register */
	assert_param(huart->txdma);
	assert_param(huart->txgdma);
	
	txdma_reg = huart->txdma;
	txgdma_reg = huart->txgdma;

	if (uart_circ_empty(huart->xmit)) 
	{
		__stop_tx_irq(huart);
		huart->gState = HAL_UART_STATE_READY;
		huart->TxCpltCallback(huart);
		return;
	}

	addr_sw = READ_REG((txdma_reg->txdma_wr_adr));
	addr_start = READ_REG((txdma_reg->txdma_start_addr));
	offset_sw = addr_sw - addr_start;
	byte_ptr = (uint8_t *)(huart->txdma_buf + offset_sw);
	tx_buf_available = __get_txdma_buf_space(huart); 

	while(tx_buf_available)
	{
		*byte_ptr = ((uint8_t *)huart->xmit.buf)[huart->xmit.tail];

		huart->xmit.tail = (huart->xmit.tail + 1) & (UATXDMA_BUF_SZ - 1);
		byte_ptr++;
		addr_sw++;
		offset_sw++;
		tx_buf_available--;
		if (offset_sw == UATXDMA_BUF_SZ) 
		{
			offset_sw = 0;
			addr_sw = (uint32_t)(huart->txdma_buf);
			byte_ptr = (uint8_t *)(huart->txdma_buf);
		}
		if (uart_circ_empty(huart->xmit)) 
		{
			break;
		}
	}

	WRITE_REG(txdma_reg->txdma_wr_adr,	addr_sw);
	if (uart_circ_empty(huart->xmit))
	{
		huart->gState = HAL_UART_STATE_READY;
		__stop_tx_irq(huart);
		huart->TxCpltCallback(huart);
	}
}

static void __uart_rx_irq_handler(UART_HandleTypeDef *huart)
{
	assert_param(huart->pRxBuffPtr);
	
	if(huart->RxState != HAL_UART_STATE_BUSY_RX)
		return;
	
	/*  receive data by uart register */
	if(huart->pRxBuffPtr)
	{
		while (huart->Instance->lsr & SP_UART_LSR_RX) 
		{
			if((huart->Instance->lsr & SP_UART_LSR_FE) || (huart->Instance->lsr & SP_UART_LSR_OE) || \
				(huart->Instance->lsr & SP_UART_LSR_PE))
			{
				huart->RxState = HAL_UART_STATE_READY;
				huart->ErrorCallback(huart);
				continue;
			}
			/* get data */	
			if((huart->Instance->lsr & UART_LSR_RECEIVE_FIFO_STATUS) == UART_LSR_RECEIVE_FIFO_NOT_EMPTY){
				char ch = READ_REG(huart->Instance->dr);
				huart->pRxBuffPtr[huart->rx_index++] = ch;
				if(huart->rx_size == huart->rx_index)
				{
					huart->rx_index = 0;
					huart->RxState = HAL_UART_STATE_READY;
					huart->RxCpltCallback(huart);
					return; 
				}
			}			
		} 
		
		huart->rx_index = 0;
		huart->RxState = HAL_UART_STATE_READY;
		huart->RxCpltCallback(huart);
		return;	
	}
}


static void __uart_txdma_config(UART_HandleTypeDef *huart)
{
	assert_param(huart);
	assert_param(huart->txdma);
	assert_param(huart->txgdma);
	assert_param(huart->Instance);

	UART_Txdma *txdma_reg = huart->txdma;
	UART_TxGdma *txgdma_reg = huart->txgdma;
	if(huart->txdma_buf == NULL)
	{
		if(g_dma_buffer[0].tx_used == 0)
		{
			huart->txdma_buf = (int8_t *)g_dma_buffer[0].tx_buffer;
			huart->xmit.buf = (int8_t *)g_dma_buffer[0].tx_data_buffer;
			g_dma_buffer[0].tx_used = 1;
		}
		else if(g_dma_buffer[1].tx_used == 0)
		{
			huart->txdma_buf = (int8_t *)g_dma_buffer[1].tx_buffer;
			huart->xmit.buf = (int8_t *)g_dma_buffer[1].tx_data_buffer;
			g_dma_buffer[1].tx_used = 1;
		}
		else
		{
			return;
		}

		huart->xmit.head = huart->xmit.tail = 0;
		WRITE_REG(txdma_reg->txdma_tmr_unit,	202500000 / 1000);  	/* 1 msec */
		/* must be set before txdma_start_addr */
		WRITE_REG(txdma_reg->txdma_wr_adr,		huart->txdma_buf);
		/* txdma_reg->txdma_rd_adr is updated by h/w too */
		WRITE_REG(txdma_reg->txdma_start_addr,	huart->txdma_buf); 	
		/* txdma_reg set ring-buffer of end_addr  */
		WRITE_REG(txdma_reg->txdma_end_addr,	huart->txdma_buf + UATXDMA_BUF_SZ - 1); 
		/* bind uartx to dma */
		WRITE_REG(txdma_reg->txdma_sel,			huart->uart_idx); 

		WRITE_REG(txgdma_reg->gdma_int_en,		0x41); 
		/* Use ring buffer for UART's Tx */
		WRITE_REG(txdma_reg->txdma_enable,		0x5); 
	}
}

static void __uart_rxdma_config(UART_HandleTypeDef *huart)
{
	uint8_t ch;

	assert_param(huart);
	assert_param(huart->rxdma);
	assert_param(huart->Instance);

	UART_Rxdma *rxdma_reg = huart->rxdma;

	if(huart->rxdma_buf == NULL)
	{

		/* Drop data in Rx FIFO */
		while (huart->Instance->lsr & SP_UART_LSR_RX)
		{
			ch = READ_REG(huart->Instance->dr);
		}
		if(g_dma_buffer[0].rx_used == 0)
		{
			huart->rxdma_buf = (uint8_t* )g_dma_buffer[0].rx_buffer;
			g_dma_buffer[0].rx_used = 1;
		}
		else if(g_dma_buffer[1].rx_used == 0)
		{
			huart->rxdma_buf = (uint8_t* )g_dma_buffer[1].rx_buffer;
			g_dma_buffer[1].rx_used = 1;
		}
		else
		{
			return;
		}
		WRITE_REG(rxdma_reg->rxdma_start_addr,		huart->rxdma_buf);
		WRITE_REG(rxdma_reg->rxdma_rd_adr,			huart->rxdma_buf);

		/* Force to use CLK_HIGH_UART in this mode */
		/* Switch clock source when setting baud rate */

		WRITE_REG(rxdma_reg->rxdma_timeout_set,	((202500000 / 2) / 1000000 * 1000));  /* 1 msec */
		/*
		 * When there are only rxdma_length_thr[15:0] bytes of free buffer
		 * => Trigger interrupt
		 */
		WRITE_REG(rxdma_reg->rxdma_length_thr,		((UARXDMA_BUF_SZ << 16) | (UARXDMA_BUF_SZ - MAX_SZ_RXDMA_ISR)));

		WRITE_REG(rxdma_reg->rxdma_enable_sel,		(READ_REG(rxdma_reg->rxdma_enable_sel) & (~DMA_SEL_UARTX_MASK)));

		WRITE_REG(rxdma_reg->rxdma_enable_sel,		(READ_REG(rxdma_reg->rxdma_enable_sel) | (DMA_INIT | (huart->uart_idx << DMA_SEL_UARTX_SHIFT))));

		WRITE_REG(rxdma_reg->rxdma_enable_sel,		(READ_REG(rxdma_reg->rxdma_enable_sel) & (~DMA_INIT)));

		WRITE_REG(rxdma_reg->rxdma_enable_sel,		(READ_REG(rxdma_reg->rxdma_enable_sel)| (DMA_SW_RST_B | DMA_AUTO_ENABLE | DMA_TIMEOUT_INT_EN | DMA_ENABLE)));

		WRITE_REG(rxdma_reg->rxdma_enable_sel,		(READ_REG(rxdma_reg->rxdma_enable_sel) | (DMA_GO)));
	}
}

int HAL_UART_Get_TX_FIFO_Space(UART_HandleTypeDef *huart)
{
	assert_param(huart);
	return (READ_REG(huart->Instance->txr)&0x3f);
}

void HAL_UART_Rxdma_IRQ_Handler(UART_HandleTypeDef *huart)
{
	uint32_t offset_sw, offset_hw, rx_size, dma_start;
	uint32_t icount_rx,tmp_u32;
	uint8_t *sw_ptr, *buf_end_ptr, *u8_ptr;
	uint8_t tmp_buf[UART_DMA_ALIGN_SIZE];
	int i=0;

	UART_Rxdma *rxdma_reg = huart->rxdma;

	dma_start = READ_REG(rxdma_reg->rxdma_start_addr);
	offset_sw = READ_REG(rxdma_reg->rxdma_rd_adr) - dma_start;
	offset_hw = READ_REG(rxdma_reg->rxdma_wr_adr) - dma_start;

	if (offset_hw >= offset_sw) 
	{
		rx_size = offset_hw - offset_sw;
	} 
	else 
	{
		rx_size = offset_hw + UARXDMA_BUF_SZ - offset_sw;
	}

	sw_ptr = (uint8_t *)(huart->rxdma_buf + offset_sw);
	buf_end_ptr = (uint8_t *)(huart->rxdma_buf + UARXDMA_BUF_SZ);

	icount_rx = 0;
	while (rx_size > icount_rx && huart->rx_index < huart->rx_size) 
	{
		if (!(((unsigned long)(sw_ptr)) & (UART_DMA_ALIGN_SIZE - 1))	/* 32-byte aligned */
		    && ((rx_size - icount_rx) >= UART_DMA_ALIGN_SIZE) && ((huart->rx_size - icount_rx) >= UART_DMA_ALIGN_SIZE)) 
		{
			memcpy(tmp_buf, sw_ptr, UART_DMA_ALIGN_SIZE);
			u8_ptr = (uint8_t *)(tmp_buf);
			for (i = 0; i < UART_DMA_ALIGN_SIZE; i++) 
			{
				huart->pRxBuffPtr[huart->rx_index++] = u8_ptr[i];
			}
			sw_ptr += UART_DMA_ALIGN_SIZE;
			icount_rx += UART_DMA_ALIGN_SIZE;
		} 
		else 
		{
			huart->pRxBuffPtr[huart->rx_index++] = *sw_ptr;
			sw_ptr++;
			icount_rx++;
		}
		if (sw_ptr >= buf_end_ptr) 
		{
			sw_ptr = (uint8_t *)(huart->rxdma_buf);
		}
	}
	tmp_u32 = READ_REG(rxdma_reg->rxdma_rd_adr) + rx_size;
	/* start receive data by set the rd_addr register */
	if (tmp_u32 <= READ_REG(rxdma_reg->rxdma_end_addr))
	{
		WRITE_REG(rxdma_reg->rxdma_rd_adr,tmp_u32);
	}
	else
	{
		WRITE_REG(rxdma_reg->rxdma_rd_adr,(tmp_u32 - UARXDMA_BUF_SZ));
	}
	/* do dma for the next data*/
	WRITE_REG(rxdma_reg->rxdma_enable_sel,READ_REG((rxdma_reg->rxdma_enable_sel)) | DMA_INT);
	WRITE_REG(rxdma_reg->rxdma_enable_sel,READ_REG((rxdma_reg->rxdma_enable_sel)) | DMA_GO);
}

void HAL_UART_IRQHandler(void *arg)
{
	assert_param(arg);

	UART_HandleTypeDef *huart = (UART_HandleTypeDef *)arg;

	assert_param(huart->Instance);

	unsigned int isc_temp = huart->Instance->isc;

	if(isc_temp & SP_UART_ISC_RX)
	{
		__uart_rx_irq_handler(huart);
	}
	if(isc_temp & SP_UART_ISC_TX)
	{
		__uart_tx_irq_handler(huart);
	}
}

HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	int c = 0;

	assert_param(huart);
	assert_param(huart->txdma);
	assert_param(huart->txgdma);
	assert_param(huart->Instance);

	if (huart->gState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0U))
	    {
	      return  HAL_ERROR;
	    }
	    __HAL_LOCK(huart);

	    huart->gState = HAL_UART_STATE_BUSY_TX;
	    huart->pTxBuffPtr = NULL;
	    huart->tx_size = Size;
		/*
			0. config tx dma
	    */
	    __uart_txdma_config(huart);

		__HAL_UNLOCK(huart);

		/*
		    2. copy data to fix dma position
		*/
		while (1) 
		{

			c = CIRC_SPACE_TO_END(huart->xmit.head, huart->xmit.tail, UATXDMA_BUF_SZ);
			if (Size < c)
				c = Size;
			if (c <= 0)
				break;
			memcpy(huart->xmit.buf + huart->xmit.head, pData, c);
			huart->xmit.head = (huart->xmit.head + c) & (UATXDMA_BUF_SZ - 1);
			pData += c;
			Size -= c;
		}
		/* Writting zero to flush RX FIFO */
		WRITE_REG(huart->Instance->rxr, 0);
		/*
		    3. enable uart irq. send data in irq by dma mode
		*/
		__start_tx_irq(huart);

		return HAL_OK;
	}
	else
	{
	    return HAL_BUSY;
	}
}

HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{

	assert_param(huart);
	assert_param(huart->rxdma);
	assert_param(huart->Instance);

	if (huart->RxState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0U))
	    {
	      return  HAL_ERROR;
	    }

	   /* Init tickstart for timeout managment*/
	    __HAL_LOCK(huart);
	    
	    huart->RxState = HAL_UART_STATE_BUSY_RX;
	    huart->pRxBuffPtr = pData;
	    huart->rx_size = Size;
	    huart->rx_index = 0;

	    /* config rxdma  */
	    __uart_rxdma_config(huart);

	    __HAL_UNLOCK(huart);

	    return HAL_OK;
	}
	else
	{
	    return HAL_BUSY;
	}
}


HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	assert_param(huart);
	if (huart->gState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size <= 0U))
	    {
	      return  HAL_ERROR;
	    }
	   /* Init tickstart for timeout managment*/
	    __HAL_LOCK(huart);
	    huart->gState = HAL_UART_STATE_BUSY_TX;
	    huart->pTxBuffPtr = pData;
	    huart->tx_size = Size;
	    huart->tx_index = 0;
		__start_tx_irq(huart);
	    __HAL_UNLOCK(huart);

	    return HAL_OK;
	}
	else
	{
	    return HAL_BUSY;
	}
}

HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	assert_param(huart);

	if (huart->RxState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0U))
	    {
	      return  HAL_ERROR;
	    }

	   /* Init tickstart for timeout managment*/
	    __HAL_LOCK(huart);
	    huart->RxState = HAL_UART_STATE_BUSY_RX;
	    huart->pRxBuffPtr = pData;
	    huart->rx_size = Size;
		huart->rx_index = 0;

		/* start rx irq */
		__start_rx_irq(huart);

	    __HAL_UNLOCK(huart);

	    return HAL_OK;
	}
	else
	{
	    return HAL_BUSY;
	}
}


HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint8_t  *pdata8bits;
	uint32_t tickstart;

	assert_param(huart);
	assert_param(huart->Instance);
	if (huart->gState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0U))
	    {
	      return  HAL_ERROR;
	    }

	   /* Init tickstart for timeout managment*/
		tickstart = HAL_GetTick();

	    __HAL_LOCK(huart);
	    
	    huart->gState = HAL_UART_STATE_BUSY_TX;
	    pdata8bits = pData;

 		__HAL_UNLOCK(huart);

	    while (Size > 0U)
	    {
	    	/* wait transmit fifo not full.*/
			if (UART_WaitOnFlagUntilTimeout(huart, UART_LSR_TRANSMIT_FIFO_STATUS, UART_LSR_TRANSMIT_FIFO_NOT_FULL, tickstart, Timeout) != HAL_OK)
			{
				return HAL_TIMEOUT;
			}

			WRITE_REG(huart->Instance->dr,(*pdata8bits & 0xFF));
			pdata8bits++;
			Size--;
	    }
	    /* At end of Tx process, restore huart->gState to Ready */
	    huart->gState = HAL_UART_STATE_READY;

	    return HAL_OK;
	}
	else
	{
	    return HAL_BUSY;
	}
}


HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint8_t  *pdata8bits;
	uint32_t tickstart;

	assert_param(huart);
	assert_param(huart->Instance);


	if (huart->RxState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0U))
	    {
	      return  HAL_ERROR;
	    }

	   /* Init tickstart for timeout managment*/
		tickstart = HAL_GetTick();

	    __HAL_LOCK(huart);
	    huart->RxState = HAL_UART_STATE_BUSY_RX;
	    pdata8bits = pData;

	    __HAL_UNLOCK(huart);
	    while ( (Size > 0U))
		//while ((huart->Instance->lsr & SP_UART_LSR_RX) && (Size > 0U))
	    {   	
	    	
	    	/* wait transmit fifo not full.*/
			if (UART_WaitOnFlagUntilTimeout(huart, UART_LSR_RECEIVE_FIFO_STATUS, UART_LSR_RECEIVE_FIFO_NOT_EMPTY, tickstart, Timeout) != HAL_OK)
			{

				return HAL_TIMEOUT;
			}
			if((huart->Instance->lsr & SP_UART_LSR_FE) || (huart->Instance->lsr & SP_UART_LSR_OE) || \
				(huart->Instance->lsr & SP_UART_LSR_PE))
			{
				huart->ErrorCallback(huart);
				continue;
			}
			*pData++ = READ_REG(huart->Instance->dr);
			Size--;
	    }
	    /* At end of Rx process, restore huart->gState to Ready */
	    huart->RxState = HAL_UART_STATE_READY;
	    return HAL_OK;
	}
	else
	{
	    return HAL_BUSY;
	}
}


HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart)
{
	uint8_t ch;

	assert_param(huart);
	assert_param(huart->Instance);

	__HAL_LOCK(huart);

	huart->rx_size = 0;

	 __HAL_UNLOCK(huart);

	__stop_rx_irq(huart);

	/* clear fifo if there is data in rxfifo */
	WRITE_REG(huart->Instance->txr, 0);

	huart->RxState = HAL_UART_STATE_READY;

	 return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart)
{
	assert_param(huart);

	__HAL_LOCK(huart);

	huart->tx_size = 0;

	 __HAL_UNLOCK(huart);

	__stop_tx_irq(huart);

	huart->gState = HAL_UART_STATE_READY;

	 return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart)
{
	assert_param(huart);
	assert_param(huart->Instance);

	UART_Rxdma *rxdma_reg = huart->rxdma;
	uint8_t ch;

	__stop_tx_irq(huart);

	__stop_rx_irq(huart);

	/* Drop whatever is still in buffer */
	if(rxdma_reg)
	{
		WRITE_REG(rxdma_reg->rxdma_rd_adr,READ_REG(rxdma_reg->rxdma_wr_adr));
	}

	/* Writting zero to flush TX FIFO */
	WRITE_REG(huart->Instance->txr, 0);

	/* Writting zero to flush RX FIFO */
	WRITE_REG(huart->Instance->rxr, 0);

	/* clear all interrupt */
	WRITE_REG(huart->Instance->isc, 0);

	huart->gState  = HAL_UART_STATE_READY;
    huart->RxState = HAL_UART_STATE_READY;
}

uint32_t HAL_UART_GetState(UART_HandleTypeDef *huart)
{
  uint32_t temp1;
  uint32_t temp2;
  temp1 = huart->gState;
  temp2 = huart->RxState;
  return (uint32_t)(temp1 | temp2);
}

HAL_StatusTypeDef HAL_UART_Config_DMA_Buf(uint32_t* dma_start_addr)
{
	if((uint32_t)dma_start_addr & 0xfff)
	{
		return HAL_ERROR;
	}

	memset(g_dma_buffer,0,sizeof(DMA_Buffer_t) * 2);

	g_dma_buffer[0].tx_buffer = 		(uint32_t *)dma_start_addr;
	g_dma_buffer[0].tx_data_buffer = 	g_dma_buffer[0].tx_buffer + UATXDMA_BUF_SZ;
	g_dma_buffer[0].rx_buffer = 		g_dma_buffer[0].tx_data_buffer + UATXDMA_BUF_SZ;
	g_dma_buffer[0].tx_used = 0;

	g_dma_buffer[1].tx_buffer = 		g_dma_buffer[0].rx_buffer + UATXDMA_BUF_SZ;
	g_dma_buffer[1].tx_data_buffer = 	g_dma_buffer[1].tx_buffer + UATXDMA_BUF_SZ;
	g_dma_buffer[1].rx_buffer = 		g_dma_buffer[1].tx_data_buffer + UATXDMA_BUF_SZ;
	g_dma_buffer[1].tx_used = 0;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
{

	assert_param(IS_UART_INSTANCE(huart->Instance));
	
	/* step 1: config pinmux */
	if(huart->Instance == SP_UART0)
	{
		huart->uart_idx = 0;
	}
	else if(huart->Instance == SP_UART1)
	{
		huart->uart_idx = 1;
	}
	else if(huart->Instance == SP_UART2)
	{
		huart->uart_idx = 2;
	}
	else if(huart->Instance == SP_UART3)
	{
		huart->uart_idx = 3;
	}
	else if(huart->Instance == SP_UART4)
	{
		huart->uart_idx = 4;
	}
	else
	{
		return HAL_ERROR;	
	}

	/* step 3: set uart baudrate */
	huart->Instance->div_l = UART_BAUD_DIV_L(huart->Init.BaudRate, UART_SRC_CLK);
	huart->Instance->div_h = UART_BAUD_DIV_H(huart->Init.BaudRate, UART_SRC_CLK);

	/* config line control register */
	if(huart->Init.LcrInit.LCR_Init != UART_INIT_USE_DEFAULT_VALUE)
	{
		__uart_lcr_config(huart);
	}

	if(huart->Init.McrInit.MCR_Init != UART_INIT_USE_DEFAULT_VALUE)
	{
		__uart_mcr_config(huart);
	}

	UART_InitCallbacksToDefault(huart);
	huart->gState = HAL_UART_STATE_READY;
	huart->RxState = HAL_UART_STATE_READY;
	huart->Lock = HAL_UNLOCKED;
	return HAL_OK;
}


HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart)
{
  if (huart == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(huart->Instance);
  assert_param(IS_UART_INSTANCE(huart->Instance));

  if(huart->txdma_buf == g_dma_buffer[0].tx_buffer)
  {
  	memset(&g_dma_buffer[0],0,sizeof(DMA_Buffer_t));
  }
  else if(huart->txdma_buf == g_dma_buffer[1].tx_buffer)
  {
  	memset(&g_dma_buffer[1],0,sizeof(DMA_Buffer_t));
  }

  huart->gState = HAL_UART_STATE_READY;
  huart->RxState = HAL_UART_STATE_READY;

  __HAL_UNLOCK(huart);

  return HAL_OK;
}
