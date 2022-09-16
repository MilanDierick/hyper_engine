#ifndef HYPER_RENDERER_API_H
#define HYPER_RENDERER_API_H

#include "hyper/renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace hp
{
	class HP_API renderer_api
	{
	 public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	 public:
		virtual ~renderer_api() = default;
		
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		
		virtual void SetLineWidth(float width) = 0;
		
		static API GetAPI()
		{
			return s_API;
		}
		
		static std::unique_ptr<renderer_api> Create();
	 private:
		static API s_API;
	};
	
}  // namespace hp

#endif