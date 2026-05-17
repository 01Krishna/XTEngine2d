#include "Window.h"
#include "GLFWWindow.h"

namespace XTEngine2d
{
	Window* Window::Create()
	{
		return new GLFWWindow(800, 600);  // platform-specific
	}

}