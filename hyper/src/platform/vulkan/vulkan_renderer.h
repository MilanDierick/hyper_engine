// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_VULKAN_RENDERER_H
#define PLAYGROUND_VULKAN_RENDERER_H

#include "platform/vulkan/vulkan_graphics_context.h"

namespace hp
{
	class vulkan_renderer
	{
	public:
		void init();
		void shutdown();

		void draw_frame();

		static vulkan_renderer& get_instance();

	private:
		vulkan_graphics_context* m_context;
		VkSemaphore m_image_available_semaphore;
		VkSemaphore m_render_finished_semaphore;
		VkFence m_in_flight_fence;

		void create_sync_objects();
	};
} // namespace hp

#endif //PLAYGROUND_VULKAN_RENDERER_H
