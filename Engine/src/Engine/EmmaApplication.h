#pragma once
#include "Core.h"
#include "EmmaWindow.h"

namespace Emma
{
	class EMMA_API EmmaApplication
	{
	public:
		EmmaApplication();
		virtual ~EmmaApplication();

		virtual void Run();

		bool isRunning = true;

	private:
		EmmaWindow *window;
	};

	EmmaApplication *CreateEmmaApplication();
}
