#include "ViewportPanel.h"



ViewportPanel::ViewportPanel()
{

}

ViewportPanel::~ViewportPanel()
{
}


void ViewportPanel::Init(Window* window, CommandHistory* cmdHistory)
{
	m_Framebuffer.Init(window->GetWidth(), window->GetHeight());

	CMDHISTORY = cmdHistory;

	m_LineRenderer.Init();
	Shader LineRendererShader("Assets\\shaders\\LineRendererVertexShader.vert", "Assets\\shaders\\LineRendererFragmentShader.frag");
	m_LineRenderer.m_Shader = LineRendererShader;

	m_BatchRenderer.Init();
	Shader BatchRendererShader("Assets\\shaders\\BatchRendererVertexShader.vert", "Assets\\shaders\\BatchRendererFragmentShader.frag");
	m_BatchRenderer.m_Shader = BatchRendererShader;
}



void ViewportPanel::OnImGuiRender(std::shared_ptr<Scene> EditorScene, std::shared_ptr<Scene> RuntimeScene, Renderer& renderer, Window* window, Entity& SelectedEntity, SceneState& sceneState, float delta,float accumulated, int selectedTile)
{	
	m_SelectedEntity = SelectedEntity;
	m_DeltaTime = delta;
	m_AccumulatedTime = accumulated;
	ImGui::Begin("Viewport");

	m_ViewportState.MousePosition = ImGui::GetMousePos();
	m_ViewportState.ViewportSize = ImGui::GetContentRegionAvail();

	m_ViewportState.Hovered = ImGui::IsWindowHovered();
	m_ViewportState.Focused = ImGui::IsWindowFocused();

	m_ViewportState.ViewportPosition = ImGui::GetCursorScreenPos();

	ImGui::Begin("Debug");
	ImGui::Text("Viewport Position: (%.2f, %.2f)", m_ViewportState.ViewportPosition.x, m_ViewportState.ViewportPosition.y);
	ImGui::Text("Camera Position: (%.2f, %.2f)", m_ViewportState.CameraPosition.x, m_ViewportState.CameraPosition.y);
	ImGui::Text("Viewport Size: (%.2f, %.2f)", m_ViewportState.ViewportSize.x, m_ViewportState.ViewportSize.y);
	
	Tag tag;
	
	if (EditorScene->m_Registry.HasComponent<Tag>(m_SelectedEntity))
		tag = EditorScene->m_Registry.GetComponent<Tag>(m_SelectedEntity);

	ImGui::Text("Selected Entity: %s", tag.name.c_str());
	ImGui::End();

	uint32_t width = (uint32_t)m_ViewportState.ViewportSize.x;
	uint32_t height = (uint32_t)m_ViewportState.ViewportSize.y;

	if (width > 0 && height > 0)
	{
		if (m_Framebuffer.GetWidth() != width ||
			m_Framebuffer.GetHeight() != height)
		{
			m_Framebuffer.Resize(width, height);
		}
	}


	switch (sceneState)
	{
	case SceneState::Edit:
		if (RuntimeScene)
		{
			RuntimeScene->m_Registry.Clear();
			RuntimeScene.reset();
		}

		runtimeSceneInitialized = false;
		m_Framebuffer.Bind();

		UpdateEditorCamera(EditorScene.get());
		UpdateViewport(EditorScene.get(), renderer, window);
		DrawGrid(EditorScene.get(), renderer, m_ViewportState.ViewProjection);
		UpdateGizmoState(EditorScene.get());
		DrawGizmo(EditorScene.get(), renderer, m_ViewportState.ViewProjection);
		DrawOutline(EditorScene.get(), renderer, m_ViewportState.ViewProjection);
		UpdateTileMap(EditorScene.get(),selectedTile);
		DrawTileMap(EditorScene.get(), renderer, m_EditorCamera);
		EditorScene->Render(renderer, m_ViewportState.ViewProjection);

		ImGui::Begin("Renderer Stats");

		ImGui::Text("Draw Calls: %u", m_BatchRenderer.m_Stats.DrawCalls);
		ImGui::Text("Quads: %u", m_BatchRenderer.m_Stats.QuadCount);
		ImGui::Text("Vertices: %u", m_BatchRenderer.m_Stats.VertexCount);
		ImGui::Text("Indices: %u", m_BatchRenderer.m_Stats.IndexCount);

		ImGui::End();
		m_BatchRenderer.m_Stats.Reset();

		if (m_ViewportState.Hovered)
		{
			HandleZoom(EditorScene.get());
			HandlePicking(EditorScene.get(), renderer);
		}
		m_Framebuffer.Unbind();
		break;
	case SceneState::Play:
		if (!runtimeSceneInitialized) 
		{
			serializer.Serialize("Assets\\scenes\\TempScene.json", "TempScene", EditorScene.get());
			serializer.Deserialize("Assets\\scenes\\TempScene.json", "TempScene", RuntimeScene.get());
			runtimeSceneInitialized = true;
		}
		m_Framebuffer.Bind();

		UpdatePlayCamera(RuntimeScene.get());
		
		UpdateViewport(RuntimeScene.get(), renderer, window);
		DrawTileMap(RuntimeScene.get(), renderer, RuntimeScene.get()->GetPrimaryCamera());
		AnimationSystem(RuntimeScene.get(), delta);
		PlayScene(RuntimeScene.get(), renderer, window);
		ImGui::Begin("Renderer Stats");

		ImGui::Text("Draw Calls: %u", m_BatchRenderer.m_Stats.DrawCalls);
		ImGui::Text("Quads: %u", m_BatchRenderer.m_Stats.QuadCount);
		ImGui::Text("Vertices: %u", m_BatchRenderer.m_Stats.VertexCount);
		ImGui::Text("Indices: %u", m_BatchRenderer.m_Stats.IndexCount);

		ImGui::End();
		m_BatchRenderer.m_Stats.Reset();
		m_Framebuffer.Unbind();
		break;
	case SceneState::Pause:
		break;
	}




	ImGui::Image((ImTextureID)(uintptr_t)m_Framebuffer.GetColorAttachment(), m_ViewportState.ViewportSize, ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload =
			ImGui::AcceptDragDropPayload("ASSET_PATH"))
		{
			const char* path = (const char*)payload->Data;
			std::filesystem::path filePath(path);

			std::string extension =	filePath.extension().string();
			if (extension == ".png" || extension == ".jpg")
			{
				if (sceneState == SceneState::Edit)
				{
					auto cmd = std::make_shared<CreateSpriteEntityCommand>(EditorScene.get(),std::string(path));
					CMDHISTORY->ExecuteCommand(cmd);
					m_SelectedEntity = cmd->GetCreatedEntity();
				}
			}
			else if (extension == ".json")
			{
				if(sceneState == SceneState::Edit)
					LoadSceneFromDrop(EditorScene.get(), std::string(path));
			}
			else if (extension == ".prefab")
			{
				if (sceneState == SceneState::Edit)
				{
					PrefabSerializer serializer;
					m_SelectedEntity = serializer.DeserializeEntity(EditorScene.get(), std::string(path));
				}
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::End();

}

Entity ViewportPanel::GetSelectedEntity()
{
	return m_SelectedEntity;
}

void ViewportPanel::UpdateViewport(Scene* scene, Renderer& renderer, Window* window)
{
	glViewport(0, 0, m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ViewportPanel::UpdateEditorCamera(Scene* scene)
{

	m_EditorCamera.UpdateCamera();

	m_EditorCamera.SetViewPortSize(m_Framebuffer.GetWidth(),m_Framebuffer.GetHeight());

	m_ViewportState.CameraPosition = m_EditorCamera.position;
	m_ViewportState.ViewProjection = m_EditorCamera.GetProjection() * m_EditorCamera.GetViewMatrix(m_EditorCamera.position);
	m_ViewportState.VisibleWidth = m_Framebuffer.GetWidth() * m_EditorCamera.zoom;
	m_ViewportState.VisibleHeight = m_Framebuffer.GetHeight() * m_EditorCamera.zoom;
}



void ViewportPanel::DrawGrid(Scene* scene, Renderer& renderer, glm::mat4 viewProjection)
{
	float startX = 	floor(m_ViewportState.CameraPosition.x / m_GridSize) * m_GridSize;
	float startY =	floor(m_ViewportState.CameraPosition.y / m_GridSize) * m_GridSize;
			
	for (float x = startX; x < m_ViewportState.CameraPosition.x + m_ViewportState.VisibleWidth; x += m_GridSize)
	{
		m_LineRenderer.DrawLine({ x, startY }, { x, startY + m_ViewportState.VisibleHeight + m_GridSize }, { 1.f, 1.f, 1.f, 1.0f }, viewProjection);
	}

	for (float y = startY; y < m_ViewportState.CameraPosition.y + m_ViewportState.VisibleHeight; y += m_GridSize)
	{
		m_LineRenderer.DrawLine({ startX, y }, { startX + m_ViewportState.VisibleWidth + m_GridSize, y }, { 1.f, 1.f, 1.f, 1.0f }, viewProjection);
	}
}


void ViewportPanel::DrawGizmo(Scene* scene, Renderer& renderer, glm::mat4 viewProjection)
{
	if (m_SelectedEntity && scene->m_Registry.ValidateEntity(m_SelectedEntity))
	{
		if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
		{
			auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


			glm::vec4 ColorX = { 1,0,0,1 };
			glm::vec4 ColorY = { 0,1,0,1 };

			if (m_ActiveAxis == GizmoAxis::X)
				ColorX = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(1, 0, 0, 1);
			else if (m_ActiveAxis == GizmoAxis::Y)
				ColorY = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 1, 0, 1);

			if (m_HoveredAxis == GizmoAxis::X)
			{
				glLineWidth(8.0f);
			}
			else if (m_HoveredAxis == GizmoAxis::Y)
			{
				glLineWidth(8.0f);
			}
			else
				glLineWidth(2.0f);

			m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x + m_GizmoSize, m_GizmoCenter.y }, ColorX, viewProjection);
			m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x, m_GizmoCenter.y + m_GizmoSize }, ColorY, viewProjection);
			glLineWidth(1.0f);
		}


			//if (ImGui::Button("Translate"))
			//	m_CurrentOperation = GizmoOperation::Translate;
			//if (ImGui::Button("Rotate"))
			//	m_CurrentOperation = GizmoOperation::Rotate;
			//if (ImGui::Button("Scale"))
			//	m_CurrentOperation = GizmoOperation::Scale;

			//switch (m_CurrentOperation)
			//{
			//case GizmoOperation::Translate:
			//	if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
			//	{
			//		auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


			//		glm::vec4 ColorX = { 1,0,0,1 };
			//		glm::vec4 ColorY = { 0,1,0,1 };

			//		if (m_ActiveAxis == GizmoAxis::X)
			//			ColorX = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(1, 0, 0, 1);
			//		else if (m_ActiveAxis == GizmoAxis::Y)
			//			ColorY = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 1, 0, 1);

			//		if (m_HoveredAxis == GizmoAxis::X)
			//		{
			//			glLineWidth(8.0f);
			//		}
			//		else if (m_HoveredAxis == GizmoAxis::Y)
			//		{
			//			glLineWidth(8.0f);
			//		}
			//		else
			//			glLineWidth(2.0f);

			//		m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x + m_GizmoSize, m_GizmoCenter.y }, ColorX, viewProjection);
			//		m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x, m_GizmoCenter.y + m_GizmoSize }, ColorY, viewProjection);
			//		glLineWidth(1.0f);
			//	}
			//	break;
			//case GizmoOperation::Rotate:
			//	if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
			//	{
			//		auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


			//		glm::vec4 ColorX = { 0,0,1,1 };
			//		glm::vec4 ColorY = { 0,1,0,1 };

			//		if (m_ActiveAxis == GizmoAxis::X)
			//			ColorX = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 1, 1);
			//		else if (m_ActiveAxis == GizmoAxis::Y)
			//			ColorY = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 1, 0, 1);

			//		if (m_HoveredAxis == GizmoAxis::X)
			//		{
			//			glLineWidth(8.0f);
			//		}
			//		else if (m_HoveredAxis == GizmoAxis::Y)
			//		{
			//			glLineWidth(8.0f);
			//		}
			//		else
			//			glLineWidth(2.0f);

			//		m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x + m_GizmoSize, m_GizmoCenter.y }, ColorX, viewProjection);
			//		m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x, m_GizmoCenter.y + m_GizmoSize }, ColorY, viewProjection);
			//		glLineWidth(1.0f);
			//	}
			//	break;
			//case GizmoOperation::Scale:
			//	if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
			//	{
			//		auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


			//		glm::vec4 ColorX = { 1,0,0,1 };
			//		glm::vec4 ColorY = { 0,1,0,1 };

			//		if (m_ActiveAxis == GizmoAxis::X)
			//			ColorX = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(1, 0, 0, 1);
			//		else if (m_ActiveAxis == GizmoAxis::Y)
			//			ColorY = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 1, 0, 1);

			//		if (m_HoveredAxis == GizmoAxis::X)
			//		{
			//			glLineWidth(8.0f);
			//		}
			//		else if (m_HoveredAxis == GizmoAxis::Y)
			//		{
			//			glLineWidth(8.0f);
			//		}
			//		else
			//			glLineWidth(2.0f);

			//		m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x + m_GizmoSize, m_GizmoCenter.y }, ColorX, viewProjection);
			//		m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x, m_GizmoCenter.y + m_GizmoSize }, ColorY, viewProjection);
			//		glLineWidth(1.0f);
			//	}
			//	break;
			//}
	}
}
void ViewportPanel::DrawOutline(Scene* scene, Renderer& renderer, glm::mat4 viewProjection)
{
	if (m_SelectedEntity && scene->m_Registry.ValidateEntity(m_SelectedEntity))
	{
		if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity) && scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity))
		{
			auto& transform =
				scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


			Transform outlineTransform;
			outlineTransform.worldPosition = transform.worldPosition - glm::vec2(2, 2);
			outlineTransform.size = transform.size + glm::vec2(4, 4);
			outlineTransform.localRotation = transform.localRotation;
			glLineWidth(5.0f);
			m_LineRenderer.DrawLine(outlineTransform.worldPosition, { outlineTransform.worldPosition.x + outlineTransform.size.x, outlineTransform.worldPosition.y }, { 1,1,1,1 }, viewProjection);
			m_LineRenderer.DrawLine({ outlineTransform.worldPosition.x + outlineTransform.size.x, outlineTransform.worldPosition.y }, { outlineTransform.worldPosition.x + outlineTransform.size.x, outlineTransform.worldPosition.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
			m_LineRenderer.DrawLine(outlineTransform.worldPosition, { outlineTransform.worldPosition.x, outlineTransform.worldPosition.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
			m_LineRenderer.DrawLine({ outlineTransform.worldPosition.x, outlineTransform.worldPosition.y + outlineTransform.size.y }, { outlineTransform.worldPosition.x + outlineTransform.size.x, outlineTransform.worldPosition.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
			glLineWidth(1.0f);

		}

	}

}

void ViewportPanel::UpdateGizmoState(Scene* scene)
{
	if (!m_SelectedEntity || !scene->m_Registry.ValidateEntity(m_SelectedEntity))
		return;
	
	if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
	{
		auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);
		m_GizmoCenter =
		{
			transform.localPosition.x + transform.size.x * 0.5f,
			transform.localPosition.y + transform.size.y * 0.5f
		};
	}


}
void ViewportPanel::DrawTileMap(Scene* scene,Renderer& renderer, Entity camera)
{
	auto tilemaps =	scene->m_Registry.View<TileMap, Transform>();

	float cameraZoom = 1.0f;
	glm::vec2 cameraPosition = {0.0f, 0.0f};


	if (scene->m_Registry.HasComponent<Transform>(camera))
	{
		auto& transform = scene->m_Registry.GetComponent<Transform>(camera);
		cameraPosition = transform.localPosition;
		if (scene->m_Registry.HasComponent<Camera>(camera))
		{
			auto& cameraComponent = scene->m_Registry.GetComponent<Camera>(camera);
			cameraZoom = cameraComponent.zoom;
		}
	}

	for (auto entity : tilemaps)
	{
		m_BatchRenderer.BeginBatch();

		auto& tilemap =	scene->m_Registry.GetComponent<TileMap>(entity);

		auto& transform = scene->m_Registry.GetComponent<Transform>(entity);

		if (!tilemap.texture)
			continue;

		float localCamX =
			cameraPosition.x -
			transform.localPosition.x;

		float localCamY =
			cameraPosition.y -
			transform.localPosition.y;


		float visibleWidth = m_Framebuffer.GetWidth() * cameraZoom;

		float visibleHeight = m_Framebuffer.GetHeight() * cameraZoom;



		int startX =
			localCamX / tilemap.gridSize;

		int endX =
			(localCamX + visibleWidth) /
			tilemap.gridSize;

		int startY =
			localCamY / tilemap.gridSize;

		int endY =
			(localCamY + visibleHeight) /
			tilemap.gridSize;

		startX = std::max(0, startX);
		startY = std::max(0, startY);

		endX = std::min(tilemap.width - 1, endX);
		endY = std::min(tilemap.height - 1, endY);

		ImGui::Begin("Debug");
		ImGui::Text("Camera Position: (%.2f, %.2f)", cameraPosition.x, cameraPosition.y);
		ImGui::Text("Camera Zoom: %.2f", cameraZoom);
		ImGui::Text("Visible Width: %.2f", visibleWidth);
		ImGui::Text("Visible Height: %.2f", visibleHeight);
		ImGui::Text("startX %d", startX);
		ImGui::Text("endX %d", endX);

		ImGui::Text("startY %d", startY);
		ImGui::Text("endY %d", endY);
		ImGui::End();

		SpriteSheet sheet(tilemap.texture.get(),tilemap.gridSize,tilemap.gridSize);

		for (int y = startY; y <= endY; y++)
		{
			for (int x = startX; x <= endX; x++)
			{
				int index =	y * tilemap.width + x;

				if (index >= tilemap.tiles.size())
					continue;

				int tile = tilemap.tiles[index];

				if (tile == -1)
					continue;

				float worldX = transform.localPosition.x + x * tilemap.gridSize;

				float worldY = transform.localPosition.y + y * tilemap.gridSize;

				glm::vec2 pos =
				{
					worldX,
					worldY
				};

				glm::vec4 uv = sheet.GetUVFromFrame(tile);
				m_BatchRenderer.SubmitQuad(pos, glm::vec2(tilemap.gridSize, tilemap.gridSize), glm::vec4(1, 1, 1,1), uv);
			
				//renderer.DrawQuad(scene->m_Shader,tilemap.texture.get(),pos,uv, glm::vec2(tilemap.gridSize, tilemap.gridSize));
			}
		}
		m_BatchRenderer.EndBatch();
		m_BatchRenderer.Flush(tilemap.texture, m_ViewportState.ViewProjection);
	}
}

void ViewportPanel::DrawTileMap(Scene* scene,Renderer& renderer, EditorCamera camera)
{
	auto tilemaps = scene->m_Registry.View<TileMap, Transform>();


	for (auto entity : tilemaps)
	{
		m_BatchRenderer.BeginBatch();
		auto& tilemap = scene->m_Registry.GetComponent<TileMap>(entity);

		auto& transform = scene->m_Registry.GetComponent<Transform>(entity);

		if (!tilemap.texture)
			continue;


		float localCamX =
			m_ViewportState.CameraPosition.x -
			transform.localPosition.x;

		float localCamY =
			m_ViewportState.CameraPosition.y -
			transform.localPosition.y;



		float visibleWidth = m_Framebuffer.GetWidth() * camera.zoom;

		float visibleHeight = m_Framebuffer.GetHeight() * camera.zoom;

		int startX =
			localCamX / tilemap.gridSize;

		int endX =
			(localCamX + visibleWidth) /
			tilemap.gridSize;

		int startY =
			localCamY / tilemap.gridSize;

		int endY =
			(localCamY + visibleHeight) /
			tilemap.gridSize;


		//int startX = cameraPosition.x / tilemap.gridSize;
		//int endX = (cameraPosition.x + visibleWidth) / tilemap.gridSize;

		//int startY = cameraPosition.y / tilemap.gridSize;
		//int endY = (cameraPosition.y + visibleHeight) / tilemap.gridSize;

		startX = std::max(0, startX);
		startY = std::max(0, startY);

		endX = std::min(tilemap.width - 1, endX);
		endY = std::min(tilemap.height - 1, endY);
		
		ImGui::Begin("Debug");
		ImGui::Text("Camera Position: (%.2f, %.2f)", camera.position.x, camera.position.y);
		ImGui::Text("Camera Zoom: %.2f", camera.zoom);
		ImGui::Text("Visible Width: %.2f", m_ViewportState.VisibleWidth);
		ImGui::Text("Visible Height: %.2f", m_ViewportState.VisibleHeight);
		ImGui::Text("startX %d", startX);
		ImGui::Text("endX %d", endX);

		ImGui::Text("startY %d", startY);
		ImGui::Text("endY %d", endY);
		ImGui::End();

		SpriteSheet sheet(tilemap.texture.get(), tilemap.gridSize, tilemap.gridSize);

		for (int y = startY; y <= endY; y++)
		{
			for (int x = startX; x <= endX; x++)
			{
				int index = y * tilemap.width + x;

				if (index >= tilemap.tiles.size())
					continue;
				

				int baseTile = tilemap.tiles[index];
				if (baseTile == -1) continue;

				int finalTile = baseTile;
				
				if (tilemap.isAnimated && tilemap.m_FrameCount > 0) {
					// Calculates current frame index based on game time
					int currentFrame = (int)(m_AccumulatedTime * tilemap.animationSpeed) % tilemap.m_FrameCount;
					finalTile = baseTile + currentFrame;
				}

				float worldX = transform.localPosition.x + x * tilemap.gridSize;

				float worldY = transform.localPosition.y + y * tilemap.gridSize;

				glm::vec2 pos =
				{
					worldX,
					worldY
				};

				glm::vec4 uv = sheet.GetUVFromFrame(finalTile);

				m_BatchRenderer.SubmitQuad(pos, glm::vec2(tilemap.gridSize, tilemap.gridSize), glm::vec4(1, 1, 1, 1), uv);
			}
		}
		m_BatchRenderer.EndBatch();
		m_BatchRenderer.Flush(tilemap.texture, m_ViewportState.ViewProjection);
	}
}

void ViewportPanel::UpdateTileMap(Scene* scene, int selectedTile)
{
	auto entities = scene->m_Registry.View<Transform, TileMap>();
	bool insideTilemap = false;


	ImVec2 localMouse =
	{
		m_ViewportState.MousePosition.x - m_ViewportState.ViewportPosition.x,
		m_ViewportState.MousePosition.y - m_ViewportState.ViewportPosition.y
	};

	float x = localMouse.x / m_ViewportState.ViewportSize.x;
	float y = localMouse.y / m_ViewportState.ViewportSize.y;


	float halfWidth = m_ViewportState.VisibleWidth * 0.5f;
	float halfHeight = m_ViewportState.VisibleHeight * 0.5f;

	float worldX =
		m_ViewportState.CameraPosition.x +
		x * m_ViewportState.VisibleWidth;

	float worldY =
		m_ViewportState.CameraPosition.y +
		y * m_ViewportState.VisibleHeight;


	for (auto entity : entities)
	{
		if (m_SelectedEntity == entity)
		{
			auto& transform = scene->m_Registry.GetComponent<Transform>(entity);
			auto& tilemap = scene->m_Registry.GetComponent<TileMap>(entity);

			if (worldX < transform.localPosition.x || worldY < transform.localPosition.y ||
				worldX > transform.localPosition.x + tilemap.width * tilemap.gridSize ||
				worldY > transform.localPosition.y + tilemap.height * tilemap.gridSize)
			{
				insideTilemap = true;
				continue;
			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && m_ViewportState.Hovered)
			{
				int tileX = (int)((worldX - transform.localPosition.x) / tilemap.gridSize);
				int tileY = (int)((worldY - transform.localPosition.y) / tilemap.gridSize);

				int index = tileY * tilemap.width + tileX;

				if (index >= 0 && index < tilemap.tiles.size())
				{
					int prevTile = tilemap.tiles[index];
					if (!m_OriginalTiles.contains(index))
					{
						m_OriginalTiles[index] = prevTile;
					}
					if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
					{
						if (tilemap.tiles[index] != -1)
						{
							isPaintedTileModified = true;
							tilemap.tiles[index] = -1;
							m_ModifiedTiles[index] = -1;
						}
					}
					else if (ImGui::IsKeyDown(ImGuiKey_Space))
					{
						if (tileX >= 4 && tileX <= tilemap.tiles.size() - 4 && tileY >= 4 && tileY <= tilemap.tiles.size() - 4)
						{
							isPaintedTileModified = true;

							tilemap.tiles[index] = selectedTile;
							m_ModifiedTiles[index] = selectedTile;

							if (tileY > 4 && tileY < tilemap.height - 4 && tileX > 4 && tileX < tilemap.width - 4)
							{
								int radius = 4;

								for (int y = -radius; y <= radius; y++)
								{
									for (int x = -radius; x <= radius; x++)
									{
										int tx = tileX + x;
										int ty = tileY + y;

										if (tx < 0 || tx >= tilemap.width)
											continue;

										if (ty < 0 || ty >= tilemap.height)
											continue;

										int index = ty * tilemap.width + tx;

										tilemap.tiles[index] = selectedTile;
										m_ModifiedTiles[index] = selectedTile;
									}
								}
							}

						}
					}
					else
					{
						if (tilemap.tiles[index] != selectedTile)
						{
							isPaintedTileModified= true;
							tilemap.tiles[index] = selectedTile;
							m_ModifiedTiles[index] = selectedTile;
						}
					}
				}
			}

			if (ImGui::IsMouseReleased(0))
			{
				if (isPaintedTileModified)
				{
					auto cmd = std::make_shared<TilePaintCommand>(scene, entity, m_ModifiedTiles, m_OriginalTiles);
					CMDHISTORY->ExecuteCommand(cmd);
					isPaintedTileModified = false;
				}
				m_ModifiedTiles.clear();
				m_OriginalTiles.clear();
			}
		}
	}
}

void ViewportPanel::CreateSpriteEntity(Scene* scene, std::string path)
{
	Entity entity = scene->m_Registry.CreateEntity();

	Transform transform;
	Sprite sprite;
	SpriteSheet sheet;

	Tag tag;
	tag.name = "Entity";

	sprite.texture = AssetManager::LoadTexture(path);
	sheet .SetTexture(sprite.texture.get(), sprite.texture->GetWidth(), sprite.texture->GetHeight());
	sprite.uv = sheet.GetUV(1, 1);
	scene->m_Registry.AddComponent(entity, transform);
	scene->m_Registry.AddComponent(entity, sprite);
	scene->m_Registry.AddComponent(entity, sheet);
	scene->m_Registry.AddComponent(entity, tag);
	m_SelectedEntity = entity;
		
}

void ViewportPanel::LoadSceneFromDrop(Scene* scene, std::string path)
{
	if (scene)
	{
		std::string SceneName = "New Scene";
		scene->Clear();
		SceneSerializer serialize;
		serialize.Deserialize(path, SceneName, scene);
	}
}

void ViewportPanel::UpdatePlayCamera(Scene* scene)
{

	scene->CheckPrimaryCameraAvailability();
	if (!scene->HasPrimaryCamera() && !scene->m_Registry.ValidateEntity(scene->GetPrimaryCamera()))
		return;

	m_ViewportState.ViewProjection = glm::mat4(1.0f);
	Camera playCamera = scene->m_Registry.GetComponent<Camera>(scene->GetPrimaryCamera());

	if (scene->m_Registry.HasComponent<Transform>(scene->GetPrimaryCamera()))
	{
		auto& camTransform = scene->m_Registry.GetComponent<Transform>(scene->GetPrimaryCamera());
		playCamera.SetViewPortSize(m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight());

		m_ViewportState.CameraPosition = camTransform.localPosition;
		m_ViewportState.ViewProjection = playCamera.GetProjection() * playCamera.GetViewMatrix(camTransform);
		m_ViewportState.VisibleWidth = m_Framebuffer.GetWidth() * playCamera.zoom;
		m_ViewportState.VisibleHeight = m_Framebuffer.GetHeight() * playCamera.zoom;
	}
}

void ViewportPanel::PlayScene(Scene* scene, Renderer& renderer, Window* window)
{
	if (!scene->GetPrimaryCamera() || !scene->m_Registry.ValidateEntity(scene->GetPrimaryCamera()))
		return;

	scene->Render(renderer, m_ViewportState.ViewProjection);
}

void ViewportPanel::HandlePicking(Scene* scene, Renderer& renderer)
{
	auto entities = scene->m_Registry.View<Transform, Sprite>();


	ImVec2 localMouse =
	{
		m_ViewportState.MousePosition.x - m_ViewportState.ViewportPosition.x,
		m_ViewportState.MousePosition.y - m_ViewportState.ViewportPosition.y
	};
	ImGui::Begin("Debug");
	ImGui::Text("Local Mouse Position: (%.2f, %.2f)", localMouse.x, localMouse.y);
	ImGui::End();
	// Check mouse inside viewport
	if (localMouse.x < 0 || localMouse.y < 0 ||
		localMouse.x > m_ViewportState.ViewportSize.x ||
		localMouse.y > m_ViewportState.ViewportSize.y)
	{
		ImGui::Begin("Debug");
		ImGui::Text("Mouse outside viewport");
		ImGui::End();
		return;
	}

	float x = localMouse.x / m_ViewportState.ViewportSize.x;
	float y = localMouse.y / m_ViewportState.ViewportSize.y;

	float halfWidth = m_ViewportState.VisibleWidth * 0.5f;
	float halfHeight = m_ViewportState.VisibleHeight * 0.5f;

	float worldX =
		m_ViewportState.CameraPosition.x +
		x * m_ViewportState.VisibleWidth;

	float worldY =
		m_ViewportState.CameraPosition.y +
		y * m_ViewportState.VisibleHeight;

	if (m_SelectedEntity && scene->m_Registry.ValidateEntity(m_SelectedEntity))
	{
		if (!scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
			return;

		auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


		glm::vec2 xStart = m_GizmoCenter;

		glm::vec2 xEnd =
		{
			m_GizmoCenter.x + m_GizmoSize,
			m_GizmoCenter.y
		};

		glm::vec2 yStart = m_GizmoCenter;

		glm::vec2 yEnd =
		{
			m_GizmoCenter.x,
			m_GizmoCenter.y + m_GizmoSize
		};
		glm::vec2 mouse =
		{
			worldX,
			worldY
		};

		float distX = DistanceToLineSegment(mouse, xStart, xEnd);

		float distY = DistanceToLineSegment(mouse, yStart, yEnd);

		float thickness = 8.0f;

		if (distX < thickness)
		{
			m_ActiveAxis = GizmoAxis::X;
			m_HoveredAxis = GizmoAxis::X;
		}
		else if (distY < thickness)
		{
			m_ActiveAxis = GizmoAxis::Y;
			m_HoveredAxis = GizmoAxis::Y;
		}
		else
			m_HoveredAxis = GizmoAxis::None;

	}

	if (ImGui::IsMouseClicked(0))
	{
		ImGui::Begin("Debug");
		ImGui::Text("Mouse Clicked at: (%.2f, %.2f)", worldX, worldY);
		ImGui::End();
		for (auto entity : entities)
		{
			if (!scene->m_Registry.HasComponent<Transform>(entity))
				continue;

			if (!scene->m_Registry.HasComponent<Sprite>(entity))
				continue;

			auto& transform =
				scene->m_Registry.GetComponent<Transform>(entity);


			float left = transform.localPosition.x;
			float right = transform.localPosition.x + transform.size.x;
			float top = transform.localPosition.y;
			float bottom = transform.localPosition.y + transform.size.y;

			if (worldX >= left && worldX <= right &&
				worldY >= top && worldY <= bottom)
			{
				m_SelectedEntity = entity;
				insideSelectedEntity = true;
				oldPosition = transform.localPosition;
		
				ImGui::Begin("Debug");
				Tag tag;
				if (scene->m_Registry.HasComponent<Tag>(m_SelectedEntity))
					tag = scene->m_Registry.GetComponent<Tag>(m_SelectedEntity);
				
				ImGui::Text("Selected Entity On Mouse Click: %s", tag.name.c_str());
				ImGui::End();

				m_Dragging = true;

				m_DragOffset =
				{
					worldX - transform.localPosition.x,
					worldY - transform.localPosition.y
				};
				break;
			}

		}
	}

	
	if (m_Dragging && ImGui::IsMouseDown(0))
	{
		if (!scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
			return;
		auto& transform =
			scene->m_Registry.GetComponent<Transform>(
				m_SelectedEntity);
		float targetX =
			worldX - m_DragOffset.x;

		float targetY =
			worldY - m_DragOffset.y;




		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		{
			transform.localPosition.x =
				round(targetX / m_GridSize)
				* m_GridSize;

			transform.localPosition.y =
				round(targetY / m_GridSize)
				* m_GridSize;
		}

		else if (m_ActiveAxis == GizmoAxis::X)
		{
			transform.localPosition.x = targetX;
			m_GizmoDragging = true;
		}
		else if (m_ActiveAxis == GizmoAxis::Y)
		{
			transform.localPosition.y = targetY;
			m_GizmoDragging = true;
		}
		else
		{
			transform.localPosition.x = targetX;
			transform.localPosition.y = targetY;
			m_GizmoDragging = false;

		}

		insideSelectedEntity = true;
	}

	if (ImGui::IsMouseReleased(0))
	{
		m_Dragging = false;
		m_ActiveAxis = GizmoAxis::None;
		m_GizmoDragging = false;
		if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
		{
			auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);
			newPosition = transform.localPosition;
			if (insideSelectedEntity)
			{
				if (oldPosition != newPosition)
				{
					auto cmd = std::make_shared<MoveEntityCommand>(scene, m_SelectedEntity, newPosition, oldPosition);
					CMDHISTORY->ExecuteCommand(cmd);
				}
			}
		}
		insideSelectedEntity = false;
	}
}

void ViewportPanel::HandleZoom(Scene* scene)
{

	if (ImGui::IsWindowHovered())
	{
		float scroll = ImGui::GetIO().MouseWheel;
		m_EditorCamera.zoom -= scroll * 0.1f;
		m_EditorCamera.zoom = glm::clamp(m_EditorCamera.zoom, 0.1f, 10.0f);
		if (ImGui::IsMouseDown(2))
		{
			ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;

			m_EditorCamera.position.x -= mouseDelta.x * m_EditorCamera.zoom;
			m_EditorCamera.position.y -= mouseDelta.y * m_EditorCamera.zoom;
		}

	}
	m_GizmoSize = 40.0f * m_EditorCamera.zoom;
}

float ViewportPanel::DistanceToLineSegment(glm::vec2 p, glm::vec2 a, glm::vec2 b)
{
	glm::vec2 ab = b - a;

	float t =
		glm::dot(p - a, ab)
		/ glm::dot(ab, ab);

	t = glm::clamp(t, 0.0f, 1.0f);

	glm::vec2 closest =
		a + ab * t;	

	return glm::distance(p, closest);
}
