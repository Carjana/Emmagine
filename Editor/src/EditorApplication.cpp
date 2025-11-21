#include "Emmagine.h"

class EmmaEditor : public Emma::EmmaApplication
{
public:

	void Run() override
	{
		while (true);
	}
};

Emma::EmmaApplication* Emma::CreateEmmaApplication()
{
	return new EmmaEditor();
}