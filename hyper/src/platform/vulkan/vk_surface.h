#ifndef PLAYGROUND_VK_SURFACE_H
#define PLAYGROUND_VK_SURFACE_H

#include "platform/vulkan/vk_instance.h"

namespace hp
{
	class vk_surface
	{
	public:
		vk_surface();
		~vk_surface() = default;

		vk_surface(const vk_surface& other)                = delete;
		vk_surface(vk_surface&& other) noexcept            = delete;
		vk_surface& operator=(const vk_surface& other)     = delete;
		vk_surface& operator=(vk_surface&& other) noexcept = delete;

		VkSurfaceKHR& get_surface();

	private:
		VkSurfaceKHR m_surface;
	};

	void create_vk_surface(vk_surface& surface, vk_instance& instance, GLFWwindow* window);
	void destroy_vk_surface(vk_surface& surface, vk_instance& instance);
} // namespace hp

#endif //PLAYGROUND_VK_SURFACE_H
