#include "hyper/renderer/texture.h"

#include "hyper/renderer/renderer.h"

namespace hp
{
	std::shared_ptr<texture_2d> texture_2d::create(uint32_t width, uint32_t height)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::None:
				HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
				return nullptr;
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}

	std::shared_ptr<texture_2d> texture_2d::create(const std::string& path)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::None:
				HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
				return nullptr;
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
} // namespace hp