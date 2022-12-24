#include "vk_vertex.h"

namespace hp
{
	VkVertexInputBindingDescription vk_vertex::get_binding_description()
	{
		VkVertexInputBindingDescription binding_description = {};
		binding_description.binding                         = 0;
		binding_description.stride                          = sizeof(vk_vertex);
		binding_description.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

		return binding_description;
	}

	std::array<VkVertexInputAttributeDescription, 2> vk_vertex::get_attribute_descriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions = {};

		attribute_descriptions[0].binding  = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
		attribute_descriptions[0].offset   = offsetof(vk_vertex, position);

		attribute_descriptions[1].binding  = 0;
		attribute_descriptions[1].location = 1;
		attribute_descriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[1].offset   = offsetof(vk_vertex, color);

		return attribute_descriptions;
	}
} // namespace hp