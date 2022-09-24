#ifndef HYPER_OPENGL_BUFFER_H
#define HYPER_OPENGL_BUFFER_H

#include "hyper/renderer/buffer.h"

namespace hp
{
	class opengl_vertex_buffer : public vertex_buffer
	{
	 public:
		explicit opengl_vertex_buffer(uint32_t size);
		opengl_vertex_buffer(float* vertices, uint32_t size);
		~opengl_vertex_buffer() override;
		
		opengl_vertex_buffer(const opengl_vertex_buffer& other) = delete;
		opengl_vertex_buffer(opengl_vertex_buffer&& other) noexcept = delete;
		opengl_vertex_buffer& operator=(const opengl_vertex_buffer& other) = delete;
		opengl_vertex_buffer& operator=(opengl_vertex_buffer&& other) = delete;
		
		void bind() const override;
		void Unbind() const override;
		
		void set_data(const void* data, uint32_t size) override;
		[[nodiscard]] const buffer_layout& get_layout() const override;
		void set_layout(const buffer_layout& layout) override;
	 
	 private:
		uint32_t m_renderer_id;
		buffer_layout m_layout;
	};
	
	class opengl_index_buffer : public index_buffer
	{
	 public:
		opengl_index_buffer(uint32_t* indices, uint32_t count);
		~opengl_index_buffer() override;
		
		opengl_index_buffer(const opengl_index_buffer& other) = delete;
		opengl_index_buffer(opengl_index_buffer&& other) noexcept = delete;
		opengl_index_buffer& operator=(const opengl_index_buffer& other) = delete;
		opengl_index_buffer& operator=(opengl_index_buffer&& other) = delete;
		
		void bind() const override;
		void Unbind() const override;
		
		[[nodiscard]] uint32_t get_count() const override;
	 
	 private:
		uint32_t m_renderer_id;
		uint32_t m_count;
	};
}  // namespace hp

#endif