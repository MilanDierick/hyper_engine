// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "playground_layer.h"

namespace pg
{
	playground_layer::playground_layer()
		: layer("Playground"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	
	}
	
	void playground_layer::on_attach()
	{
		m_CheckerboardTexture = hp::texture_2d::Create("assets/textures/Checkerboard.png");
	}
	
	void playground_layer::on_detach()
	{
	
	}
	
	void playground_layer::on_update(uint64_t ticks)
	{
		// Update
		m_CameraController.on_update(ticks);
	}
	
	void playground_layer::on_render()
	{
		static float rotation = 0.0f;
		rotation += 0 * 50.0f;
		
		// Render
		hp::renderer_2d::reset_stats();
		hp::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
		hp::render_command::clear();
		
		hp::renderer_2d::begin_scene(m_CameraController.GetCamera());
		hp::renderer_2d::draw_rotated_quad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		hp::renderer_2d::draw_quad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		hp::renderer_2d::draw_quad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		hp::renderer_2d::draw_quad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		hp::renderer_2d::draw_rotated_quad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
		hp::renderer_2d::end_scene();
		
		hp::renderer_2d::begin_scene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				hp::renderer_2d::draw_quad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		hp::renderer_2d::end_scene();
	}
	
	void playground_layer::om_imgui_render()
	{
	
	}
} // hp