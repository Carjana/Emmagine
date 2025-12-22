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
}