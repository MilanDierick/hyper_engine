// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "playground_layer.h"

namespace pg
{
	playground_layer::playground_layer()
	    : layer("Playground"),
	      m_SquareColor({0.2F, 0.3F, 0.8F, 1.0F})
	{
	}

	void playground_layer::on_attach()
	{
	}

	void playground_layer::on_detach()
	{
	}

	void playground_layer::on_update(uint64_t ticks)
	{
		UNUSED(ticks);

		if (hp::input::is_key_pressed(hp::key_code::Escape))
		{
			hp::application::instance()->terminate();
		}
	}

	void playground_layer::on_render()
	{
		static float rotation = 0.0F;
		rotation += 0 * 50.0F;
	}

	void playground_layer::om_imgui_render()
	{
	}
} // namespace pg