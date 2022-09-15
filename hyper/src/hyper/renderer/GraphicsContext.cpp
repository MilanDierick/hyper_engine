#include "hyper/renderer/GraphicsContext.h"

#include "hyper/renderer/Renderer.h"
#include "platform/opengl/OpenGLContext.h"

namespace hp {

	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    HP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		HP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}