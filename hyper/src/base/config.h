// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_HYPER_SRC_BASE_CONFIG_H_
#define PLAYGROUND_HYPER_SRC_BASE_CONFIG_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
#define HP_WINDOWS
#else
#define HP_WINDOWS
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
#define HP_ANDROID
#elif __linux__
#define HP_LINUX
#elif __unix__ // all unices not caught above
#define HP_UNIX
#elif defined(_POSIX_VERSION)
#define HP_POSIX
#else
#   error "Unknown compiler"
#endif

#ifdef HP_WINDOWS
#ifdef DLL_EXPORT
#define HP_API __declspec(dllexport)
#else
#define HP_API __declspec(dllimport)
#endif
#else
#define HP_API
#endif

#endif //PLAYGROUND_HYPER_SRC_BASE_CONFIG_H_
