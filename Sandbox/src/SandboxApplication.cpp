#include "XTEngine2d.h"


//This is the main client application. It creates a class that inherits from the XTEngine2d::Application class
// and implements the CreateApplication function to return an instance of the Sandbox class.
//The run function is called in the main function defined in the EntryPoint.h file, which starts the application loop.

class Sandbox : public XTEngine2d::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}
};


XTEngine2d::Application* XTEngine2d::CreateApplication()
{
	return new Sandbox();
} 