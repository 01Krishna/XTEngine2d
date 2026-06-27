#include "GLFWWindow.h"


namespace XTEngine2d
{
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
		glViewport(0, 0, width, height);
		win->w_Width = width;
		win->w_Height = height;
		XT_CORE_INFO("Window resized: {}  {}", width, height);
	}
	GLFWWindow::GLFWWindow(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}
	GLFWWindow::~GLFWWindow()
	{
		Shutdown();
	}

	bool GLFWWindow::Close()
	{
		return !glfwWindowShouldClose(m_Window);
	}

	void GLFWWindow::Clear()
	{
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void GLFWWindow::RenderUI()
	{


	}
	void GLFWWindow::OnUpdate()
	{
		Clear();
		
		glfwPollEvents();
	}

	void GLFWWindow::OnRender()
	{
		glfwSwapBuffers(m_Window);
	}
	void GLFWWindow::Init()
	{
		if (!glfwInit())
		{
			XT_CORE_ERROR("GLFW init failed!");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		m_Window = glfwCreateWindow(m_Width, m_Height, "XTEngine2d", nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);

		if (!m_Window)
		{
			XT_CORE_ERROR("Window creation failed!");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

		if (!gladLoadGL())
		{
			XT_CORE_ERROR("GLAD init failed!");
			return;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, m_Width, m_Height);


	}



	void GLFWWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}