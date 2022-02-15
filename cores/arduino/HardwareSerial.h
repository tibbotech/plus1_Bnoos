
#ifndef __HARDWARESERIAL_H_
#define __HARDWARESERIAL_H_

#include <inttypes.h>
#include "Stream.h"
#include "uart.h"
#include "WSerial.h"

#define SERIAL_TX_BUFFER_SIZE 128
#define SERIAL_RX_BUFFER_SIZE 128

typedef uint8_t tx_buffer_index_t;
typedef uint8_t rx_buffer_index_t;

typedef enum {
  HALF_DUPLEX_DISABLED,
  HALF_DUPLEX_ENABLED
} HalfDuplexMode_t;


class HardwareSerial : public Stream {
  protected:
    // Has any byte been written to the UART since begin()
    bool _written;

    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd
    // instruction.
    unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
    unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

    serial_t _serial;

  public:
    HardwareSerial(uint32_t _rx, uint32_t _tx);
    HardwareSerial(void);
    HardwareSerial(void* peripheral);
    void begin(unsigned long baud);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
      return write((uint8_t)n);
    }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool()
    {
      return true;
    }
    static void _rx_complete_irq(serial_t *obj);
    static int _tx_complete_irq(serial_t *obj);
  private:
    bool _rx_enabled;
    uint8_t _config;
    unsigned long _baud;
    void init(uint32_t _rx, uint32_t _tx);

};

#if defined(HAVE_HWSERIAL1)
  extern HardwareSerial Serial1;
#endif
#if defined(HAVE_HWSERIAL2)
  extern HardwareSerial Serial2;
#endif
#if defined(HAVE_HWSERIAL3)
  extern HardwareSerial Serial3;
#endif
#if defined(HAVE_HWSERIAL4)
  extern HardwareSerial Serial4;
#endif

#endif

