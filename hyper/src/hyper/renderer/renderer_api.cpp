#include "hyper/renderer/renderer_api.h"

#include "platform/opengl/OpenGLRendererAPI.h"

namespace hp
{
	renderer_api::API renderer_api::s_API = renderer_api::API::OpenGL;
	
	std::unique_ptr<renderer_api> renderer_api::Create()
	{
		switch (s_API)
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_unique<OpenGLRendererAPI>();
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
}  // namespace hp