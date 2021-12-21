#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"

#if defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3) || defined(HAVE_HWSERIAL4) 
  #if defined(HAVE_HWSERIAL1)
    HardwareSerial Serial1(SP_UART1);
    void serialEvent1() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL2)
    HardwareSerial Serial2(SP_UART2);
    void serialEvent2() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL3)
    HardwareSerial Serial3(SP_UART3);
    void serialEvent3() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL4)
    HardwareSerial Serial4(SP_UART4);
    void serialEvent4() __attribute__((weak));
  #endif
#endif 

HardwareSerial::HardwareSerial(void* peripheral)
{
  _serial.uart = (UART_CTRL_Type *)peripheral;
  
    #if defined(PIN_SERIAL_TX)
    init(PIN_SERIAL_RX,PIN_SERIAL_TX);
    #endif
}

HardwareSerial::HardwareSerial(void)
{
    
    #if defined(HAVE_HWSERIAL1)
    _serial.uart = SP_UART1;
    #elif defined(HAVE_HWSERIAL2)
    _serial.uart = SP_UART2;
    #elif defined(HAVE_HWSERIAL3)
    _serial.uart = SP_UART3;
    #elif defined(HAVE_HWSERIAL4)
    _serial.uart = SP_UART4;
    #endif

    #if defined(PIN_SERIAL_TX)
    init(PIN_SERIAL_RX,PIN_SERIAL_TX);
    #endif
 
}
HardwareSerial::HardwareSerial(uint32_t _rx, uint32_t _tx)
{

#ifdef HAVE_HWSERIAL1
	_serial.uart = SP_UART1;
#elif defined(HAVE_HWSERIAL2)  
	_serial.uart = SP_UART2;
#elif defined(HAVE_HWSERIAL3)  
	_serial.uart = SP_UART3;
#elif defined(HAVE_HWSERIAL4)  
	_serial.uart = SP_UART4;  
#endif

  init(_rx,_tx);
}

void HardwareSerial::init(uint32_t _rx, uint32_t _tx)
{
  if (_rx == _tx) {
    _serial.pin_rx = -1;
  } else {
    _serial.pin_rx = _rx;
  }
  _serial.pin_tx = _tx;
  _serial.rx_buff = _rx_buffer;
  _serial.rx_head = 0;
  _serial.rx_tail = 0;
  _serial.tx_buff = _tx_buffer;
  _serial.tx_head = 0;
  _serial.tx_tail = 0;
}

void HardwareSerial::_rx_complete_irq(serial_t *obj)
{
  unsigned char c;

  if (uart_getc(obj, &c) == 0) {
    rx_buffer_index_t i = (unsigned int)(obj->rx_head + 1) % SERIAL_RX_BUFFER_SIZE;
    if (i != obj->rx_tail) {
      obj->rx_buff[obj->rx_head] = c;
      obj->rx_head = i;
    }
  }
}

int HardwareSerial::_tx_complete_irq(serial_t *obj)
{
  obj->tx_tail = (obj->tx_tail + obj->tx_cnt) % SERIAL_TX_BUFFER_SIZE;

  if (obj->tx_head == obj->tx_tail) {
    return -1;
  }

  return 0;
}


void HardwareSerial::begin(unsigned long baud)
{

  _baud = baud;
 
  uart_init(&_serial, (uint32_t)baud);

  uart_attach_rx_callback(&_serial, _rx_complete_irq);
}

void HardwareSerial::end()
{
  
  flush();

  uart_deinit(&_serial);

  _serial.rx_head = _serial.rx_tail;
}

int HardwareSerial::available(void)
{
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _serial.rx_head - _serial.rx_tail)) % SERIAL_RX_BUFFER_SIZE;
}

int HardwareSerial::peek(void)
{
  if (_serial.rx_head == _serial.rx_tail) {
    return -1;
  } else {
    return _serial.rx_buff[_serial.rx_tail];
  }
}

int HardwareSerial::read(void)
{
  if (_serial.rx_head == _serial.rx_tail) {
    return -1;
  } else {
    unsigned char c = _serial.rx_buff[_serial.rx_tail];
    _serial.rx_tail = (rx_buffer_index_t)(_serial.rx_tail + 1) % SERIAL_RX_BUFFER_SIZE;

    return c;
  }
}

int HardwareSerial::availableForWrite(void)
{
  tx_buffer_index_t head = _serial.tx_head;
  tx_buffer_index_t tail = _serial.tx_tail;

  if (head >= tail) {
    return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
  }
  return tail - head - 1;
}

void HardwareSerial::flush()
{
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written) {
    return;
  }

  while ((_serial.tx_head != _serial.tx_tail)) {
    // nop, the interrupt handler will free up space for us
  }
  // If we get here, nothing is queued anymore (DRIE is disabled) and
  // the hardware finished tranmission (TXC is set).
}

size_t HardwareSerial::write(uint8_t c)
{
  _written = true;
  _serial.tx_cnt = 1;
  tx_buffer_index_t i = (_serial.tx_head + 1) % SERIAL_TX_BUFFER_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  while (i == _serial.tx_tail) {
    // nop, the interrupt handler will free up space for us
  }

  _serial.tx_buff[_serial.tx_head] = c;
  _serial.tx_head = i;

  if (!serial_tx_active(&_serial)) {
    uart_attach_tx_callback(&_serial, _tx_complete_irq);
  }
    return 1;
}

