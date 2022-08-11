// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "application.h"
#include "log.h"

namespace hyper
{
	application* application::m_instance = nullptr;

	application::application()
	{
		m_instance = this;
	}

	void application::execute()
	{
	}

	void application::terminate()
	{
		log::info("Application termination requested...");
		m_is_running = false;
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