// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "hyper/core/application.h"
#include "hyper/core/arena_allocator.h"
#include "hyper/core/asserts.h"
#include "hyper/core/log.h"
#include "hyper/events/event.h"

#include <iostream>

extern hp::application* hp::create_application();

int main(int argc, char** argv) // NOLINT(misc-definitions-in-headers)
{
	UNUSED(argc);
	UNUSED(argv);

	hp::log::init();

	struct matrix
	{
		int64_t x;
		int64_t y;
		int64_t z;
	};

	hp::log::info("Size of Matrix: {0}", sizeof(matrix));

	hp::mem::arena_allocator<30> allocator;

	auto* some_matrix = allocator.allocate<matrix>();
	auto* some_matrix2 = allocator.allocate<matrix>();

	//hp::log::trace("Hello Hyper!");

	//hp::application* application = hp::create_application();

	//application->execute();
	//application->terminate();

	return 0;
}
