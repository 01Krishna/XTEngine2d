#pragma once

#include <GLFW/glfw3.h>

struct MouseKeys
{
	bool Key_Left = false;
	bool Key_Right = false;
	bool Key_Middle = false;
};

struct MousePosition
{
	double x; 
	double y;
};


class MouseInputHandle
{
public:
	MouseKeys m_Keys;
	MousePosition m_Position;


	void UpdateInput(GLFWwindow* window)
	{
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			m_Keys.Key_Left = true;
		}
		else
			m_Keys.Key_Left = false;


		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			m_Keys.Key_Right = true;
		}
		else
			m_Keys.Key_Right = false;

		glfwGetCursorPos(window, &m_Position.x, &m_Position.y);

	}
};