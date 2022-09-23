#ifndef HYPER_OPENGLFRAMEBUFFER_H
#define HYPER_OPENGLFRAMEBUFFER_H

#include "hyper/renderer/framebuffer.h"

namespace hp
{
	class OpenGLFramebuffer : public framebuffer
	{
	 public:
		OpenGLFramebuffer(const framebuffer_specification& spec);
		virtual ~OpenGLFramebuffer();
		
		void Invalidate();
		
		virtual void bind() override;
		virtual void unbind() override;
		
		virtual void resize(uint32_t width, uint32_t height) override;
		virtual int read_pixel(uint32_t attachmentIndex, int x, int y) override;
		
		virtual void clear_attachment(uint32_t attachmentIndex, int value) override;
		
		virtual uint32_t get_color_attachment_renderer_id(uint32_t index = 0) const override
		{
			HP_CORE_ASSERT(index < m_ColorAttachments.size());
			return m_ColorAttachments[index];
		}
		
		virtual const framebuffer_specification& get_specification() const override
		{
			return m_Specification;
		}
	 
	 private:
		uint32_t                  m_RendererID = 0;
		framebuffer_specification m_Specification;
		
		std::vector<framebuffer_texture_specification> m_ColorAttachmentSpecifications;
		framebuffer_texture_specification              m_DepthAttachmentSpecification =
			                                               static_cast<framebuffer_texture_specification>(framebuffer_texture_format::None);
		
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t              m_DepthAttachment = 0;
	};
}  // namespace hp

#endif