#ifndef HYPER_RENDER_COMMAND_H
#define HYPER_RENDER_COMMAND_H

#include "hyper/renderer/renderer_api.h"

namespace hp
{
	class HP_API render_command
	{
	public:
		static void init();

		static void set_viewport(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height);
		static void set_clear_color(const glm::vec4& color);
		static void clear();
		static void draw_indexed(const std::shared_ptr<vertex_array>& vertex_array, uint32_t index_count = 0);
		static void draw_lines(const std::shared_ptr<vertex_array>& vertex_array, uint32_t vertex_count);
		static void set_line_width(float width);

	private:
		static std::unique_ptr<renderer_api> s_renderer_api;
	};
} // namespace hp

#endif //HYPER_RENDER_COMMAND_H