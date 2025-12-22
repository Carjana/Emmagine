#pragma once
#include "Event.h"

namespace Emma
{
	class EMMA_API WindowMoveEvent : public Event
	{
	public:
		const int xPos;
		const int yPos;
		const unsigned int WindowId;

		WindowMoveEvent(const int x,const int y, const unsigned int windowId) : xPos(x), yPos(y), WindowId(windowId) {}

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
		const unsigned int WindowId;

		WindowResizeEvent(const int w, const int h, const unsigned int windowId) : Width(w), Height(h), WindowId(windowId) {}

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
		unsigned int WindowId;

		WindowCloseRequestEvent(const unsigned int windowId) : WindowId(windowId){}

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
		unsigned int WindowId;
		bool gainedFocus;
		WindowFocusEvent(const unsigned int windowId, const bool gainedFocus) : WindowId(windowId), gainedFocus(gainedFocus){}

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
		unsigned int WindowId;
		bool gainedFocus;
		WindowMouseFocusEvent(const unsigned int windowId, const bool gainedFocus) : WindowId(windowId), gainedFocus(gainedFocus){}

		std::string ToString() override
		{
			std::stringstream stream;
			stream << GetName() ADD_VAR(WindowId) ADD_VAR(gainedFocus);
			return stream.str();
		}

		SETUP_EVENT_TYPE(EventType::WindowMouseFocusEvent);
	};
}