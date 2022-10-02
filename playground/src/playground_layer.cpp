// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "playground_layer.h"

namespace pg
{
	playground_layer::playground_layer()
		: layer("Playground"), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
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
	}
	
	void playground_layer::on_render()
	{
		static float rotation = 0.0f;
		rotation += 0 * 50.0f;
	}
	
	void playground_layer::om_imgui_render()
	{
	}
} // hp