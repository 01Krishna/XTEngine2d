#include "ViewportPanel.h"



ViewportPanel::ViewportPanel()
{

}

ViewportPanel::~ViewportPanel()
{
}


void ViewportPanel::Init(Window* window)
{
	m_Framebuffer.Init(window->GetWidth(), window->GetHeight());

	m_LineRenderer.Init();
	Shader LineRendererShader("Assets\\shaders\\LineRendererVertexShader.vert", "Assets\\shaders\\LineRendererFragmentShader.frag");
	m_LineRenderer.m_Shader = LineRendererShader;
}



void ViewportPanel::OnImGuiRender(std::shared_ptr<Scene> EditorScene, std::shared_ptr<Scene> RuntimeScene, Renderer& renderer, Window* window, Entity& SelectedEntity, SceneState& sceneState, float delta)
{	
	m_SelectedEntity = SelectedEntity;

	ImGui::Begin("Viewport");

	m_ViewportState.MousePosition = ImGui::GetMousePos();
	m_ViewportState.ViewportSize = ImGui::GetContentRegionAvail();

	m_ViewportState.Hovered = ImGui::IsWindowHovered();
	m_ViewportState.Focused = ImGui::IsWindowFocused();

	m_ViewportState.ViewportPosition = ImGui::GetCursorScreenPos();

	ImGui::Begin("Debug");
	ImGui::Text("Mouse Position: (%.2f, %.2f)", m_ViewportState.MousePosition.x, m_ViewportState.MousePosition.y);
	ImGui::Text("Viewport Position: (%.2f, %.2f)", m_ViewportState.ViewportPosition.x, m_ViewportState.ViewportPosition.y);
	ImGui::Text("Camera Position: (%.2f, %.2f)", m_ViewportState.CameraPosition.x, m_ViewportState.CameraPosition.y);
	ImGui::Text("Viewport Size: (%.2f, %.2f)", m_ViewportState.ViewportSize.x, m_ViewportState.ViewportSize.y);
	ImGui::Text("Hovered: %s", m_ViewportState.Hovered ? "Yes" : "No");
	ImGui::Text("Focused: %s", m_ViewportState.Focused ? "Yes" : "No");
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
		EditorScene->Render(renderer, m_ViewportState.ViewProjection);
		UpdateGizmoState(EditorScene.get());
		DrawGizmo(EditorScene.get(), renderer, m_ViewportState.ViewProjection);
		DrawOutline(EditorScene.get(), renderer, m_ViewportState.ViewProjection);
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
		PlayScene(RuntimeScene.get(), renderer, window);
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
				if(sceneState == SceneState::Edit)
					CreateSpriteEntity(EditorScene.get(), std::string(path));
			}
			else if (extension == ".json")
			{
				if(sceneState == SceneState::Edit)
					LoadSceneFromDrop(EditorScene.get(), std::string(path));
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
			auto& transform =  scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


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

			m_LineRenderer.DrawLine(m_GizmoCenter, { m_GizmoCenter.x + m_GizmoSize, m_GizmoCenter.y },ColorX,viewProjection);
			m_LineRenderer.DrawLine(m_GizmoCenter,	{ m_GizmoCenter.x, m_GizmoCenter.y + m_GizmoSize },ColorY,viewProjection);
			glLineWidth(1.0f);
		}
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
			outlineTransform.position = transform.position - glm::vec2(2, 2);
			outlineTransform.size = transform.size + glm::vec2(4, 4);
			outlineTransform.rotation = transform.rotation;
			glLineWidth(3.0f);
			m_LineRenderer.DrawLine(outlineTransform.position, { outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y }, { 1,1,1,1 }, viewProjection);
			m_LineRenderer.DrawLine({ outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y }, { outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
			m_LineRenderer.DrawLine(outlineTransform.position, { outlineTransform.position.x, outlineTransform.position.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
			m_LineRenderer.DrawLine({ outlineTransform.position.x, outlineTransform.position.y + outlineTransform.size.y }, { outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
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
			transform.position.x + transform.size.x * 0.5f,
			transform.position.y + transform.size.y * 0.5f
		};
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

		m_ViewportState.CameraPosition = camTransform.position;
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

			float left = transform.position.x;
			float right = transform.position.x + transform.size.x;
			float top = transform.position.y;
			float bottom = transform.position.y + transform.size.y;

			if (worldX >= left && worldX <= right &&
				worldY >= top && worldY <= bottom)
			{
				m_SelectedEntity = entity;

				ImGui::Begin("Debug");
				Tag tag;
				if (scene->m_Registry.HasComponent<Tag>(m_SelectedEntity))
					tag = scene->m_Registry.GetComponent<Tag>(m_SelectedEntity);
				
				ImGui::Text("Selected Entity On Mouse Click: %s", tag.name.c_str());
				ImGui::End();

				m_Dragging = true;

				m_DragOffset =
				{
					worldX - transform.position.x,
					worldY - transform.position.y
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
			transform.position.x =
				round(targetX / m_GridSize)
				* m_GridSize;

			transform.position.y =
				round(targetY / m_GridSize)
				* m_GridSize;
		}

		else if (m_ActiveAxis == GizmoAxis::X)
		{
			transform.position.x = targetX;
			m_GizmoDragging = true;
		}
		else if (m_ActiveAxis == GizmoAxis::Y)
		{
			transform.position.y = targetY;
			m_GizmoDragging = true;
		}
		else
		{
			transform.position.x = targetX;
			transform.position.y = targetY;
			m_GizmoDragging = false;

		}
	}

	if (ImGui::IsMouseReleased(0))
	{
		m_Dragging = false;
		m_ActiveAxis = GizmoAxis::None;
		m_GizmoDragging = false;
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
