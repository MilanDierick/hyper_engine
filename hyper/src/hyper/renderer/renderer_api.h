#ifndef HYPER_RENDERER_API_H
#define HYPER_RENDERER_API_H

#include "hyper/renderer/vertex_array.h"

#include <glm/glm.hpp>

namespace hp
{
	class HP_API renderer_api
	{
	public:
		enum class API
		{
			none   = 0,
			vulkan = 1
		};

		virtual ~renderer_api() = default;

		virtual void init()                                                                = 0;
		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void set_clear_color(const glm::vec4& color)                               = 0;
		virtual void clear()                                                               = 0;

		virtual void draw_indexed(const std::shared_ptr<vertex_array>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void draw_lines(const std::shared_ptr<vertex_array>& vertexArray, uint32_t vertexCount)      = 0;

		virtual void set_line_width(float width) = 0;

		static API get_api()
		{
			return s_api;
		}

		static std::unique_ptr<renderer_api> create();

	private:
		static API s_api;
	};

} // namespace hp

#endif