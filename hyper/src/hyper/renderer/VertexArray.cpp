#include "hyper/renderer/VertexArray.h"

#include "hyper/renderer/renderer.h"
#include "platform/opengl/OpenGLVertexArray.h"

namespace hp
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
}