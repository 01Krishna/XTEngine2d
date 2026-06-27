#pragma once
#include "KeyboardInputHandle.h"
#include "MouseInputHandle.h"


namespace XTEngine2d
{
	class InputManager
	{
	public:
		static void Init(GLFWwindow* window);
	
		static void Update();
		static bool IsKeyPressed(int key);
		bool IsKeyJustPressed(int key);
		static bool IsMousePressed(int button);

		static double GetMouseX();
		static double GetMouseY();
	

	private:
		static GLFWwindow* s_Window;
	public:
		static KeyboardInputHandle s_Keyboard;
		static MouseInputHandle s_Mouse;
	};
};

