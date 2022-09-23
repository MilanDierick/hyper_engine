#ifndef HYPER_OPENGL_BUFFER_H
#define HYPER_OPENGL_BUFFER_H

#include "hyper/renderer/buffer.h"

namespace hp
{
	class OpenGLVertexBuffer : public vertex_buffer
	{
	 public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
		
		virtual void bind() const override;
		virtual void Unbind() const override;
		
		virtual void set_data(const void* data, uint32_t size) override;
		
		virtual const buffer_layout& get_layout() const override
		{
			return m_Layout;
		}
		
		virtual void set_layout(const buffer_layout& layout) override
		{
			m_Layout = layout;
		}
	 
	 private:
		uint32_t      m_RendererID;
		buffer_layout m_Layout;
	};
	
	class OpenGLIndexBuffer : public index_buffer
	{
	 public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();
		
		virtual void bind() const;
		virtual void Unbind() const;
		
		virtual uint32_t get_count() const
		{
			return m_Count;
		}
	 
	 private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
	
}  // namespace hp

#endif