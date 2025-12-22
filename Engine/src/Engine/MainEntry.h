#pragma once

#ifdef PLATFORM_WINDOWS
#include "SDL_init.h"

int main(int argc, char** argv)
{
	Emma::Logger::Init();
	SDL_Init(SDL_INIT_VIDEO);
	Emma::EmmaApplication *app = Emma::CreateEmmaApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
