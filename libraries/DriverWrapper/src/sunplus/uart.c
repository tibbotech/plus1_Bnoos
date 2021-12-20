#include "core_debug.h"
#include "uart.h"
#include "Arduino.h"


#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_UART_BAUDRATE  115200
/* @brief uart caracteristics */
typedef enum {
	UART0_INDEX=0,
	UART1_INDEX,
	UART2_INDEX,
	UART3_INDEX,
	UART4_INDEX,
 	UART_NUM
} uart_index_t;

static UART_HandleTypeDef *uart_handlers[UART_NUM] = {NULL};

static serial_t serial_debug = { .uart = SP_UART0, .index = UART0_INDEX };

serial_t *get_serial_obj(UART_HandleTypeDef *huart)
{
  struct serial_s *obj_s;
  serial_t *obj;

  obj_s = (struct serial_s *)((char *)huart - offsetof(struct serial_s, handle));
  obj = (serial_t *)((char *)obj_s - offsetof(serial_t, uart));

  return (obj);
}

/**
  * @brief  Function called to initialize the uart interface
  * @param  obj : pointer to serial_t structure
  * @retval None
  */
void uart_init(serial_t *obj, uint32_t baudrate)
{
    uint32_t uart_tx,uart_rx;
	if (obj == NULL) {
 	  	return;
  	}

  UART_HandleTypeDef *huart = &(obj->handle);

  if (obj->uart == NULL)
  {
    core_debug("ERROR: uart register set error!\n");
    return;
  }

  if(obj->uart != SP_UART0)
  { 
    uart_tx = GPIO_TO_PINMUX(obj->pin_tx);
    uart_rx = GPIO_TO_PINMUX(obj->pin_rx);

    if(IS_VALID_PINMUX(uart_tx) == 0)
    {
    	core_debug("ERROR: [UART] Tx pin error!\n");
      return;
    }

    if(IS_VALID_PINMUX(uart_rx) == 0)
    {
    	core_debug("ERROR: [UART] Rx pin error!\n");
      return;
    }
  }

  /* Enable USART clock */
  if(obj->uart == SP_UART0)
  { 
      /* enable clk */
      HAL_Module_Clock_enable(UART0, 1);
      HAL_Module_Clock_gate(UART0, 1);
      HAL_Module_Reset(UART0, 0);
      /* pinmux set */
      HAL_PINMUX_Cfg(PINMUX_UART0,1);
      /*interrupt set */
      obj->index = UART0_INDEX;
      obj->irq = UART0_IRQn;
      IRQ_SetHandler(obj->irq, UART0_IRQHandler);
      IRQ_Enable(obj->irq);
  }
  else if(obj->uart == SP_UART1)
  {
  		/* enable clk */
	    HAL_Module_Clock_enable(UART1, 1);
  		HAL_Module_Clock_gate(UART1, 1);
  		HAL_Module_Reset(UART1, 0);
  		/* pinmux set */
  		HAL_PINMUX_Cfg(PINMUX_UA1_TX,uart_tx);
  		HAL_PINMUX_Cfg(PINMUX_UA1_RX,uart_rx);
  		/*interrupt set */
	    obj->index = UART1_INDEX;
	    obj->irq = UART1_IRQn;
	    IRQ_SetHandler(obj->irq, UART1_IRQHandler);
	    IRQ_Enable(obj->irq);
	}
  else if(obj->uart == SP_UART2)
  {
  		/* enable clk */
	    HAL_Module_Clock_enable(UART2, 1);
  		HAL_Module_Clock_gate(UART2, 1);
  		HAL_Module_Reset(UART2, 0);
  		/* pinmux set */
  		HAL_PINMUX_Cfg(PINMUX_UA2_TX,uart_tx);
  		HAL_PINMUX_Cfg(PINMUX_UA2_RX,uart_rx);
  		/*interrupt set */
	    obj->index = UART2_INDEX;
	    obj->irq = UART2_IRQn;
	    IRQ_SetHandler(obj->irq, UART2_IRQHandler);
	    IRQ_Enable(obj->irq);
  }
  else if(obj->uart == SP_UART3)
  {
  		/* enable clk */
	    HAL_Module_Clock_enable(UART3, 1);
  		HAL_Module_Clock_gate(UART3, 1);
  		HAL_Module_Reset(UART3, 0);
  		/* pinmux set */
  		HAL_PINMUX_Cfg(PINMUX_UA3_TX,uart_tx);
  		HAL_PINMUX_Cfg(PINMUX_UA3_RX,uart_rx);
  		/*interrupt set */
	    obj->index = UART3_INDEX;
	    obj->irq = UART3_IRQn;
	    IRQ_SetHandler(obj->irq, UART3_IRQHandler);
	    IRQ_Enable(obj->irq);
  }
  else if(obj->uart == SP_UART4)
  {
  		/* enable clk */
	    HAL_Module_Clock_enable(UART4, 1);
  		HAL_Module_Clock_gate(UART4, 1);
  		HAL_Module_Reset(UART4, 0);
  		/* pinmux set */
  		HAL_PINMUX_Cfg(PINMUX_UA4_TX,uart_tx);
  		HAL_PINMUX_Cfg(PINMUX_UA4_RX,uart_rx);
  		/*interrupt set */
	    obj->index = UART4_INDEX;
	    obj->irq = UART4_IRQn;
	    IRQ_SetHandler(obj->irq, UART4_IRQHandler);
	    IRQ_Enable(obj->irq);
	}
	else
  {
		return;
  }

  /* Configure uart */
  uart_handlers[obj->index] = huart;
  huart->Instance          = (UART_CTRL_Type *)(obj->uart);
  huart->Init.BaudRate     = baudrate;

  huart->Init.LcrInit.LCR_Init = UART_INIT_USE_DEFAULT_VALUE;
  huart->Init.McrInit.MCR_Init = UART_INIT_USE_DEFAULT_VALUE;
  huart->txdma = NULL;
  huart->txgdma = NULL;
  huart->rxdma = NULL;

  if (HAL_UART_Init(huart) != HAL_OK) 
  {
    return;
  }

}

