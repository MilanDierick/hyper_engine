#ifndef HYPER_RENDER_COMMAND_H
#define HYPER_RENDER_COMMAND_H

#include "hyper/renderer/RendererAPI.h"

namespace hp
{
	class HP_API RenderCommand
	{
	 public:
		static void Init()
		{
			s_RendererAPI->Init();
		}
		
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		
		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		
		static void Clear()
		{
			s_RendererAPI->Clear();
		}
		
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}
		
		static void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}
		
		static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}
	 
	 private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};
	
}  // namespace hp

#endif