#ifndef HYPER_OPENGL_VERTEX_ARRAY_H
#define HYPER_OPENGL_VERTEX_ARRAY_H

#include "hyper/renderer/vertex_array.h"

namespace hp
{
	class opengl_vertex_array : public vertex_array
	{
	 public:
		opengl_vertex_array();
		~opengl_vertex_array() override;
		
		opengl_vertex_array(const opengl_vertex_array& other) = delete;
		opengl_vertex_array(opengl_vertex_array&& other) noexcept = delete;
		opengl_vertex_array& operator=(const opengl_vertex_array& other) = delete;
		opengl_vertex_array& operator=(opengl_vertex_array&& other) = delete;
		
		void bind() const override;
		void unbind() const override;
		
		void add_vertex_buffer(const std::shared_ptr<vertex_buffer>& vertex_buffer) override;
		void set_index_buffer(const std::shared_ptr<index_buffer>& index_buffer) override;
		
		[[nodiscard]] const std::vector<std::shared_ptr<vertex_buffer>>& get_vertex_buffers() const override;
		[[nodiscard]] const std::shared_ptr<index_buffer>& get_index_buffer() const override;
	 
	 private:
		uint32_t m_renderer_id;
		uint32_t m_vertex_buffer_index = 0;
		std::vector<std::shared_ptr<vertex_buffer>> m_vertex_buffers;
		std::shared_ptr<index_buffer> m_index_buffer;
	};
}  // namespace hp

#endif //HYPER_OPENGL_VERTEX_ARRAY_H