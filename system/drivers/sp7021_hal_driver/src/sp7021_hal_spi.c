#include "sp7021_hal_spi.h"

void __Spi_receive_data(SPI_HandleTypeDef *hspi,uint8_t vaild_data)
{
	uint8_t rxdata;

	//SPI_TypeDef * hspi->Instance = SPI0;
	/* drop the rx data while transmit data*/
	if(hspi->TxXferSize > 0 && hspi->TxXferCount < hspi->TxXferSize)
	{
		vaild_data = 0;
	}
	/* receive data after tx complete ,update rx fifo in other cases*/
	if(hspi->pRxBuffPtr && vaild_data && (hspi->RxXferCount < hspi->RxXferSize))
	{
		//*((uint8_t *)hspi->pRxBuffPtr) = hspi->Instance->fifo_data;
		memcpy((uint8_t *)hspi->pRxBuffPtr,&hspi->Instance->fifo_data,sizeof(uint8_t));
		hspi->pRxBuffPtr++;
		hspi->RxXferCount++;
	}
	else
	{
		memcpy(&rxdata,&hspi->Instance->fifo_data,sizeof(uint8_t));
	}
}

void __Spi_transmit_data(SPI_HandleTypeDef *hspi)
{

	if(hspi->pTxBuffPtr)
	{
		memcpy(&hspi->Instance->fifo_data,(uint8_t *)hspi->pTxBuffPtr,sizeof(uint8_t));
		hspi->pTxBuffPtr++;
		hspi->TxXferCount++;
	}	
}

void __SPI_Config_Set(SPI_HandleTypeDef *hspi,uint32_t flag)
{
	uint32_t temp_reg=0;
	uint32_t div;

	/*  spi config set */
	div = SPI_CLK_RATE / hspi->Init.spiclk ;
	div = (div / 2) - 1;

	/* config full duplex mode */
	temp_reg = FD_SEL | ((div & 0xffff) << 16);
	/* set fifo to 16byte mode */
	temp_reg &= CLEAN_RW_BYTE;
	temp_reg |= WRITE_BYTE(0) | READ_BYTE(0);
	temp_reg &= CLEAN_SPI_MODE;	
	/* lsb select  0:msb  1:lsb*/
	temp_reg |= hspi->Init.FirstBit;

	/* CPOL/CPHA select */
	switch(hspi->Init.spi_mode)
	{
		case SPI_MODE0:
			temp_reg |= CPHA_W;
			break;
		case SPI_MODE1:
			temp_reg |= CPHA_R;
			break;
		case SPI_MODE2:
			temp_reg |= CPHA_W;
			temp_reg |= CPOL;  
			break;
		case SPI_MODE3:
			temp_reg |= CPHA_R;
			temp_reg |= CPOL;  
			break;
		default:
			break;
	}
	temp_reg |= flag;
	/*config the spi_config register*/
	hspi->Instance->spi_config = temp_reg;
}

__weak void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  UNUSED(hspi);
}

__weak void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  UNUSED(hspi);
}

__weak void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  UNUSED(hspi);
}

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
	uint32_t temp_reg=0;
	uint32_t div;

	if (hspi == NULL)
	{
	  return HAL_ERROR;
	}

	assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

    assert_param(IS_SPI_MODE(hspi->Init.spi_mode));
	assert_param(IS_SPI_FIRST_BIT(hspi->Init.FirstBit));
	assert_param(IS_VALID_FREQ(hspi->Init.spiclk));

	hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  	hspi->State     = HAL_SPI_STATE_READY;

  	hspi->TxCpltCallback       = HAL_SPI_TxCpltCallback;
    hspi->RxCpltCallback       = HAL_SPI_RxCpltCallback;
    hspi->TxRxCpltCallback     = HAL_SPI_TxRxCpltCallback;

  	return HAL_OK;
}

HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi)
{
  if (hspi == NULL)
  {
    return HAL_ERROR;
  }

  assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

  hspi->Instance->spi_config &= CLEAN_FLUG_MASK;
  /* reset regist ,clear fifo data */
  hspi->Instance->spi_status |= SPI_SW_RST;

  hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  hspi->State = HAL_SPI_STATE_RESET;

  __HAL_UNLOCK(hspi);

  return HAL_OK;
}

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	unsigned int temp_reg;
	unsigned int i,tx_cnt;
	unsigned char rxdata;
	uint32_t tickstart;

	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto error;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		errorcode = HAL_ERROR;
		goto error;
	}

  	__HAL_LOCK(hspi);

	tickstart = HAL_GetTick();

	hspi->State       = HAL_SPI_STATE_BUSY_TX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pData;
	hspi->TxXferCount = 0U;
	hspi->TxXferSize  = Size;

	/*Init field not used in handle to zero */
	hspi->pRxBuffPtr  = (uint8_t *)NULL;
	hspi->RxXferSize  = 0U;
	hspi->RxXferCount = 0U;

	__SPI_Config_Set(hspi, FINISH_FLAG_MASK|TX_EMP_FLAG_MASK);

	/* Maximum transmission of 16 bytes at a time */
	tx_cnt  = (Size >= SPI_FIFO_MAX_LENGTH)? SPI_FIFO_MAX_LENGTH:Size;
	while (hspi->TxXferCount < tx_cnt)
    {
        __Spi_transmit_data(hspi);
	}

	hspi->Instance->spi_status = TOTAL_LENGTH(Size) | TX_LENGTH(Size);
	hspi->Instance->spi_status |= SPI_START_FD;

	while((hspi->Instance->spi_status & SPI_BUSY) == SPI_BUSY)
	{
		if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          errorcode = HAL_TIMEOUT;
		  goto error;
        }
     
	    if((hspi->TxXferCount < Size) || ((hspi->Instance->spi_status & TX_EMP_FLAG) == TX_EMP_FLAG))
	    {
   	        if((hspi->Instance->spi_status & RX_FULL_FLAG)==RX_FULL_FLAG)
   	        {
		        for(i=0;i<SPI_FIFO_MAX_LENGTH;i++)
		        {
		           /* drop the rx fifo data */	
			       __Spi_receive_data(hspi,false);  
		        }
	        }

	        while((hspi->Instance->spi_status & RX_CNT ))
	        {	
		         __Spi_receive_data(hspi,false);  
			   if((hspi->TxXferCount < Size) && ((hspi->Instance->spi_status & TX_FULL_FLAG) != TX_FULL_FLAG))
			   {
				  __Spi_transmit_data(hspi);
			   }   
	       }
 		    while(hspi->TxXferCount < Size)
 		    {
				if((hspi->Instance->spi_status & TX_FULL_FLAG) == TX_FULL_FLAG)
			    	break;
				__Spi_transmit_data(hspi);
        	}  
		}
		else if((hspi->Instance->spi_status & RX_FULL_FLAG)==RX_FULL_FLAG)
		{
		    for(i=0;i<SPI_FIFO_MAX_LENGTH;i++)
		    {
	           /* drop the rx fifo data */	
		        __Spi_receive_data(hspi,false);  
	        }
		}
		else 
		{
			while((hspi->Instance->spi_status & RX_CNT ))
			{	
			   __Spi_receive_data(hspi,false); 
			}
		}
	}

	while((hspi->Instance->spi_status & FINISH_FLAG)!=FINISH_FLAG)
	{
		/* wait finish */
	};

	hspi->Instance->spi_config &= CLEAN_FLUG_MASK;
	hspi->Instance->spi_status |= SPI_SW_RST;

error:
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;

}

HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	unsigned int i;	
    uint32_t tickstart;
    HAL_StatusTypeDef errorcode = HAL_OK;

    /* Process Locked */
	__HAL_LOCK(hspi);

	/* Init tickstart for timeout management*/
	tickstart = HAL_GetTick();

	if (hspi->State != HAL_SPI_STATE_READY)
	{
	  errorcode = HAL_BUSY;
	  goto error;
	}

	if ((pData == NULL) || (Size == 0U))
	{
	  errorcode = HAL_ERROR;
	  goto error;
	}

	/* Set the transaction information */
	hspi->State       = HAL_SPI_STATE_BUSY_RX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pData;
	hspi->RxXferSize  = Size;
	hspi->RxXferCount = 0;

	/*Init field not used in handle to zero */
	hspi->pTxBuffPtr  = (uint8_t *)NULL;
	hspi->TxXferSize  = 0U;
	hspi->TxXferCount = 0U;

    hspi->Instance->fifo_data = 0x00;

    __SPI_Config_Set(hspi, FINISH_FLAG_MASK|RX_FULL_FLAG_MASK);
	
	hspi->Instance->spi_status = TOTAL_LENGTH(Size) | TX_LENGTH(0);
	hspi->Instance->spi_status |= SPI_START_FD;
	
	while((hspi->Instance->spi_status & SPI_BUSY) == SPI_BUSY)
	{
		if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          errorcode = HAL_TIMEOUT;
		  goto error;
        }

	    if((hspi->Instance->spi_status & RX_FULL_FLAG)==RX_FULL_FLAG)
	    {
		    for(i=0;i<SPI_FIFO_MAX_LENGTH;i++)
		    {
		    	__Spi_receive_data(hspi,true);
		 	}
		}
	
		while((hspi->Instance->spi_status & RX_CNT ))
		{	
	    	__Spi_receive_data(hspi,true);
		}
	};

	if((hspi->Instance->spi_status & FINISH_FLAG)==FINISH_FLAG)
	{
   	    if((hspi->Instance->spi_status & RX_FULL_FLAG)==RX_FULL_FLAG)
   	    {
		    for(i=0;i<SPI_FIFO_MAX_LENGTH;i++)
		    {
			   __Spi_receive_data(hspi,true);
		    }
	    }

	    while((hspi->Instance->spi_status & RX_CNT ))
	    {	
		    __Spi_receive_data(hspi,true);
	    }

		//memcpy(data_buf, &spi_mas_ctlr[spi_no].rx_data_buf[0], spi_mas_ctlr[spi_no].rx_cur_len);

		hspi->Instance->spi_config &= CLEAN_FLUG_MASK;
		hspi->Instance->spi_status |= SPI_SW_RST;
	}

error:
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;

}

HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t TxSize,
                                          uint16_t RxSize,uint32_t Timeout)
{
	unsigned int temp_reg;
	unsigned int i,tx_cnt;
	unsigned char rxdata;
	uint32_t tickstart;

	HAL_StatusTypeDef errorcode = HAL_OK;
	
	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto error;
	}

	if ((pTxData == NULL) || (pRxData == NULL) || (TxSize == 0) | (RxSize == 0))
	{
		errorcode = HAL_ERROR;
		goto error;
	}

  	__HAL_LOCK(hspi);

	tickstart = HAL_GetTick();

	if (hspi->State != HAL_SPI_STATE_BUSY_RX)
	{
		hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
	}

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = 0;
	hspi->TxXferSize  = TxSize;

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = 0;
	hspi->RxXferSize  = RxSize;
	
	__SPI_Config_Set(hspi, FINISH_FLAG_MASK|RX_FULL_FLAG_MASK|TX_EMP_FLAG_MASK);

	tx_cnt  = (TxSize >= SPI_FIFO_MAX_LENGTH)? SPI_FIFO_MAX_LENGTH:TxSize;

	while (hspi->TxXferCount < tx_cnt)
    {
        __Spi_transmit_data(hspi);
	}

	hspi->Instance->spi_status = TOTAL_LENGTH(RxSize+TxSize) | TX_LENGTH(TxSize);
	hspi->Instance->spi_status |= SPI_START_FD;

	while((hspi->Instance->spi_status & SPI_BUSY) == SPI_BUSY)
	{
		if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          errorcode = HAL_TIMEOUT;
		  goto error;
        }

		if((hspi->TxXferCount < TxSize) || ((hspi->Instance->spi_status & TX_EMP_FLAG) == TX_EMP_FLAG))
		{
			if((hspi->Instance->spi_status & RX_FULL_FLAG)==RX_FULL_FLAG)
			{
				for(i=0;i<SPI_FIFO_MAX_LENGTH;i++)
				{
				    __Spi_receive_data(hspi,true);
				}
			}
			while((hspi->Instance->spi_status & RX_CNT ))
			{	
				__Spi_receive_data(hspi,true);
				if((hspi->TxXferCount < TxSize) && ((hspi->Instance->spi_status & TX_FULL_FLAG) != TX_FULL_FLAG))
				{
					__Spi_transmit_data(hspi);	   
				}   
		    }
			while(hspi->TxXferCount < TxSize)
			{
			    if((hspi->Instance->spi_status & TX_FULL_FLAG)==TX_FULL_FLAG)
			    {
			        break;
			    }
			    __Spi_transmit_data(hspi);
			}  
        }
        else if((hspi->Instance->spi_status & RX_FULL_FLAG)==RX_FULL_FLAG)
        {
	        for(i=0;i<SPI_FIFO_MAX_LENGTH;i++)
	        {
		       __Spi_receive_data(hspi,true);
		    }
	    }
	    else 
	    {
	       while((hspi->Instance->spi_status & RX_CNT ))
	       {	
		      __Spi_receive_data(hspi,true);
		   }
	   }
	}

	if((hspi->Instance->spi_status & FINISH_FLAG)==FINISH_FLAG)
	{
   	    if((hspi->Instance->spi_status & RX_FULL_FLAG)==RX_FULL_FLAG)
   	    {
		    for(i=0;i<SPI_FIFO_MAX_LENGTH;i++)
		    {
			   __Spi_receive_data(hspi,true);
		    }
	    }

	    while((hspi->Instance->spi_status & RX_CNT ))
	    {	
		    __Spi_receive_data(hspi,true);
	    }

		hspi->Instance->spi_config &= CLEAN_FLUG_MASK;
		hspi->Instance->spi_status |= SPI_SW_RST;
	}

