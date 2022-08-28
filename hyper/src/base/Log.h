// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_LOG_H
#define HYPER_LOG_H

#include "Config"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace hyper
{
	class HP_API Log final
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
	constexpr void Log::trace(const T& message)
	{
		logger_->trace(message);
	}

	template<typename ... Args>
	constexpr void Log::trace(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->trace(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void Log::info(const T& message)
	{
		logger_->info(message);
	}

	template<typename ... Args>
	constexpr void Log::info(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->info(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void Log::warning(const T& message)
	{
		logger_->warn(message);
	}

	template<typename ... Args>
	constexpr void Log::warning(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->warn(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void Log::error(const T& message)
	{
		logger_->error(message);
	}

	template<typename ... Args>
	constexpr void Log::error(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->error(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr void Log::critical(const T& message)
	{
		logger_->critical(message);
		throw;
	}

	template<typename ... Args>
	constexpr void Log::critical(spdlog::format_string_t<Args...> fmt, Args&& ...args)
	{
		logger_->critical(std::forward < spdlog::format_string_t < Args... >> (fmt), std::forward<Args>(args)...);
		throw;
	}
}

#endif //HYPER_LOG_H
