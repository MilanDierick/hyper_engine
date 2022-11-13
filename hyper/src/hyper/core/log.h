// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_LOG_H
#define HYPER_LOG_H

#include "hyper/core/config.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace hp
{
	class HP_API log final
	{
	public:
		static void initialize();

		template<typename T>
		constexpr static void trace(const T& message);

		template<typename... Args>
		constexpr static void trace(spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void info(const T& message);

		template<typename... Args>
		constexpr static void info(spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void warning(const T& message);

		template<typename... Args>
		constexpr static void warning(spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void error(const T& message);

		template<typename... Args>
		constexpr static void error(spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void critical(const T& message);

		template<typename... Args>
		constexpr static void critical(spdlog::format_string_t<Args...> fmt, Args&&... args);

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};

	template<typename T>
	constexpr void log::trace(const T& message)
	{
		s_logger->trace(message);
	}

	template<typename... Args>
	constexpr void log::trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		s_logger->trace(std::forward<spdlog::format_string_t<Args...>>(fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::info(const T& message)
	{
		s_logger->info(message);
	}

	template<typename... Args>
	constexpr void log::info(spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		s_logger->info(std::forward<spdlog::format_string_t<Args...>>(fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::warning(const T& message)
	{
		s_logger->warn(message);
	}

	template<typename... Args>
	constexpr void log::warning(spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		s_logger->warn(std::forward<spdlog::format_string_t<Args...>>(fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::error(const T& message)
	{
		s_logger->error(message);
	}

	template<typename... Args>
	constexpr void log::error(spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		s_logger->error(std::forward<spdlog::format_string_t<Args...>>(fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::critical(const T& message)
	{
		s_logger->critical(message);
	}

	template<typename... Args>
	constexpr void log::critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		s_logger->critical(std::forward<spdlog::format_string_t<Args...>>(fmt), std::forward<Args>(args)...);
	}
} // namespace hp

#endif //HYPER_LOG_H
