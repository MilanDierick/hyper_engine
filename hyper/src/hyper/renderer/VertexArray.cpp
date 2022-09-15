#include "hyper/renderer/VertexArray.h"

#include "hyper/renderer/Renderer.h"
#include "platform/opengl/OpenGLVertexArray.h"

namespace hp
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		HP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}
		
		HP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}