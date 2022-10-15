// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "hyper/core/application.h"
#include "hyper/core/arena_allocator.h"
#include "hyper/core/asserts.h"
#include "hyper/core/log.h"
#include "hyper/events/event.h"

#include <iostream>

int main(int argc, char** argv) // NOLINT(misc-definitions-in-headers)
{
	UNUSED(argc);
	UNUSED(argv);

	hp::log::init();

	hp::log::trace("Hello Hyper!");

	hp::application* application = hp::create_application();

	application->execute();

	return 0;
}
