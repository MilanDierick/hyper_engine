// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "arena_allocator.h"
#include "hyper/core/log.h"

namespace hp::mem
{
	template<size_t Size>
	requires concepts::allocation_size<Size>
	arena_allocator<Size>::arena_allocator() : m_p_current(m_data) // NOLINT(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
	{
	}

	template<size_t Size>
	requires concepts::allocation_size<Size>
	template<concepts::default_constructable T>
	constexpr T* arena_allocator<Size>::allocate()
	{
		// Check if we have enough memory available, return nullptr if we don't
		if (sizeof(T) >= get_available_memory())
		{
			return nullptr;
		}

		// Placement new follows the following format: new({location}) {type}
		T* value = new (m_p_current) T();
		m_p_current += sizeof(T);
		return value;
	}

	template<size_t Size>
	requires concepts::allocation_size<Size>
	void arena_allocator<Size>::reset()
	{
		m_p_current = m_data;
	}

	template<size_t Size>
	requires concepts::allocation_size<Size>
	constexpr typename arena_allocator<Size>::size_type arena_allocator<Size>::get_available_memory() const
	{
		return m_data + Size - m_p_current;
	}
} // namespace hp::mem