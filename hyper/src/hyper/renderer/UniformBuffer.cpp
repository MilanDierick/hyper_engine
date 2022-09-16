#include "UniformBuffer.h"

#include "hyper/renderer/renderer.h"
#include "platform/opengl/OpenGLUniformBuffer.h"

namespace hp
{
	std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_shared<OpenGLUniformBuffer>(size, binding);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
}  // namespace hp
