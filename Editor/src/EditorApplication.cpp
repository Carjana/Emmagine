#include "Emmagine.h"

class EmmaEditor : public Emmagine::EmmaApplication
{
public:

	void Run() override
	{
		while (true);
	}
};

Emmagine::EmmaApplication* Emmagine::CreateEmmaApplication()
{
	return new EmmaEditor();
}