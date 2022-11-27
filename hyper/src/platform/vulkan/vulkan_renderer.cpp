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
		//vkDestroySemaphore(m_context->get_device(), m_image_available_semaphore, nullptr);
		//vkDestroySemaphore(m_context->get_device(), m_render_finished_semaphore, nullptr);
		//vkDestroyFence(m_context->get_device(), m_in_flight_fence, nullptr);

		//delete m_context;
	}
	void vulkan_renderer::draw_frame()
	{
		//vkWaitForFences(m_context->get_device(), 1, &m_in_flight_fence, VK_TRUE, UINT64_MAX);
		//vkResetFences(m_context->get_device(), 1, &m_in_flight_fence);
		//
		//uint32_t image_index = 0;
		//vkAcquireNextImageKHR(m_context->get_device(), m_context->get_swapchain(), UINT64_MAX, m_image_available_semaphore, VK_NULL_HANDLE, &image_index);
		//
		//vkResetCommandBuffer(m_context->get_command_buffers()[0], 0);
		//m_context->record_command_buffer(m_context->get_command_buffers()[0], image_index);
		//
		//VkSubmitInfo submit_info = {};
		//submit_info.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//
		//VkSemaphore waitSemaphores[]      = {m_image_available_semaphore};
		//VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		//submit_info.waitSemaphoreCount    = 1;
		//submit_info.pWaitSemaphores       = waitSemaphores;
		//submit_info.pWaitDstStageMask     = waitStages;
		//submit_info.commandBufferCount    = 1;
		//submit_info.pCommandBuffers       = m_context->get_command_buffers();
		//
		//VkSemaphore signal_semaphores[]  = {m_render_finished_semaphore};
		//submit_info.signalSemaphoreCount = 1;
		//submit_info.pSignalSemaphores    = signal_semaphores;
		//
		//if (vkQueueSubmit(m_context->get_graphics_queue(), 1, &submit_info, m_in_flight_fence) != VK_SUCCESS)
		//{
		//	throw std::runtime_error("Failed to submit draw command buffer!");
		//}
		//
		//VkPresentInfoKHR present_info = {};
		//present_info.sType            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		//
		//present_info.waitSemaphoreCount = 1;
		//present_info.pWaitSemaphores    = signal_semaphores;
		//
		//VkSwapchainKHR swapchains[] = {m_context->get_swapchain()};
		//present_info.swapchainCount = 1;
		//present_info.pSwapchains    = swapchains;
		//present_info.pImageIndices  = &image_index;
		//present_info.pResults       = nullptr;
		//
		//vkQueuePresentKHR(m_context->get_present_queue(), &present_info);
	}

	void vulkan_renderer::create_sync_objects()
	{
		//VkSemaphoreCreateInfo semaphore_info{};
		//semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		//
		//VkFenceCreateInfo fence_info{};
		//fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		//fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Workaround for the first frame
		//
		//if (vkCreateSemaphore(m_context->get_device(), &semaphore_info, nullptr, &m_image_available_semaphore) != VK_SUCCESS ||
		//    vkCreateSemaphore(m_context->get_device(), &semaphore_info, nullptr, &m_render_finished_semaphore) != VK_SUCCESS ||
		//    vkCreateFence(m_context->get_device(), &fence_info, nullptr, &m_in_flight_fence) != VK_SUCCESS)
		//{
		//	log::error("Failed to create synchronization objects for a frame!");
		//}
	}

	vulkan_renderer& vulkan_renderer::get_instance()
	{
		static vulkan_renderer instance;
		return instance;
	}
} // namespace hp