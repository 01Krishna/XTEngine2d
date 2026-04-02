#pragma once

#include "Core.h"

namespace XTEngine2d
{
	class XT_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};


	Application* CreateApplication();	
}