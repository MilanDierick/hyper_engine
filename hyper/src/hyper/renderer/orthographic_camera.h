#ifndef HYPER_ORTHOGRAPHIC_CAMERA_H
#define HYPER_ORTHOGRAPHIC_CAMERA_H

#include <glm/glm.hpp>

namespace hp
{
	class orthographic_camera
	{
	public:
		orthographic_camera(float left, float right, float bottom, float top);

		void set_projection(float left, float right, float bottom, float top);
		[[nodiscard]] const glm::vec3& get_position() const;
		void set_position(const glm::vec3& position);
		[[nodiscard]] float get_rotation() const;
		void set_rotation(float rotation);
		[[nodiscard]] const glm::mat4& get_projection_matrix() const;
		[[nodiscard]] const glm::mat4& get_view_matrix() const;
		[[nodiscard]] const glm::mat4& get_view_projection_matrix() const;

	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix;

		glm::vec3 m_position = {0.0F, 0.0F, 0.0F};
		float m_rotation     = 0.0F;

		void recalculate_view_matrix();
	};

} // namespace hp

#endif //HYPER_ORTHOGRAPHIC_CAMERA_H