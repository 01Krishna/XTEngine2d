#pragma once

#include <GLFW/glfw3.h>
#include "Log.h"

struct Input_Keys
{
	bool Key_W = false;
	bool Key_A = false;
	bool Key_S = false;
	bool Key_D = false;
	bool Key_Left = false;
	bool Key_Right = false;
	bool Key_Up = false;
	bool Key_Down = false;
	bool Key_Space = false;
};

struct Window_Keys
{
	bool Key_Close = false;
};


class KeyboardInputHandle
{
public:
	Input_Keys m_InputKeys;
	Window_Keys m_WindowKeys;

	void UpdateInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_WindowKeys.Key_Close = true;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_InputKeys.Key_W = true;
		else 
			m_InputKeys.Key_W = false;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_InputKeys.Key_A = true;
		else 
			m_InputKeys.Key_A = false;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_InputKeys.Key_S = true;
		else
			m_InputKeys.Key_S = false;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_InputKeys.Key_D = true;
		else 
			m_InputKeys.Key_D = false;

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			m_InputKeys.Key_Left = true;
		else
			m_InputKeys.Key_Left = false;

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			m_InputKeys.Key_Right = true;
		else
			m_InputKeys.Key_Right = false;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			m_InputKeys.Key_Up = true;
		else
			m_InputKeys.Key_Up = false;

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			m_InputKeys.Key_Down = true;
		else
			m_InputKeys.Key_Down = false;
	}
};
