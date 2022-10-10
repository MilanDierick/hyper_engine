// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "hyper/core/settings.h"

#include "hyper/core/parser.h"

namespace hp
{
	void settings::read_settings_from_file(settings::string_t& file_path)
	{
		try
		{
			std::ifstream file(file_path);
			std::string const input((std::istream_iterator<char>(file)), std::istream_iterator<char>());
			m_settings = parser::deserialize<settings::setting_values>(nlohmann::json::parse(input));
		}
		catch (std::exception& exception)
		{
			log::critical(exception.what());
		}
	}

	void settings::write_settings_to_file(settings::string_t& file_path)
	{
		try
		{
			std::ofstream file(file_path);
			file << parser::serialize<setting_values>(m_settings);
		}
		catch (std::exception& exception)
		{
			log::critical(exception.what());
		}
	}

	settings::setting_values settings::get_value() const
	{
		return m_settings;
	}
} // namespace hp