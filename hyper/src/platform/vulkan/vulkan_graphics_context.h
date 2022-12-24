// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_VULKAN_GRAPHICS_CONTEXT_H
#define HYPER_VULKAN_GRAPHICS_CONTEXT_H

#include "hyper/core/config.h"
#include "hyper/renderer/graphics_context.h"
#include "platform/vulkan/vk_device.h"
#include "platform/vulkan/vk_instance.h"
#include "platform/vulkan/vk_surface.h"
#include "platform/vulkan/vk_swapchain.h"
#include "platform/vulkan/vk_vertex.h"

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

		vk_device& get_device();

		void record_command_buffer(VkCommandBuffer command_buffer, uint32_t image_index);
		void recreate_swapchain();
		void cleanup_swapchain();
		void draw_frame();

	private:
		GLFWwindow* m_p_window_handle;
		vk_instance m_instance;
		vk_surface m_surface;
		vk_device m_device;
		vk_swapchain m_swapchain;
		VkPipelineLayout m_pipeline_layout;
		VkRenderPass m_render_pass;
		VkPipeline m_graphics_pipeline;
		std::vector<VkFramebuffer> m_swapchain_framebuffers;
		VkCommandPool m_command_pool;
		std::vector<VkCommandBuffer> m_command_buffers;
		std::vector<VkSemaphore> m_image_available_semaphores;
		std::vector<VkSemaphore> m_render_finished_semaphores;
		std::vector<VkFence> m_in_flight_fences;
		size_t m_current_frame = 0;
		std::vector<vk_vertex> m_vertices;
		VkBuffer m_vertex_buffer;
		VmaAllocation m_vertex_buffer_allocation;

		VkPipeline create_graphics_pipeline();
		VkRenderPass create_render_pass();
		std::vector<VkFramebuffer> create_framebuffers();
		VkCommandPool create_command_pool();
		std::vector<VkCommandBuffer> create_command_buffers();
		void create_vertex_buffer();
		void create_sync_objects();
	};
} // namespace hp

#endif //HYPER_VULKAN_GRAPHICS_CONTEXT_H
