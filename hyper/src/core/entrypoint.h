// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "core/application.h"
#include "core/log.h"
#include "core/asserts.h"
#include <iostream>

int main() // NOLINT(misc-definitions-in-headers)
{
	hp::log::initialize();
	
	hp::log::trace("Hello Hyper!");
	
	hp::application* application = hp::create_application();
	
	application->execute();
	application->terminate();
	
	return 0;
}
