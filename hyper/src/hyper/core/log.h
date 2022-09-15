// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_LOG_H
#define HYPER_LOG_H

#include "hyper/core/config.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace hp
{
	class HP_API log final
	{
	public:
		static void initialize();

		template<typename T>
		constexpr static void trace(const T& message);

		template<typename ... Args>
		constexpr static void trace(spdlog::format_string_t<Args...> fmt, Args&& ...args);

		template<typename T>
		constexpr static void info(const T& message);

		template<typename ... Args>
		constexpr static void info(spdlog::format_string_t<Args...> fmt, Args&& ...args);

		template<typename T>
		constexpr static void warning(const T& message);

		template<typename ... Args>
		constexpr static void warning(spdlog::format_string_t<Args...> fmt, Args&& ...args);

		template<typename T>
		constexpr static void error(const T& message);

		template<typename ... Args>
		constexpr static void error(spdlog::format_string_t<Args...> fmt, Args&& ...args);

		template<typename T>
		constexpr static void critical(const T& message);

		template<typename ... Args>
		constexpr static void critical(spdlog::format_string_t<Args...> fmt, Args&& ...args);

	private:
		static std::shared_ptr<spdlog::logger> logger_;
	};

	template<typename T>
	constexpr void log::trace(const T& message)
	{
		logger_->trace(message);
	}

	template<typename ... Args>
	constexpr void log::trace(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->trace(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::info(const T& message)
	{
		logger_->info(message);
	}

	template<typename ... Args>
	constexpr void log::info(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->info(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::warning(const T& message)
	{
		logger_->warn(message);
	}

	template<typename ... Args>
	constexpr void log::warning(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->warn(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::error(const T& message)
	{
		logger_->error(message);
	}

	template<typename ... Args>
	constexpr void log::error(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->error(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void log::critical(const T& message)
	{
		logger_->critical(message);
	}

	template<typename ... Args>
	constexpr void log::critical(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->critical(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}
}

#endif //HYPER_LOG_H
