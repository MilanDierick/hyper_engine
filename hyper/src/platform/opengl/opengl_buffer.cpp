#include "platform/opengl/opengl_buffer.h"

#include <glad/glad.h>

namespace hp
{
	opengl_vertex_buffer::opengl_vertex_buffer(uint32_t size) : m_renderer_id(0)
	{
		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
	}
	
	opengl_vertex_buffer::opengl_vertex_buffer(float* vertices, uint32_t size) : m_renderer_id(0)
	{
		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
	}
	
	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}
	
	void opengl_vertex_buffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}
	
	void opengl_vertex_buffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void opengl_vertex_buffer::set_data(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(size), data);
	}
	
	const buffer_layout& opengl_vertex_buffer::get_layout() const
	{
		return m_layout;
	}
	
	void opengl_vertex_buffer::set_layout(const buffer_layout& layout)
	{
		m_layout = layout;
	}
	
	opengl_index_buffer::opengl_index_buffer(uint32_t* indices, uint32_t count) : m_renderer_id(0), m_count(count)
	{
		glCreateBuffers(1, &m_renderer_id);
		
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count) * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	
	opengl_index_buffer::~opengl_index_buffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}
	
	void opengl_index_buffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	}
	
	void opengl_index_buffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	uint32_t opengl_index_buffer::get_count() const
	{
		return m_count;
	}
}  // namespace hp
