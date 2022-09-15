#include "hyper/renderer/RendererAPI.h"

#include "platform/opengl/OpenGLRendererAPI.h"

namespace hp
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
	
	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:
		HP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLRendererAPI>();
		}
		
		HP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}  // namespace hp