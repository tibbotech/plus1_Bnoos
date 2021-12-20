#include "digital_io.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(uint32_t ulPin, uint32_t ulMode)
{
  digitalModeSet(ulPin,ulMode);
}

int getpinMode(uint32_t ulPin)
{
  return digitalModeGet(ulPin);
}

void digitalWrite(uint32_t ulPin, uint32_t ulVal)
{
  digitalWriteFast(ulPin, ulVal);
}

int digitalRead(uint32_t ulPin)
{
  return digitalReadFast(ulPin);
}

void digitalToggle(uint32_t ulPin)
{
  digitalToggleFast(ulPin);
}

#ifdef __cplusplus
}
#endif
