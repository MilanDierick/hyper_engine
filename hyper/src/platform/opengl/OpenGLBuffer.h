#ifndef HYPER_OPENGL_BUFFER_H
#define HYPER_OPENGL_BUFFER_H

#include "hyper/renderer/Buffer.h"

namespace hp
{
	class OpenGLVertexBuffer : public vertex_buffer
	{
	 public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
		
		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void SetData(const void* data, uint32_t size) override;
		
		virtual const BufferLayout& GetLayout() const override
		{
			return m_Layout;
		}
		
		virtual void SetLayout(const BufferLayout& layout) override
		{
			m_Layout = layout;
		}
	 
	 private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};
	
	class OpenGLIndexBuffer : public index_buffer
	{
	 public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();
		
		virtual void Bind() const;
		virtual void Unbind() const;
		
		virtual uint32_t GetCount() const
		{
			return m_Count;
		}
	 
	 private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
	
}  // namespace hp

#endif