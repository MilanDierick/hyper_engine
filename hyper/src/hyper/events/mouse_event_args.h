// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_MOUSE_EVENT_ARGS_H
#define PLAYGROUND_MOUSE_EVENT_ARGS_H

#include "hyper/core/mouse_codes.h"
#include "event.h"

namespace hp
{
	struct mouse_moved_event_args : event_args
	{
		[[nodiscard]] mouse_moved_event_args(const float mouse_x, const float mouse_y)
			: mouse_x(mouse_x),
			  mouse_y(mouse_y)
		{
		}
		
		float mouse_x, mouse_y;
	};
	
	struct mouse_scrolled_event_args : event_args
	{
		[[nodiscard]] mouse_scrolled_event_args(const float x_offset, const float y_offset)
			: x_offset(x_offset),
			  y_offset(y_offset)
		{
		}
		
		float x_offset, y_offset;
	};
	
	struct mouse_button_pressed_event_args : event_args
	{
		[[nodiscard]] explicit mouse_button_pressed_event_args(const mouse_code button)
			: button(button)
		{
		}
		
		mouse_code button;
	};
	
	struct mouse_button_released_event_args : event_args
	{
		[[nodiscard]] explicit mouse_button_released_event_args(const mouse_code button)
			: button(button)
		{
		}
		
		mouse_code button;
	};
}  // namespace hp

#endif //PLAYGROUND_MOUSE_EVENT_ARGS_H
