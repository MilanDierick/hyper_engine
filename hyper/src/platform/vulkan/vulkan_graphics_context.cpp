// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "vulkan_graphics_context.h"

#include "hyper/core/application.h"
#include "platform/vulkan/vulkan_shader.h"

namespace hp
{
	const int MAX_FRAMES_IN_FLIGHT = 2;

	vulkan_graphics_context::vulkan_graphics_context(GLFWwindow* handle) : m_p_window_handle(handle),
	                                                                       m_instance(),
	                                                                       m_surface(),
	                                                                       m_pipeline_layout(),
	                                                                       m_render_pass(),
	                                                                       m_graphics_pipeline(),
	                                                                       m_command_pool(),
	                                                                       m_command_buffers(),
	                                                                       m_image_available_semaphores(),
	                                                                       m_render_finished_semaphores(),
	                                                                       m_in_flight_fences()
	{
	}

	vulkan_graphics_context::~vulkan_graphics_context()
	{
		cleanup_swapchain();

		for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(m_device.get_device(), m_image_available_semaphores[i], nullptr);
			vkDestroySemaphore(m_device.get_device(), m_render_finished_semaphores[i], nullptr);
			vkDestroyFence(m_device.get_device(), m_in_flight_fences[i], nullptr);
		}

		for (const auto& framebuffer: m_swapchain_framebuffers)
		{
			vkDestroyFramebuffer(m_device.get_device(), framebuffer, nullptr);
		}

