#ifndef PLAYGROUND_VK_DEVICE_H
#define PLAYGROUND_VK_DEVICE_H

#include "platform/vulkan/vk_instance.h"
#include "platform/vulkan/vk_surface.h"

namespace hp
{

	class vk_device
	{
	public:
		vk_device();
		~vk_device() = default;

		vk_device(const vk_device& other)                = delete;
		vk_device(vk_device&& other) noexcept            = delete;
		vk_device& operator=(const vk_device& other)     = delete;
		vk_device& operator=(vk_device&& other) noexcept = delete;

		hp_nodiscard VkDevice& get_device();
		hp_nodiscard std::pair<VkQueue, u8>& get_graphics_queue();
		hp_nodiscard std::pair<VkQueue, u8>& get_present_queue();

	private:
		VkDevice m_device;
		std::pair<VkQueue, u8> m_graphics_queue;
		std::pair<VkQueue, u8> m_present_queue;
	};

	void create_vk_device(vk_device& device, vk_instance& instance, vk_surface& surface);
	void destroy_vk_device(vk_device& device);
} // namespace hp

#endif //PLAYGROUND_VK_DEVICE_H
