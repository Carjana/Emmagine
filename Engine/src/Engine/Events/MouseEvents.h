#pragma once

#include "Event.h"

namespace Emma
{
	enum class MouseButton : char
	{
		Left = BIT(1),
		Middle = BIT(2),
		Right = BIT(3),
		X1 = BIT(4),
		X2 = BIT(5)
	};
	class EMMA_API MouseMovedEvent : public Event
	{
	public:
		const float X;
		const float Y;
		const float XRel;
		const float YRel;
		const float XFrac;
		const float YFrac;

		MouseMovedEvent(const float x, const float y,
			const float xRel, const float yRel,
			const float xFrac, const float yFrac):
		X(x), Y(y),
		XRel(xRel), YRel(yRel),
		XFrac(xFrac), YFrac(yFrac){}

		std::string ToString() override
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
		MouseButton ButtonID;
		bool IsPressed;

		MouseButtonEvent(const MouseButton buttonID, const bool isPressed) : ButtonID(buttonID), IsPressed(isPressed) {}

		std::string ToString() override
		{
			std::stringstream stream;

			stream << GetName();
			switch (ButtonID)
			{
				case MouseButton::Left:
					stream << ", ButtonID: Left";
					break;
				case MouseButton::Middle:
					stream << ", ButtonID: Middle";
					break;
				case MouseButton::Right:
					stream << ", ButtonID: Right";
					break;
				case MouseButton::X1:
					stream << ", ButtonID: X1";
					break;
				case MouseButton::X2:
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

		MouseWheelEvent(const float x, const float y) : X(x), Y(y) {}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(X) ADD_VAR(Y);
			return stream.str();
		}
		SETUP_EVENT_TYPE(EventType::MouseWheelEvent);
	};
}