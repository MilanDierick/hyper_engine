// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_APPLICATION_H
#define HYPER_APPLICATION_H

#include "Config"
#include "Settings.h"

namespace hyper
{
	class HP_API Application
	{
	public:
		Application();
		~Application() = default;

		Application(const Application& other) = delete;
		Application(Application&& other) noexcept = delete;
		Application& operator=(const Application& other) = delete;
		Application& operator=(Application&& other) = delete;
		
		void execute();
		void terminate();

		[[nodiscard]] Application* instance();
		[[nodiscard]] Application* instance() const;

	private:
		static Application* m_instance;
		bool m_is_running;
		std::string m_settings_path;
		Settings m_settings;
	};

	extern Application* create_application();
} // namespace hyper

#endif //HYPER_APPLICATION_H
