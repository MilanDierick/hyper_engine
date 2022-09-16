#include "hyper/renderer/graphics_context.h"

#include "hyper/renderer/renderer.h"
#include "platform/opengl/opengl_context.h"

namespace hp
{
	std::unique_ptr<graphics_context> graphics_context::create(void* window)
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!")
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_unique<opengl_context>(static_cast<GLFWwindow*>(window));
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}  // namespace hp