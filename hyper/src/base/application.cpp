// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "application.h"
#include "log.h"

namespace hyper
{
	application* application::m_instance = nullptr;

	application::application() : m_is_running(true), m_settings_path("settings.json")//, m_settings()
	{
		m_instance = this;
		//m_settings.read_settings_from_file(m_settings_path);
	}

	void application::execute()
	{
	
	}

	void application::terminate()
	{
		log::info("application termination requested...");
		m_is_running = false;
		//m_settings.write_settings_to_file(m_settings_path); // settings changed after this point won't be saved.
	}

	application* application::instance()
	{
		return m_instance;
	}

	application* application::instance() const
	{
		return m_instance;
	}
}  // namespace hyper