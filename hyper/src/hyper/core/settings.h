// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_SETTINGS_H
#define HYPER_SETTINGS_H

#include "nlohmann/json.hpp"
#include <string>

namespace hp
{
	class settings
	{
		using string_t = std::basic_string<char>;

	public:
		struct setting_values
		{
			unsigned version_major;
			unsigned version_minor;
			unsigned version_patch;
			unsigned window_width;
			unsigned window_height;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(setting_values, version_major, version_minor, version_patch, window_width, window_height);
		};

		settings() = default;
		~settings() = default;

		settings(const settings &other) = delete;
		settings(settings &&other) noexcept = delete;
		settings &operator=(const settings &other) = delete;
		settings &operator=(settings &&other) = delete;

		void read_settings_from_file(string_t &file_path);
		void write_settings_to_file(string_t &file_path);

		[[nodiscard]] setting_values get_value() const;

	private:
		setting_values m_settings;
	};

}  // namespace hp

#endif //HYPER_SETTINGS_H
