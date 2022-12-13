#include "hyper/renderer/texture.h"

#include "hyper/renderer/renderer.h"

namespace hp
{
	std::shared_ptr<texture_2d> texture_2d::create(uint32_t width, uint32_t height)
	{
		UNUSED(width);
		UNUSED(height);

		switch (renderer::get_api())
		{
			case renderer_api::API::none:
				HP_CORE_ASSERT(false, "renderer_api::none is currently not supported!");
				return nullptr;
			case renderer_api::API::vulkan:
				return nullptr;
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}

	std::shared_ptr<texture_2d> texture_2d::create(const std::string& path)
	{
		UNUSED(path);

		switch (renderer::get_api())
		{
			case renderer_api::API::none:
				HP_CORE_ASSERT(false, "renderer_api::none is currently not supported!");
				return nullptr;
			case renderer_api::API::vulkan:
				break;
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
} // namespace hp