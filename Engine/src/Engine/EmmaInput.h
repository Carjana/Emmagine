#pragma once

#include "Core.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"

namespace Emma
{
	class EMMA_API EmmaInput
	{
	public:
		static bool IsKeyPressed(SDL_Keycode keycode);
		static bool IsMouseButtonPressed(SDL_MouseButtonFlags button);
		static float GetMouseX();
		static float GetMouseY();
		static void GetMouseXY(float *x, float *y);

	};
}
