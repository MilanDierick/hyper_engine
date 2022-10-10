// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_INPUT_H
#define HYPER_INPUT_H

#include "hyper/core/config.h"
#include "hyper/core/key_codes.h"
#include "hyper/core/mouse_codes.h"
#include "hyper/events/event.h"
#include "hyper/events/key_event_args.h"
#include "hyper/events/mouse_event_args.h"

namespace hp
{
	class HP_API input final
	{
	 public:
		static event<key_pressed_event_args> key_pressed_event;
		static event<key_released_event_args> key_released_event;
		static event<key_typed_event_args> key_typed_event;
		static event<mouse_button_pressed_event_args> mouse_button_pressed_event;
		static event<mouse_button_released_event_args> mouse_button_released_event;
		static event<mouse_moved_event_args> mouse_moved_event;
		static event<mouse_scrolled_event_args> mouse_scrolled_event;
		
		static bool is_key_pressed(key_code key);
		static bool is_mouse_button_pressed(mouse_code button);
		static std::pair<float, float> get_mouse_position();
		static float get_mouse_x();
		static float get_mouse_y();
	};
}

#endif //HYPER_INPUT_H
