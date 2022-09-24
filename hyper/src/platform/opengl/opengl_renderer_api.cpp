#include "platform/opengl/opengl_renderer_api.h"

#include <glad/glad.h>

namespace hp
{
	void APIENTRY opengl_message_callback(
		GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
	{
		UNUSED(user_param);
		UNUSED(length);
		UNUSED(id);
		UNUSED(type);
		UNUSED(source);
		
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH: log::critical(message);
				return;
			case GL_DEBUG_SEVERITY_MEDIUM: log::error(message);
				return;
			case GL_DEBUG_SEVERITY_LOW: log::warning(message);
				return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: log::trace(message);
				return;
		}
		
		HP_CORE_ASSERT(false, "Unknown severity level!");
	}
	
	void opengl_renderer_api::init()
	{
		#ifdef HP_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(opengl_message_callback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}
	
	void opengl_renderer_api::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
	
	void opengl_renderer_api::set_clear_color(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	
	void opengl_renderer_api::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void opengl_renderer_api::draw_indexed(const std::shared_ptr<vertex_array>& vertex_array, uint32_t index_count)
	{
		vertex_array->bind();
		uint32_t count = index_count ? index_count : vertex_array->get_index_buffer()->get_count();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	
	void opengl_renderer_api::draw_lines(const std::shared_ptr<vertex_array>& vertex_array, uint32_t vertex_count)
	{
		vertex_array->bind();
		glDrawArrays(GL_LINES, 0, vertex_count);
	}
	
	void opengl_renderer_api::set_line_width(float width)
	{
		glLineWidth(width);
	}
}  // namespace hp
