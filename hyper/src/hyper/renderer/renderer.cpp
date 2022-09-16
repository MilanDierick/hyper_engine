#include "hyper/renderer/renderer.h"

#include "hyper/renderer/Renderer2D.h"

namespace hp
{
	std::unique_ptr<renderer::SceneData> renderer::s_SceneData = std::make_unique<renderer::SceneData>();
	
	void renderer::Init()
	{
		render_command::init();
		Renderer2D::Init();
	}
	
	void renderer::Shutdown()
	{
		Renderer2D::Shutdown();
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
	
	void renderer::Submit(const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<vertex_array>& vertexArray,
		const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);
		
		vertexArray->Bind();
		render_command::draw_indexed(vertexArray);
	}
	
}  // namespace hp
