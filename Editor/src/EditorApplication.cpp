#include "Emmagine.h"

class EmmaEditor : public Emma::EmmaApplication
{
public:
};

Emma::EmmaApplication* Emma::CreateEmmaApplication()
{
	return new EmmaEditor();
}