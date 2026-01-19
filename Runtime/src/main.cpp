#include "Emmagine.h"
#include "Engine/MainEntry.h"


namespace EmmaEditor
{
	class RuntimeApplication : public Emma::EmmaApplication
	{
		void OnInit() override
		{
		}
		void OnQuit() override
		{
		}
	};
}

Emma::EmmaApplication* Emma::CreateEmmaApplication()
{
	return new EmmaEditor::RuntimeApplication();
}