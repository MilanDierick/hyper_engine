// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_KEY_EVENT_ARGS_H
#define HYPER_KEY_EVENT_ARGS_H

#include "event.h"
#include "hyper/core/key_codes.h"

namespace hp
{
	struct key_event_args : public event_args
	{
		[[nodiscard]] explicit key_event_args(const key_code key_code)
		    : key_code(key_code)
		{
		}

		key_code key_code;
	};
	
	struct key_pressed_event_args : event_args
	{
		[[nodiscard]] key_pressed_event_args(const key_code key_code, const int repeatCount)
		    : key_code(key_code),
		      RepeatCount(repeatCount)
		{
		}

		key_code key_code;
		int RepeatCount;
	};

	struct key_released_event_args : event_args
	{
		[[nodiscard]] explicit key_released_event_args(const key_code key_code)
		    : key_code(key_code)
		{
		}

		key_code key_code;
	};

	struct key_typed_event_args : event_args
	{
		[[nodiscard]] explicit key_typed_event_args(const key_code key_code)
		    : key_code(key_code)
		{
		}

		key_code key_code;
	};
} // namespace hp

#endif //HYPER_KEY_EVENT_ARGS_H
