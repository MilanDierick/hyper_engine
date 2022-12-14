#include "vk_device.h"

namespace hp
{
	namespace detail
	{
		struct queue_family_indices
		{
			std::optional<u8> graphics_family;
			std::optional<u8> present_family;

			hp_nodiscard bool is_complete() const
			{
				return graphics_family.has_value() && present_family.has_value();
			}
		};

		queue_family_indices find_queue_families(VkPhysicalDevice device, vk_surface& surface)
		{
			queue_family_indices indices;

			u32 queue_family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

			std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

			u8 index = 0;
			for (const auto& queue_family: queue_families)
			{
				if ((queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_TRUE)
				{
					indices.graphics_family = index;
				}

				VkBool32 present_support = VK_FALSE;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface.get_surface(), &present_support);

				if (present_support == VK_TRUE)
				{
					indices.present_family = index;
				}

				if (indices.is_complete())
				{
					break;
				}

				index++;
			}

			return indices;
		}

		bool is_device_suitable(VkPhysicalDevice device, vk_surface& surface)
		{
			return find_queue_families(device, surface).is_complete();
		}

		VkPhysicalDevice pick_physical_device(vk_instance& instance, vk_surface& surface)
		{
			u32 device_count = 0;
			vkEnumeratePhysicalDevices(instance.get_instance(), &device_count, nullptr);

			if (device_count == 0)
			{
				log::error("Failed to find GPUs with Vulkan support!");
			}

			std::vector<VkPhysicalDevice> devices(device_count);
			vkEnumeratePhysicalDevices(instance.get_instance(), &device_count, devices.data());

			for (const auto& device: devices)
			{
				if (is_device_suitable(device, surface))
				{
					return device;
				}
			}

			log::error("Failed to find a suitable GPU!");

			return VK_NULL_HANDLE;
		}
	} // namespace detail

	vk_device::vk_device()
	    : m_device(VK_NULL_HANDLE)
	{
	}

	VkDevice& vk_device::get_device()
	{
		return m_device;
	}

	std::pair<VkQueue, u8>& vk_device::get_graphics_queue()
	{
		return m_graphics_queue;
	}

	std::pair<VkQueue, u8>& vk_device::get_present_queue()
	{
		return m_present_queue;
	}

	void create_vk_device(vk_device& device, vk_instance& instance, vk_surface& surface)
	{
		VkPhysicalDevice physical_device = detail::pick_physical_device(instance, surface);
		detail::queue_family_indices const indices = detail::find_queue_families(physical_device, surface);

		float const queue_priority = 1.0F;

		VkDeviceQueueCreateInfo const queue_create_info = {
		        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		        .queueFamilyIndex = 0,
		        .queueCount       = 1,
		        .pQueuePriorities = &queue_priority,
		};

		VkPhysicalDeviceFeatures const device_features = {};

		VkDeviceCreateInfo const create_info = {
		        .sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		        .queueCreateInfoCount  = 1,
		        .pQueueCreateInfos     = &queue_create_info,
		        .enabledExtensionCount = 0,
		        .pEnabledFeatures      = &device_features,
		};

		if (vkCreateDevice(physical_device, &create_info, nullptr, &device.get_device()) != VK_SUCCESS)
		{
			log::error("Failed to create logical device!");
		}

		device.get_graphics_queue().second = indices.graphics_family.value();
		device.get_present_queue().second  = indices.present_family.value();

		vkGetDeviceQueue(device.get_device(), indices.graphics_family.value(), 0, &device.get_graphics_queue().first);
		vkGetDeviceQueue(device.get_device(), indices.present_family.value(), 0, &device.get_present_queue().first);
	}

	void destroy_vk_device(vk_device& device)
	{
		vkDestroyDevice(device.get_device(), nullptr);
	}
} // namespace hp