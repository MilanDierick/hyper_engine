// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "Settings.h"
#include "Parser.h"

namespace hyper
{
	void Settings::read_settings_from_file(Settings::string_t& file_path)
	{
		try
		{
			std::ifstream file(file_path);
			std::string input((std::istream_iterator<char>(file)), std::istream_iterator<char>());
			m_settings = parser::deserialize<Settings::SettingValues>(nlohmann::json::parse(input));
		}
		catch(std::exception& exception)
		{
			Log::critical(exception.what());
		}
	}
	
	void Settings::write_settings_to_file(Settings::string_t& file_path)
	{
		try
		{
			std::ofstream file(file_path);
			file << parser::serialize<SettingValues>(m_settings);
		}
		catch(std::exception& exception)
		{
			Log::critical(exception.what());
		}
	}
	
	Settings::SettingValues Settings::get_value() const
	{
		return m_settings;
	}
}  // namespace hyper