#include "hyper/renderer/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace hp
{
	orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
	    : m_projection_matrix(glm::ortho(left, right, bottom, top, -1.0F, 1.0F)),
	      m_view_matrix(1.0F),
	      m_view_projection_matrix(m_projection_matrix * m_view_matrix)
	{
	}

	void orthographic_camera::set_projection(float left, float right, float bottom, float top)
	{
		m_projection_matrix      = glm::ortho(left, right, bottom, top, -1.0F, 1.0F);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void orthographic_camera::recalculate_view_matrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0F), m_position) * glm::rotate(glm::mat4(1.0F), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_view_matrix            = glm::inverse(transform);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	const glm::vec3& orthographic_camera::get_position() const
	{
		return m_position;
	}

	void orthographic_camera::set_position(const glm::vec3& position)
	{
		m_position = position;
		recalculate_view_matrix();
	}

	float orthographic_camera::get_rotation() const
	{
		return m_rotation;
	}

	void orthographic_camera::set_rotation(float rotation)
	{
		m_rotation = rotation;
		recalculate_view_matrix();
	}

	const glm::mat4& orthographic_camera::get_projection_matrix() const
	{
		return m_projection_matrix;
	}

	const glm::mat4& orthographic_camera::get_view_matrix() const
	{
		return m_view_matrix;
	}

	const glm::mat4& orthographic_camera::get_view_projection_matrix() const
	{
		return m_view_projection_matrix;
	}
} // namespace hp