// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_APPLICATION_EVENT_ARGS_H
#define PLAYGROUND_APPLICATION_EVENT_ARGS_H

#include "event.h"

namespace hp
{
	struct window_resized_event_args : public event_args
	{
		[[nodiscard]] window_resized_event_args(const unsigned width, const unsigned height)
			: width(width), height(height)
		{
		}
		
		unsigned int width, height;
	};
	
	struct window_closed_event_args : event_args
	{
	};
	
	struct app_tick_event_args : event_args
	{
	};
	
	struct app_updated_event_args : event_args
	{
	};
	
	struct app_render_event_args : event_args
	{
	};
}  // namespace hp

#endif //PLAYGROUND_APPLICATION_EVENT_ARGS_H
