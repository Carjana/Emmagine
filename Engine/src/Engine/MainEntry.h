#pragma once
#include "EmmaApplication.h"

extern Emmagine::EmmaApplication* Emmagine::CreateEmmaApplication();

#ifdef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Emmagine::EmmaApplication *app = Emmagine::CreateEmmaApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
