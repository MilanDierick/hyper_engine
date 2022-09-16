// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include <hyper/renderer/renderer.h>
#include "hyper/core/timer.h"
#include "hyper/core/application.h"
#include "hyper/core/log.h"

#define MS_PER_TICK 1000.0F / 144.0F

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
		
		renderer::Init();
	}
	
	application::~application()
	{
		renderer::Shutdown();
	}
	
	void application::execute()
	{
		std::chrono::steady_clock::time_point previousTimePoint = std::chrono::steady_clock::now();
		double lag = 0.0;
		
		while (m_is_running)
		{
			const std::chrono::time_point<std::chrono::steady_clock>
				currentTimePoint = std::chrono::steady_clock::now();
			
			const auto elapsedTime =
				std::chrono::time_point_cast<std::chrono::milliseconds>(currentTimePoint).time_since_epoch()
					- std::chrono::time_point_cast<std::chrono::milliseconds>(previousTimePoint).time_since_epoch();
			
			previousTimePoint = currentTimePoint;
			
			lag += elapsedTime.count();
			
			m_window->on_update();
			
//			if (m_minimized)
//			{
//				continue;
//			}
			
			while (lag >= MS_PER_TICK)
			{
				for (layer* layer : m_layerstack)
				{
					layer->on_update(static_cast<uint64_t>((1000 / MS_PER_TICK)));
				}
				
				lag -= MS_PER_TICK;
			}
			
			for (layer* layer : m_layerstack)
			{
				layer->on_render();
			}
		}
	}
	
	void application::terminate()
	{
		log::info("application termination requested...");
		m_is_running = false;
		m_settings.write_settings_to_file(m_settings_path); // settings changed after this point won't be saved.
	}
	
	void application::push_layer(layer* layer)
	{
		m_layerstack.push_layer(layer);
		layer->on_attach();
	}
	
	void application::push_overlay(layer* layer)
	{
		m_layerstack.push_overlay(layer);
		layer->on_attach();
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