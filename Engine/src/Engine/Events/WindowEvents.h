#pragma once
#include "Event.h"

namespace Emma
{
	class EMMA_API WindowMoveEvent : public Event
	{
	public:
		const int xPos;
		const int yPos;

		WindowMoveEvent(const unsigned int windowId, const int x,const int y) : Event(windowId), xPos(x), yPos(y) {}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(WindowId) ADD_VAR(xPos) ADD_VAR(yPos);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::WindowMoveEvent);
	};

	class EMMA_API WindowResizeEvent : public Event
	{
	public:
		const int Width;
		const int Height;

		WindowResizeEvent(const unsigned int windowId, const int w, const int h) : Event(windowId), Width(w), Height(h) {}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(WindowId) ADD_VAR(Width) ADD_VAR(Height);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::WindowResizeEvent);
	};

	class EMMA_API WindowCloseRequestEvent : public Event
	{
	public:

		WindowCloseRequestEvent(const unsigned int windowId) : Event(windowId){}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(WindowId);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::WindowCloseEvent);
	};

	class EMMA_API WindowFocusEvent : public Event
	{
	public:
		bool gainedFocus;
		WindowFocusEvent(const unsigned int windowId, const bool gainedFocus) : Event(windowId), gainedFocus(gainedFocus){}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(WindowId) ADD_VAR(gainedFocus);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::WindowFocusEvent);
	};

	class EMMA_API WindowMouseFocusEvent : public Event
	{
	public:
		bool gainedFocus;
		WindowMouseFocusEvent(const unsigned int windowId, const bool gainedFocus) : Event(windowId), gainedFocus(gainedFocus){}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(WindowId) ADD_VAR(gainedFocus);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::WindowMouseFocusEvent);
	};
}