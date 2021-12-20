#ifndef SP7021_HAL_SPI_H
#define SP7021_HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif


#include "sp7021_hal_def.h"
#include "sp7021_arm926.h"
#include "sp70xx.h" 	




typedef enum
{
  HAL_SPI_STATE_RESET      = 0x00U,
  HAL_SPI_STATE_READY      = 0x01U,
  HAL_SPI_STATE_BUSY       = 0x02U,
  HAL_SPI_STATE_BUSY_TX    = 0x03U,
  HAL_SPI_STATE_BUSY_RX    = 0x04U,
  HAL_SPI_STATE_BUSY_TX_RX = 0x05U,
  HAL_SPI_STATE_ERROR      = 0x06U,
  HAL_SPI_STATE_ABORT      = 0x07U
} HAL_SPI_StateTypeDef;

typedef enum {
  SPI_MODE0 = 0x00,
  SPI_MODE1 = 0x01,
  SPI_MODE2 = 0x02,
  SPI_MODE3 = 0x03
} spi_mode;

typedef struct
{
  uint32_t spiclk;
  spi_mode spi_mode;
  uint32_t FirstBit;
} SPI_InitTypeDef;

typedef struct __SPI_HandleTypeDef
{
  /* user set */
  SPI_TypeDef                *Instance;      /*!< SPI registers base address               */

  SPI_InitTypeDef            Init;           /*!< SPI communication parameters             */


  /* driver used */
  uint8_t                    *pTxBuffPtr;    /*!< Pointer to SPI Tx transfer Buffer        */

  uint16_t                   TxXferSize;     /*!< SPI Tx Transfer size                     */

  __IO uint16_t              TxXferCount;    /*!< SPI Tx Transfer Counter                  */

  uint8_t                    *pRxBuffPtr;    /*!< Pointer to SPI Rx transfer Buffer        */

  uint16_t                   RxXferSize;     /*!< SPI Rx Transfer size                     */

  __IO uint16_t              RxXferCount;    /*!< SPI Rx Transfer Counter                  */


  HAL_LockTypeDef            Lock;           /*!< Locking object                           */

  __IO HAL_SPI_StateTypeDef  State;          /*!< SPI communication state                  */

  __IO uint32_t              ErrorCode;      /*!< SPI Error code                           */

  void (* TxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Tx Completed callback          */
  void (* RxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Rx Completed callback          */
  void (* TxRxCpltCallback)(struct __SPI_HandleTypeDef *hspi);           /*!< SPI TxRx Completed callback        */

} SPI_HandleTypeDef;


#define HAL_SPI_ERROR_NONE              (0x00000000U)
#define HAL_SPI_ERROR_MODF              (0x00000001U)
#define HAL_SPI_ERROR_CRC               (0x00000002U)
#define HAL_SPI_ERROR_OVR               (0x00000004U)
#define HAL_SPI_ERROR_FRE               (0x00000008U)
#define HAL_SPI_ERROR_DMA               (0x00000010U)
#define HAL_SPI_ERROR_FLAG              (0x00000020U)
#define HAL_SPI_ERROR_ABORT             (0x00000040U)
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
#define HAL_SPI_ERROR_INVALID_CALLBACK  (0x00000080U)
#endif

#define SPI_FIRSTBIT_MSB                (0x00000000U)
#define SPI_FIRSTBIT_LSB                (0x00000001U)

#define SPI_FREQ_MIN                    (40000)     /* spi clk  min is 40k */
#define SPI_FREQ_MAX                    (50000000)  /* spi clk  max is 50M */
/* SPI MST DMA_SIZE */
#define MST_DMA_RSIZE(x)       (x<<16)
#define MST_DMA_WSIZE(x)       (x<<0)

/* SPI MST DMA config */
#define MST_DMA_EN             (1<<1)
#define MST_DMA_START          (1<<0)

/* full duplex, fifo length is config 16 byte */
#define SPI_FIFO_MAX_LENGTH     (16)

/* SPI MST STATUS */


#define CLEAN_FLUG_MASK (~0xF800)

#define CLEAR_IRQ_MASK         (~0xF800)
#define CLEAN_RW_BYTE          (~0x780)
#define CLEAN_SPI_MODE         (~0x7)

#define SPI_CLK_RATE           202500000


#define IS_SPI_MODE(__MODE__)      (((__MODE__) >= SPI_MODE0) &&\
                                    ((__MODE__) <= SPI_MODE3))

#define IS_SPI_FIRST_BIT(__BIT__)  (((__BIT__) == SPI_FIRSTBIT_MSB) || \
                                    ((__BIT__) == SPI_FIRSTBIT_LSB))

#define IS_VALID_FREQ(__freq__)  (((__freq__) >= SPI_FREQ_MIN) && \
                                    ((__freq__) <= SPI_FREQ_MAX))

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t TxSize,
                                           uint16_t RxSize,uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,  uint16_t TxSize,uint16_t RxSize);

HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi);
HAL_SPI_StateTypeDef HAL_SPI_GetState(SPI_HandleTypeDef *hs);

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *arg);

#ifdef __cplusplus
}
#endif

#endif