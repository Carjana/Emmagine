#pragma once
#include "Engine/Core.h"

namespace Emma
{
	enum class EventType
	{
		WindowMoveEvent,
		WindowResizeEvent,
		WindowCloseEvent,

		EventTypeCount
	};

	inline const char* EventTypeName[] = {
		"WindowMoveEvent",
		"WindowResizeEvent",
		"WindowCloseEvent",
	};
	static_assert(ArrayCount(EventTypeName) == (int)EventType::EventTypeCount);


	enum class EventCategory
	{
		None = 0,
		Application = BIT(1),
	};

	class EMMA_API Event
	{
		friend class EventDispatcher;
	public:
		virtual ~Event() {};

		virtual EventType GetEventType() = 0;
		virtual int GetCategoryFlags() = 0;
		virtual const char* GetName() = 0;
		virtual std::string ToString()
		{
			return GetName();
		};

	protected:
		bool isHandled = false;
	};

	class EMMA_API EventDispatcher
	{
	public:

		template<typename T>
		using EventFunction = std::function<bool(T&)>;

		EventDispatcher(Event &_event) : event(_event) {}

		template<typename T>
		bool DispatchEvent(EventFunction<T> function)
		{
			if (event.GetEventType() == T::GetStaticEventType())
			{
				event.isHandled = function(*(T*)&event);
				return true;
			}
			return false;
		}

	private:
		Event &event;
	};
}