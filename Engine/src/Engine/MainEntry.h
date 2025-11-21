#pragma once
#include "EmmaApplication.h"
#include "Logger.h"

extern Emma::EmmaApplication* Emma::CreateEmmaApplication();

#ifdef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Emma::Logger::Init();
	LOG_INFO("Loading Emma App...");
	Emma::EmmaApplication *app = Emma::CreateEmmaApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
