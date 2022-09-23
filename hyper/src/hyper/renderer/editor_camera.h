#ifndef HYPER_EDITOR_CAMERA_H
#define HYPER_EDITOR_CAMERA_H

#include "hyper/renderer/camera.h"
#include "hyper/events/event.h"
#include "hyper/events/mouse_event_args.h"

#include <glm/glm.hpp>

namespace hp
{
	
	class editor_camera : public camera
	{
	 public:
		editor_camera(float fov, float aspectRatio, float nearClip, float farClip);
		
		void on_update(uint64_t ticks);
		
		[[nodiscard]] inline float get_distance() const;
		inline void set_distance(float distance);
		inline void set_viewport_size(float width, float height);
		[[nodiscard]] const glm::mat4& get_view_matrix() const;
		[[nodiscard]] glm::mat4 get_view_projection() const;
		[[nodiscard]] glm::vec3 get_up_direction() const;
		[[nodiscard]] glm::vec3 get_right_direction() const;
		[[nodiscard]] glm::vec3 get_forward_direction() const;
		[[nodiscard]] const glm::vec3& get_position() const;
		[[nodiscard]] glm::quat get_orientation() const;
		[[nodiscard]] float get_pitch() const;
		[[nodiscard]] float get_yaw() const;
	 
	 private:
		
		glm::mat4 m_view_matrix;
		glm::vec3 m_position;
		glm::vec3 m_focal_point;
		glm::vec2 m_initial_mouse_position;
		float     m_fov             = 45.0F;
		float     m_aspect_ratio    = 1.778F;
		float     m_near_clip       = 0.1F;
		float     m_far_clip        = 1000.0F;
		float     m_distance        = 10.0F;
		float     m_pitch;
		float     m_yaw;
		float     m_viewport_width  = 1280;
		float     m_viewport_height = 720;
		
		void update_projection();
		void update_view();
		
		void on_mouse_scroll(mouse_scrolled_event_args& args);
		
		void mouse_pan(const glm::vec2& delta);
		void mouse_rotate(const glm::vec2& delta);
		void mouse_zoom(float delta);
		
		[[nodiscard]] glm::vec3 calculate_position() const;
		[[nodiscard]] std::pair<float, float> pan_speed() const;
		[[nodiscard]] float rotation_speed() const;
		[[nodiscard]] float zoom_speed() const;
	};
}  // namespace hp

#endif
