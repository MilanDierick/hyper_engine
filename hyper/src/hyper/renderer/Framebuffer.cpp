#include "hyper/renderer/renderer.h"
#include "hyper/renderer/renderer_api.h"
#include "platform/opengl/OpenGLFramebuffer.h"

namespace hp {
	
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::None:    HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!"); return nullptr;
			case renderer_api::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec);
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}

}

