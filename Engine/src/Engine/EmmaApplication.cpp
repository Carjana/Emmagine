#include "EmmaApplication.h"

#include "Logger.h"
#include "Events/WindowEvents.h"

namespace Emma
{
	EmmaApplication::EmmaApplication()= default;

	EmmaApplication::~EmmaApplication()= default;

	bool OnEvent(Event &e)
	{
		std::stringstream stream;
		stream << "Event Received: " << e.ToString();
		LOG_CORE_TRACE(stream.str());
		return true;
	}

	void EmmaApplication::Run()
	{
		WindowMoveEvent event(1,1);
		WindowCloseEvent closeEvent;
		WindowResizeEvent resizeEvent(1,1);
		LOG_INFO(event.ToString());
		LOG_INFO(closeEvent.ToString())
		LOG_INFO(resizeEvent.ToString())

		EventDispatcher dispatcher(event);
		EventDispatcher d1(closeEvent);
		d1.DispatchEvent(EventDispatcher::EventFunction<WindowCloseEvent>(OnEvent));
		dispatcher.DispatchEvent(EventDispatcher::EventFunction<WindowMoveEvent>(OnEvent));
		while (true)
		{

		}
	}
}
