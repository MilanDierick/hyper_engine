// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_APPLICATION_H
#define HYPER_APPLICATION_H

#include "core/config.h"
#include "core/settings.h"
#include "core/window.h"
#include "events/application_event_args.h"
#include <string>

namespace hp
{
	class HP_API application
	{
	public:
		application();
		~application() = default;

		application(const application& other) = delete;
		application(application&& other) noexcept = delete;
		application& operator=(const application& other) = delete;
		application& operator=(application&& other) = delete;
		
		void execute();
		void terminate();

		[[nodiscard]] window& get_window() const;
		
		[[nodiscard]] static application* instance();

	private:
		void on_window_closed_event(window_closed_event_args args);
		void on_window_resized_event(window_resized_event_args args);
		
		static application* m_instance;
		bool m_is_running;
		bool m_minimized;
		std::string m_settings_path;
		settings m_settings;
		std::unique_ptr<window> m_window;
	};

	extern application* create_application();
} // namespace hp

#endif //HYPER_APPLICATION_H
