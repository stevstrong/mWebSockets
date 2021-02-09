#pragma once

/** @file */

#include <stdint.h>

//
// Board architectures:
//

/** @cond */
#define PLATFORM_ARCHITECTURE_AVR		1
#define PLATFORM_ARCHITECTURE_ESP8266	2
#define PLATFORM_ARCHITECTURE_ESP32		3
#define PLATFORM_ARCHITECTURE_SAMD21	4
#define PLATFORM_ARCHITECTURE_STM32		5
/** @endcond */

#if defined(__AVR__)
#  define PLATFORM_ARCH PLATFORM_ARCHITECTURE_AVR
#elif defined(ESP8266)
#  define PLATFORM_ARCH PLATFORM_ARCHITECTURE_ESP8266
#elif defined(ESP32)
#  define PLATFORM_ARCH PLATFORM_ARCHITECTURE_ESP32
#elif defined(ARDUINO_ARCH_SAMD)
#  define PLATFORM_ARCH PLATFORM_ARCHITECTURE_SAMD21
#elif defined(ARDUINO_ARCH_STM32) || defined (ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32F1)
#  define PLATFORM_ARCH PLATFORM_ARCHITECTURE_STM32
#else
#  error "Unsupported platform"
#endif

//
// Ethernet/WiFi devices:
//

/** @cond */
#define ETHERNET_CONTROLLER_W5X00		1
#define ETHERNET_CONTROLLER_ENC28J60	2
#define NETWORK_CONTROLLER_WIFI			3
/** @endcond */

#include "config.h"

#if NETWORK_CONTROLLER == ETHERNET_CONTROLLER_W5X00
#  include <Ethernet.h>
/** Specifies maximum number of clients connected to server. */
constexpr uint8_t kMaxConnections{ MAX_SOCK_NUM };
#elif NETWORK_CONTROLLER == ETHERNET_CONTROLLER_ENC28J60
#  include <IPAddress.h>
#  include <UIPEthernet.h>
constexpr uint8_t kMaxConnections{ 4 };
#elif NETWORK_CONTROLLER == NETWORK_CONTROLLER_WIFI
#  if PLATFORM_ARCH == PLATFORM_ARCHITECTURE_ESP8266
#    include <ESP8266WiFi.h>
#  else
#    include <WiFi.h>
#  endif
#  include <WiFiClient.h>
#  include <WiFiServer.h>
constexpr uint8_t kMaxConnections{ 8 };
#else
#  error "Network controller is required!"
#endif

/** @cond */
#if NETWORK_CONTROLLER == NETWORK_CONTROLLER_WIFI
using NetClient = WiFiClient;
using NetServer = WiFiServer;
#else
using NetClient = EthernetClient;
using NetServer = EthernetServer;
#endif
/** @endcond */

/**
 * @def PLATFORM_ARCH
 * @def NETWORK_CONTROLLER
 */
