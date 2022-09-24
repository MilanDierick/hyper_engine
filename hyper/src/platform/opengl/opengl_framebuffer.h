#ifndef HYPER_OPENGLFRAMEBUFFER_H
#define HYPER_OPENGLFRAMEBUFFER_H

#include "hyper/renderer/framebuffer.h"

namespace hp
{
	class opengl_framebuffer : public framebuffer
	{
	 public:
		opengl_framebuffer(const framebuffer_specification& spec);
		virtual ~opengl_framebuffer();
		
		void invalidate();
		
		virtual void bind() override;
		virtual void unbind() override;
		
		virtual void resize(uint32_t width, uint32_t height) override;
		virtual int read_pixel(uint32_t attachment_index, int x, int y) override;
		
		virtual void clear_attachment(uint32_t attachment_index, int value) override;
		
		virtual uint32_t get_color_attachment_renderer_id(uint32_t index = 0) const override
		{
			HP_CORE_ASSERT(index < m_color_attachments.size());
			return m_color_attachments[index];
		}
		
		virtual const framebuffer_specification& get_specification() const override
		{
			return m_specification;
		}
	 
	 private:
		uint32_t m_renderer_id;
		framebuffer_specification m_specification;
		
		std::vector<framebuffer_texture_specification> m_color_attachment_specifications;
		framebuffer_texture_specification m_depth_attachment_specification;
		
		std::vector<uint32_t> m_color_attachments;
		uint32_t m_depth_attachment;
	};
}  // namespace hp

#endif