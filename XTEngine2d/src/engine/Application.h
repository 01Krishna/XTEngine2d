#pragma once

#include "Core.h"
#include "Window.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Camera.h"
#include "Editor.h"
#include "FrameBuffer.h"

namespace XTEngine2d
{
	class XT_API Application
	{
	private:
		Window* m_Window;
		InputManager m_Input;
		Renderer m_Renderer;
		float m_DeltaTime;
		float m_LastTime = 0.0f;
		float col[4] = { 255,255,255,255 };
		Entity m_SelectedEntity = 0;
		char buffer[256];
		XTEngine2d::Editor m_Editor; 

	public:
		float m_AccumulatedTime = 0.0f;

	public:
		Application();
		virtual ~Application();
		virtual void Init();
		virtual void Run();
		virtual void DrawImGui();
	};
	Application* CreateApplication();	
}