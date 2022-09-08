// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_PARSER_H_
#define HYPER_PARSER_H_

#include "core/log.h"
#include "nlohmann/json.hpp"
#include "core/settings.h"

namespace hp::parser
{
	using json_t = nlohmann::json;
	
	template<class T>
	static json_t serialize(T object);
	
	template<class T>
	static T deserialize(const json_t& json);
	
	template<class T>
	json_t serialize(T object)
	{
		return json_t(object);
	}
	
	template<class T>
	T deserialize(const json_t& json)
	{
		return json.get<T>();
	}
	
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(settings::SettingValues,
		version_major,
		version_minor,
		version_patch,
		window_width,
		window_height)
}  // namespace hp::parser

#endif //HYPER_PARSER_H_
