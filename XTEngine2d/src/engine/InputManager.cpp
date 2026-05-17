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
		s_Keyboard.UpdateInput(s_Window);
		s_Mouse.UpdateInput(s_Window);
	}

	bool InputManager::IsKeyPressed(int key)
	{
		switch (key)
		{
			case GLFW_KEY_W: return s_Keyboard.m_InputKeys.Key_W; break;
			case GLFW_KEY_A: return s_Keyboard.m_InputKeys.Key_A; break;
			case GLFW_KEY_S: return s_Keyboard.m_InputKeys.Key_S; break;
			case GLFW_KEY_D: return s_Keyboard.m_InputKeys.Key_D; break;
			case GLFW_KEY_LEFT: return s_Keyboard.m_InputKeys.Key_Left; break;
			case GLFW_KEY_RIGHT: return s_Keyboard.m_InputKeys.Key_Right; break;
			case GLFW_KEY_UP: return s_Keyboard.m_InputKeys.Key_Up; break;
			case GLFW_KEY_DOWN: return s_Keyboard.m_InputKeys.Key_Down; break;
			default: return false;
		}
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