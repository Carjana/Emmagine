#pragma once

#ifdef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Emma::Logger::Init();

	Emma::EmmaApplication *app = Emma::CreateEmmaApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
