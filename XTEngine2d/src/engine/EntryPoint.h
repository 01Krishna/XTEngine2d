#pragma once

#ifdef XT_PLATFORM_WINDOWS


extern XTEngine2d::Application* XTEngine2d::CreateApplication();

int main()
{	
	auto app = XTEngine2d::CreateApplication();
	app->Run(); 
	delete app;
}	

#endif