#include "Application.h"
	
namespace XTEngine2d
{
	Application::Application()
	{

	}
	Application::~Application()
	{
	}

	void Application::DrawImGui()
	{
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f", 1.0f / m_DeltaTime);
		ImGui::End();
	}

	void Application::Init()
	{
		m_Window = Window::Create();
		QuadMesh::Init();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		GLFWwindow* window = (GLFWwindow*)m_Window->GetNativeWindow();
		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		m_Input.Init(window);

		m_Renderer.Init();
		m_Editor.Init(m_Window);
	}

	void Application::Run()
	{

		while (m_Window->Close())
		{
			float currentTime = glfwGetTime();
			m_DeltaTime = currentTime - m_LastTime;
			m_LastTime = currentTime;

			m_AccumulatedTime += m_DeltaTime;

			m_Window->OnUpdate();
			m_Input.Update();
			m_Editor.BeginFrame();
			m_Editor.OnImGuiRender(m_Renderer, m_Window, m_DeltaTime, m_AccumulatedTime, m_Input);
			DrawImGui();
			m_Editor.EndFrame();
			m_Window->OnRender();

		}
	}
}