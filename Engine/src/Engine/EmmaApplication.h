#pragma once
#include "Core.h"

namespace Emma
{
	class EMMA_API EmmaApplication
	{
	public:
		EmmaApplication();
		virtual ~EmmaApplication();

		virtual void Run();
	};

	EmmaApplication *CreateEmmaApplication();
}
