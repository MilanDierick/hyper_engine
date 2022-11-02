#ifndef HYPER_TEXTURE_H
#define HYPER_TEXTURE_H

#include "hyper/core/base.h"

#include <string>

namespace hp
{
	class HP_API texture
	{
	public:
		virtual ~texture() = default;

		[[nodiscard]] virtual uint32_t get_width() const       = 0;
		[[nodiscard]] virtual uint32_t get_height() const      = 0;
		[[nodiscard]] virtual uint32_t get_renderer_id() const = 0;

		[[nodiscard]] virtual const std::string& get_path() const = 0;

		virtual void set_data(void* data, uint32_t size) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;

		[[nodiscard]] virtual bool is_loaded() const = 0;

		virtual bool operator==(const texture& other) const = 0;
	};

	class HP_API texture_2d : public texture
	{
	public:
		static std::shared_ptr<texture_2d> create(uint32_t width, uint32_t height);
		static std::shared_ptr<texture_2d> create(const std::string& path);
	};

} // namespace hp

#endif