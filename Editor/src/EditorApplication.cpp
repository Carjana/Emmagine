#include "Emmagine.h"

class EmmaEditor : public Emma::EmmaApplication
{

};

Emma::EmmaApplication* Emma::CreateEmmaApplication()
{
	return new EmmaEditor();
}