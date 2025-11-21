#pragma once
#include "Core.h"

namespace Emmagine
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
