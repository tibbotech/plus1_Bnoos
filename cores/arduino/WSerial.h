#ifndef WIRING_SERIAL_H
#define WIRING_SERIAL_H

#include "variant.h"
#include "HardwareSerial.h"
//#include "USBSerial.h"
#ifdef VIRTIOCON
#include "VirtIOSerial.h"
#endif

#if defined (USBCON) && defined(USBD_USE_CDC)
  #ifndef DISABLE_GENERIC_SERIALUSB
    #define ENABLE_SERIALUSB
    #if !defined(Serial)
      #define Serial SerialUSB
      #define serialEvent serialEventUSB
    #endif
  #endif

  #if defined(ENABLE_SERIALUSB)
    #define HAVE_SERIALUSB
  #endif

  extern void serialEventUSB(void) __attribute__((weak));
#endif /* USBCON && USBD_USE_CDC */

#if defined(VIRTIOCON)
  #ifndef DISABLE_GENERIC_SERIALVIRTIO
    #define ENABLE_SERIALVIRTIO
    #if !defined(Serial)
      #define Serial SerialVirtIO
      #define serialEvent serialEventVirtIO
    #endif
  #endif

  #if defined(ENABLE_SERIALVIRTIO)
    #define HAVE_SERIALVIRTIO
  #endif

  extern void serialEventVirtIO(void) __attribute__((weak));
#endif /* VIRTIOCON */


  #if !defined(HWSERIAL_NONE) && defined(SERIAL_UART_INSTANCE)
    #if  SERIAL_UART_INSTANCE == 1
      #define ENABLE_HWSERIAL1
      #if !defined(Serial)
        #define Serial Serial1
        #define serialEvent serialEvent1
      #endif
    #elif SERIAL_UART_INSTANCE == 2
      #define ENABLE_HWSERIAL2
      #if !defined(Serial)
        #define Serial Serial2
        #define serialEvent serialEvent2
      #endif
    #elif SERIAL_UART_INSTANCE == 3
      #define ENABLE_HWSERIAL3
      #if !defined(Serial)
        #define Serial Serial3
        #define serialEvent serialEvent3
      #endif
    #elif SERIAL_UART_INSTANCE == 4
      #define ENABLE_HWSERIAL4
      #if !defined(Serial)
        #define Serial Serial4
        #define serialEvent serialEvent4
      #endif
    #else
      #if !defined(Serial)
        #warning "No generic 'Serial' defined!"
      #endif
    #endif /* SERIAL_UART_INSTANCE == x */
  #endif /* !HWSERIAL_NONE && SERIAL_UART_INSTANCE */

  #if defined(ENABLE_HWSERIAL1)
    #if defined(UART1_BASE)
      #define HAVE_HWSERIAL1
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL2)
    #if defined(UART2_BASE)
      #define HAVE_HWSERIAL2
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL3)
    #if defined(UART3_BASE)
      #define HAVE_HWSERIAL3
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL4)
    #if defined(UART4_BASE)
      #define HAVE_HWSERIAL4
    #endif
  #endif


  #if defined(HAVE_HWSERIAL1)
    extern void serialEvent1(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL2)
    extern void serialEvent2(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL3)
    extern void serialEvent3(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL4)
    extern void serialEvent4(void) __attribute__((weak));
  #endif
  

extern void serialEventRun(void);

#endif /* WIRING_SERIAL_H */
