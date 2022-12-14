#include "vk_surface.h"

namespace hp
{
	vk_surface::vk_surface()
	    : m_surface(VK_NULL_HANDLE)
	{
	}

	VkSurfaceKHR& vk_surface::get_surface()
	{
		return m_surface;
	}

	void create_vk_surface(vk_surface& surface, vk_instance& instance, GLFWwindow* window)
	{
		if (glfwCreateWindowSurface(instance.get_instance(), window, nullptr, &surface.get_surface()) != VK_SUCCESS)
		{
			log::error("Failed to create window surface!");
		}
	}

	void destroy_vk_surface(vk_surface& surface, vk_instance& instance)
	{
		vkDestroySurfaceKHR(instance.get_instance(), surface.get_surface(), nullptr);
	}
} // namespace hp