// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_ARENA_ALLOCATOR_H
#define HYPER_ARENA_ALLOCATOR_H

#include "hyper/core/concepts.h"
#include "hyper/core/config.h"

#include <array>
#include <set>

namespace hp::mem
{
	template<size_t Size>
	requires concepts::allocation_size<Size>
	class arena_allocator // TODO: Handle memory deallocation
	{
	public:
		static_assert(Size > 0);

		using value_type = std::byte;
		using size_type  = size_t;
		using pointer    = std::byte*;

		arena_allocator();
		~arena_allocator()                                       = default;

		arena_allocator(const arena_allocator& other)            = delete;
		arena_allocator(arena_allocator&& other) noexcept        = delete;
		arena_allocator& operator=(const arena_allocator& other) = delete;
		arena_allocator& operator=(arena_allocator&& other)      = delete;

		template<concepts::default_constructable T>
		[[nodiscard]] constexpr T* allocate();

		void reset();

	private:
		value_type m_data[Size]; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
		pointer m_p_current;

		[[nodiscard]] constexpr size_type get_available_memory() const;
	};

} // namespace hp::mem

#include "hyper/core/arena_allocator.inl"

#endif //HYPER_ARENA_ALLOCATOR_H
