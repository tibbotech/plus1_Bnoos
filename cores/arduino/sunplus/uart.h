
#ifndef __UART_H_
#define __UART_H_
  
#include "sunplus_def.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define serial_t void*

/* Exported types ------------------------------------------------------------*/
typedef struct serial_s serial_t;

struct serial_s {

  UART_CTRL_Type *uart;
  UART_HandleTypeDef handle;
  void (*rx_callback)(serial_t *);
  int (*tx_callback)(serial_t *);
  uint8_t pin_tx;
  uint8_t pin_rx;
  IRQn_Type irq;
  uint8_t tx_cnt;
  uint8_t index;
  uint8_t recv;
  uint8_t *rx_buff;
  uint8_t *tx_buff;
  uint16_t rx_tail;
  uint16_t tx_head;
  volatile uint16_t rx_head;
  volatile uint16_t tx_tail;
};

/* Exported constants --------------------------------------------------------*/
#define TX_TIMEOUT  1000

void uart_init(serial_t *obj, uint32_t baudrate);

void uart_deinit(serial_t *obj);

void uart_config_lowpower(serial_t *obj);

size_t uart_write(serial_t *obj, uint8_t data, uint16_t size);

int uart_getc(serial_t *obj, unsigned char *c);

void uart_attach_rx_callback(serial_t *obj, void (*callback)(serial_t *));

void uart_attach_tx_callback(serial_t *obj, int (*callback)(serial_t *));

uint8_t serial_tx_active(serial_t *obj);

uint8_t serial_rx_active(serial_t *obj);

int uart_get_tx_fifo_space(serial_t *obj);

//size_t uart_debug_write(uint8_t *data, uint32_t size);
extern size_t uart_debug_write(uint8_t *data, uint32_t size);

void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void UART3_IRQHandler(void);
void UART4_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H */


