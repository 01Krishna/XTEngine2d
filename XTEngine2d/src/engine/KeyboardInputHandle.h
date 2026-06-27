#pragma once

#include <GLFW/glfw3.h>
#include "Log.h"

class KeyboardInputHandle
{
public:
	bool m_Keys[1024] = { false };
	bool m_PreviousKeys[1024] = { false };


	void Update(GLFWwindow* window)
	{
		memcpy(m_PreviousKeys, m_Keys, sizeof(m_Keys));

		for (int i = 0; i < 1024; i++)
		{
			m_Keys[i] =	glfwGetKey(window, i) == GLFW_PRESS;
		}
	}

};
