#ifndef HYPER_UNIFORM_BUFFER_H
#define HYPER_UNIFORM_BUFFER_H

#include "hyper/core/base.h"

namespace hp
{
	class uniform_buffer
	{
	public:
		virtual ~uniform_buffer() = default;

		virtual void set_data(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static std::shared_ptr<uniform_buffer> create(uint32_t size, uint32_t binding);
	};

} // namespace hp

#endif