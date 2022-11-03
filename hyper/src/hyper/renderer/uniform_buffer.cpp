#include "uniform_buffer.h"

#include "hyper/renderer/renderer.h"

namespace hp
{
	std::shared_ptr<uniform_buffer> uniform_buffer::create(uint32_t size, uint32_t binding)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::none:
				HP_CORE_ASSERT(false, "renderer_api::none is currently not supported!");
				return nullptr;
		}

		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
} // namespace hp
