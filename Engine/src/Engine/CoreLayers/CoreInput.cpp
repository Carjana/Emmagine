#include "CoreInput.h"

#include "SDL3/SDL_keyboard.h"

namespace Emma
{
	CoreInput* CoreInput::Instance = nullptr;

	void CoreInput::OnAttach()
	{
		Instance = this;
	}

	void CoreInput::OnUpdate()
	{
		KeyIsPressed = SDL_GetKeyboardState(nullptr);
		MouseButtonFlags = SDL_GetMouseState(&MouseX, &MouseY);
	}

	// STATIC

	bool CoreInput::IsKeyPressed(SDL_Scancode scanCode)
	{
		return Instance->KeyIsPressed[scanCode];
	}

	bool CoreInput::IsKeyPressed(SDL_Keycode keyCode)
	{
		return IsKeyPressed(SDL_GetScancodeFromKey(keyCode, nullptr));
	}

	bool CoreInput::IsMouseButtonPressed(SDL_MouseButtonFlags flags)
	{
		return Instance->MouseButtonFlags & flags;
	}

	float CoreInput::GetMouseX()
	{
		return Instance->MouseX;
	}

	float CoreInput::GetMouseY()
	{
		return Instance->MouseY;
	}

	void CoreInput::GetMouseXY(float *x, float *y)
	{
		if (x)
			*x = Instance->MouseX;
		if (y)
			*y = Instance->MouseY;
	}
}
