#include "Editor.h"
#include "Renderer.h"

namespace XTEngine2d
{
	Editor::Editor()
	{
	}
	Editor::~Editor()
	{
	}
	void Editor::Init(Window* window)
	{
		m_ViewportPanel.Init(window, &m_CommandHistory);
		m_InspectorPanel.Init(&m_CommandHistory);
		m_EntityManagerPanel.Init(&m_CommandHistory);
		m_HierarchyPanel.Init(&m_CommandHistory);
		m_EditorScene = std::make_shared<Scene>();
		m_RuntimeScene = std::make_shared<Scene>();
		XTEngine2d::Shader shader("Assets\\shaders\\VertexShader.vert", "Assets\\shaders\\FragmentShader.frag");
		m_EditorScene->m_Shader = shader;
		m_RuntimeScene->m_Shader = shader;

		m_AssetsBrowserPanel.LoadIcons();

	}

	void Editor::OnImGuiRender(Renderer& renderer, Window* window, float delta,float accumulatedtime, InputManager& input)
	{
		m_ViewportPanel.OnImGuiRender(m_EditorScene, m_RuntimeScene, renderer, window, m_SelectedEntity, m_SceneState, delta,accumulatedtime, m_InspectorPanel.getSelectedTile());
		m_SelectedEntity = m_ViewportPanel.GetSelectedEntity();
		m_HierarchyPanel.OnImGuiRender(GetActiveScene().get(), m_SelectedEntity);
		m_InspectorPanel.OnImGuiRender(GetActiveScene().get(), m_SelectedEntity, m_AssetsBrowserPanel.GetSelectedAsset());
		m_ScenePanel.OnImGuiRender(GetActiveScene().get(), m_SceneState);
		m_EntityManagerPanel.OnImGuiRender(GetActiveScene().get(), m_SelectedEntity);
		m_AssetsBrowserPanel.OnImGuiRender();
		GetActiveScene()->Update(delta, input);
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z"))
				m_CommandHistory.Undo();

			if (ImGui::MenuItem("Redo", "Ctrl+Y"))
				m_CommandHistory.Redo();

			ImGui::EndMenu();
		}
		if (input.IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && input.IsKeyJustPressed(GLFW_KEY_Z))
		{
			m_CommandHistory.Undo();
		}
		if (input.IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && input.IsKeyJustPressed(GLFW_KEY_Y))
		{
			m_CommandHistory.Redo();
		}

		if (input.IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && input.IsKeyJustPressed(GLFW_KEY_D))
		{
			auto cmd = std::make_shared<DuplicateEntityCommand>(m_EditorScene.get(), m_SelectedEntity);
			m_CommandHistory.ExecuteCommand(cmd);

			Entity entity = cmd->GetEntity();
			m_SelectedEntity = entity;

		}

		ImGui::Begin("Undo Stack");
		m_CommandHistory.ShowUndoStack(m_CommandHistory.m_UndoStack, undoCommandRetrival);
		while (!undoCommandRetrival.empty())
		{
			ImGui::Text(undoCommandRetrival.top().c_str());
			undoCommandRetrival.pop();
		}
		ImGui::End();

		ImGui::Begin("Redo Stack");
		m_CommandHistory.ShowRedoStack(m_CommandHistory.m_RedoStack, redoCommandRetrival);
		while (!redoCommandRetrival.empty())
		{
			ImGui::Text(redoCommandRetrival.top().c_str());
			redoCommandRetrival.pop();
		}
		ImGui::End();
	}
	void Editor::CreateDefaultCamera(Scene* scene)
	{
		scene->CheckPrimaryCameraAvailability();
		if (scene->HasPrimaryCamera())
			return;


		Entity camera = scene->m_Registry.CreateEntity();
		Camera cameraComponent;
		cameraComponent.primary = true;
		Tag tag;
		tag.name = "Default Camera";
		scene->m_Registry.AddComponent(camera, tag);
		scene->m_Registry.AddComponent(camera, Transform{});
		scene->m_Registry.AddComponent(camera, cameraComponent);
		scene->m_Registry.AddComponent(camera, CameraController{});

	}

	void Editor::InitDocking()
	{

		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport =
			ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		window_flags |= ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("DockSpace", nullptr, window_flags);

		ImGui::PopStyleVar(2);

		ImGuiID dockspace_id =
			ImGui::GetID("MyDockSpace");

		ImGui::DockSpace(
			dockspace_id,
			ImVec2(0.0f, 0.0f));

		ImGui::End();

	}
	void Editor::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		InitDocking();
	}
	void Editor::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}