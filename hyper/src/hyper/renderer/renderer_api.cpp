#include "hyper/renderer/renderer_api.h"

namespace hp
{
	renderer_api::API renderer_api::s_api = renderer_api::API::none;

	std::unique_ptr<renderer_api> renderer_api::create()
	{
		switch (s_api)
		{
			case renderer_api::API::none:
				HP_CORE_ASSERT(false, "renderer_api::none is currently not supported!");
				return nullptr;
			case API::vulkan:
				break;
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}

} // namespace hp