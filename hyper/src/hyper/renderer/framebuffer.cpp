#include "hyper/renderer/framebuffer.h"

#include "hyper/renderer/renderer.h"
#include "hyper/renderer/renderer_api.h"

namespace hp
{

	std::shared_ptr<framebuffer> framebuffer::create(const framebuffer_specification& spec)
	{
		hp_unused(spec);

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
