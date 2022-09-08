// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_CONFIG_H
#define HYPER_CONFIG_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
#define HP_PLATFORM_WINDOWS
#else
#define HP_PLATFORM_WINDOWS
#endif
#elif __APPLE__
#include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS, tvOS, or watchOS Simulator
    #elif TARGET_OS_MACCATALYST
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
        // Other kinds of Apple platforms
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __ANDROID__
#define HP_PLATFORM_ANDROID
#elif __linux__
#define HP_PLATFORM_LINUX
#elif __unix__ // all unices not caught above
#define HP_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
#define HP_PLATFORM_POSIX
#else
#   error "Unknown compiler"
#endif

#ifdef HP_PLATFORM_WINDOWS
#ifdef DLL_EXPORT
#define HP_API __declspec(dllexport)
#else
#define HP_API __declspec(dllimport)
#endif
#else
#define HP_API
#endif

#define UNUSED(x) static_cast<void>(x)

#endif //HYPER_CONFIG_H
