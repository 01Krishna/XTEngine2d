#pragma once
#include "Scene.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imconfig.h"
#include "Utility.h"
#include "portable-file-dialogs.h"
#include "AssetManager.h"
#include "SceneSerializer.h"
#include "Framebuffer.h"
#include "Window.h"
#include "Camera.h"
#include "LineRenderer.h"
#include "Editor/Panels/ViewportPanel.h"
#include "Editor/Panels/InspectorPanel.h"
#include "Editor/Panels/ScenePanel.h"
#include "Editor/Panels/EntityManagerPanel.h"
#include "Editor/Panels/HierarchyPanel.h"
#include "Editor/Panels/AssetsBrowserPanel.h"
#include "Commands/DuplicateEntityCommad.h"
#include "CommandHistory.h"
#include "InputManager.h"

#include <stack>

namespace XTEngine2d
{
	

	class Renderer;

	class Editor
	{
	public:
		Editor();
		~Editor();

		void Init(Window* window);
		void OnImGuiRender(Renderer& renderer, Window* window, float delta, float accumulatedtime, InputManager &input);
		void InitDocking();
		void BeginFrame();
		void EndFrame();
	public:
		int m_GridSize = 32;

	private:

		void CreateDefaultCamera(Scene* scene);

	private:
		Entity m_SelectedEntity = 0;
		ViewportPanel m_ViewportPanel;
		InspectorPanel m_InspectorPanel;
		ScenePanel m_ScenePanel;
		EntityManagerPanel m_EntityManagerPanel;
		HierarchyPanel m_HierarchyPanel;
		AssetsBrowserPanel m_AssetsBrowserPanel;


		CommandHistory m_CommandHistory;
		std::stack<std::string> undoCommandRetrival;
		std::stack<std::string> redoCommandRetrival;

		SceneState m_SceneState = SceneState::Edit;

		std::shared_ptr<Scene> m_EditorScene;
		std::shared_ptr<Scene> m_RuntimeScene;

		bool undo = false;
		bool redo = false;

	public:
		std::shared_ptr<Scene> GetActiveScene()
		{
			if (m_SceneState == SceneState::Play)
				return m_RuntimeScene;
			else
				return m_EditorScene;
		}

	public:
		const SceneState& GetSceneState() const { return m_SceneState; }

	};

}