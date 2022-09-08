// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_SETTINGS_H
#define PLAYGROUND_SETTINGS_H

#include "nlohmann/json.hpp"
#include <string>

namespace hyper
{
	class settings
	{
		using string_t = std::basic_string<char>;
	 
	 public:
		struct SettingValues
		{
			unsigned version_major;
			unsigned version_minor;
			unsigned version_patch;
			unsigned window_width;
			unsigned window_height;
			
			NLOHMANN_DEFINE_TYPE_INTRUSIVE(SettingValues,
				version_major,
				version_minor,
				version_patch,
				window_width,
				window_height);
		};
		
		settings() = default;
		~settings() = default;
		
		settings(const settings& other) = delete;
		settings(settings&& other) noexcept = delete;
		settings& operator=(const settings& other) = delete;
		settings& operator=(settings&& other) = delete;
		
		void read_settings_from_file(string_t& file_path);
		void write_settings_to_file(string_t& file_path);
		[[nodiscard]] SettingValues get_value() const;
	 
	 private:
		SettingValues m_settings;
	};
	
}  // namespace hyper

#endif //PLAYGROUND_SETTINGS_H
