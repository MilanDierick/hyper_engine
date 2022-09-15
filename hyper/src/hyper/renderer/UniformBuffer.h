#ifndef HYPER_UNIFORM_BUFFER_H
#define HYPER_UNIFORM_BUFFER_H

#include "hyper/core/base.h"

namespace hp
{
	class UniformBuffer
	{
	 public:
		virtual ~UniformBuffer()
		{
		}
		
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		
		static std::shared_ptr<UniformBuffer> Create(uint32_t size, uint32_t binding);
	};
	
}  // namespace hp

#endif