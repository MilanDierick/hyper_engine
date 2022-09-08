// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "application.h"
#include "log.h"
#include "asserts.h"
#include "events/Event_Old.h"
#include <iostream>
#include <fstream>

int main() // NOLINT(misc-definitions-in-headers)
{
	hyper::log::initialize();
	
	hyper::log::trace("Hello Hyper!");
	
	hyper::application* application = hyper::create_application();
	
	application->execute();
	application->terminate();
	
	return 0;
}
