#include "Emmagine.h"

class SandboxGame : public Emmagine::EmmaApplication
{
public:
	SandboxGame()
	{

	}

	~SandboxGame()
	{

	}

	void Run() override
	{

	}
};

Emmagine::EmmaApplication* Emmagine::CreateEmmaApplication()
{
	return new SandboxGame();
}
