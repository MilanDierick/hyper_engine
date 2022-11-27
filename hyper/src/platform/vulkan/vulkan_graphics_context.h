// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_VULKAN_GRAPHICS_CONTEXT_H
#define HYPER_VULKAN_GRAPHICS_CONTEXT_H

#include "hyper/core/config.h"
#include "hyper/renderer/graphics_context.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>

namespace hp
{
	class HP_API vulkan_graphics_context : public graphics_context
	{
	public:
		explicit vulkan_graphics_context(GLFWwindow* handle);
		~vulkan_graphics_context() override;

		vulkan_graphics_context(const vulkan_graphics_context& other)            = delete;
		vulkan_graphics_context(vulkan_graphics_context&& other) noexcept        = delete;
		vulkan_graphics_context& operator=(const vulkan_graphics_context& other) = delete;
		vulkan_graphics_context& operator=(vulkan_graphics_context&& other)      = delete;

		void init() override;
		void swap_buffers() override;

		vkb::Device get_device();

		void record_command_buffer(VkCommandBuffer command_buffer, uint32_t image_index);
		void draw_frame();

	private:
		GLFWwindow* m_p_window_handle;
		vkb::Instance m_instance;
		VkSurfaceKHR m_surface;
		vkb::Device m_device;
		VkQueue m_graphics_queue;
		VkQueue m_present_queue;
		vkb::Swapchain m_swapchain;
		VkPipelineLayout m_pipeline_layout;
		VkRenderPass m_render_pass;
		VkPipeline m_graphics_pipeline;
		std::vector<VkFramebuffer> m_swapchain_framebuffers;
		VkCommandPool m_command_pool;
		VkCommandBuffer m_command_buffer;
		VkSemaphore m_image_available_semaphore;
		VkSemaphore m_render_finished_semaphore;
		VkFence m_in_flight_fence;

		vkb::Instance create_instance();
		VkSurfaceKHR create_surface();
		vkb::Device create_device();
		VkQueue create_graphics_queue();
		VkQueue create_present_queue();
		vkb::Swapchain create_swapchain();
		VkPipeline create_graphics_pipeline();
		VkRenderPass create_render_pass();
		std::vector<VkFramebuffer> create_framebuffers();
		VkCommandPool create_command_pool();
		VkCommandBuffer create_command_buffer();
		void create_sync_objects();
	};
} // namespace hp

#endif //HYPER_VULKAN_GRAPHICS_CONTEXT_H
