#pragma once

#define SUBFRAME_PLATFORM_ARCH_ARM    0
#define SUBFRAME_PLATFORM_ARCH_ARM64  1
#define SUBFRAME_PLATFORM_ARCH_x86    2
#define SUBFRAME_PLATFORM_ARCH_x86_64 3

#if defined(__arm__)
	#define SUBFRAME_PLATFORM_ARCH SUBFRAME_PLATFORM_ARCH_ARM
#elif defined(__aarch64__)
	#define SUBFRAME_PLATFORM_ARCH SUBFRAME_PLATFORM_ARCH_ARM_64
#elif defined(__i386__)
	#define SUBFRAME_PLATFORM_ARCH SUBFRAME_PLATFORM_ARCH_x86
#elif defined(__x86_64__)
	#define SUBFRAME_PLATFORM_ARCH SUBFRAME_PLATFORM_ARCH_x86_64
#else
	#error ("ERR :: Building subframe for unrecognized or unsupported system architecture")
#endif
