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


namespace XTEngine2d
{
	

	class Renderer;

	class Editor
	{
	public:
		Editor();
		~Editor();

		void Init(Window* window);
		//void FrameBufferInit(int width, int height);
		void OnImGuiRender(Renderer& renderer, Window* window, float delta, InputManager &input);
		//void ViewportSetup(Scene* scene, Renderer& renderer, Window* window);
		//void HandlePicking(Scene* scene, Renderer& renderer);
		//void HandleZoom(Scene* scene);
		//float DistanceToLineSegment(glm::vec2 p, glm::vec2 a, glm::vec2 b);
		//void DrawGrid(Scene* scene, Renderer& renderer, glm::mat4 viewProjection);
		//void DrawGizmo(Scene* scene, Renderer& renderer, glm::mat4 viewProjection);
		//void DrawOutline(Scene* scene, Renderer& renderer, glm::mat4 viewProjection);
		void InitDocking();
		void BeginFrame();
		void EndFrame();
	public:
		int m_GridSize = 32;

	private:
		//void DrawHierarchy(std::vector<Entity>& entities, Scene* scene);
		//void DrawInspector(std::vector<Entity>& entities, Scene* scene);
		//void DrawSceneControls(std::vector<Entity>& entities, Scene* scene);
		//void DrawEntityManager(std::vector<Entity>& entities, Scene* scene);

		//void RenderViewport(Scene* scene, Renderer& renderer, Window* window);

		void CreateDefaultCamera(Scene* scene);

	private:
		Entity m_SelectedEntity = 0;
		ViewportPanel m_ViewportPanel;
		InspectorPanel m_InspectorPanel;
		ScenePanel m_ScenePanel;
		EntityManagerPanel m_EntityManagerPanel;
		HierarchyPanel m_HierarchyPanel;
		AssetsBrowserPanel m_AssetsBrowserPanel;

		SceneState m_SceneState = SceneState::Edit;

		std::shared_ptr<Scene> m_EditorScene;
		std::shared_ptr<Scene> m_RuntimeScene;

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