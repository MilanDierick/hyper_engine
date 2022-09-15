#include "hyper/renderer/OrthographicCameraController.h"

#include "hyper/core/input.h"
#include "hyper/core/key_codes.h"

namespace hp
{
	
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : m_AspectRatio(
		aspectRatio),
	                                                                                               m_Camera(
		                                                                                               -m_AspectRatio
			                                                                                               * m_ZoomLevel,
		                                                                                               m_AspectRatio
			                                                                                               * m_ZoomLevel,
		                                                                                               -m_ZoomLevel,
		                                                                                               m_ZoomLevel),
	                                                                                               m_Rotation(rotation)
	{
	}
	
	void OrthographicCameraController::OnUpdate(uint64_t ts)
	{
		if (input::is_key_pressed(key_code::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (input::is_key_pressed(key_code::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		
		if (input::is_key_pressed(key_code::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (input::is_key_pressed(key_code::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		
		if (m_Rotation)
		{
			if (input::is_key_pressed(key_code::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (input::is_key_pressed(key_code::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			
			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;
			
			m_Camera.SetRotation(m_CameraRotation);
		}
		
		m_Camera.SetPosition(m_CameraPosition);
		
		m_CameraTranslationSpeed = m_ZoomLevel;
	}
	
//	void OrthographicCameraController::OnEvent(Event& e)
//	{
//		EventDispatcher dispatcher(e);
//		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
//		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
//	}
	
	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
	
//	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
//	{
//
//		m_ZoomLevel -= e.GetYOffset() * 0.25f;
//		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
//		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
//		return false;
//	}
	
//	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
//	{
//
//		OnResize((float)e.GetWidth(), (float)e.GetHeight());
//		return false;
//	}
	
}  // namespace hp