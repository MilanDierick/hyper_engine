// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_UNIVERSAL_WINDOW_H
#define PLAYGROUND_UNIVERSAL_WINDOW_H

#include "hyper/renderer/graphics_context.h"
#include "hyper/core/window.h"
#include <GLFW/glfw3.h>

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
	
	class universal_window : public window
	{
	 public:
		explicit universal_window(const window_parameters& parameters);
		~universal_window() override;
		
		universal_window(const universal_window& other) = delete;
		universal_window(universal_window&& other) noexcept = delete;
		universal_window& operator=(const universal_window& other) = delete;
		universal_window& operator=(universal_window&& other) = delete;
		
		void on_update() override;
		
		[[nodiscard]] uint32_t get_width() const override;
		[[nodiscard]] uint32_t get_height() const override;
		void set_vsync(bool enabled) override;
		[[nodiscard]] bool get_vsync() const override;
		[[nodiscard]] void* get_native_window() const override;
	 
	 private:
		window_data m_data;
		GLFWwindow* m_window;
		std::unique_ptr<graphics_context> m_context;
		
		void init(const window_parameters& parameters);
		void shutdown() const;
	};
}

#endif //PLAYGROUND_UNIVERSAL_WINDOW_H
