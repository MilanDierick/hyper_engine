#ifndef HYPER_FRAMEBUFFER_H
#define HYPER_FRAMEBUFFER_H

#include "hyper/core/base.h"

namespace hp
{
	enum class framebuffer_texture_format
	{
		None = 0,

		// color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct framebuffer_texture_specification
	{
		framebuffer_texture_specification()  = default;
		~framebuffer_texture_specification() = default;

		framebuffer_texture_specification(const framebuffer_texture_specification& other)            = default;
		framebuffer_texture_specification(framebuffer_texture_specification&& other) noexcept        = default;
		framebuffer_texture_specification& operator=(const framebuffer_texture_specification& other) = default;
		framebuffer_texture_specification& operator=(framebuffer_texture_specification&& other)      = default;

		explicit framebuffer_texture_specification(framebuffer_texture_format format) : texture_format(format)
		{
		}

		framebuffer_texture_format texture_format = framebuffer_texture_format::None;
		// TODO: filtering/wrap
	};

	struct framebuffer_attachment_specification
	{
		framebuffer_attachment_specification()  = default;
		~framebuffer_attachment_specification() = default;

		framebuffer_attachment_specification(const framebuffer_attachment_specification& other)            = default;
		framebuffer_attachment_specification(framebuffer_attachment_specification&& other) noexcept        = default;
		framebuffer_attachment_specification& operator=(const framebuffer_attachment_specification& other) = default;
		framebuffer_attachment_specification& operator=(framebuffer_attachment_specification&& other)      = default;

		framebuffer_attachment_specification(std::initializer_list<framebuffer_texture_specification> attachments) : attachments(attachments)
		{
		}

		std::vector<framebuffer_texture_specification> attachments;
	};

	struct framebuffer_specification
	{
		uint32_t width;
		uint32_t height;
		framebuffer_attachment_specification attachments;
		uint32_t samples;
		bool swap_chain_target;

		framebuffer_specification() : width(),
		                              height(),
		                              samples(1),
		                              swap_chain_target()
		{
		}
	};

	class framebuffer
	{
	public:
		framebuffer()          = default;
		virtual ~framebuffer() = default;

		framebuffer(const framebuffer& other)            = delete;
		framebuffer(framebuffer&& other) noexcept        = delete;
		framebuffer& operator=(const framebuffer& other) = delete;
		framebuffer& operator=(framebuffer&& other)      = delete;

		virtual void bind()   = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height)                       = 0;
		virtual int read_pixel(uint32_t attachmentIndex, int x_coord, int y_coord) = 0;

		virtual void clear_attachment(uint32_t attachmentIndex, int value) = 0;

		[[nodiscard]] virtual uint32_t get_color_attachment_renderer_id(uint32_t index) const = 0;
		[[nodiscard]] virtual const framebuffer_specification& get_specification() const      = 0;

		static std::shared_ptr<framebuffer> create(const framebuffer_specification& spec);
	};
} // namespace hp

#endif
