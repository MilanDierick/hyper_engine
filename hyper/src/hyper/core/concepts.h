// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_CONCEPTS_H
#define PLAYGROUND_CONCEPTS_H

#include <type_traits>

namespace hp::concepts
{
	template<typename T> concept default_constructable = std::is_default_constructible_v<T>;
	template<std::size_t Size> concept allocation_size = Size > 0 && Size < 256000000; // 256MB, should be more than sufficient for any needs
}

#endif //PLAYGROUND_CONCEPTS_H
