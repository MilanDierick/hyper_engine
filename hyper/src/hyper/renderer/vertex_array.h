#ifndef HYPER_VERTEX_ARRAY_H
#define HYPER_VERTEX_ARRAY_H

#include "hyper/renderer/buffer.h"

#include <memory>

namespace hp
{
	class vertex_array
	{
	public:
		vertex_array()          = default;
		virtual ~vertex_array() = default;

		vertex_array(const vertex_array& other)            = delete;
		vertex_array(vertex_array&& other) noexcept        = delete;
		vertex_array& operator=(const vertex_array& other) = delete;
		vertex_array& operator=(vertex_array&& other)      = delete;

		virtual void bind() const   = 0;
		virtual void unbind() const = 0;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer>& vertex_buffer) = 0;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer>& index_buffer)    = 0;

		[[nodiscard]] virtual const std::vector<std::shared_ptr<vertex_buffer>>& get_vertex_buffers() const = 0;
		[[nodiscard]] virtual const std::shared_ptr<index_buffer>& get_index_buffer() const                 = 0;

		static std::shared_ptr<vertex_array> create();
	};
} // namespace hp

#endif //HYPER_VERTEX_ARRAY_H