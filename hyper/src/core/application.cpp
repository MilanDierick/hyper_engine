// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "core/application.h"
#include "core/log.h"

namespace hp
{
	application* application::m_instance = nullptr;

	application::application() : m_is_running(true), m_settings_path("settings.json"), m_settings()
	{
		m_instance = this;
		m_settings.read_settings_from_file(m_settings_path);
		m_window = std::unique_ptr<window>(window::create());
		m_window->window_closed_event.bind(&application::on_window_closed_event, this);
		m_window->window_resized_event.bind(&application::on_window_resized_event, this);
	}

	void application::execute()
	{
		while (m_is_running)
		{
			m_window->on_update();
			
			if (m_minimized)
			{
				continue;
			}
		}
	}

	void application::terminate()
	{
		log::info("application termination requested...");
		m_is_running = false;
		m_settings.write_settings_to_file(m_settings_path); // settings changed after this point won't be saved.
	}
	
	window& application::get_window() const
	{
		return *m_window;
	}
	
	application* application::instance()
	{
		return m_instance;
	}
	
	void application::on_window_closed_event(window_closed_event_args args)
	{
		UNUSED(args);
		m_is_running = false;
	}
	
	void application::on_window_resized_event(window_resized_event_args args)
	{
		if (args.width == 0 || args.height == 0)
		{
			m_minimized = true;
		}
		
		m_minimized = false;
		//renderer::on_window_resize(args.width, args.height);
	}
}  // namespace hp