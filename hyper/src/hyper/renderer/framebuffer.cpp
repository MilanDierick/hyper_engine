#include "hyper/renderer/renderer.h"
#include "hyper/renderer/renderer_api.h"
#include "platform/opengl/opengl_framebuffer.h"

namespace hp {
	
	std::shared_ptr<framebuffer> framebuffer::create(const framebuffer_specification& spec)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::None:    HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!"); return nullptr;
			case renderer_api::API::OpenGL:  return std::make_shared<opengl_framebuffer>(spec);
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}

}

