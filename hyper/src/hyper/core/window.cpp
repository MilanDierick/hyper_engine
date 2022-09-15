// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "hyper/core/window.h"

#include <utility>
#include "platform/universal/universal_window.h"

namespace hp
{
	window_parameters::window_parameters() : title("Hyper Engine"), width(2560), height(1440)
	{
	}
	
	window_parameters::window_parameters(const char* title, const int32_t width, const int32_t height)
		: title(title), width(width), height(height)
	{
	}
} // namespace hp

