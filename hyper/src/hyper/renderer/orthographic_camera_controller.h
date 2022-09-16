#ifndef HYPER_ORTHOGRAPHIC_CAMERA_CONTROLLER_H
#define HYPER_ORTHOGRAPHIC_CAMERA_CONTROLLER_H

#include "hyper/events/application_event_args.h"
#include "hyper/events/mouse_event_args.h"
#include "hyper/renderer/OrthographicCamera.h"

namespace hp {

	class HP_API orthographic_camera_controller
	{
	public:
		explicit orthographic_camera_controller(float aspect_ratio, bool rotation = false);

		void on_update(uint64_t ticks);
		void on_resize(float width, float height);

		OrthographicCamera& GetCamera() { return m_camera; }
		[[nodiscard]] const OrthographicCamera& GetCamera() const { return m_camera; }

		[[nodiscard]] float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0F;
		OrthographicCamera m_camera;

		bool m_Rotation;

		glm::vec3 m_camera_position = { 0.0F, 0.0F, 0.0F };
		float m_camera_rotation = 0.0F; //In degrees, in the anti-clockwise direction
		float m_camera_translation_speed = 5.0F, m_CameraRotationSpeed = 180.0F;
		
		void on_mouse_scrolled(mouse_scrolled_event_args args);
		void on_window_resized(window_resized_event_args args);
	};
}  // namespace hp

#endif //HYPER_ORTHOGRAPHIC_CAMERA_CONTROLLER_H