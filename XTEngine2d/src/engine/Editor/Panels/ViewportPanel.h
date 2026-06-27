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
#include "../../CommandHistory.h"
#include "../../Commands/MoveEntityCommand.h"
#include "../../Commands/CreateSpriteEntityCommand.h"
#include "../../TileMap.h"
#include "../../Commands/TilePaintCommand.h"
#include "../../BatchRenderer.h"



using namespace XTEngine2d;

struct ViewportState
{

	glm::mat4 ViewProjection;

	glm::vec2 CameraPosition;
	glm::vec2 CameraSize;
	float CameraZoom;

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

enum class GizmoOperation
{
	Translate,
	Rotate,
	Scale
};



class ViewportPanel
{
public:
	ViewportPanel();
	~ViewportPanel();
public:
	void Init(Window* window, CommandHistory* cmdHistory);
	void OnImGuiRender(std::shared_ptr<Scene> EditorScene, std::shared_ptr<Scene> RuntimeScene, Renderer& renderer, Window* window, Entity& SelectedEntity, SceneState& sceneState, float delta, float accumulated,  int selectedTile);
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
	GizmoOperation m_CurrentOperation;

	glm::vec2 oldPosition = { 0.0f,0.0f };
	glm::vec2 newPosition = { 0.0f,0.0f };

	bool isMoved = false;
	bool isPaintedTileModified = false;

	bool insideSelectedEntity = false;


	CommandHistory* CMDHISTORY;

	EditorCamera m_EditorCamera;

	SceneSerializer serializer;
	bool runtimeSceneInitialized = false;
	
	BatchRenderer m_BatchRenderer;

	Entity m_ActiveCamera = 0;

	std::unordered_map<int, int> m_ModifiedTiles;
	std::unordered_map<int, int> m_OriginalTiles;


	float m_DeltaTime = 0.0f;
	float m_AccumulatedTime = 0.0f; 

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

	void DrawTileMap(Scene* scene,Renderer& renderer, Entity camera);
	void DrawTileMap(Scene* scene,Renderer& renderer,EditorCamera camera);
	void UpdateTileMap(Scene* scene,int selectedTile);


	void CreateSpriteEntity(Scene* scene, std::string path);
	void LoadSceneFromDrop(Scene* scene, std::string path);

	void PlayScene(Scene* scene, Renderer& renderer, Window* window);
	void UpdatePlayCamera(Scene* scene);

	const glm::mat4& GetViewProjection() const { return m_ViewportState.ViewProjection; }
};

