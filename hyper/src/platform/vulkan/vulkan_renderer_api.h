// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_VULKAN_RENDERER_API_H
#define PLAYGROUND_VULKAN_RENDERER_API_H

#include "hyper/core/config.h"
#include "hyper/renderer/renderer_api.h"

namespace hp
{
	class HP_API vulkan_renderer_api : public renderer_api
	{
	public:
		vulkan_renderer_api()           = default;
		~vulkan_renderer_api() override = default;

		vulkan_renderer_api(const vulkan_renderer_api& other)            = delete;
		vulkan_renderer_api(vulkan_renderer_api&& other) noexcept        = delete;
		vulkan_renderer_api& operator=(const vulkan_renderer_api& other) = delete;
		vulkan_renderer_api& operator=(vulkan_renderer_api&& other)      = delete;

		void init() override;
		void set_viewport(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height) override;
		void set_clear_color(const glm::vec4& color) override;
		void clear() override;

		void draw_indexed(const std::shared_ptr<vertex_array>& vertex_array, uint32_t index_count) override;
		void draw_lines(const std::shared_ptr<vertex_array>& vertex_array, uint32_t vertex_count) override;

		void set_line_width(float width) override;

		// 275 * 275 * 256 bytes =
	};
} // namespace hp

#endif //PLAYGROUND_VULKAN_RENDERER_API_H
