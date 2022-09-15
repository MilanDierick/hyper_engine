#include "hyper/renderer/Renderer.h"
#include "hyper/renderer/RendererAPI.h"
#include "platform/opengl/OpenGLFramebuffer.h"

namespace hp {
	
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    HP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec);
		}

		HP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

