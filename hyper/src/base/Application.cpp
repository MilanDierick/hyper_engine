// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "Application.h"
#include "Log.h"

namespace hyper
{
	Application* Application::m_instance = nullptr;

	Application::Application()
	{
		m_instance = this;
	}

	void Application::execute()
	{
	}

	void Application::terminate()
	{
		Log::info("Application termination requested...");
		m_is_running = false;
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