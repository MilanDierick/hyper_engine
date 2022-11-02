// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_BASE_H
#define PLAYGROUND_BASE_H

#include "hyper/core/config.h"

#ifdef HP_DEBUG
#if defined(HP_PLATFORM_WINDOWS)
		#define HP_DEBUGBREAK() __debugbreak()
	#elif defined(HP_PLATFORM_LINUX)
		#include <signal.h>
		#define HP_DEBUGBREAK() raise(SIGTRAP)
	#elif defined(HP_PLATFORM_MACOS)
	#define HP_DEBUGBREAK() __builtin_trap() // No idea if this works, unable to test due to missing hardware
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	//#define HP_ENABLE_ASSERTS
#else
#define HP_DEBUGBREAK()
#endif

#define HP_EXPAND_MACRO(x) x
#define HP_STRINGIFY_MACRO(x) #x

#endif //PLAYGROUND_BASE_H
