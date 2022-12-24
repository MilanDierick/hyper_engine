// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "vulkan_renderer.h"

#include "hyper/core/application.h"
#include "vulkan_window.h"

namespace hp
{
	void vulkan_renderer::init()
	{
		m_context = dynamic_cast<class vulkan_window*>(&application::instance()->get_window())->get_context();
		create_sync_objects();
	}

	void vulkan_renderer::shutdown()
	{
		vkDestroySemaphore(m_context->get_device().get_device(), m_image_available_semaphore, nullptr);
		vkDestroySemaphore(m_context->get_device().get_device(), m_render_finished_semaphore, nullptr);
		vkDestroyFence(m_context->get_device().get_device(), m_in_flight_fence, nullptr);

		delete m_context;
	}

	void vulkan_renderer::create_sync_objects()
	{
		VkSemaphoreCreateInfo semaphore_info{};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fence_info{};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Workaround for the first frame

		if (vkCreateSemaphore(m_context->get_device().get_device(), &semaphore_info, nullptr, &m_image_available_semaphore) != VK_SUCCESS ||
		    vkCreateSemaphore(m_context->get_device().get_device(), &semaphore_info, nullptr, &m_render_finished_semaphore) != VK_SUCCESS ||
		    vkCreateFence(m_context->get_device().get_device(), &fence_info, nullptr, &m_in_flight_fence) != VK_SUCCESS)
		{
			log::error("Failed to create synchronization objects for a frame!");
		}
	}

	void vulkan_renderer::draw_frame()
	{
		m_context->draw_frame();
	}

	vulkan_renderer& vulkan_renderer::get_instance()
	{
		static vulkan_renderer instance;
		return instance;
	}
} // namespace hp