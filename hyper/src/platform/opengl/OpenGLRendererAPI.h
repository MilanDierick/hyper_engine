#ifndef HYPER_RENDERER_API
#define HYPER_RENDERER_API

#include "hyper/renderer/renderer_api.h"

namespace hp
{
	class OpenGLRendererAPI : public renderer_api
	{
	 public:
		virtual void init() override;
		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		
		virtual void set_clear_color(const glm::vec4& color) override;
		virtual void clear() override;
		
		virtual void draw_indexed(const std::shared_ptr<vertex_array>& vertexArray, uint32_t indexCount = 0) override;
		virtual void draw_lines(const std::shared_ptr<vertex_array>& vertexArray, uint32_t vertexCount) override;
		
		virtual void set_line_width(float width) override;
	};
}  // namespace hp

#endif