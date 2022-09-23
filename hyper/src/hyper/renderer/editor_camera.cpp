#include "editor_camera.h"

#include "hyper/core/input.h"
#include "hyper/core/key_codes.h"
#include "hyper/core/mouse_codes.h"

#include "GLFW/glfw3.h"

#include <glm/gtx/quaternion.hpp>

namespace hp
{
	
	editor_camera::editor_camera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_fov(fov),
		  m_aspect_ratio(aspectRatio),
		  m_near_clip(nearClip),
		  m_far_clip(farClip),
		  m_view_matrix(),
		  m_position(),
		  m_focal_point(),
		  m_initial_mouse_position(),
		  m_pitch(),
		  m_yaw(),
		  camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		update_view();
	}
	
	void editor_camera::update_projection()
	{
		m_aspect_ratio = m_viewport_width / m_viewport_height;
		m_projection   = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_clip, m_far_clip);
	}
	
	void editor_camera::update_view()
	{
		// m_yaw = m_pitch = 0.0f; // Lock the camera's rotation
		m_position = calculate_position();
		
		glm::quat orientation = get_orientation();
		m_view_matrix = glm::translate(glm::mat4(1.0F), m_position) * glm::toMat4(orientation);
		m_view_matrix = glm::inverse(m_view_matrix);
	}
	
	std::pair<float, float> editor_camera::pan_speed() const
	{
		float x_coord  = std::min(m_viewport_width / 1000.0F, 2.4F); // max = 2.4f
		float x_factor = 0.0366F * (x_coord * x_coord) - 0.1778F * x_coord + 0.3021F;
		
		float y_coord  = std::min(m_viewport_height / 1000.0F, 2.4F); // max = 2.4f
		float y_factor = 0.0366F * (y_coord * y_coord) - 0.1778F * y_coord + 0.3021F;
		
		return { x_factor, y_factor };
	}
	
	float editor_camera::rotation_speed() const
	{
		return 0.8F;
	}
	
	float editor_camera::zoom_speed() const
	{
		float distance = m_distance * 0.2F;
		distance = std::max(distance, 0.0F);
		float speed = distance * distance;
		speed = std::min(speed, 100.0F); // max speed = 100
		return speed;
	}
	
	void editor_camera::on_update(uint64_t ticks)
	{
		UNUSED(ticks);
		
		if (input::is_key_pressed(key_code::LeftAlt))
		{
			const glm::vec2& mouse{ input::get_mouse_x(), input::get_mouse_y() };
			glm::vec2 delta = (mouse - m_initial_mouse_position) * 0.003F;
			m_initial_mouse_position = mouse;
			
			if (input::is_mouse_button_pressed(mouse_code::ButtonMiddle))
			{
				mouse_pan(delta);
			}
			else if (input::is_mouse_button_pressed(mouse_code::ButtonLeft))
			{
				mouse_rotate(delta);
			}
			else if (input::is_mouse_button_pressed(mouse_code::ButtonRight))
			{
				mouse_zoom(delta.y);
			}
		}
		
		update_view();
	}
	
	void editor_camera::on_mouse_scroll(mouse_scrolled_event_args& args)
	{
		float delta = args.y_offset * 0.1F;
		mouse_zoom(delta);
		update_view();
	}
	
	void editor_camera::mouse_pan(const glm::vec2& delta)
	{
		auto [x_speed, y_speed] = pan_speed();
		m_focal_point += -get_right_direction() * delta.x * x_speed * m_distance;
		m_focal_point += get_up_direction() * delta.y * y_speed * m_distance;
	}
	
	void editor_camera::mouse_rotate(const glm::vec2& delta)
	{
		float yaw_sign = get_up_direction().y < 0 ? -1.0F : 1.0F;
		m_yaw += yaw_sign * delta.x * rotation_speed();
		m_pitch += delta.y * rotation_speed();
	}
	
	void editor_camera::mouse_zoom(float delta)
	{
		m_distance -= delta * zoom_speed();
		if (m_distance < 1.0F)
		{
			m_focal_point += get_forward_direction();
			m_distance = 1.0F;
		}
	}
	
	glm::vec3 editor_camera::calculate_position() const
	{
		return m_focal_point - get_forward_direction() * m_distance;
	}
	
	glm::quat editor_camera::get_orientation() const
	{
		return { glm::vec3(-m_pitch, -m_yaw, 0.0F) };
	}
	
	float editor_camera::get_distance() const
	{
		return m_distance;
	}
	
	void editor_camera::set_distance(float distance)
	{
		m_distance = distance;
	}
	
	void editor_camera::set_viewport_size(float width, float height)
	{
		m_viewport_width  = width;
		m_viewport_height = height;
		update_projection();
	}
	
	const glm::mat4& editor_camera::get_view_matrix() const
	{
		return m_view_matrix;
	}
	
	glm::mat4 editor_camera::get_view_projection() const
	{
		return m_projection * m_view_matrix;
	}
	
	glm::vec3 editor_camera::get_up_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0F, 1.0F, 0.0F));
	}
	
	glm::vec3 editor_camera::get_right_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(1.0F, 0.0F, 0.0F));
	}
	
	glm::vec3 editor_camera::get_forward_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0F, 0.0F, -1.0F));
	}
	
	const glm::vec3& editor_camera::get_position() const
	{
		return m_position;
	}
	
	float editor_camera::get_pitch() const
	{
		return m_pitch;
	}
	
	float editor_camera::get_yaw() const
	{
		return m_yaw;
	}
	
}
