#ifndef __VIRT_UART_H
#define __VIRT_UART_H

#include "virtio_config.h"
#include "virtio_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

int virt_uart_open(void);
void virt_uart_close(void);
void virt_uart_poll(void);
int virt_uart_read(void *buf, int len);
int virt_uart_write(void *buf, int len);
int virt_uart_available(void);
int virt_uart_peek(void);

#ifdef __cplusplus
}
#endif

#endif /* __VIRT_UART_H */
