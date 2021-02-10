#pragma once

#include "platform.h"

#define SAFE_DELETE(ptr)                                                       \
  {                                                                            \
    if (ptr != nullptr) {                                                      \
      delete ptr;                                                              \
      ptr = nullptr;                                                           \
    }                                                                          \
  }

#define SAFE_DELETE_ARRAY(ptr)                                                 \
  {                                                                            \
    if (ptr != nullptr) {                                                      \
      delete[] ptr;                                                            \
      ptr = nullptr;                                                           \
    }                                                                          \
  }


//void printf(const __FlashStringHelper *fmt, ...);

#ifdef _DEBUG
extern char dbg[];
# define printf(...) { sprintf(dbg, __VA_ARGS__); Serial.print(dbg); }
#else
# define printf(...)
#endif

# define __debugOutput printf
# define PRINTF printf


IPAddress fetchRemoteIp(const NetClient &client);

