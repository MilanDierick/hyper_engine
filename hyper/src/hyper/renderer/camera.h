#ifndef HYPER_CAMERA_H
#define HYPER_CAMERA_H

#include <glm/glm.hpp>

namespace hp
{
	class camera
	{
	 public:
		explicit camera(const glm::mat4& projection) : m_projection(projection)
		{
		}
		
		virtual ~camera() = default;
		
		camera(const camera& other) = delete;
		camera(camera&& other) noexcept = delete;
		camera& operator=(const camera& other) = delete;
		camera& operator=(camera&& other) = delete;
		
		[[nodiscard]] const glm::mat4& get_projection() const
		{
			return m_projection;
		}
	 
	 protected:
		glm::mat4 m_projection;
	};
}  // namespace hp

#endif