#include "hyper/renderer/renderer.h"

#include "hyper/renderer/renderer_2d.h"

namespace hp
{
	std::unique_ptr<renderer::scene_data> renderer::s_scene_data = std::make_unique<renderer::scene_data>();

	void renderer::init()
	{
		render_command::init();
		renderer_2d::init();
	}

	void renderer::shutdown()
	{
		renderer_2d::shutdown();
	}

	void renderer::on_window_resize(uint32_t width, uint32_t height)
	{
		render_command::set_viewport(0, 0, width, height);
	}

	void renderer::begin_scene(orthographic_camera& camera)
	{
		s_scene_data->view_projection_matrix = camera.get_view_projection_matrix();
	}

	void renderer::end_scene()
	{
	}

	void renderer::submit(const std::shared_ptr<shader>& shader, const std::shared_ptr<vertex_array>& vertex_array, const glm::mat4& transform)
	{
		shader->bind();
		shader->set_mat_4("u_ViewProjection", s_scene_data->view_projection_matrix);
		shader->set_mat_4("u_Transform", transform);

		vertex_array->bind();
		render_command::draw_indexed(vertex_array);
	}

} // namespace hp
