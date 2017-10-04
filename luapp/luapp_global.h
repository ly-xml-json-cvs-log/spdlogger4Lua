#ifndef LUAPP_GLOBAL_H
#define LUAPP_GLOBAL_H

#include <cassert>
#include <iostream>
#include <sstream>
#include <tuple>
#include <memory>
#include <functional>
#include <lua.hpp>

#include "g_config.h"

#if defined(LUAPP_LIBRARY)
#  define LUAPPSHARED_C_EXPORT __attribute__ ((visibility("default"))) extern "C"
#  define LUAPPSHARED_CPP_EXPORT __attribute__ ((visibility("default")))
#else
#  define LUAPPSHARED_C_EXPORT  extern "C"
#  define LUAPPSHARED_CPP_EXPORT
#endif

#endif // LUAPP_GLOBAL_H
