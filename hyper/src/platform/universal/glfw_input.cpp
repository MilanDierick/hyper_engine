// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include <application.h>
#include "hyper/core/input.h"
#include <cmath>

namespace hp
{
	event<key_pressed_event_args> input::key_pressed_event = event<key_pressed_event_args>();
	event<key_released_event_args> input::key_released_event = event<key_released_event_args>();
	event<key_typed_event_args> input::key_typed_event = event<key_typed_event_args>();
	event<mouse_button_pressed_event_args> input::mouse_button_pressed_event = event<mouse_button_pressed_event_args>();
	event<mouse_button_released_event_args> input::mouse_button_released_event = event<mouse_button_released_event_args>();
	event<mouse_moved_event_args> input::mouse_moved_event = event<mouse_moved_event_args>();
	event<mouse_scrolled_event_args> input::mouse_scrolled_event = event<mouse_scrolled_event_args>();

	bool input::is_key_pressed(key_code key)
	{
		auto* window = static_cast<GLFWwindow*>(application::instance()->get_window().get_native_window());
		const int state = glfwGetKey(window, static_cast<int>(key));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool input::is_mouse_button_pressed(mouse_code button)
	{
		auto* window = static_cast<GLFWwindow*>(application::instance()->get_window().get_native_window());
		const int state = glfwGetMouseButton(window, static_cast<int>(button));

		return state == GLFW_PRESS;
	}

	std::pair<float, float> input::get_mouse_position()
	{
		auto* window = static_cast<GLFWwindow*>(application::instance()->get_window().get_native_window());
		double x_pos = NAN;
		double y_pos = NAN;
		glfwGetCursorPos(window, &x_pos, &y_pos);

		return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
	}

	float input::get_mouse_x()
	{
		auto [x, y] = get_mouse_position();

		return x;
	}

	float input::get_mouse_y()
	{
		auto [x, y] = get_mouse_position();

		return y;
	}
}
