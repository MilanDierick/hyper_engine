#include "hyper/renderer/renderer.h"

#include "hyper/renderer/renderer_2d.h"

namespace hp
{
	std::unique_ptr<renderer::SceneData> renderer::s_SceneData = std::make_unique<renderer::SceneData>();
	
	void renderer::Init()
	{
		render_command::init();
		renderer_2d::init();
	}
	
	void renderer::Shutdown()
	{
		renderer_2d::shutdown();
	}
	
	void renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		render_command::set_viewport(0, 0, width, height);
	}
	
	void renderer::BeginScene(orthographic_camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.get_view_projection_matrix();
	}
	
	void renderer::EndScene()
	{
	}
	
	void renderer::Submit(const std::shared_ptr<shader>& shader,
		const std::shared_ptr<vertex_array>& vertexArray,
		const glm::mat4& transform)
	{
		shader->bind();
		shader->set_mat_4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->set_mat_4("u_Transform", transform);
		
		vertexArray->bind();
		render_command::draw_indexed(vertexArray);
	}
	
}  // namespace hp
