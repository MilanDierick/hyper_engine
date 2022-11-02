// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_MOUSE_CODES_H
#define PLAYGROUND_MOUSE_CODES_H

#include <ostream>

using mouse_code = enum class mouse_code : uint16_t
{
	// From glfw3.h
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,
	
	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2
};

inline std::ostream& operator<<(std::ostream& os, mouse_code mouse_code) // NOLINT(readability-identifier-length)
{
	os << static_cast<int32_t>(mouse_code);
	return os;
}

#endif //PLAYGROUND_MOUSE_CODES_H
