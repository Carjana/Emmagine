#pragma once

#ifdef PLATFORM_WINDOWS

extern Emma::EmmaApplication* Emma::CreateEmmaApplication();

int main(int argc, char** argv)
{
	Emma::Logger::Init();

	Emma::EmmaApplication *app = Emma::CreateEmmaApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
