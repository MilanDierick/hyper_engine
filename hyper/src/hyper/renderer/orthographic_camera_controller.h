#ifndef HYPER_ORTHOGRAPHIC_CAMERA_CONTROLLER_H
#define HYPER_ORTHOGRAPHIC_CAMERA_CONTROLLER_H

#include "hyper/events/application_event_args.h"
#include "hyper/events/mouse_event_args.h"
#include "hyper/renderer/orthographic_camera.h"

namespace hp
{
	class HP_API orthographic_camera_controller
	{
	public:
		explicit orthographic_camera_controller(float aspect_ratio, bool rotation = false);

		void on_update(uint64_t ticks);
		void on_resize(float width, float height);

		orthographic_camera& get_camera() { return m_camera; }
		[[nodiscard]] const orthographic_camera& get_camera() const { return m_camera; }

		[[nodiscard]] float get_zoom_level() const { return m_zoom_level; }
		void set_zoom_level(float level) { m_zoom_level = level; }

	private:
		float m_aspect_ratio;
		float m_zoom_level = 1.0F;
		orthographic_camera m_camera;

		bool m_rotation;

		glm::vec3 m_camera_position      = {0.0F, 0.0F, 0.0F};
		float m_camera_rotation          = 0.0F; //In degrees, in the anti-clockwise direction
		float m_camera_translation_speed = 5.0F, m_camera_rotation_speed = 180.0F;

		void on_mouse_scrolled(mouse_scrolled_event_args args);
		void on_window_resized(window_resized_event_args args);
	};
} // namespace hp

#endif //HYPER_ORTHOGRAPHIC_CAMERA_CONTROLLER_H