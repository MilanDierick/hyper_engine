#ifndef PLAYGROUND_VK_VERTEX_H
#define PLAYGROUND_VK_VERTEX_H

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace hp
{
	struct vk_vertex
	{
		glm::vec2 position;
		glm::vec3 color;

		static VkVertexInputBindingDescription get_binding_description();
		static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions();
	};
} // namespace hp

#endif //PLAYGROUND_VK_VERTEX_H
