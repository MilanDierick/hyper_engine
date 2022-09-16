#include "platform/opengl/opengl_vertex_array.h"

#include <glad/glad.h>

namespace hp
{
	static GLenum shader_data_type_to_opengl_base_type(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float: return GL_FLOAT; // NOLINT()
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3: return GL_FLOAT;
			case ShaderDataType::Mat4: return GL_FLOAT;
			case ShaderDataType::Int: return GL_INT;
			case ShaderDataType::Int2: return GL_INT;
			case ShaderDataType::Int3: return GL_INT;
			case ShaderDataType::Int4: return GL_INT;
			case ShaderDataType::Bool: return GL_BOOL;
			case ShaderDataType::None:
			HP_CORE_ASSERT(false, "Unknown ShaderDataType!");
				break;
		}
		
		HP_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
	
	opengl_vertex_array::opengl_vertex_array() : m_renderer_id(0)
	{
		glCreateVertexArrays(1, &m_renderer_id);
	}
	
	opengl_vertex_array::~opengl_vertex_array()
	{
		glDeleteVertexArrays(1, &m_renderer_id);
	}
	
	void opengl_vertex_array::bind() const
	{
		glBindVertexArray(m_renderer_id);
	}
	
	void opengl_vertex_array::unbind() const
	{
		glBindVertexArray(0);
	}
	
	void opengl_vertex_array::add_vertex_buffer(const std::shared_ptr<vertex_buffer>& vertex_buffer)
	{
		HP_CORE_ASSERT(!vertex_buffer->get_layout().get_elements().empty(), "Vertex Buffer has no layout!");
		
		glBindVertexArray(m_renderer_id);
		vertex_buffer->bind();
		
		const auto     & layout = vertex_buffer->get_layout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(m_vertex_buffer_index);
					glVertexAttribPointer(
						m_vertex_buffer_index,
						static_cast<GLint>(element.get_component_count()),
						shader_data_type_to_opengl_base_type(element.Type),
						element.normalized ? GL_TRUE : GL_FALSE,
						static_cast<GLsizei>(layout.get_stride()),
						reinterpret_cast<const void*>(element.offset)); // NOLINT(performance-no-int-to-ptr)
					m_vertex_buffer_index++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(m_vertex_buffer_index);
					glVertexAttribIPointer(
						m_vertex_buffer_index,
						static_cast<GLint>(element.get_component_count()),
						shader_data_type_to_opengl_base_type(element.Type),
						static_cast<GLsizei>(layout.get_stride()),
						reinterpret_cast<const void*>(element.offset)); // NOLINT(performance-no-int-to-ptr)
					m_vertex_buffer_index++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					auto count = static_cast<uint8_t>(element.get_component_count());
					
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_vertex_buffer_index);
						glVertexAttribPointer(
							m_vertex_buffer_index,
							count,
							shader_data_type_to_opengl_base_type(element.Type),
							element.normalized ? GL_TRUE : GL_FALSE,
							static_cast<GLint>(layout.get_stride()),
							reinterpret_cast<const void*>(element.offset // NOLINT(performance-no-int-to-ptr)
							                              + sizeof(float) * count * i));
						glVertexAttribDivisor(m_vertex_buffer_index, 1);
						m_vertex_buffer_index++;
					}
					break;
				}
				default:
				HP_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}
		
		m_vertex_buffers.push_back(vertex_buffer);
	}
	
	void opengl_vertex_array::set_index_buffer(const std::shared_ptr<index_buffer>& index_buffer)
	{
		glBindVertexArray(m_renderer_id);
		index_buffer->bind();
		
		m_index_buffer = index_buffer;
	}
	
	const std::vector<std::shared_ptr<vertex_buffer>>& opengl_vertex_array::get_vertex_buffers() const
	{
		return m_vertex_buffers;
	}
	
	const std::shared_ptr<index_buffer>& opengl_vertex_array::get_index_buffer() const
	{
		return m_index_buffer;
	}
}  // namespace hp
