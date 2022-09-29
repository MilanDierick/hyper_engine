#include "hyper/renderer/Texture.h"

#include "hyper/renderer/renderer.h"
#include "platform/opengl/OpenGLTexture.h"

namespace hp
{
	std::shared_ptr<texture_2d> texture_2d::Create(uint32_t width, uint32_t height)
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(width, height);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
	std::shared_ptr<texture_2d> texture_2d::Create(const std::string& path)
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
}  // namespace hp