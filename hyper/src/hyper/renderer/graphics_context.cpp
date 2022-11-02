#include "hyper/renderer/graphics_context.h"

#include "hyper/renderer/renderer.h"

namespace hp
{
	std::unique_ptr<graphics_context> graphics_context::create(void* window)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::None:
				HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!")
				return nullptr;
		}

		HP_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
} // namespace hp