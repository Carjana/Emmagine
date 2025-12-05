#pragma once
#include "Event.h"

namespace Emma
{
#define WINDOW_EVENT_TYPE(eventType) \
	EventType GetEventType() override \
	{ \
		return eventType; \
	} \
	int GetCategoryFlags() override \
	{ \
		return (int)EventCategory::Application; \
	} \
	const char *GetName() override \
	{ \
		return EventTypeName[(int)eventType]; \
	}\
	static EventType GetStaticEventType()\
	{\
		return eventType;\
	}

	class EMMA_API WindowMoveEvent : public Event
	{
	public:
		const int xPos;
		const int yPos;

		WindowMoveEvent(const int x,const int y) : xPos(x), yPos(y){}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() << ": NewPos: x:" << xPos << ", y:" << yPos;
			return stream.str();
		}

		WINDOW_EVENT_TYPE(EventType::WindowMoveEvent);
	};

	class EMMA_API WindowResizeEvent : public Event
	{
	public:
		const int width;
		const int height;

		WindowResizeEvent(const int w, const int h) : width(w), height(h){}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() << ": NewSize: w:" << width << ", h:" << height;
			return stream.str();
		}

		WINDOW_EVENT_TYPE(EventType::WindowResizeEvent);
	};

	class EMMA_API WindowCloseEvent : public Event
	{
	public:
		WINDOW_EVENT_TYPE(EventType::WindowCloseEvent);
	};
}

#undef WINDOW_EVENT_TYPE