/**
  * @brief  Function called to deinitialize the uart interface
  * @param  obj : pointer to serial_t structure
  * @retval None
  */
void uart_deinit(serial_t *obj)
{
  /* Reset UART and disable clock */
  switch (obj->index) {
  	case UART0_INDEX:
	    HAL_Module_Clock_enable(UART0, 0);
  		HAL_Module_Clock_gate(UART0, 0);
      break;  	
    case UART1_INDEX:
	    HAL_Module_Clock_enable(UART1, 0);
  		HAL_Module_Clock_gate(UART1, 0);
      break;

    case UART2_INDEX:
	    HAL_Module_Clock_enable(UART2, 0);
  		HAL_Module_Clock_gate(UART2, 0);
      break;

    case UART3_INDEX:
	    HAL_Module_Clock_enable(UART3, 0);
  		HAL_Module_Clock_gate(UART3, 0);
      break;

    case UART4_INDEX:
	    HAL_Module_Clock_enable(UART4, 0);
  		HAL_Module_Clock_gate(UART4, 0);
      break;
  }

  HAL_UART_DeInit(uart_handlers[obj->index]);
  /* Release uart debug to ensure init */
  if (serial_debug.index == obj->index) 
  {
    serial_debug.index = UART_NUM;
  }

}

/**
  * @brief  write the data on the uart
  * @param  obj : pointer to serial_t structure
  * @param  data : byte to write
  * @param  size : number of data to write
  * @retval The number of bytes written
  */
size_t uart_write(serial_t *obj, uint8_t data, uint16_t size)
{
  if (HAL_UART_Transmit(uart_handlers[obj->index], &data, size, TX_TIMEOUT) == HAL_OK) {
    return size;
  } else {
    return 0;
  }
}

/**
 * Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
  return ((HAL_UART_GetState(uart_handlers[obj->index]) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX);
}

/**
 * Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
  return ((HAL_UART_GetState(uart_handlers[obj->index]) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX);
}

/**
  * @brief  Read receive byte from uart
  * @param  obj : pointer to serial_t structure
  * @retval last character received
  */
int uart_getc(serial_t *obj, unsigned char *c)
{
  if (obj == NULL) {
    return -1;
  }

  if (serial_rx_active(obj)) {
    return -1; /* Transaction ongoing */
  }

  *c = (unsigned char)(obj->recv);
  /* Restart RX irq */
  HAL_UART_Receive_IT(uart_handlers[obj->index], &(obj->recv), 1);

  return 0;
}

int uart_get_tx_fifo_space(serial_t *obj)
{
  if (obj == NULL) {
    return 0;
  }

  if (serial_rx_active(obj)) {
    return 0; /* Transaction ongoing */
  }
  return (HAL_UART_Get_TX_FIFO_Space(uart_handlers[obj->index]));

}


