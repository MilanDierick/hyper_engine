#ifndef PLAYGROUND_VK_SWAPCHAIN_H
#define PLAYGROUND_VK_SWAPCHAIN_H

#include "platform/vulkan/vk_device.h"
#include "platform/vulkan/vk_instance.h"

#include <vulkan/vulkan.h>

namespace hp
{
	struct vk_swapchain_parameters
	{
		VkSurfaceFormatKHR surface_format;
		VkPresentModeKHR present_mode;
		VkExtent2D extent;
	};

	class vk_swapchain
	{
	public:
		vk_swapchain();
		~vk_swapchain() = default;

		vk_swapchain(const vk_swapchain& other)                = delete;
		vk_swapchain(vk_swapchain&& other) noexcept            = delete;
		vk_swapchain& operator=(const vk_swapchain& other)     = delete;
		vk_swapchain& operator=(vk_swapchain&& other) noexcept = delete;

		hp_nodiscard VkSwapchainKHR& get_swapchain();
		hp_nodiscard std::vector<VkImage>& get_images();
		hp_nodiscard std::vector<VkImageView>& get_image_views();
		hp_nodiscard VkExtent2D& get_extent();
		hp_nodiscard VkFormat& get_image_format();

	private:
		VkSwapchainKHR m_swapchain;
		std::vector<VkImage> m_swapchain_images;
		std::vector<VkImageView> m_swapchain_image_views;
		VkExtent2D m_swapchain_extent;
		VkFormat m_swapchain_image_format;
	};

	void create_vk_swapchain(vk_swapchain& swapchain, vk_device& device, vk_surface& surface, vk_swapchain_parameters const& parameters);
	void destroy_vk_swapchain(vk_swapchain& swapchain, vk_device& device);
} // namespace hp

#endif //PLAYGROUND_VK_SWAPCHAIN_H
