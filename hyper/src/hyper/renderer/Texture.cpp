#include "hyper/renderer/Texture.h"

#include "hyper/renderer/Renderer.h"
#include "platform/opengl/OpenGLTexture.h"

namespace hp
{
	std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		HP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(width, height);
		}
		
		HP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		HP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}
		
		HP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}  // namespace hp