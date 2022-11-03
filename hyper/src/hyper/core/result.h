// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_RESULT_H
#define HYPER_RESULT_H

#include "asserts.h"

#include <system_error>
#include <vulkan/vulkan.h> // VkResult

namespace hp
{
	struct error
	{
		std::error_code type;
		VkResult vk_result = VK_SUCCESS; // optional error value if a vulkan call failed
	};

	template<typename T>
	class result
	{
	public:
		result(const T& value) noexcept : m_result{value},
		                                  m_init{true} {}

		result(T&& value) noexcept : m_result{std::move(value)},
		                             m_init{true} {}

		result(std::error_code code) noexcept : m_error({code}) {};

		result(error error) noexcept : m_error{error},
		                               m_init{false} {}

		result(std::error_code error_code, VkResult result) noexcept
		    : m_error{error_code, result},
		      m_init{false} {}

		~result() noexcept { destroy(); }

		result(result const& expected) noexcept : m_init(expected.m_init)
		{
			if (m_init)
			{
				new (&m_result) T{expected.m_value};
			}
			else
			{
				m_error = expected.m_error;
			}
		}

		result& operator=(result const& result) noexcept
		{
			m_init = result.m_init;

			if (m_init)
			{
				new (&m_result) T{result.m_value};
			}
			else
			{
				m_error = result.m_error;
			}
		}

		result(result&& expected) noexcept : m_init(expected.m_init)
		{
			if (m_init)
			{
				new (&m_result) T{std::move(expected.m_value)};
			}
			else
			{
				m_error = std::move(expected.m_error);
			}

			expected.destroy();
		}

		result& operator=(result&& result) noexcept
		{
			m_init = result.m_init;

			if (m_init)
			{
				new (&m_result) T{std::move(result.m_value)};
			}
			else
			{
				m_error = std::move(result.m_error);
			}
		}

		result& operator=(const T& expect) noexcept
		{
			destroy();
			m_init = true;
			new (&m_result) T{expect};
			return *this;
		}

		result& operator=(T&& expect) noexcept
		{
			destroy();
			m_init = true;
			new (&m_result) T{std::move(expect)};
			return *this;
		}

		result& operator=(const error& error) noexcept
		{
			destroy();
			m_init  = false;
			m_error = error;
			return *this;
		}

		result& operator=(error&& error) noexcept
		{
			destroy();
			m_init  = false;
			m_error = error;
			return *this;
		}

		// clang-format off
		const T* operator-> () const noexcept { HP_CORE_ASSERT(m_init); return &m_result; }
		T*       operator-> ()       noexcept { HP_CORE_ASSERT(m_init); return &m_result; }
		const T& operator* () const& noexcept { HP_CORE_ASSERT(m_init); return m_result; }
		T&       operator* () &      noexcept { HP_CORE_ASSERT(m_init); return m_result; }
		T&&      operator* () &&	 noexcept { HP_CORE_ASSERT(m_init); return std::move (m_result); }
		const T&  value () const&    noexcept { HP_CORE_ASSERT(m_init); return m_result; }
		T&        value () &         noexcept { HP_CORE_ASSERT(m_init); return m_result; }
		const T&& value () const&&   noexcept { HP_CORE_ASSERT(m_init); return std::move (m_result); }
		T&&       value () &&        noexcept { HP_CORE_ASSERT(m_init); return std::move (m_result); }

		[[nodiscard]] std::error_code error() const { HP_CORE_ASSERT(!m_init); return m_error.type; }
		[[nodiscard]] VkResult vk_result() const { HP_CORE_ASSERT(!m_init); return m_error.vk_result; }
		[[nodiscard]] struct error full_error() const {	HP_CORE_ASSERT(!m_init); return m_error; }

		// clang-format on

		template<typename E>
		bool matches_error(E error_enum_value) const
		{
			return !m_init && static_cast<E>(m_error.type.value()) == error_enum_value;
		}

		[[nodiscard]] bool has_value() const { return m_init; }
		explicit operator bool() const { return m_init; }

	private:
		void destroy()
		{
			if (m_init)
			{
				m_result.~T();
			}
		}

		bool m_init;

		union
		{
			T m_result;
			struct error m_error;
		};
	};
} // namespace hp

#endif //HYPER_RESULT_H