		vkDestroyPipeline(m_device.get_device(), m_graphics_pipeline, nullptr);
		vkDestroyPipelineLayout(m_device.get_device(), m_pipeline_layout, nullptr);
		vkDestroyRenderPass(m_device.get_device(), m_render_pass, nullptr);
		vkb::destroy_swapchain(m_swapchain);
		vkDestroyCommandPool(m_device.get_device(), m_command_pool, nullptr);
		destroy_vk_device(m_device);
		destroy_vk_surface(m_surface, m_instance);
		destroy_vk_instance(m_instance);
	}

	void vulkan_graphics_context::init()
	{
		vk_instance_parameters const instance_parameters = {
		        .application_name          = "Hyper",
		        .engine_name               = "Hyper",
		        .application_version       = VK_MAKE_API_VERSION(0, 0, 1, 0),
		        .engine_version            = VK_MAKE_API_VERSION(0, 0, 1, 0),
		        .api_version               = VK_MAKE_API_VERSION(0, 1, 0, 0),
		        .validation_layers_enabled = true,
		};

		create_vk_instance(m_instance, instance_parameters);
		create_vk_surface(m_surface, m_instance, m_p_window_handle);
		create_vk_device(m_device, m_instance, m_surface);
		m_swapchain              = create_swapchain();
		m_render_pass            = create_render_pass();
		m_graphics_pipeline      = create_graphics_pipeline();
		m_swapchain_framebuffers = create_framebuffers();
		m_command_pool           = create_command_pool();
		m_command_buffers        = create_command_buffers();

		create_sync_objects();
	}

	void vulkan_graphics_context::swap_buffers()
	{
	}

	vk_device& vulkan_graphics_context::get_device()
	{
		return m_device;
	}

	void vulkan_graphics_context::record_command_buffer(VkCommandBuffer command_buffer, uint32_t image_index)
	{
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
		{
			log::error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass        = m_render_pass;
		render_pass_info.framebuffer       = m_swapchain_framebuffers[image_index];
		render_pass_info.renderArea.offset = {0, 0};
		render_pass_info.renderArea.extent = m_swapchain.extent;

		const VkClearValue clear_color   = {};
		render_pass_info.clearValueCount = 1;
		render_pass_info.pClearValues    = &clear_color;

		vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

		VkViewport viewport{};
		viewport.x        = 0.0F;
		viewport.y        = 0.0F;
		viewport.width    = static_cast<float>(m_swapchain.extent.width);
		viewport.height   = static_cast<float>(m_swapchain.extent.height);
		viewport.minDepth = 0.0F;
		viewport.maxDepth = 1.0F;
		vkCmdSetViewport(command_buffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = m_swapchain.extent;
		vkCmdSetScissor(command_buffer, 0, 1, &scissor);

		vkCmdDraw(command_buffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(command_buffer);

		if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
		{
			log::error("Failed to record command buffer!");
		}
	}

	vkb::Swapchain vulkan_graphics_context::recreate_swapchain()
	{
		vkDeviceWaitIdle(m_device.get_device());

		cleanup_swapchain();
		vkb::destroy_swapchain(m_swapchain);

		m_swapchain              = create_swapchain();
		m_swapchain_framebuffers = create_framebuffers();
		m_command_buffers        = create_command_buffers();

		return m_swapchain;
	}

	void vulkan_graphics_context::cleanup_swapchain()
	{
		for (const auto& framebuffer: m_swapchain_framebuffers)
		{
			vkDestroyFramebuffer(m_device.get_device(), framebuffer, nullptr);
		}
	}

	void vulkan_graphics_context::draw_frame()
	{
		vkWaitForFences(m_device.get_device(), 1, &m_in_flight_fences[m_current_frame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex                  = 0;
		VkResult const result_next_image_khr = vkAcquireNextImageKHR(m_device.get_device(), m_swapchain, UINT64_MAX, m_image_available_semaphores[m_current_frame], VK_NULL_HANDLE, &imageIndex);

		if (result_next_image_khr == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreate_swapchain();
			return;
		}

		if (result_next_image_khr != VK_SUCCESS && result_next_image_khr != VK_SUBOPTIMAL_KHR)
		{
			log::error("Failed to acquire swap chain image!");
		}

		vkResetFences(m_device.get_device(), 1, &m_in_flight_fences[m_current_frame]);

		vkResetCommandBuffer(m_command_buffers[m_current_frame], 0);
		record_command_buffer(m_command_buffers[m_current_frame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[]      = {m_image_available_semaphores[m_current_frame]};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount     = 1;
		submitInfo.pWaitSemaphores        = waitSemaphores;
		submitInfo.pWaitDstStageMask      = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers    = &m_command_buffers[m_current_frame];

		VkSemaphore signalSemaphores[]  = {m_render_finished_semaphores[m_current_frame]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores    = signalSemaphores;

		if (vkQueueSubmit(m_device.get_graphics_queue().first, 1, &submitInfo, m_in_flight_fences[m_current_frame]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores    = signalSemaphores;

		VkSwapchainKHR swapChains[] = {m_swapchain};
		presentInfo.swapchainCount  = 1;
		presentInfo.pSwapchains     = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		VkResult const result_queue_present = vkQueuePresentKHR(m_device.get_present_queue().first, &presentInfo);

		if (result_queue_present == VK_ERROR_OUT_OF_DATE_KHR || result_queue_present == VK_SUBOPTIMAL_KHR)
		{
			recreate_swapchain();
		}
		else if (result_queue_present != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	vkb::Swapchain vulkan_graphics_context::create_swapchain()
	{
		// Create the swapchain builder and set the surface and present queue.
		vkb::SwapchainBuilder swapchain_builder{m_device, m_surface};
		const auto swapchain_ret = swapchain_builder.set_desired_present_mode(VK_PRESENT_MODE_MAILBOX_KHR)
		                                   .add_fallback_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		                                   .set_desired_format({VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
		                                   .set_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
		                                   .set_composite_alpha_flags(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
		                                   .set_clipped()
		                                   .build();

		// If the swapchain creation failed, the 'std::optional' will be empty.
		if (!swapchain_ret.has_value())
		{
			log::error("Failed to create swapchain: {0}", swapchain_ret.error().message());
		}

		return swapchain_ret.value();
	}

	VkPipeline vulkan_graphics_context::create_graphics_pipeline()
	{
		const vulkan_shader vert_shader("assets/shaders/triangle.vert.spv", m_device);
		const vulkan_shader frag_shader("assets/shaders/triangle.frag.spv", m_device);

		VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
		vert_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vert_shader_stage_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
		vert_shader_stage_info.module = static_cast<VkShaderModule>(vert_shader);
		vert_shader_stage_info.pName  = "main";

		VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
		frag_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		frag_shader_stage_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
		frag_shader_stage_info.module = static_cast<VkShaderModule>(frag_shader);
		frag_shader_stage_info.pName  = "main";

		VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

		VkPipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_info.vertexBindingDescriptionCount   = 0;
		vertex_input_info.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
		input_assembly_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly_info.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewport_state_info{};
		viewport_state_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state_info.viewportCount = 1;
		viewport_state_info.scissorCount  = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer_info{};
		rasterizer_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer_info.depthClampEnable        = VK_FALSE;                // If true, fragments beyond the near and far planes are clamped to them as opposed to discarding them.
		rasterizer_info.rasterizerDiscardEnable = VK_FALSE;                // If true, geometry never passes through the rasterizer stage. This basically disables any output to the framebuffer.
		rasterizer_info.polygonMode             = VK_POLYGON_MODE_FILL;    // How fragments are generated for geometry. Can be set to fill polygons, draw lines or points.
		rasterizer_info.lineWidth               = 1.0F;                    // Line width if polygonMode is set to VK_POLYGON_MODE_LINE.
		rasterizer_info.cullMode                = VK_CULL_MODE_BACK_BIT;   // Which face of a triangle to cull. Can be set to none, back, front or both.
		rasterizer_info.frontFace               = VK_FRONT_FACE_CLOCKWISE; // Which vertex order is considered front-facing and can be culled if cullMode is not set to none.
		rasterizer_info.depthBiasEnable         = VK_FALSE;                // If true, you can configure depth bias to avoid shadow acne.

		VkPipelineMultisampleStateCreateInfo multisampling_info{};
		multisampling_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling_info.sampleShadingEnable   = VK_FALSE;              // Enable multisampling in the fragment shader.
		multisampling_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT; // Number of samples to use per fragment.
		multisampling_info.minSampleShading      = 1.0F;                  // Optional. Min fraction for sample shading; closer to one is smoother.
		multisampling_info.pSampleMask           = VK_NULL_HANDLE;        // Optional. Sample mask to use if sampleShadingEnable is set to VK_TRUE.
		multisampling_info.alphaToCoverageEnable = VK_FALSE;              // Optional. Enable alpha to coverage.
		multisampling_info.alphaToOneEnable      = VK_FALSE;              // Optional. Enable alpha to one.

		VkPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT; // Which color components to write to the framebuffer.
		color_blend_attachment.blendEnable         = VK_FALSE;                                                                                                  // If true, you can configure blending.
		color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment.colorBlendOp        = VK_BLEND_OP_ADD; // How to combine the source and destination colors.
		color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment.alphaBlendOp        = VK_BLEND_OP_ADD; // How to combine the source and destination alpha values.

		VkPipelineColorBlendStateCreateInfo color_blending_info{};
		color_blending_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blending_info.logicOpEnable     = VK_FALSE;         // If true, you can configure logic operations.
		color_blending_info.logicOp           = VK_LOGIC_OP_COPY; // How to combine the source and destination data if logicOpEnable is set to VK_TRUE.
		color_blending_info.attachmentCount   = 1;
		color_blending_info.pAttachments      = &color_blend_attachment;
		color_blending_info.blendConstants[0] = 0.0F; // Optional. Blend constants used by the blending equations.
		color_blending_info.blendConstants[1] = 0.0F; // Optional. Blend constants used by the blending equations.
		color_blending_info.blendConstants[2] = 0.0F; // Optional. Blend constants used by the blending equations.
		color_blending_info.blendConstants[3] = 0.0F; // Optional. Blend constants used by the blending equations.

		VkPipelineLayoutCreateInfo layout_info{};
		layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layout_info.setLayoutCount         = 0;              // Optional. Number of descriptor set layouts.
		layout_info.pSetLayouts            = VK_NULL_HANDLE; // Optional. Array of descriptor set layouts.
		layout_info.pushConstantRangeCount = 0;              // Optional. Number of push constant ranges.
		layout_info.pPushConstantRanges    = VK_NULL_HANDLE; // Optional. Array of push constant ranges.

		if (vkCreatePipelineLayout(m_device.get_device(), &layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}

		std::vector<VkDynamicState> dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

		VkPipelineDynamicStateCreateInfo dynamic_state_info{};
		dynamic_state_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
		dynamic_state_info.pDynamicStates    = dynamic_states.data();

		VkGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount          = 2;
		pipeline_info.pStages             = shader_stages;
		pipeline_info.pVertexInputState   = &vertex_input_info;
		pipeline_info.pInputAssemblyState = &input_assembly_info;
		pipeline_info.pViewportState      = &viewport_state_info;
		pipeline_info.pRasterizationState = &rasterizer_info;
		pipeline_info.pMultisampleState   = &multisampling_info;
		pipeline_info.pDepthStencilState  = nullptr; // Optional. Depth and stencil testing.
		pipeline_info.pColorBlendState    = &color_blending_info;
		pipeline_info.pDynamicState       = &dynamic_state_info;
		pipeline_info.layout              = m_pipeline_layout;
		pipeline_info.renderPass          = m_render_pass;
		pipeline_info.subpass             = 0;              // Index of the subpass where this pipeline will be used.
		pipeline_info.basePipelineHandle  = VK_NULL_HANDLE; // Optional. Handle to a pipeline to derive from.
		pipeline_info.basePipelineIndex   = -1;             // Optional. Index of a pipeline to derive from.

		VkPipeline pipeline = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(m_device.get_device(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline!");
		}

		return pipeline;
	}

	VkRenderPass vulkan_graphics_context::create_render_pass()
	{
		VkRenderPass render_pass = VK_NULL_HANDLE;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format         = m_swapchain.image_format;
		colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments    = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass    = 0;
		dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments    = &colorAttachment;
		renderPassInfo.subpassCount    = 1;
		renderPassInfo.pSubpasses      = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies   = &dependency;

		if (vkCreateRenderPass(m_device.get_device(), &renderPassInfo, nullptr, &render_pass) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create render pass!");
		}

		return render_pass;
	}

	std::vector<VkFramebuffer> vulkan_graphics_context::create_framebuffers()
	{
		std::vector<VkFramebuffer> framebuffers(m_swapchain.get_image_views().value().size());

		for (size_t i = 0; i < m_swapchain.get_image_views().value().size(); i++)
		{
			VkImageView attachments[] = {m_swapchain.get_image_views().value()[i]};

			VkFramebufferCreateInfo framebuffer_info{};
			framebuffer_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_info.renderPass      = m_render_pass;
			framebuffer_info.attachmentCount = 1;
			framebuffer_info.pAttachments    = attachments;
			framebuffer_info.width           = m_swapchain.extent.width;
			framebuffer_info.height          = m_swapchain.extent.height;
			framebuffer_info.layers          = 1;

			if (vkCreateFramebuffer(m_device.get_device(), &framebuffer_info, nullptr, &framebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create framebuffer!");
			}
		}

		return framebuffers;
	}

	VkCommandPool vulkan_graphics_context::create_command_pool()
	{
		VkCommandPool command_pool = VK_NULL_HANDLE;

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = m_device.get_graphics_queue().second;
		poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(m_device.get_device(), &poolInfo, nullptr, &command_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create command pool!");
		}

		return command_pool;
	}

	std::vector<VkCommandBuffer> vulkan_graphics_context::create_command_buffers()
	{
		std::vector<VkCommandBuffer> command_buffers(m_swapchain.get_image_views().value().size());

		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool        = m_command_pool;
		alloc_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		if (vkAllocateCommandBuffers(m_device.get_device(), &alloc_info, command_buffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}

		for (size_t i = 0; i < command_buffers.size(); i++)
		{
			VkCommandBufferBeginInfo begin_info{};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(command_buffers[i], &begin_info) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo render_pass_info{};
			render_pass_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			render_pass_info.renderPass        = m_render_pass;
			render_pass_info.framebuffer       = m_swapchain_framebuffers[i];
			render_pass_info.renderArea.offset = {0, 0};
			render_pass_info.renderArea.extent = m_swapchain.extent;

			VkClearValue const clear_color   = {};
			render_pass_info.clearValueCount = 1;
			render_pass_info.pClearValues    = &clear_color;

			VkViewport viewport{};
			viewport.x        = 0.0F;
			viewport.y        = 0.0F;
			viewport.width    = static_cast<float>(m_swapchain.extent.width);
			viewport.height   = static_cast<float>(m_swapchain.extent.height);
			viewport.minDepth = 0.0F;
			viewport.maxDepth = 1.0F;

			vkCmdSetViewport(command_buffers[i], 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.offset = {0, 0};
			scissor.extent = m_swapchain.extent;

			vkCmdSetScissor(command_buffers[i], 0, 1, &scissor);

			vkCmdBeginRenderPass(command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

			vkCmdDraw(command_buffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(command_buffers[i]);

			if (vkEndCommandBuffer(command_buffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to record command buffer!");
			}
		}

		return command_buffers;
	}

	void vulkan_graphics_context::create_sync_objects()
	{
		m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphore_info{};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fence_info{};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(m_device.get_device(), &semaphore_info, nullptr, &m_image_available_semaphores[i]) != VK_SUCCESS ||
			    vkCreateSemaphore(m_device.get_device(), &semaphore_info, nullptr, &m_render_finished_semaphores[i]) != VK_SUCCESS ||
			    vkCreateFence(m_device.get_device(), &fence_info, nullptr, &m_in_flight_fences[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create synchronization objects for a frame!");
			}
		}
	}
} // namespace hp
