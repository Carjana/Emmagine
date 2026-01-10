#pragma once

#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include "Engine/Layer/Layer.h"

namespace Emma
{
	class EMMA_API CoreInput : public Layer
	{
	public:
		void OnAttach() override;
		void OnUpdate() override;

		inline static bool IsKeyPressed(SDL_Scancode scanCode);
		inline static bool IsKeyPressed(SDL_Keycode keyCode);

		inline static bool IsMouseButtonPressed(SDL_MouseButtonFlags flags);

		inline static float GetMouseX();
		inline static float GetMouseY();
		inline static void GetMouseXY(float *x, float *y);

		// expandable with window ID, when using events and update for ourself;

		const bool *KeyIsPressed;

		SDL_MouseButtonFlags MouseButtonFlags;
		float MouseX;
		float MouseY;

		static CoreInput* Instance;
	};
}
