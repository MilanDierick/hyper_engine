// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "core/log.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace hp
{
	std::shared_ptr<spdlog::logger> log::logger_ = nullptr;

	void log::initialize()
	{
		std::vector<spdlog::sink_ptr> log_sinks{};
		log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Hyper.log", true));

		log_sinks[0]->set_pattern("[%ArgsType] [%^%l%$] %v");
		log_sinks[1]->set_pattern("[%ArgsType] [%l] %v");

		logger_ = std::make_shared<spdlog::logger>("Hyper", begin(log_sinks), end(log_sinks));
		register_logger(logger_);
		logger_->set_level(spdlog::level::trace);
		logger_->flush_on(spdlog::level::trace);
	}
}