#pragma once
#include "Engine/Core.h"

namespace Emma
{
	enum class EventType : char
	{
	// Application Events
		WindowMoveEvent,
		WindowResizeEvent,
		WindowCloseEvent,
		WindowFocusEvent,
		WindowMouseFocusEvent,

	// Mouse Events
		MouseMovedEvent,
		MouseButtonEvent,
		MouseWheelEvent,

	// Keyboad Events
		KeyEvent,
		KeyTextEvent,

		EventTypeCount,
	};

	enum class EventCategory
	{
		None = 0,
		Application = BIT(1),
		Mouse = BIT(2),
		Keyboard = BIT(3),
	};

	inline const char* EventTypeName[] = {
		"WindowMoveEvent",
		"WindowResizeEvent",
		"WindowCloseEvent",
		"WindowFocusEvent",
		"WindowMouseFocusEvent",

		"MouseMovedEvent",
		"MouseButtonEvent",
		"MouseWheelEvent",

		"KeyEvent",
		"KeyTextEvent",
	};
	static_assert(ArrayCount(EventTypeName) == (int)EventType::EventTypeCount);

	inline constexpr EventCategory EventCategoryFlags[]
	{
		EventCategory::Application,		// Window Move
		EventCategory::Application,		// Window Resize
		EventCategory::Application,		// Window Close
		EventCategory::Application,		// Window Focus
		EventCategory::Application,		// Window Mouse Focus
		EventCategory::Mouse,			// Mouse Moved
		EventCategory::Mouse,			// Mouse Button
		EventCategory::Mouse,			// Mouse Wheel
		EventCategory::Keyboard,			// Key Event
		EventCategory::Keyboard			// KeyText Event
	};
	static_assert(ArrayCount(EventCategoryFlags) == (int)EventType::EventTypeCount);

inline constexpr bool EventShouldLog[]
{
	// TODO:: Move this to a like config class or something, so it can be toggled in the editor
	false,	// Window Move
	false,	// Window Resize
	false,	// Window Close
	false,	// Window Focus
	false,	// Window Mouse Focus
	false,	// Mouse Moved
	false,	// Mouse Button
	false, 	// Mouse Wheel
	false,	// Key Event
	false,	// KeyText Event
};
	static_assert(ArrayCount(EventShouldLog) == (int)EventType::EventTypeCount);

#define ADD_VAR(Var) << ", " << #Var << ": " << Var
#define SETUP_EVENT_TYPE(eventType) \
	EventType GetEventType() override {return eventType;} \
	int GetCategoryFlags() override {return (int)EventCategoryFlags[(int)eventType];} \
	const char *GetName() override {return EventTypeName[(int)eventType];} \
	static EventType GetStaticEventType() {return eventType;}

	class EMMA_API Event
	{
		friend class EventDispatcher;
	public:

		Event(const unsigned int windowID) : WindowId(windowID) {};
		virtual ~Event(){};

		virtual EventType GetEventType() = 0;
		virtual int GetCategoryFlags() = 0;
		virtual const char* GetName() = 0;
		virtual std::string ToString()
		{
			return GetName();
		};

		unsigned int WindowId;
		bool IsHandled = false;
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
				event.IsHandled = function(*(T*)&event);
				return true;
			}
			return false;
		}

	private:
		Event &event;
	};
}