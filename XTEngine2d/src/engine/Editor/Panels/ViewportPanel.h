#pragma once

#include "../../vendor/glm/glm.hpp"
#include "../../FrameBuffer.h"
#include "../../Scene.h"
#include "../../Renderer.h"
#include "../../Window.h"
#include "../../LineRenderer.h"
#include "../EditorCamera.h"
#include "../../Utility.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../SceneSerializer.h"
#include "../../AssetManager.h"
#include "../../PrefabSerializer.h"


using namespace XTEngine2d;

struct ViewportState
{

	glm::mat4 ViewProjection;

	glm::vec2 CameraPosition;

	float VisibleWidth;
	float VisibleHeight;

	ImVec2 ViewportSize;
	ImVec2 ViewportPosition;	

    bool Hovered;
    bool Focused;

    ImVec2 MousePosition;
};

enum class GizmoAxis
{
	None,
	X,
	Y
};



class ViewportPanel
{
public:
	ViewportPanel();
	~ViewportPanel();
public:
	void Init(Window* window);
	void OnImGuiRender(std::shared_ptr<Scene> EditorScene, std::shared_ptr<Scene> RuntimeScene, Renderer& renderer, Window* window, Entity& SelectedEntity, SceneState& sceneState, float delta);
	Entity GetSelectedEntity();
private:
	ViewportState m_ViewportState;
	XTEngine2d::Framebuffer m_Framebuffer;
	Entity m_SelectedEntity = 0;
	int m_GridSize = 32;
	GizmoAxis m_ActiveAxis = GizmoAxis::None;
	GizmoAxis m_HoveredAxis = GizmoAxis::None;
	LineRenderer m_LineRenderer;
	bool m_Dragging;
	bool m_GizmoDragging;
	float m_GizmoSize = 10.0f;
	glm::vec2 m_GizmoCenter = { 0,0 };
	glm::vec2 m_DragOffset = { 0,0 };

	EditorCamera m_EditorCamera;

	SceneSerializer serializer;
	bool runtimeSceneInitialized = false;

	Entity m_ActiveCamera = 0;



private:
	void UpdateViewport(Scene* scene, Renderer& renderer, Window* window);
	void UpdateEditorCamera(Scene* scene);
	void HandlePicking(Scene* scene, Renderer& renderer);
	void HandleZoom(Scene* scene);
	float DistanceToLineSegment(glm::vec2 p, glm::vec2 a, glm::vec2 b);
	void DrawGrid(Scene* scene, Renderer& renderer, glm::mat4 viewProjection);
	void DrawGizmo(Scene* scene, Renderer& renderer, glm::mat4 viewProjection);
	void DrawOutline(Scene* scene, Renderer& renderer, glm::mat4 viewProjection);
	void UpdateGizmoState(Scene* scene);


	void CreateSpriteEntity(Scene* scene, std::string path);
	void LoadSceneFromDrop(Scene* scene, std::string path);

	void PlayScene(Scene* scene, Renderer& renderer, Window* window);
	void UpdatePlayCamera(Scene* scene);

	const glm::mat4& GetViewProjection() const { return m_ViewportState.ViewProjection; }
};

