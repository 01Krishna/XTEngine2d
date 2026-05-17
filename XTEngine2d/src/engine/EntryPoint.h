#pragma once

#ifdef XT_PLATFORM_WINDOWS


extern XTEngine2d::Application* XTEngine2d::CreateApplication();

int main()
{	
	XTEngine2d::Log::Init();


	auto app = XTEngine2d::CreateApplication();
	app->Init();	
	app->Run(); 
	delete app;
}	

#endif