error:
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;

}

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *arg)
{
	uint32_t fd_status = 0,i;
	uint8_t isrdone = FALSE;

	unsigned int tx_len, rx_cnt, tx_cnt;

	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)arg;

	fd_status = hspi->Instance->spi_status;

	tx_cnt = GET_TX_CNT( fd_status);
	tx_len = GET_TX_LEN( fd_status);
	
	if ( (fd_status & TX_EMP_FLAG) && ( fd_status & RX_EMP_FLAG) || (GET_LEN(fd_status) == 0)) {
		goto fin_irq;
	}

	rx_cnt = GET_RX_CNT( fd_status);

	if ( fd_status & RX_FULL_FLAG) rx_cnt = SPI_FIFO_MAX_LENGTH;

	tx_cnt = (tx_len - hspi->TxXferCount)<(SPI_FIFO_MAX_LENGTH - tx_cnt)?(tx_len - hspi->TxXferCount):(SPI_FIFO_MAX_LENGTH - tx_cnt);

	/* receive data if rx fifo is not empty */
	for ( i = 0; i < rx_cnt; i++)
	{ 
		__Spi_receive_data(hspi,true);
	}

	/* transmit data if tx fifo is not full */
	for ( i = 0; i < tx_cnt; i++)
	{
		__Spi_transmit_data(hspi);
	}

	fd_status = hspi->Instance->spi_status;

	if (( fd_status & FINISH_FLAG) || (GET_TX_LEN(fd_status) == hspi->TxXferCount))
	{
		while((GET_LEN(fd_status)-tx_len) != hspi->RxXferCount)
		{
		    fd_status = hspi->Instance->spi_status;
		    if (fd_status & RX_FULL_FLAG)
		    {
				rx_cnt = SPI_FIFO_MAX_LENGTH;
		    }
		    else
		    {
				rx_cnt = GET_RX_CNT(fd_status);
		    }	
		    /* receive data */
		    for (i = 0; i < rx_cnt; i++)
	    	{ 
				__Spi_receive_data(hspi,true);
			}
		}
		isrdone = true;
		hspi->Instance->spi_int_busy |= CLEAR_MASTER_INT;
		hspi->Instance->spi_config &= CLEAN_FLUG_MASK;
		hspi->Instance->spi_status |= SPI_SW_RST;
	}

fin_irq:
	if(isrdone)
	{
		hspi->State = HAL_SPI_STATE_READY;
		if(hspi->TxXferSize && hspi->RxXferSize){
			hspi->TxRxCpltCallback(hspi);
		}
		else if(hspi->TxXferSize){
			hspi->TxCpltCallback(hspi);
		}
		else if(hspi->RxXferSize){
			hspi->RxCpltCallback(hspi);
		}
	}
	return;
}

HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
	unsigned int tx_cnt;
	HAL_StatusTypeDef errorcode = HAL_OK;

    __HAL_LOCK(hspi);

	if ((pData == NULL) || (Size == 0U))
	{
		errorcode = HAL_ERROR;
		goto error;
	}

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto error;
	}

  	hspi->State       = HAL_SPI_STATE_BUSY_TX;

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pData;
	hspi->TxXferCount = 0;
	hspi->TxXferSize  = Size;

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = NULL;
	hspi->RxXferCount = 0;
	hspi->RxXferSize  = 0;

	__SPI_Config_Set(hspi, 0);

	tx_cnt  = (Size >= SPI_FIFO_MAX_LENGTH)? SPI_FIFO_MAX_LENGTH:Size;

	while (hspi->TxXferCount < tx_cnt)
    {
        __Spi_transmit_data(hspi);
	}

	hspi->Instance->spi_status = TOTAL_LENGTH(Size) | TX_LENGTH(Size);

	hspi->Instance->spi_config |= FINISH_FLAG_MASK|TX_EMP_FLAG_MASK;

	hspi->Instance->spi_status |= SPI_START_FD;

error :
 	__HAL_UNLOCK(hspi);
  	return errorcode;
}

HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{

	unsigned int temp_reg;
  	HAL_StatusTypeDef    errorcode = HAL_OK;

	__HAL_LOCK(hspi);

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto error;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		errorcode = HAL_ERROR;
		goto error;
	}

    hspi->State       = HAL_SPI_STATE_BUSY_RX;

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = NULL;
	hspi->TxXferCount = 0;
	hspi->TxXferSize  = 0;

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pData;
	hspi->RxXferCount = 0;
	hspi->RxXferSize  = Size;


	__SPI_Config_Set(hspi, FINISH_FLAG_MASK|RX_FULL_FLAG_MASK);

	hspi->Instance->spi_status = TOTAL_LENGTH(Size) | TX_LENGTH(0);
	hspi->Instance->spi_status |= SPI_START_FD;

error :
  __HAL_UNLOCK(hspi);
  return errorcode;
}

HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,
                                             uint16_t TxSize,uint16_t RxSize)
{
	unsigned int temp_reg;
	unsigned int tx_cnt;

  	HAL_StatusTypeDef  errorcode = HAL_OK;

  	__HAL_LOCK(hspi);

  	if ((pTxData == NULL) || (pRxData == NULL) || (RxSize == 0U) || (TxSize == 0U))
	{
		errorcode = HAL_ERROR;
		goto error;
	}

	if (hspi->State != HAL_SPI_STATE_BUSY_RX)
	{
		hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
	}

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = 0;
	hspi->TxXferSize  = TxSize;

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = 0;
	hspi->RxXferSize  = RxSize;


	__SPI_Config_Set(hspi, 0);

	tx_cnt  = (TxSize >= SPI_FIFO_MAX_LENGTH)? SPI_FIFO_MAX_LENGTH:TxSize;
	while (hspi->TxXferCount < tx_cnt)
    {
        __Spi_transmit_data(hspi);
	}

	hspi->Instance->spi_status = TOTAL_LENGTH(RxSize+TxSize) | TX_LENGTH(TxSize);

	hspi->Instance->spi_config |= FINISH_FLAG_MASK|RX_FULL_FLAG_MASK|TX_EMP_FLAG_MASK;

	hspi->Instance->spi_status |= SPI_START_FD;

error :
  __HAL_UNLOCK(hspi);
  return errorcode;
}

HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi)
{
	HAL_StatusTypeDef errorcode = HAL_OK;

	if ((hspi == NULL))
	{
		errorcode = HAL_ERROR;
		goto error;
	}
	__HAL_LOCK(hspi);

	hspi->TxXferSize  = 0;
	hspi->TxXferCount = 0;

	hspi->RxXferSize  = 0;
	hspi->RxXferCount = 0;

	/*  clear the irq mask */
	__SPI_Config_Set(hspi,0);
	/* set total len and tx len to zero */
	hspi->Instance->spi_status = TOTAL_LENGTH(0) | TX_LENGTH(0);

	/* Clear SPI master interrupt */
	WRITE_REG(hspi->Instance->spi_int_busy,READ_REG(hspi->Instance->spi_int_busy) | CLEAR_MASTER_INT);

	hspi->State = HAL_SPI_STATE_READY;

error:
	__HAL_UNLOCK(hspi);
	return errorcode;
}
