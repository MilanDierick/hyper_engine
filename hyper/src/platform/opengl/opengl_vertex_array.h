#ifndef HYPER_OPENGL_VERTEX_ARRAY_H
#define HYPER_OPENGL_VERTEX_ARRAY_H

#include "hyper/renderer/vertex_array.h"

namespace hp {

	class opengl_vertex_array : public vertex_array
	{
	public:
		opengl_vertex_array();
		virtual ~opengl_vertex_array();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer>& vertexBuffer) override;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<vertex_buffer>>& get_vertex_buffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<index_buffer>& get_index_buffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<vertex_buffer>> m_VertexBuffers;
		std::shared_ptr<index_buffer> m_IndexBuffer;
	};
}  // namespace hp

#endif