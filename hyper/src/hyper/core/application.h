// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_APPLICATION_H
#define HYPER_APPLICATION_H

#include "hyper/core/config.h"
#include "hyper/core/layer.h"
#include "hyper/core/layer_stack.h"
#include "hyper/core/settings.h"
#include "hyper/core/window.h"
#include "hyper/events/application_event_args.h"

#include <string>

namespace hp
{
	/*
	 * @brief The application class.
	 *
	 * @note The application class is the base class for all applications. It is responsible for managing the application's window and layers.
	 * @note The application class is a singleton class. Only one instance of the application class can exist at a time.
	 */
	class HP_API application
	{
	public:
		application();
		~application();

		application(const application& other) = delete;
		application(application&& other) noexcept = delete;
		application& operator=(const application& other) = delete;
		application& operator=(application&& other) = delete;

		/*
		 * @brief Executes the application.
		 *
		 * @note This function is responsible for running the application.
		 */
		void execute();

		/*
		 * @brief Terminates the application.
		 *
		 * @note This function is responsible for terminating the application.
		 */
		void terminate();

		/*
		 * @brief Pushes a layer to the application.
		 *
		 * @param layer The layer to push.
		 *
		 * @note This function is responsible for pushing a layer to the application.
		 * @note The layer is pushed to the top of the layer stack.
		 * @note The layer is initialized when it is pushed to the application.
		 * @note The layer is terminated when it is popped from the application.
		 */
		void push_layer(layer* layer);

		/*
		 * @brief Pushes an overlay to the application.
		 *
		 * @param overlay The overlay to push.
		 *
		 * @note This function is responsible for pushing an overlay to the application.
		 * @note The overlay is pushed to the top of the layer stack.
		 * @note The overlay is initialized when it is pushed to the application.
		 * @note The overlay is terminated when it is popped from the application.
		 */
		void push_overlay(layer* layer);

		/*
		 * @brief Gets the application's window.
		 *
		 * @return The application's window.
		 */
		[[nodiscard]] window& get_window() const;

		/*
		 * @brief Gets an instance of the application.
		 *
		 * @return An instance of the application.
		 */
		[[nodiscard]] static application* instance();

	private:
		/*
		 * @brief Event handler for the application's window close event.
		 *
		 * @param args The event arguments.
		 */
		void on_window_closed_event(window_closed_event_args args);

		/*
		 * @brief Event handler for the application's window resize event.
		 *
		 * @param args The event arguments.
		 */
		void on_window_resized_event(window_resized_event_args args);

		static application* m_instance;
		bool m_is_running;
		bool m_minimized;
		std::string m_settings_path;
		settings m_settings;
		layer_stack m_layerstack;
		std::unique_ptr<window> m_window;
	};

	/*
	 * @brief Creates an application.
	 *
	 * @return An application.
	 *
	 * @note This function is implemented by the client application.
	 */
	extern application* create_application();
} // namespace hp

#endif //HYPER_APPLICATION_H
