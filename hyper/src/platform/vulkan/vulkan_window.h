// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_VULKAN_WINDOW_H
#define HYPER_VULKAN_WINDOW_H

#include "hyper/core/window.h"
#include "hyper/events/application_event_args.h"

#include <vulkan/vulkan.h>

namespace hp
{
	struct HP_API window_data
	{
		const char* title;
		uint32_t width;
		uint32_t height;
		bool vsync;

		event<window_resized_event_args>* p_window_resized_event;
		event<window_closed_event_args>* p_window_closed_event;
	};

	class vulkan_window : public window
	{
	public:
		explicit vulkan_window(const window_parameters& parameters);
		~vulkan_window() override;

		vulkan_window(const vulkan_window& other)            = delete;
		vulkan_window(vulkan_window&& other) noexcept        = delete;
		vulkan_window& operator=(const vulkan_window& other) = delete;
		vulkan_window& operator=(vulkan_window&& other)      = delete;

		void on_update() override;

		[[nodiscard]] uint32_t get_width() const override;
		[[nodiscard]] uint32_t get_height() const override;
		void set_vsync(bool enabled) override;
		[[nodiscard]] bool get_vsync() const override;
		[[nodiscard]] void* get_native_window() const override;

	private:
		window_data m_data;
		GLFWwindow* m_window;
		VkInstance m_vulkan_instance;
		VkDebugUtilsMessengerEXT m_vulkan_debug_messenger;
		//std::unique_ptr<graphics_context> m_context;

		void init(const window_parameters& parameters);
		void shutdown() const;

		void init_glfw(const window_parameters& parameters);

		void init_vulkan();
		void create_vulkan_instance();
		std::vector<const char*> get_required_vulkan_extensions();

#ifdef HP_VULKAN_VALIDATION_LAYERS
		bool check_vulkan_validation_layer_support();
		void setup_vulkan_debug_messenger();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
		        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		        VkDebugUtilsMessageTypeFlagsEXT message_type,
		        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
		        void* p_user_data);
#endif
	};
} // namespace hp

#endif //HYPER_VULKAN_WINDOW_H
