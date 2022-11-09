// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "hyper/core/application.h"
#include "hyper/core/log.h"
#include "hyper/core/asserts.h"
#include "hyper/events/event.h"
#include <iostream>

extern hp::application* hp::create_application();

/*
 * @brief The entry point of the application.
 *
 * @param argc The number of arguments passed to the application.
 * @param argv The arguments passed to the application.
 *
 * @return The exit code of the application.
 *
 * @note The entry point of the application is the main function. This function is called by the operating system when the application is started.
 */
int main(int argc, char** argv) // NOLINT(misc-definitions-in-headers)
{
	UNUSED(argc);
	UNUSED(argv);
	
	hp::log::initialize();
	
	hp::log::trace("Hello Hyper!");
	
	hp::application* application = hp::create_application();
	
	application->execute();
	application->terminate();
	
	return 0;
}
