#include "hyper/renderer/render_command.h"

namespace hp
{
	std::unique_ptr<renderer_api> render_command::s_renderer_api = renderer_api::Create();

	void render_command::init()
	{
		s_renderer_api->init();
	}

	void render_command::set_viewport(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height)
	{
		s_renderer_api->set_viewport(x_pos, y_pos, width, height);
	}

	void render_command::set_clear_color(const glm::vec4& color)
	{
		s_renderer_api->set_clear_color(color);
	}

	void render_command::clear()
	{
		s_renderer_api->clear();
	}

	void render_command::draw_indexed(const std::shared_ptr<vertex_array>& vertex_array, uint32_t index_count)
	{
		s_renderer_api->draw_indexed(vertex_array, index_count);
	}

	void render_command::draw_lines(const std::shared_ptr<vertex_array>& vertex_array, uint32_t vertex_count)
	{
		s_renderer_api->draw_lines(vertex_array, vertex_count);
	}

	void render_command::set_line_width(float width)
	{
		s_renderer_api->set_line_width(width);
	}
} // namespace hp