#include "platform/opengl/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace hp
{
	void OpenGLMessageCallback(unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		UNUSED(userParam);
		UNUSED(length);
		UNUSED(id);
		UNUSED(type);
		UNUSED(source);
		
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			log::critical(message);
			return;
		case GL_DEBUG_SEVERITY_MEDIUM:
			log::error(message);
			return;
		case GL_DEBUG_SEVERITY_LOW:
			log::warning(message);
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			log::trace(message);
			return;
		}
		
		HP_CORE_ASSERT(false, "Unknown severity level!");
	}
	
	void OpenGLRendererAPI::init()
	{
		
		#ifdef HZ_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}
	
	void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
	
	void OpenGLRendererAPI::set_clear_color(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	
	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void OpenGLRendererAPI::draw_indexed(const std::shared_ptr<vertex_array>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	
	void OpenGLRendererAPI::draw_lines(const std::shared_ptr<vertex_array>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}
	
	void OpenGLRendererAPI::set_line_width(float width)
	{
		glLineWidth(width);
	}
}  // namespace hp
