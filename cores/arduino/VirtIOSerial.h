#ifndef _VIRTIOSERIAL_H_
#define _VIRTIOSERIAL_H_


#if defined (VIRTIOCON)


#include "Stream.h"

class VirtIOSerial : public Stream {
  public:
    void begin(void);
    void begin(uint32_t);
    void begin(uint32_t, uint8_t);
    void end(void);

    virtual int available(void);
    virtual int availableForWrite(void);
    virtual int peek(void);
    virtual int read(void);
    virtual size_t readBytes(char *buffer, size_t length);  // read chars from stream into buffer
    virtual String readString(void);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t length);

    using Print::write; // pull in write(str) from Print
    operator bool(void)
    {
      return true;
    }
};

extern VirtIOSerial SerialVirtIO;


#endif

#endif /* _VIRTIOSERIAL_H_ */

