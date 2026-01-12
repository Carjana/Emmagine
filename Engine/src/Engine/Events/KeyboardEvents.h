#pragma once

#include "Event.h"
#include "SDL3/SDL_keycode.h"
#include "emmapch.h"
#include "SDL3/SDL_keyboard.h"

namespace Emma
{
	class EMMA_API KeyEvent : public Event
	{
	public:
		SDL_Keycode Keycode;
		SDL_Scancode Scancode;
		SDL_Keymod Keymod;

		bool IsPressed;

		KeyEvent(unsigned int windowID, const SDL_Keycode keycode, const SDL_Scancode scancode, const SDL_Keymod keymod, const bool isPressed)
		: Event(windowID), Keycode(keycode), Scancode(scancode), Keymod(keymod), IsPressed(isPressed) {}

		std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() << ", " << SDL_GetKeyName(Keycode) ADD_VAR(IsPressed);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::KeyEvent);
	};

	class EMMA_API KeyTextEvent : public Event
	{
	public:

		const char* Text;

		KeyTextEvent(const unsigned int windowID, const char* text)
		: Event(windowID), Text(text) {}

		std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() << ", " << "Text: " << Text;
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::KeyTextEvent);
	};
}
