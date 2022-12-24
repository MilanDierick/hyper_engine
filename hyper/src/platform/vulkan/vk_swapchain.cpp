#include "vk_swapchain.h"

namespace hp
{
	namespace detail
	{
		VkSurfaceCapabilitiesKHR query_surface_capabilities(vk_device& device, vk_surface& surface)
		{
			VkSurfaceCapabilitiesKHR surface_capabilities;
			if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.get_physical_device(), surface.get_surface(), &surface_capabilities) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to query surface capabilities!");
			}

			return surface_capabilities;
		}

		VkSurfaceFormatKHR query_surface_format(vk_device& device, vk_surface& surface, const vk_swapchain_parameters& parameters)
		{
			u32 format_count = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device.get_physical_device(), surface.get_surface(), &format_count, nullptr);

			if (format_count == 0)
			{
				throw std::runtime_error("failed to query surface formats!");
			}

			std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device.get_physical_device(), surface.get_surface(), &format_count, surface_formats.data());

			if (surface_formats.size() == 1 && surface_formats[0].format == VK_FORMAT_UNDEFINED)
			{
				return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
			}

			for (const auto& surface_format: surface_formats)
			{
				if (surface_format.format == parameters.surface_format.format && surface_format.colorSpace == parameters.surface_format.colorSpace)
				{
					return surface_format;
				}
			}

			return surface_formats[0];
		}

		VkPresentModeKHR query_present_mode(vk_device& device, vk_surface& surface, const vk_swapchain_parameters& parameters)
		{
			u32 present_mode_count = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device.get_physical_device(), surface.get_surface(), &present_mode_count, nullptr);

			if (present_mode_count == 0)
			{
				throw std::runtime_error("failed to query present modes!");
			}

			std::vector<VkPresentModeKHR> present_modes(present_mode_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device.get_physical_device(), surface.get_surface(), &present_mode_count, present_modes.data());

			for (const auto& present_mode: present_modes)
			{
				if (present_mode == parameters.present_mode)
				{
					return present_mode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D query_swapchain_extent(vk_device& device, vk_surface& surface, const vk_swapchain_parameters& parameters)
		{
			VkSurfaceCapabilitiesKHR const surface_capabilities = query_surface_capabilities(device, surface);

			if (surface_capabilities.currentExtent.width != std::numeric_limits<u32>::max())
			{
				return surface_capabilities.currentExtent;
			}

			VkExtent2D actual_extent = {parameters.extent.width, parameters.extent.height};

			actual_extent.width  = std::max(surface_capabilities.minImageExtent.width, std::min(surface_capabilities.maxImageExtent.width, actual_extent.width));
			actual_extent.height = std::max(surface_capabilities.minImageExtent.height, std::min(surface_capabilities.maxImageExtent.height, actual_extent.height));

			return actual_extent;
		}

		u32 query_swapchain_image_count(vk_device& device, vk_surface& surface)
		{
			VkSurfaceCapabilitiesKHR const surface_capabilities = query_surface_capabilities(device, surface);

			u32 image_count = surface_capabilities.minImageCount + 1;

			if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount)
			{
				image_count = surface_capabilities.maxImageCount;
			}

			return image_count;
		}

		b8 is_swapchain_suitable(vk_device& device, vk_surface& surface)
		{
			VkSurfaceCapabilitiesKHR const surface_capabilities = query_surface_capabilities(device, surface);

			if (surface_capabilities.currentExtent.width == std::numeric_limits<u32>::max())
			{
				return false;
			}

			u32 format_count = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device.get_physical_device(), surface.get_surface(), &format_count, nullptr);

			if (format_count == 0)
			{
				return false;
			}

			u32 present_mode_count = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device.get_physical_device(), surface.get_surface(), &present_mode_count, nullptr);

			return present_mode_count != 0;
		}

		void create_swapchain_images(vk_device& device, vk_swapchain& swapchain)
		{
			u32 image_count = 0;
			vkGetSwapchainImagesKHR(device.get_device(), swapchain.get_swapchain(), &image_count, nullptr);

			if (image_count == 0)
			{
				throw std::runtime_error("failed to query swapchain images!");
			}

			std::vector<VkImage> images(image_count);
			vkGetSwapchainImagesKHR(device.get_device(), swapchain.get_swapchain(), &image_count, images.data());

			swapchain.get_images().resize(image_count);
			swapchain.get_images() = images;
		}

		void create_swapchain_image_views(vk_device& device, vk_swapchain& swapchain)
		{
			swapchain.get_image_views().resize(swapchain.get_images().size());

			for (u32 i = 0; i < swapchain.get_images().size(); i++)
			{
				VkImageViewCreateInfo const create_info = {
				        .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				        .image      = swapchain.get_images()[i],
				        .viewType   = VK_IMAGE_VIEW_TYPE_2D,
				        .format     = swapchain.get_image_format(),
				        .components = {
				                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
				                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
				                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
				                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
				        },
				        .subresourceRange = {
				                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
				                .baseMipLevel   = 0,
				                .levelCount     = 1,
				                .baseArrayLayer = 0,
				                .layerCount     = 1,
				        },
				};

				if (vkCreateImageView(device.get_device(), &create_info, nullptr, &swapchain.get_image_views()[i]) != VK_SUCCESS)
				{
					throw std::runtime_error("failed to create image views!");
				}
			}
		}

		void destroy_swapchain_image_views(vk_device& device, vk_swapchain& swapchain)
		{
			for (auto& image_view: swapchain.get_image_views())
			{
				vkDestroyImageView(device.get_device(), image_view, nullptr);
			}
		}
	} // namespace detail

	vk_swapchain::vk_swapchain()
	    : m_swapchain(VK_NULL_HANDLE),
	      m_swapchain_images({}),
	      m_swapchain_image_views({}),
	      m_swapchain_extent({}),
	      m_swapchain_image_format(VK_FORMAT_UNDEFINED)
	{
	}

	VkSwapchainKHR& vk_swapchain::get_swapchain()
	{
		return m_swapchain;
	}

	std::vector<VkImage>& vk_swapchain::get_images()
	{
		return m_swapchain_images;
	}

	std::vector<VkImageView>& vk_swapchain::get_image_views()
	{
		return m_swapchain_image_views;
	}

	VkExtent2D& vk_swapchain::get_extent()
	{
		return m_swapchain_extent;
	}

	VkFormat& vk_swapchain::get_image_format()
	{
		return m_swapchain_image_format;
	}

	void create_vk_swapchain(vk_swapchain& swapchain, vk_device& device, vk_surface& surface, vk_swapchain_parameters const& parameters)
	{
		VkSurfaceFormatKHR const surface_format = detail::query_surface_format(device, surface, parameters);
		VkPresentModeKHR const present_mode     = detail::query_present_mode(device, surface, parameters);
		VkExtent2D const extent                 = detail::query_swapchain_extent(device, surface, parameters);
		u32 const image_count                   = detail::query_swapchain_image_count(device, surface);

		swapchain.get_extent()       = extent;
		swapchain.get_image_format() = surface_format.format;

		if (!detail::is_swapchain_suitable(device, surface))
		{
			throw std::runtime_error("failed to create swapchain!");
		}

		if (device.get_graphics_queue().second != device.get_present_queue().second)
		{
			u32 queue_family_indices[] = {static_cast<u32>(device.get_graphics_queue().second), static_cast<u32>(device.get_present_queue().second)};

			VkSwapchainCreateInfoKHR const swapchain_create_info = {
			        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			        .surface               = surface.get_surface(),
			        .minImageCount         = image_count,
			        .imageFormat           = surface_format.format,
			        .imageColorSpace       = surface_format.colorSpace,
			        .imageExtent           = extent,
			        .imageArrayLayers      = 1,
			        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			        .imageSharingMode      = VK_SHARING_MODE_CONCURRENT,
			        .queueFamilyIndexCount = 2,
			        .pQueueFamilyIndices   = queue_family_indices,
			        .preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			        .presentMode           = present_mode,
			        .clipped               = VK_TRUE,
			        .oldSwapchain          = VK_NULL_HANDLE,
			};

			if (vkCreateSwapchainKHR(device.get_device(), &swapchain_create_info, nullptr, &swapchain.get_swapchain()) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create swapchain!");
			}
		}
		else
		{
			VkSwapchainCreateInfoKHR const swapchain_create_info = {
			        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			        .surface               = surface.get_surface(),
			        .minImageCount         = image_count,
			        .imageFormat           = surface_format.format,
			        .imageColorSpace       = surface_format.colorSpace,
			        .imageExtent           = extent,
			        .imageArrayLayers      = 1,
			        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
			        .queueFamilyIndexCount = 0,
			        .pQueueFamilyIndices   = nullptr,
			        .preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			        .presentMode           = present_mode,
			        .clipped               = VK_TRUE,
			        .oldSwapchain          = VK_NULL_HANDLE,
			};

			if (vkCreateSwapchainKHR(device.get_device(), &swapchain_create_info, nullptr, &swapchain.get_swapchain()) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create swapchain!");
			}
		}

		detail::create_swapchain_images(device, swapchain);
		detail::create_swapchain_image_views(device, swapchain);
	}

	void destroy_vk_swapchain(vk_swapchain& swapchain, vk_device& device)
	{
		detail::destroy_swapchain_image_views(device, swapchain);
		vkDestroySwapchainKHR(device.get_device(), swapchain.get_swapchain(), nullptr);
	}
} // namespace hp