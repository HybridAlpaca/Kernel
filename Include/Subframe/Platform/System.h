#pragma once

#define SUBFRAME_PLATFORM_SYSTEM_WINDOWS 0
#define SUBFRAME_PLATFORM_SYSTEM_OSX     1
#define SUBFRAME_PLATFORM_SYSTEM_IOS     2
#define SUBFRAME_PLATFORM_SYSTEM_LINUX   3
#define SUBFRAME_PLATFORM_SYSTEM_ANDROID 4

#if (defined(_WIN32) || defined(_WIN64))
	#define SUBFRAME_PLATFORM_SYSTEM SUBFRAME_PLATFORM_SYSTEM_WINDOWS
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
	#if defined(TARGET_OS_IPHONE)
		#define SUBFRAME_PLATFORM_SYSTEM SUBFRAME_PLATFORM_SYSTEM_IOS
	#else
		#define SUBFRAME_PLATFORM_SYSTEM SUBFRAME_PLATFORM_SYSTEM_OSX
	#endif
#elif defined(__linux__)
	#if defined(ANDROID)
		#define SUBFRAME_PLATFORM_SYSTEM SUBFRAME_PLATFORM_SYSTEM_ANDROID
	#else
		#define SUBFRAME_PLATFORM_SYSTEM SUBFRAME_PLATFORM_SYSTEM_LINUX
	#endif
#else
	#error ("ERR :: Building subframe for unrecognized or unsupported operating system")
#endif
