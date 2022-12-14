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
		hp_unused(x_pos);
		hp_unused(y_pos);
		hp_unused(width);
		hp_unused(height);
	}

	void vulkan_renderer_api::set_clear_color(const glm::vec4& color)
	{
		hp_unused(color);
	}

	void vulkan_renderer_api::clear()
	{
	}

	void vulkan_renderer_api::draw_indexed(const std::shared_ptr<vertex_array>& vertex_array, uint32_t index_count)
	{
		hp_unused(vertex_array);
		hp_unused(index_count);
	}

	void vulkan_renderer_api::draw_lines(const std::shared_ptr<vertex_array>& vertex_array, uint32_t vertex_count)
	{
		hp_unused(vertex_array);
		hp_unused(vertex_count);
	}

	void vulkan_renderer_api::set_line_width(float width)
	{
		hp_unused(width);
	}
} // namespace hp
