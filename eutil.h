// Copyright (c) 2014 ASMlover. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __EUTIL_HEADER_H__
#define __EUTIL_HEADER_H__

#if !defined(EUTIL_WIN) || !defined(EUTIL_LINUX) || !defined(EUTIL_MAC)
# undef EUTIL_WIN
# undef EUTIL_LINUX
# undef EUTIL_MAC

# if defined(_WINDOWS_) || defined(_MSC_VER) || defined(__MINGW32)
#   define EUTIL_WIN
# elif defined(WIN32_LEAN_AND_MEAN)
#   define EUTIL_WIN
# elif defined(__linux__)
#   define EUTIL_LINUX
# elif defined(__APPLE__) || defined(__MACH__)
#   define EUTIL_MAC
# else
#   error "DOES NOT SUPPORT THIS PLATFORM !!!"
# endif
#endif

#if defined(EUTIL_WIN) && defined(_MSC_VER)
# define COMPILER_MSVC
#elif defined(EUTIL_LINUX) && defined(__GNUC__)
# define COMPILER_GCC
#elif defined(EUTIL_MAC) && defined(__clang__)
# define COMPILER_CLANG
#endif

#if defined(COMPILER_GCC)
# define GCC_VER  (__GNUC__ * 100 + __GNUC_MINOR__ * 10)
#elif defined(COMPILER_CLANG)
# define GCC_VER  (__clang_major__ * 100 + __clang_minor__ * 10)
#endif

#if (defined(COMPILER_MSVC) && (_MSC_VER < 1700))\
  || (defined(COMPILER_GCC) && (GCC_VER < 470))\
  || (defined(COMPILER_CLANG) && (GCC_VER < 330))
# error "PLEASE USE A HIGHER VERSION OF THE COMPILER !!!"
#endif

// SYSTEM INTERFACES HEADERS
#if defined(EUTIL_WIN)
# include <windows.h>
# include <direct.h>
# include <mmsystem.h>
# include <process.h>
# include <io.h>

# undef __func__
# define __func__ __FUNCSIG__
#else
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <unistd.h>

# define MAX_PATH PATH_MAX
# if defined(EUTIL_MAC)
#   include <libkern/OSAtomic.h>
#   include <mach/mach.h>
#   include <mach/mach_time.h>
# endif
#endif

// ANSI C HEADERS
#include <assert.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ANSI C++ HEADERS
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

// STL HEADERS
#include <algorithm>
#include <map>
#include <queue>
#include <unordered_map>
#include <vector>

// Have our own assert, so we are sure it does not get
// optomized away in a release build.
# include "el_io.h"
# define EL_ASSERT(condition) do {\
  if (!(condition)) {\
    el::ColorFprintf(stderr, \
        el::ColorType::COLORTYPE_RED, \
        "[%s:%d] Assertion failed in %s(): %s\n", \
        __FILE__, \
        __LINE__, \
        __func__, \
        #condition);\
    fflush(stderr);\
    abort();\
  }\
} while (0)

#define EL_ASSERTX(condition, message) do {\
  if (!(condition)) {\
    el::ColorFprintf(stderr, \
        el::ColorType::COLORTYPE_RED, \
        "[%s:%d] Assertion failed in %s(): %s\n", \
        __FILE__, \
        __LINE__, \
        __func__, \
        (message));\
    fflush(stderr);\
    abort();\
  }\
} while (0)

// Assertion to indicate that the given point in
// the code should never be reached.
#define EL_UNREACHABLE()\
  EL_ASSERTX(false, "This line should not be reached.")

#if !defined(MIN)
# define MIN(x, y)  ((x) < (y) ? (x) : (y))
#endif

#if !defined(MAX)
# define MAX(x, y)  ((x) > (y) ? (x) : (y))
#endif

#if defined(EUTIL_WIN)
# ifndef snprintf
#   define snprintf _snprintf
# endif

# define EL_ARRAY(type, name, size)\
    type* name = (type*)_alloca((size) * sizeof(type))
#else
# define EL_ARRAY(type, name, size) type name[size]
#endif

#include "el_uncopyable.h"
#include "el_locker.h"
#include "el_condition.h"
#include "el_singleton.h"
#include "el_object_pool.h"
#include "el_object_mgr.h"
#include "el_thread.h"
#include "el_thread_pool.h"

#endif  // __EUTIL_HEADER_H__
