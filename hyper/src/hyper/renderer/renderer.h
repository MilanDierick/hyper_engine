#ifndef HYPER_RENDERER_H
#define HYPER_RENDERER_H

#include "hyper/renderer/orthographic_camera.h"
#include "hyper/renderer/render_command.h"
#include "hyper/renderer/shader.h"

namespace hp {

	class renderer
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(orthographic_camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<shader>& shader, const std::shared_ptr<vertex_array>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static renderer_api::API get_api() { return renderer_api::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static std::unique_ptr<SceneData> s_SceneData;
	};
}

#endif