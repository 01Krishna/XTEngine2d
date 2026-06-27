#include "InputManager.h"

namespace XTEngine2d
{
	GLFWwindow* InputManager::s_Window = nullptr;
	KeyboardInputHandle InputManager::s_Keyboard;
	MouseInputHandle InputManager::s_Mouse;

	void InputManager::Init(GLFWwindow* window)
	{
		s_Window = window;
	}
	
	void InputManager::Update()
	{
		s_Keyboard.Update(s_Window);
		s_Mouse.UpdateInput(s_Window);
	}

	bool InputManager::IsKeyPressed(int key)
	{
		return s_Keyboard.m_Keys[key];
	}

	// New: True ONLY on the exact frame it was pushed down
	bool InputManager::IsKeyJustPressed(int key)
	{
		return s_Keyboard.m_Keys[key] && !s_Keyboard.m_PreviousKeys[key];
	}


	bool InputManager::IsMousePressed(int button)
	{
		switch (button)
		{
			case GLFW_MOUSE_BUTTON_LEFT: return s_Mouse.m_Keys.Key_Left;
			case GLFW_MOUSE_BUTTON_RIGHT: return s_Mouse.m_Keys.Key_Right;
			default: return false;
		}
	}

	double InputManager::GetMouseX()
	{
		return s_Mouse.m_Position.x;
	}

	double InputManager::GetMouseY()
	{
		return s_Mouse.m_Position.y;
	}

};