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

		[[nodiscard]] const vkb::Device& get_device() const;

	private:
		GLFWwindow* m_p_window_handle;
		vkb::Instance m_instance;
		VkSurfaceKHR m_surface;
		vkb::Device m_device;
		VkQueue m_graphics_queue;
		VkQueue m_present_queue;
		vkb::Swapchain m_swapchain;
		VkPipeline m_pipeline;
		VkPipelineLayout m_pipeline_layout;
		VkRenderPass m_render_pass;
		VkPipeline m_graphics_pipeline;

		vkb::Instance create_instance();
		VkSurfaceKHR create_surface();
		vkb::Device create_device();
		VkQueue create_graphics_queue();
		VkQueue create_present_queue();
		vkb::Swapchain create_swapchain();
		VkPipeline create_graphics_pipeline();
		VkRenderPass create_render_pass();
	};
} // namespace hp

#endif //HYPER_VULKAN_GRAPHICS_CONTEXT_H
