#include "hyper/renderer/buffer.h"

#include "hyper/renderer/renderer.h"

namespace hp
{

	std::shared_ptr<vertex_buffer> vertex_buffer::create(uint32_t size)
	{
		UNUSED(size);

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

	std::shared_ptr<vertex_buffer> vertex_buffer::create(float* vertices, uint32_t size)
	{
		UNUSED(vertices);
		UNUSED(size);

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

	std::shared_ptr<index_buffer> index_buffer::create(uint32_t* indices, uint32_t count)
	{
		UNUSED(indices);
		UNUSED(count);

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