#include "utility.h"
#include <stdarg.h>


#ifdef _DEBUG
char dbg[512];
#endif


#if 0

void printf(const __FlashStringHelper *fmt, ...) {
  char buffer[256]{};
  va_list args;
  va_start(args, fmt);

#if (PLATFORM_ARCH == PLATFORM_ARCHITECTURE_AVR) ||                            \
  (PLATFORM_ARCH == PLATFORM_ARCHITECTURE_ESP8266)
  vsnprintf_P(buffer, sizeof(buffer), (const char *)fmt, args);
#else
  vsnprintf(buffer, sizeof(buffer), (const char *)fmt, args);
#endif

  va_end(args);

#if PLATFORM_ARCH == PLATFORM_ARCHITECTURE_SAMD21
  SerialUSB.print(buffer);
#else
  Serial.print(buffer);
#endif
}

#endif



IPAddress fetchRemoteIp(const NetClient &client) {
#if (PLATFORM_ARCH == PLATFORM_ARCHITECTURE_ESP8266) &&                        \
  (NETWORK_CONTROLLER == ETHERNET_CONTROLLER_W5X00)
  // EthernetClient class in ESP8266 doesn't implement remoteIP()
  return IPAddress();
#else
  // EthernetClient class is not "const correct"
  return const_cast<NetClient &>(client).remoteIP();
#endif
}
