#include "EmmaInput.h"

namespace Emma
{
	inline bool EmmaInput::IsKeyPressed(SDL_Keycode keycode)
	{
		// TODO: Core Input Layer To cache states?
		const bool *keyState = SDL_GetKeyboardState(nullptr);
		return keyState[SDL_GetScancodeFromKey(keycode, nullptr)];
	}

	inline bool EmmaInput::IsMouseButtonPressed(SDL_MouseButtonFlags button)
	{
		SDL_MouseButtonFlags mouseState = SDL_GetMouseState(nullptr, nullptr);
		return (mouseState & button) != 0;
	}

	inline float EmmaInput::GetMouseX()
	{
		float x;
		GetMouseXY(&x, nullptr);
		return x;
	}

	inline float EmmaInput::GetMouseY()
	{
		float y;
		GetMouseXY(nullptr, &y);
		return y;
	}

	inline void EmmaInput::GetMouseXY(float *x, float *y)
	{
		SDL_GetMouseState(x, y);
	}
}
