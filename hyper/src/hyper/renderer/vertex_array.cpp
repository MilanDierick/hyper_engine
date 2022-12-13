#include "hyper/renderer/vertex_array.h"

#include "hyper/renderer/renderer.h"

namespace hp
{
	std::shared_ptr<vertex_array> vertex_array::create()
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::none:
				HP_CORE_ASSERT(false, "renderer_api::none is currently not supported!");
				return nullptr;
			case renderer_api::API::vulkan:
				break;
		}

		HP_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
} // namespace hp