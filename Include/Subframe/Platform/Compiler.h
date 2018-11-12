#pragma once

#define SUBFRAME_PLATFORM_COMPILER_MSVC  0
#define SUBFRAME_PLATFORM_COMPILER_GCC   1
#define SUBFRAME_PLATFORM_COMPILER_CLANG 2

#if defined(_MSC_VER)
  #define SUBFRAME_PLATFORM_COMPILER SUBFRAME_PLATFORM_COMPILER_MSVC
#elif defined(__GNUC__)
  #define SUBFRAME_PLATFORM_COMPILER SUBFRAME_PLATFORM_COMPILER_GCC
#elif defined(__clang__)
  #define SUBFRAME_PLATFORM_COMPILER SUBFRAME_PLATFORM_COMPILER_CLANG
#else
  #error ("ERR :: Building subframe with unrecognized or unsupported compiler toolchain")
#endif
