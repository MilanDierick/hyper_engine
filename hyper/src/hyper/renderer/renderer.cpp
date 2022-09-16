#include "hyper/renderer/renderer.h"

#include "hyper/renderer/Renderer2D.h"

namespace hp
{
	std::unique_ptr<renderer::SceneData> renderer::s_SceneData = std::make_unique<renderer::SceneData>();
	
	void renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}
	
	void renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}
	
	void renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
	
	void renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void renderer::EndScene()
	{
	}
	
	void renderer::Submit(const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);
		
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	
}  // namespace hp