/**************  uart debug *****************/
void uart_debug_init(void)
{
   uart_init(&serial_debug, DEBUG_UART_BAUDRATE);
}

size_t uart_debug_write(uint8_t *data, uint32_t size)
{
  uint32_t tickstart = HAL_GetTick();

  if (serial_debug.index >= UART_NUM) 
  {
      /* DEBUG_UART not initialized */
      uart_debug_init();
      if (serial_debug.index >= UART_NUM) 
      {
        return 0;
      }
  } 
  else 
  {
      serial_t *obj = get_serial_obj(uart_handlers[serial_debug.index]);
      if (obj) 
      {
        serial_debug.irq = obj->irq;
      }
  }

  IRQ_Disable(serial_debug.irq);
  while (HAL_UART_Transmit(uart_handlers[serial_debug.index], data, size, TX_TIMEOUT) != HAL_OK) {
    if ((HAL_GetTick() - tickstart) >=  TX_TIMEOUT) {
      size = 0;
      break;
    }
  }
  IRQ_Enable(serial_debug.irq);

  return size;
}


/**
 * Begin asynchronous RX transfer (enable interrupt for data collecting)
 *
 * @param obj : pointer to serial_t structure
 * @param callback : function call at the end of reception
 * @retval none
 */
void uart_attach_rx_callback(serial_t *obj, void (*callback)(serial_t *))
{
  if (obj == NULL) {
    return;
  }

  /* Exit if a reception is already on-going */
  if (serial_rx_active(obj)) {
    return;
  }
  obj->rx_callback = callback;

  /* Must disable interrupt to prevent handle lock contention */
  IRQ_Disable(obj->irq);

  HAL_UART_Receive_IT(uart_handlers[obj->index], &(obj->recv), 1);

  /* Enable interrupt */
  IRQ_Enable(obj->irq);
}

/**
 * Begin asynchronous TX transfer.
 *
 * @param obj : pointer to serial_t structure
 * @param callback : function call at the end of transmission
 * @retval none
 */
void uart_attach_tx_callback(serial_t *obj, int (*callback)(serial_t *))
{
  if (obj == NULL) {
    return;
  }
  obj->tx_callback = callback;

  /* Must disable interrupt to prevent handle lock contention */
  IRQ_Disable(obj->irq);

  /* The following function will enable UART_IT_TXE and error interrupts */
  HAL_UART_Transmit_IT(uart_handlers[obj->index], &obj->tx_buff[obj->tx_tail], obj->tx_cnt);

  /* Enable interrupt */
  IRQ_Enable(obj->irq);
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  serial_t *obj = get_serial_obj(huart);
  if (obj) {
    obj->rx_callback(obj);
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

  serial_t *obj = get_serial_obj(huart);
  
  if (obj && obj->tx_callback(obj) != -1) {
    if (HAL_UART_Transmit_IT(huart, &obj->tx_buff[obj->tx_tail], 1) != HAL_OK) 
	{
      return;
    }
  }
}

/**
  * @brief  error callback from UART
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	serial_t *obj = get_serial_obj(huart);
	if (obj && !serial_rx_active(obj)) {
	  HAL_UART_Receive_IT(huart, &(obj->recv), 1);
	}
}

#if defined(UART0_BASE)
void UART0_IRQHandler(void)
{
  HAL_UART_IRQHandler(uart_handlers[UART0_INDEX]);
}
#endif

#if defined(UART1_BASE)
void UART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(uart_handlers[UART1_INDEX]);
}
#endif

#if defined(UART2_BASE)
void UART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(uart_handlers[UART2_INDEX]);
}
#endif

#if defined(UART3_BASE)
void UART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(uart_handlers[UART3_INDEX]);
}
#endif

#if defined(UART4_BASE)
void UART4_IRQHandler(void)
{
  HAL_UART_IRQHandler(uart_handlers[UART4_INDEX]);
}
#endif

#if 0
#if defined(UART_RXDMA0_BASE)
void UART_DMA0_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(UART_DMA0_IRQn);
  HAL_UART_Rxdma_IRQ_Handler(uart_handlers[UART_DMA0_IRQn]);
}
#endif

#if defined(UART_RXDMA1_BASE)
void UART_DMA1_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(UART_DMA1_IRQn);
  HAL_UART_Rxdma_IRQ_Handler(uart_handlers[UART_DMA1_IRQn]);
}
#endif
#endif

#ifdef __cplusplus
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
