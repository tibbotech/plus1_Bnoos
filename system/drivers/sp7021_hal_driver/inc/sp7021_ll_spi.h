#ifndef __SP7021_LL_STC_H__
#define __SP7021_LL_STC_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_CLK_RATE           202500000
#define CLEAN_RW_BYTE          (~0x780)
#define CLEAN_SPI_MODE         (~0x7)


__STATIC_INLINE void LL_SPI_Config_Set(SPI_TypeDef *SPIx,uint32_t spiclk,uint32_t spi_mode,uint32_t msb)
{
	uint32_t temp_reg=0;
	uint32_t div;

	/*  spi config set */
	div = SPI_CLK_RATE / spiclk ;
	div = (div / 2) - 1;

	/* config full duplex mode */
	temp_reg = FD_SEL | ((div & 0xffff) << 16);
	/* set fifo to 16byte mode */
	temp_reg &= CLEAN_RW_BYTE;
	temp_reg |= WRITE_BYTE(0) | READ_BYTE(0);
	temp_reg &= CLEAN_SPI_MODE;	
	/* lsb select  0:msb  1:lsb*/
	temp_reg |= msb;

	/* CPOL/CPHA select */
	switch(spi_mode)
	{
		case 0:	temp_reg |= CPHA_W;	break;
		case 1:	temp_reg |= CPHA_R;	break;
		case 2:	temp_reg |= CPHA_W;	temp_reg |= CPOL; break;
		case 3:	temp_reg |= CPHA_R;	temp_reg |= CPOL; break;
		default:
			break;
	}
	/*config the spi_config register*/
	WRITE_REG(SPIx->spi_config,temp_reg);

}

__STATIC_INLINE void LL_SPI_StartMasterTransfer(SPI_TypeDef *SPIx)
{
	MODIFY_REG(SPIx->spi_status, SPI_START_FD, SPI_START_FD);
}

__STATIC_INLINE void LL_SPI_SetTransferSize(SPI_TypeDef *SPIx, uint32_t Count)
{
  MODIFY_REG(SPIx->spi_status, SPI_TOTAL_SIZE, TOTAL_LENGTH(Count));
  MODIFY_REG(SPIx->spi_status, SPI_TX_SIZE, TX_LENGTH(Count));
}

__STATIC_INLINE void LL_SPI_TransmitData8(SPI_TypeDef *SPIx, uint8_t TxData)
{
  *((__IO uint32_t *)&SPIx->fifo_data) = TxData;
}

__STATIC_INLINE uint8_t LL_SPI_ReceiveData8(SPI_TypeDef *SPIx)
{
  return (*((__IO uint8_t *)&SPIx->fifo_data));
}

__STATIC_INLINE uint32_t LL_SPI_IsActiveFlag_RXNE(SPI_TypeDef *SPIx)
{
  return ((READ_BIT(SPIx->spi_status, RX_CNT)) ? 1UL : 0UL);
}

__STATIC_INLINE uint32_t LL_SPI_IsActiveFlag_TXE(SPI_TypeDef *SPIx)
{
  return ((READ_BIT(SPIx->spi_status, TX_EMP_FLAG) == (TX_EMP_FLAG)) ? 1UL : 0UL);
}

#ifdef __cplusplus
}
#endif

#endif