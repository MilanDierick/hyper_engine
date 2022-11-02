// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_EXTENSIONS_H
#define HYPER_EXTENSIONS_H

#include <functional>

namespace hp
{
	template<typename Func, typename... Param>
	auto bind_back(Func&& func, Param&&... param) -> decltype(auto)
	{
		return [func = std::forward<Func>(func), ... param = std::forward<Param>(param)]<typename... Inner>(Inner&&... inner) -> decltype(auto)
		{
			return std::invoke(func, std::forward<Inner>(inner)..., param...);
		};
	}
} // namespace hp

#endif //HYPER_EXTENSIONS_H
