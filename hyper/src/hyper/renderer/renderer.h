#ifndef HYPER_RENDERER_H
#define HYPER_RENDERER_H

#include "hyper/renderer/orthographic_camera.h"
#include "hyper/renderer/render_command.h"
#include "hyper/renderer/shader.h"

namespace hp
{

	class renderer
	{
	public:
		static void init();
		static void shutdown();

		static void on_window_resize(uint32_t width, uint32_t height);

		static void begin_scene(orthographic_camera& camera);
		static void end_scene();

		static void submit(const std::shared_ptr<shader>& shader, const std::shared_ptr<vertex_array>& vertex_array, const glm::mat4& transform = glm::mat4(1.0f));

		static renderer_api::API get_api() { return renderer_api::get_api(); }

	private:
		struct scene_data
		{
			glm::mat4 view_projection_matrix;
		};

		static std::unique_ptr<scene_data> s_scene_data;
	};
} // namespace hp

#endif