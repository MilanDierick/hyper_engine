// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "Application.h"
#include "Log.h"

namespace hyper
{
	Application* Application::m_instance = nullptr;

	Application::Application() : m_is_running(true), m_settings_path("settings.json"), m_settings()
	{
		m_instance = this;
		m_settings.read_settings_from_file(m_settings_path);
	}

	void Application::execute()
	{
	
	}

	void Application::terminate()
	{
		Log::info("Application termination requested...");
		m_is_running = false;
		m_settings.write_settings_to_file(m_settings_path); // Settings changed after this point won't be saved.
	}

	Application* Application::instance()
	{
		return m_instance;
	}

	Application* Application::instance() const
	{
		return m_instance;
	}
}  // namespace hyper