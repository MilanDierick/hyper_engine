// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_SFML_WINDOW_H
#define PLAYGROUND_SFML_WINDOW_H

#include "hyper/core/window.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

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

	class sfml_window : public window
	{
	public:
		explicit sfml_window(const window_parameters& parameters);
		~sfml_window() override;

		sfml_window(const sfml_window& other)            = delete;
		sfml_window(sfml_window&& other) noexcept        = delete;
		sfml_window& operator=(const sfml_window& other) = delete;
		sfml_window& operator=(sfml_window&& other)      = delete;

		void on_update() override;
		[[nodiscard]] uint32_t get_width() const override;
		[[nodiscard]] uint32_t get_height() const override;
		void set_vsync(bool enabled) override;
		[[nodiscard]] bool get_vsync() const override;
		[[nodiscard]] void* get_native_window() const override;

	private:
		window_data m_data;
		sf::RenderWindow* m_p_window;
		GLFWwindow* m_p_native_window;

		void init(const window_parameters& parameters);
		void shutdown() const;
	};
} // namespace hp

#endif //PLAYGROUND_SFML_WINDOW_H
