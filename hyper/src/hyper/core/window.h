// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_WINDOW_H
#define PLAYGROUND_WINDOW_H

#include "hyper/events/event.h"
#include "hyper/events/application_event_args.h"

namespace hp
{
	struct window_parameters
	{
		const char* title;
		uint32_t width;
		uint32_t height;
		
		window_parameters();
		window_parameters(const char* title, uint32_t width, uint32_t height);
	};
	
	class HP_API window
	{
	 public:
		window() = default;
		virtual ~window() = default;
		
		window(const window& other) = delete;
		window(window&& other) noexcept = delete;
		window& operator=(const window& other) = delete;
		window& operator=(window&& other) = delete;
		
		event<window_resized_event_args> window_resized_event;
		event<window_closed_event_args> window_closed_event;
		
		virtual void on_update() = 0;
		[[nodiscard]] virtual uint32_t get_width() const = 0;
		[[nodiscard]] virtual uint32_t get_height() const = 0;
		virtual void set_vsync(bool enabled) = 0;
		[[nodiscard]] virtual bool get_vsync() const = 0;
		[[nodiscard]] virtual void* get_native_window() const = 0;
		
		static window* create(const window_parameters& parameters = window_parameters());
	};
}

#endif //PLAYGROUND_WINDOW_H
