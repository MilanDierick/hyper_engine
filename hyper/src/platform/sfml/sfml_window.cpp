// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "sfml_window.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <hyper/core/input.h>
#include <hyper/core/key_codes.h>
#include <hyper/events/key_event_args.h>
#include <SFML/Window.hpp>

namespace hp
{
	static bool s_glfw_initialized = false;

	static void glfw_error_callback(const int32_t error, const char* description)
	{
		log::error("GLFW error ({0}): {1}", error, description);
	}

	window* window::create(const window_parameters& parameters)
	{
		return new sfml_window(parameters);
	}

	sfml_window::sfml_window(const window_parameters& parameters)
	{
		init(parameters);
	}

	sfml_window::~sfml_window()
	{
		shutdown();
	}

	void sfml_window::on_update()
	{
		glfwPollEvents();

		m_p_window->clear(sf::Color::Black);
		m_p_window->display();
	}

	uint32_t sfml_window::get_width() const
	{
		return m_data.width;
	}

	uint32_t sfml_window::get_height() const
	{
		return m_data.height;
	}

	void sfml_window::set_vsync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(0);
		}
		else
		{
			m_data.vsync = enabled;
		}
	}

	bool sfml_window::get_vsync() const
	{
		return m_data.vsync;
	}

	void* sfml_window::get_native_window() const
	{
		return m_p_native_window;
	}

	void sfml_window::init(const window_parameters& parameters)
	{
		m_data.title                  = parameters.title;
		m_data.width                  = parameters.width;
		m_data.height                 = parameters.height;
		m_data.p_window_resized_event = &window_resized_event;
		m_data.p_window_closed_event  = &window_closed_event;

		log::info("Creating window {0} ({1} {2})", parameters.title, parameters.width, parameters.height);

		if (!s_glfw_initialized)
		{
			const int32_t success = glfwInit();
			HP_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(glfw_error_callback);
			s_glfw_initialized = true;
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
		glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_p_native_window = glfwCreateWindow(parameters.width, parameters.height, parameters.title, nullptr, nullptr);

		//m_context = std::make_unique<opengl_context>(m_p_window);
		//m_context->init();

		glfwSetWindowUserPointer(m_p_native_window, &m_data);
		set_vsync(false);

		glfwSetWindowSizeCallback(
		        m_p_native_window,
		        [](GLFWwindow* window, const int width, const int height)
		        {
			        auto* data   = static_cast<window_data*>(glfwGetWindowUserPointer(window));
			        data->width  = width;
			        data->height = height;

			        const window_resized_event_args eventArgs(width, height);
			        data->p_window_resized_event->invoke(eventArgs);
		        });

		glfwSetWindowCloseCallback(
		        m_p_native_window,
		        [](GLFWwindow* window)
		        {
			        auto* data = static_cast<window_data*>(glfwGetWindowUserPointer(window));

			        const window_closed_event_args eventArgs;
			        data->p_window_closed_event->invoke(eventArgs);
		        });

		glfwSetKeyCallback(
		        m_p_native_window,
		        [](GLFWwindow*, const int key, int, const int action, int)
		        {
			        switch (action)
			        {
				        case GLFW_PRESS:
				        {
					        const key_pressed_event_args eventArgs(static_cast<key_code>(key), 0);

					        input::key_pressed_event.invoke(eventArgs);
					        break;
				        }
				        case GLFW_RELEASE:
				        {
					        const key_released_event_args eventArgs(static_cast<key_code>(key));
					        input::key_released_event.invoke(eventArgs);
					        break;
				        }
				        case GLFW_REPEAT:
				        {
					        const key_typed_event_args eventArgs(static_cast<key_code>(key));
					        input::key_typed_event.invoke(eventArgs);
					        break;
				        }
				        default:
					        log::error("Tried to process missing GLFW key action");
					        break;
			        }
		        });

		glfwSetCharCallback(
		        m_p_native_window,
		        [](GLFWwindow*, const unsigned int keycode)
		        {
			        const key_typed_event_args eventArgs(static_cast<key_code>(keycode));

			        input::key_typed_event.invoke(eventArgs);
		        });

		glfwSetMouseButtonCallback(
		        m_p_native_window,
		        [](GLFWwindow*, const int button, const int action, int)
		        {
			        switch (action)
			        {
				        case GLFW_PRESS:
				        {
					        const mouse_button_pressed_event_args event(static_cast<mouse_code>(button));
					        input::mouse_button_pressed_event.invoke(event);
					        break;
				        }
				        case GLFW_RELEASE:
				        {
					        const mouse_button_released_event_args event(static_cast<mouse_code>(button));
					        input::mouse_button_released_event.invoke(event);
					        break;
				        }
				        default:
					        log::error("Tried to process missing GLFW key action");
					        break;
			        }
		        });

		glfwSetScrollCallback(
		        m_p_native_window,
		        [](GLFWwindow*, const double x_offset, const double y_offset)
		        {
			        const mouse_scrolled_event_args
			                mouseScrolledEventArgs(static_cast<float>(x_offset), static_cast<float>(y_offset));
			        input::mouse_scrolled_event.invoke(mouseScrolledEventArgs);
		        });

		glfwSetCursorPosCallback(
		        m_p_native_window,
		        [](GLFWwindow*, const double x_pos, const double y_pos)
		        {
			        const mouse_moved_event_args event(static_cast<float>(x_pos), static_cast<float>(y_pos));
			        input::mouse_moved_event.invoke(event);
		        });

		sf::WindowHandle handle = glfwGetWin32Window(m_p_native_window);

		m_p_window = new sf::RenderWindow(handle, sf::ContextSettings(24, 8));
	}

	void sfml_window::shutdown() const
	{
		glfwDestroyWindow(m_p_native_window);
	}
} // namespace hp
