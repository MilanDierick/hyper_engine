// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "Application.h"
#include "Log.h"

int main() // NOLINT(misc-definitions-in-headers)
{
	hyper::Log::initialize();

	hyper::Log::trace("Hello Hyper!");

	hyper::Application* application = hyper::create_application();

	application->execute();
	application->terminate();

	return 0;
}
