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
		m_ViewportPanel.Init(window);
		m_EditorScene = std::make_shared<Scene>();
		m_RuntimeScene = std::make_shared<Scene>();
		XTEngine2d::Shader shader("Assets\\shaders\\VertexShader.vert", "Assets\\shaders\\FragmentShader.frag");
		m_EditorScene->m_Shader = shader;
		m_RuntimeScene->m_Shader = shader;

		m_AssetsBrowserPanel.LoadIcons();

	}
//	void Editor::FrameBufferInit(int width, int height)
	//{
		//m_Framebuffer.Init(width, height);
	//}
	void Editor::OnImGuiRender(Renderer& renderer, Window* window, float delta, InputManager& input)
	{
		m_ViewportPanel.OnImGuiRender(m_EditorScene, m_RuntimeScene, renderer, window, m_SelectedEntity, m_SceneState, delta);
		m_SelectedEntity = m_ViewportPanel.GetSelectedEntity();
		m_HierarchyPanel.OnImGuiRender(GetActiveScene().get(), m_SelectedEntity);
		m_InspectorPanel.OnImGuiRender(GetActiveScene().get(), m_SelectedEntity, m_AssetsBrowserPanel.GetSelectedAsset());
		m_ScenePanel.OnImGuiRender(GetActiveScene().get(), m_SceneState);
		m_EntityManagerPanel.OnImGuiRender(GetActiveScene().get(), m_SelectedEntity);
		m_AssetsBrowserPanel.OnImGuiRender();
		GetActiveScene()->Update(delta, input);
	}
	//void Editor::ViewportSetup(Scene* scene, Renderer& renderer, Window* window)
	//{

		//m_Framebuffer.Resize(window->GetWidth(), window->GetHeight());
		//m_Framebuffer.Bind();

		//glViewport(0, 0,
		//	m_Framebuffer.GetWidth(),
		//	m_Framebuffer.GetHeight());
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 viewProjection = glm::mat4(1.0f);


		//if (scene->m_Registry.HasComponent<Transform>(scene->m_PrimaryCamera))
		//{
		//	auto& prCamTransform = scene->m_Registry.GetComponent<Transform>(scene->m_PrimaryCamera);

		//	auto& prCam = scene->m_Registry.GetComponent<Camera>(scene->m_PrimaryCamera);

		//	if (prCam.primary)
		//	{
		//		viewProjection = prCam.GetProjection() * prCam.GetViewMatrix(prCamTransform);
		//	}
		//}

		//DrawGrid(scene, renderer, viewProjection);


		//scene->Render(renderer, viewProjection);

		//DrawOutline(scene, renderer, viewProjection);
		//DrawGizmo(scene, renderer, viewProjection);

		//m_Framebuffer.Unbind();
		//}
	//void Editor::DrawHierarchy(std::vector<Entity>& entities, Scene* scene)
	//{
	//	ImGui::Begin("Hierarchy");
	//	for (auto& ent : entities)
	//	{
	//		if (scene->m_Registry.HasComponent<Tag>(ent))
	//		{
	//			XTEngine2d::Tag tag = scene->m_Registry.GetComponent<XTEngine2d::Tag>(ent);

	//			std::string label = tag.name + "##" + std::to_string(ent);

	//			if (ImGui::Selectable(label.c_str(),
	//				m_SelectedEntity == ent))
	//			{
	//				m_SelectedEntity = ent;

	//			}
	//		}
	//	}
	//	ImGui::End();
	//}
	//void Editor::DrawInspector(std::vector<Entity>& entities, Scene* scene)
	//{
	//	ImGui::Begin("Inspector");
	//	if (m_SelectedEntity && scene->m_Registry.ValidateEntity(m_SelectedEntity))
	//	{
	//		if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
	//		{
	//			auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);
	//			ImGui::DragFloat2("Position", &transform.position.x);
	//			ImGui::DragFloat2("Size", &transform.size.x);
	//			ImGui::DragFloat3("Rotation", &transform.rotation.x);
	//		}


	//		if (ImGui::Button("Add Component"))
	//		{
	//			ImGui::OpenPopup("AddComponentPopup");
	//		}
	//		if (ImGui::BeginPopup("AddComponentPopup"))
	//		{
	//			if (ImGui::MenuItem("Transform"))
	//			{
	//				if (!scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
	//				{
	//					scene->m_Registry.AddComponent(m_SelectedEntity, Transform{});
	//				}
	//			}
	//			if (ImGui::MenuItem("Camera"))
	//			{
	//				if (!scene->m_Registry.HasComponent<Camera>(m_SelectedEntity))
	//				{
	//					Camera camera;
	//					scene->m_Registry.AddComponent(m_SelectedEntity, camera);
	//				}
	//			}

	//			if (ImGui::MenuItem("Camera Controller"))
	//			{
	//				if (!scene->m_Registry.HasComponent<CameraController>(m_SelectedEntity))
	//				{
	//					CameraController cameracontroller;
	//					scene->m_Registry.AddComponent(m_SelectedEntity, cameracontroller);
	//				}
	//			}

	//			if (ImGui::MenuItem("Sprite"))
	//			{
	//				if (!scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity))
	//				{
	//					Sprite sprite;
	//					auto selection = pfd::open_file(
	//						"Select a texture",
	//						".",
	//						{ "Image Files", "*.png *.jpg *.jpeg" }
	//					).result();

	//					if (!selection.empty())
	//					{
	//						std::string path = selection[0];

	//						sprite.texture = XTEngine2d::AssetManager::LoadTexture(path);
	//						XTEngine2d::SpriteSheet sheet3(sprite.texture.get(), sprite.texture->GetWidth(), sprite.texture->GetHeight());
	//						sprite.uv = sheet3.GetUV(1, 1);
	//						scene->m_Registry.AddComponent(m_SelectedEntity, sprite);
	//					}
	//				}
	//			}
	//			ImGui::EndPopup();
	//		}


	//		if (ImGui::Button("Remove Component"))
	//		{
	//			ImGui::OpenPopup("RemoveComponentPopup");
	//		}
	//		if (ImGui::BeginPopup("RemoveComponentPopup"))
	//		{
	//			if (ImGui::MenuItem("Transform"))
	//			{
	//				if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
	//				{
	//					scene->m_Registry.RemoveComponent<Transform>(m_SelectedEntity);
	//				}
	//			}

	//			if (ImGui::MenuItem("Sprite"))
	//			{
	//				if (scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity))
	//				{
	//					scene->m_Registry.RemoveComponent<Sprite>(m_SelectedEntity);
	//				}
	//			}
	//			ImGui::EndPopup();
	//		}
	//		if (scene->m_Registry.HasComponent<Tag>(m_SelectedEntity))
	//		{
	//			auto& tag = scene->m_Registry.GetComponent<Tag>(m_SelectedEntity);

	//			static char buffer[256];

	//			strcpy_s(buffer, tag.name.c_str());

	//			if (ImGui::InputText(
	//				"##Tag",
	//				buffer,
	//				256))
	//			{
	//				tag.name = buffer;
	//			}
	//		}
	//		if (scene->m_Registry.HasComponent<Camera>(m_SelectedEntity))
	//		{
	//			auto& primecam = scene->m_Registry.GetComponent<Camera>(m_SelectedEntity);

	//			if (ImGui::Checkbox("Primary Camera", &primecam.primary));

	//			ImGui::DragFloat("Zoom", &primecam.zoom);
	//		}
	//	}
	//	ImGui::End();
	//}

	//void Editor::DrawSceneControls(std::vector<Entity>& entities, Scene* scene)
	//{
	//	ImGui::Begin("Scene Controls");
	//	if (ImGui::Button("Save Scene", ImVec2(0, 0)))
	//	{
	//		XTEngine2d::SceneSerializer serializer;

	//		serializer.Serialize("C:\\dev\\XTEngine2d\\Sandbox\\" + sceneName + ".json", sceneName, scene);
	//	}
	//	if (ImGui::Button("Load Scene", ImVec2(0, 0)))
	//	{
	//		XTEngine2d::SceneSerializer serializer;

	//		serializer.Deserialize("C:\\dev\\XTEngine2d\\Sandbox\\" + sceneName + ".json", sceneName, scene);
	//		if (!scene->HasPrimaryCamera())
	//			CreateDefaultCamera(scene);

	//	}
	//	ImGui::End();
	//}
	//void Editor::DrawEntityManager(std::vector<Entity>& entities, Scene* scene)
	//{
	//	ImGui::Begin("Entity Manager");
	//	if (ImGui::Button("Delete Entity"))
	//	{
	//		scene->m_Registry.DestroyEntity(m_SelectedEntity);
	//		m_SelectedEntity = 0;
	//	}

	//	if (ImGui::Button("Create Entity"))
	//	{
	//		m_SelectedEntity = 0;
	//		Entity entity = scene->m_Registry.CreateEntity();
	//		Tag tag;
	//		tag.name = "Entity";

	//		if (!scene->m_Registry.HasComponent<Tag>(entity))
	//			scene->m_Registry.AddComponent<Tag>(entity, tag);

	//		m_SelectedEntity = entity;
	//	}
	//	ImGui::End();
	//}
	//void Editor::RenderViewport(Scene* scene, Renderer& renderer, Window* window)
	//{
	//	//ImGui::Begin("Viewport");

	//	//m_ViewPortPos = ImGui::GetCursorScreenPos();
	//	//m_ViewPortSize = ImGui::GetContentRegionAvail();


	//	//ImVec2 size = ImGui::GetContentRegionAvail();

	//	//ImGui::Image(
	//	//	(ImTextureID)(uintptr_t)
	//	//	m_Framebuffer.GetColorAttachment(),
	//	//	size,
	//	//	ImVec2(0, 1),
	//	//	ImVec2(1, 0));

	//	//HandleZoom(scene);

	//	//ImGui::End();

	//}
	//void Editor::HandlePicking(Scene* scene, XTEngine2d::Renderer& renderer)
	//{
		//auto entities = scene->m_Registry.View<Transform, Sprite>();

		//auto cameras = scene->m_Registry.View<CameraController, Camera>();

		//Entity cameraEntity = 0;
		//for (auto& camera : cameras)
		//{
		//	auto& primaryCamera = scene->m_Registry.GetComponent<Camera>(camera);

		//	if (primaryCamera.primary)
		//		cameraEntity = camera;
		//}

		//if (!cameraEntity)
		//	return;
		//auto& camEnt = scene->m_Registry.GetComponent<Camera>(cameraEntity);

		//auto& cameraTransform = scene->m_Registry.GetComponent<Transform>(cameraEntity);

		//ImVec2 mousePos = ImGui::GetMousePos();

		//ImVec2 localMouse =
		//{
		//	mousePos.x - m_ViewPortPos.x,
		//	mousePos.y - m_ViewPortPos.y
		//};

		//// Check mouse inside viewport
		//if (localMouse.x < 0 || localMouse.y < 0 ||
		//	localMouse.x > m_ViewPortSize.x ||
		//	localMouse.y > m_ViewPortSize.y)
		//{
		//	return;
		//}

		//float x = localMouse.x / m_ViewPortSize.x;
		//float y = localMouse.y / m_ViewPortSize.y;


		//float visibleWidth =
		//	m_Framebuffer.GetWidth() * camEnt.zoom;

		//float visibleHeight =
		//	m_Framebuffer.GetHeight() * camEnt.zoom;

		//float worldX = cameraTransform.position.x + x * visibleWidth;
		//float worldY = cameraTransform.position.y + y * visibleHeight;

		//glm::vec2 xHandle;
		//float distX;
		//glm::vec2 yHandle;
		//float distY;



		//if (m_SelectedEntity)
		//{
		//	if (!scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
		//		return;

		//	auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


		//	glm::vec2 xStart = center;

		//	glm::vec2 xEnd =
		//	{
		//		center.x + gizmoSize,
		//		center.y
		//	};

		//	glm::vec2 yStart = center;

		//	glm::vec2 yEnd =
		//	{
		//		center.x,
		//		center.y + gizmoSize
		//	};
		//	glm::vec2 mouse =
		//	{
		//		worldX,
		//		worldY
		//	};

		//	float distX = DistanceToLineSegment(mouse, xStart, xEnd);

		//	float distY = DistanceToLineSegment(mouse, yStart, yEnd);

		//	float thickness = 8.0f;

		//	if (distX < thickness)
		//	{
		//		m_ActiveAxis = GizmoAxis::X;
		//		m_HoveredAxis = GizmoAxis::X;
		//	}
		//	else if (distY < thickness)
		//	{
		//		m_ActiveAxis = GizmoAxis::Y;
		//		m_HoveredAxis = GizmoAxis::Y;
		//	}
		//	else
		//		m_GizmoHovered = false;

		//}

		//if (ImGui::IsMouseClicked(0))
		//{
		//	for (auto entity : entities)
		//	{
		//		if (!scene->m_Registry.HasComponent<Transform>(entity))
		//			continue;

		//		if (!scene->m_Registry.HasComponent<Sprite>(entity))
		//			continue;

		//		auto& transform =
		//			scene->m_Registry.GetComponent<Transform>(entity);

		//		float left = transform.position.x;
		//		float right = transform.position.x + transform.size.x;
		//		float top = transform.position.y;
		//		float bottom = transform.position.y + transform.size.y;

		//		if (worldX >= left && worldX <= right &&
		//			worldY >= top && worldY <= bottom)
		//		{
		//			m_SelectedEntity = entity;

		//			m_Dragging = true;

		//			m_DragOffset =
		//			{
		//				worldX - transform.position.x,
		//				worldY - transform.position.y
		//			};
		//			break;
		//		}

		//	}
		//}

		/////The camera is randomly getting teleported and as the camera moves mouse click becomes awkward 


		//if (m_Dragging && ImGui::IsMouseDown(0))
		//{
		//	if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
		//	{
		//		auto& transform =
		//			scene->m_Registry.GetComponent<Transform>(
		//				m_SelectedEntity);
		//		float targetX =
		//			worldX - m_DragOffset.x;

		//		float targetY =
		//			worldY - m_DragOffset.y;




		//		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		//		{
		//			transform.position.x =
		//				round(targetX / m_GridSize)
		//				* m_GridSize;

		//			transform.position.y =
		//				round(targetY / m_GridSize)
		//				* m_GridSize;
		//		}
		//		else if (m_ActiveAxis == GizmoAxis::X)
		//		{
		//			transform.position.x = targetX;
		//			m_GizmoDragging = true;
		//		}
		//		else if (m_ActiveAxis == GizmoAxis::Y)
		//		{
		//			transform.position.y = targetY;
		//			m_GizmoDragging = true;
		//		}
		//		else
		//		{
		//			transform.position.x = targetX;
		//			transform.position.y = targetY;
		//			m_GizmoDragging = false;

		//		}
		//	}
		//}

		//if (ImGui::IsMouseReleased(0))
		//{
		//	m_Dragging = false;
		//	m_ActiveAxis = GizmoAxis::None;
		//}
	//}
	//void Editor::HandleZoom(Scene* scene)
	//{
		//if (scene->m_PrimaryCamera)
		//{
		//	auto& primaryCamera = scene->m_Registry.GetComponent<Camera>(scene->m_PrimaryCamera);
		//	if (scene->m_Registry.HasComponent<Transform>(scene->m_PrimaryCamera))
		//	{
		//		auto& cameraTransform = scene->m_Registry.GetComponent<Transform>(scene->m_PrimaryCamera);

		//		if (primaryCamera.primary)
		//		{
		//			if (ImGui::IsWindowHovered())
		//			{
		//				float scroll = ImGui::GetIO().MouseWheel;
		//				primaryCamera.zoom -= scroll * 0.1f;
		//				primaryCamera.zoom = glm::clamp(primaryCamera.zoom, 0.1f, 10.0f);
		//				if (ImGui::IsMouseDown(2))
		//				{
		//					ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;

		//					cameraTransform.position.x -= mouseDelta.x * primaryCamera.zoom;
		//					cameraTransform.position.y -= mouseDelta.y * primaryCamera.zoom;
		//				}

		//			}
		//			gizmoSize = 40.0f * primaryCamera.zoom;
		//		}
		//	}
		//}
	//}

	//float Editor::DistanceToLineSegment(glm::vec2 p, glm::vec2 a, glm::vec2 b)
	//{
		//glm::vec2 ab = b - a;

		//float t =
		//	glm::dot(p - a, ab)
		//	/ glm::dot(ab, ab);

		//t = glm::clamp(t, 0.0f, 1.0f);

		//glm::vec2 closest =
		//	a + ab * t;

		//return glm::distance(p, closest);
		//return 0.0f;
	//}
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


	//void Editor::DrawGrid(Scene* scene, Renderer& renderer, glm::mat4 viewProjection)
	//{
		//if (scene->m_PrimaryCamera)
		//{
		//	auto& camEnt = scene->m_Registry.GetComponent<Camera>(scene->m_PrimaryCamera);
		//	if (camEnt.primary)
		//	{
		//		auto& cameraTransform = scene->m_Registry.GetComponent<Transform>(scene->m_PrimaryCamera);
		//		float visibleWidth =
		//			m_Framebuffer.GetWidth() * camEnt.zoom;
		//		float visibleHeight =
		//			m_Framebuffer.GetHeight() * camEnt.zoom;
		//		float startX =
		//			floor(cameraTransform.position.x / m_GridSize) * m_GridSize;
		//		float startY =
		//			floor(cameraTransform.position.y / m_GridSize) * m_GridSize;
		//		for (float x = startX; x < cameraTransform.position.x + visibleWidth; x += m_GridSize)
		//		{
		//			m_LineRenderer.DrawLine({ x, startY }, { x, startY + visibleHeight + m_GridSize }, { 1.f, 1.f, 1.f, 1.0f }, viewProjection);
		//		}
		//		for (float y = startY; y < cameraTransform.position.y + visibleHeight; y += m_GridSize)
		//		{
		//			m_LineRenderer.DrawLine({ startX, y }, { startX + visibleWidth + m_GridSize, y }, { 1.f, 1.f, 1.f, 1.0f }, viewProjection);
		//		}
		//	}
		//}
	//}
	//void Editor::DrawGizmo(Scene* scene, Renderer& renderer, glm::mat4 viewProjection)
	//{
		//if (m_SelectedEntity)
		//{
		//	if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
		//	{
		//		auto& transform =
		//			scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);

		//		center = { transform.position.x + transform.size.x * 0.5f,transform.position.y + transform.size.y * 0.5f };


		//		glm::vec4 ColorX = { 1,0,0,1 };
		//		glm::vec4 ColorY = { 0,1,0,1 };

		//		if (m_ActiveAxis == GizmoAxis::X)
		//			ColorX = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(1, 0, 0, 1);
		//		else if (m_ActiveAxis == GizmoAxis::Y)
		//			ColorY = m_GizmoDragging ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 1, 0, 1);

		//		if (m_HoveredAxis != GizmoAxis::None)
		//		{
		//			glLineWidth(8.0f);
		//		}
		//		else
		//			glLineWidth(2.0f);

		//		m_LineRenderer.DrawLine(center, { center.x + gizmoSize, center.y },
		//			ColorX,
		//			viewProjection);
		//		m_LineRenderer.DrawLine(
		//			center,
		//			{ center.x, center.y + gizmoSize },
		//			ColorY,
		//			viewProjection);
		//		glLineWidth(1.0f);
		//	}
		//}
	//}
	//void Editor::DrawOutline(Scene* scene, Renderer& renderer, glm::mat4 viewProjection)
	//{
		//if (m_SelectedEntity)
		//{
		//	if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity) && scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity))
		//	{
		//		auto& transform =
		//			scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);


		//		Transform outlineTransform;
		//		outlineTransform.position = transform.position - glm::vec2(2, 2);
		//		outlineTransform.size = transform.size + glm::vec2(4, 4);
		//		outlineTransform.rotation = transform.rotation;
		//		glLineWidth(3.0f);
		//		m_LineRenderer.DrawLine(outlineTransform.position, { outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y }, { 1,1,1,1 }, viewProjection);
		//		m_LineRenderer.DrawLine({ outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y }, { outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
		//		m_LineRenderer.DrawLine(outlineTransform.position, { outlineTransform.position.x, outlineTransform.position.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
		//		m_LineRenderer.DrawLine({ outlineTransform.position.x, outlineTransform.position.y + outlineTransform.size.y }, { outlineTransform.position.x + outlineTransform.size.x, outlineTransform.position.y + outlineTransform.size.y }, { 1,1,1,1 }, viewProjection);
		//		glLineWidth(1.0f);

		//	}

		//}

	//}
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