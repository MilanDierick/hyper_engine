// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_ASSERT_H
#define PLAYGROUND_ASSERT_H

#include "core/base.h"
#include <filesystem>

#ifdef HP_ENABLE_ASSERTS
#define HP_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { hp::log::critical(msg, __VA_ARGS__); HP_DEBUGBREAK(); } }
#define HP_INTERNAL_ASSERT_WITH_MSG(type, check, ...) HP_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define HP_INTERNAL_ASSERT_NO_MSG(type, check) HP_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", HP_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define HP_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define HP_INTERNAL_ASSERT_GET_MACRO(...) HP_EXPAND_MACRO( HP_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HP_INTERNAL_ASSERT_WITH_MSG, HP_INTERNAL_ASSERT_NO_MSG) )

#define HP_ASSERT(...) HP_EXPAND_MACRO( HP_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define HP_CORE_ASSERT(...) HP_EXPAND_MACRO( HP_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define HP_ASSERT(...)
#define HP_CORE_ASSERT(...)
#endif

#endif //PLAYGROUND_ASSERT_H
