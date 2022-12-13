// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "vulkan_renderer_api.h"

namespace hp
{
	void vulkan_renderer_api::init()
	{
	}

	void vulkan_renderer_api::set_viewport(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height)
	{
		UNUSED(x_pos);
		UNUSED(y_pos);
		UNUSED(width);
		UNUSED(height);
	}

	void vulkan_renderer_api::set_clear_color(const glm::vec4& color)
	{
		UNUSED(color);
	}

	void vulkan_renderer_api::clear()
	{
	}

	void vulkan_renderer_api::draw_indexed(const std::shared_ptr<vertex_array>& vertex_array, uint32_t index_count)
	{
		UNUSED(vertex_array);
		UNUSED(index_count);
	}

	void vulkan_renderer_api::draw_lines(const std::shared_ptr<vertex_array>& vertex_array, uint32_t vertex_count)
	{
		UNUSED(vertex_array);
		UNUSED(vertex_count);
	}

	void vulkan_renderer_api::set_line_width(float width)
	{
		UNUSED(width);
	}
} // namespace hp
