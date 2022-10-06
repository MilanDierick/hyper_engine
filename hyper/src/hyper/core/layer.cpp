// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "layer.h"

#include <utility>

namespace hp
{
	layer::layer(std::string debug_name) : m_debug_name(std::move(debug_name))
	{
	}

	const std::string &layer::get_name() const
	{
		return m_debug_name;
	}
}  // namespace hp