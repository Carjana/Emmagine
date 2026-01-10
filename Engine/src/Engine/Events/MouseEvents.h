#pragma once

#include "Event.h"

namespace Emma
{
	class EMMA_API MouseMovedEvent : public Event
	{
	public:
		const float X;
		const float Y;
		const float XRel;
		const float YRel;
		const float XFrac;
		const float YFrac;

		MouseMovedEvent(const unsigned int windowID, const float x, const float y,
			const float xRel, const float yRel,
			const float xFrac, const float yFrac) :
		Event(windowID), X(x), Y(y),
		XRel(xRel), YRel(yRel),
		XFrac(xFrac), YFrac(yFrac){}

		std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(X) ADD_VAR(Y) ADD_VAR(XRel) ADD_VAR(YRel) ADD_VAR(XFrac) ADD_VAR(YFrac);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::MouseMovedEvent);
	};

	class EMMA_API MouseButtonEvent : public Event
	{
	public:
		SDL_MouseButtonFlags ButtonID;
		bool IsPressed;

		MouseButtonEvent(const unsigned int windowID, const SDL_MouseButtonFlags buttonID, const bool isPressed) : Event(windowID), ButtonID(buttonID), IsPressed(isPressed) {}

		std::string ToString() const override
		{
			std::stringstream stream;

			stream << GetName();
			switch (ButtonID)
			{
				case SDL_BUTTON_MASK(SDL_BUTTON_LEFT):
					stream << ", ButtonID: Left";
					break;
				case SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE):
					stream << ", ButtonID: Middle";
					break;
				case SDL_BUTTON_MASK(SDL_BUTTON_RIGHT):
					stream << ", ButtonID: Right";
					break;
				case SDL_BUTTON_MASK(SDL_BUTTON_X1):
					stream << ", ButtonID: X1";
					break;
				case SDL_BUTTON_MASK(SDL_BUTTON_X2):
					stream << ", ButtonID: X2";
					break;
			}

			stream ADD_VAR(IsPressed);
			return stream.str();
		}
	public:
		SETUP_EVENT_TYPE(EventType::MouseButtonEvent);
	};

	class EMMA_API MouseWheelEvent : public Event
	{
	public:
		float X;
		float Y;

		MouseWheelEvent(const unsigned int windowID, const float x, const float y) : Event(windowID), X(x), Y(y)
		{}

		std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(X) ADD_VAR(Y);
			return stream.str();
		}
		SETUP_EVENT_TYPE(EventType::MouseWheelEvent);
	};
}
