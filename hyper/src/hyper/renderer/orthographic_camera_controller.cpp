#include "hyper/renderer/orthographic_camera_controller.h"

#include "hyper/core/application.h"
#include "hyper/core/input.h"
#include "hyper/core/key_codes.h"

namespace hp
{
	
	orthographic_camera_controller::orthographic_camera_controller(float aspect_ratio, bool rotation) : m_AspectRatio(
		aspect_ratio), m_camera(
		-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(
		rotation)
	{
		input::mouse_scrolled_event.bind(&orthographic_camera_controller::on_mouse_scrolled, this);
		application::instance()->get_window()
		                       .window_resized_event
		                       .bind(&orthographic_camera_controller::on_window_resized, this);
		
		
	}
	
	void orthographic_camera_controller::on_update(uint64_t ticks)
	{
		if (input::is_key_pressed(key_code::A))
		{
			m_camera_position.x -=
				cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
			m_camera_position.y -=
				sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
		}
		else if (input::is_key_pressed(key_code::D))
		{
			m_camera_position.x +=
				cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
			m_camera_position.y +=
				sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
		}
		
		if (input::is_key_pressed(key_code::W))
		{
			m_camera_position.x +=
				-sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
			m_camera_position.y +=
				cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
		}
		else if (input::is_key_pressed(key_code::S))
		{
			m_camera_position.x -=
				-sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
			m_camera_position.y -=
				cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * static_cast<float>(ticks) / 1000.0F;
		}
		
		if (m_Rotation)
		{
			if (input::is_key_pressed(key_code::Q))
			{
				m_camera_rotation += m_CameraRotationSpeed * static_cast<float>(ticks);
			}
			if (input::is_key_pressed(key_code::E))
			{
				m_camera_rotation -= m_CameraRotationSpeed * static_cast<float>(ticks);
			}
			
			if (m_camera_rotation > 180.0F)
			{
				m_camera_rotation -= 360.0F;
			}
			else if (m_camera_rotation <= -180.0F)
			{
				m_camera_rotation += 360.0F;
			}
			
			m_camera.set_rotation(m_camera_rotation);
		}
		
		m_camera.set_position(m_camera_position);
		
		m_camera_translation_speed = m_ZoomLevel;
	}
	
	void orthographic_camera_controller::on_resize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_camera.set_projection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
	
	void orthographic_camera_controller::on_mouse_scrolled(mouse_scrolled_event_args args)
	{
		m_ZoomLevel -= args.y_offset * 0.25F;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25F);
		m_camera.set_projection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
	
	void orthographic_camera_controller::on_window_resized(window_resized_event_args args)
	{
		on_resize(static_cast<float>(args.width), static_cast<float>(args.height));
	}
	
}  // namespace hp