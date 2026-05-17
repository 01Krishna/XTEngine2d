#include "InspectorPanel.h"

InspectorPanel::InspectorPanel()
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::OnImGuiRender(Scene* scene, Entity &m_SelectedEntity, std::string SelectedAsset)
{
	ImGui::Begin("Inspector");
	if (m_SelectedEntity && scene->m_Registry.ValidateEntity(m_SelectedEntity))
	{
		if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
		{
			auto& transform = scene->m_Registry.GetComponent<Transform>(m_SelectedEntity);
			ImGui::Text("Transform");
			ImGui::DragFloat2("Position", &transform.position.x);
			ImGui::DragFloat2("Size", &transform.size.x);
			ImGui::DragFloat3("Rotation", &transform.rotation.x);
		}

		if (scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity))
		{
			auto& sprite = scene->m_Registry.GetComponent<Sprite>(m_SelectedEntity);
			if(scene->m_Registry.HasComponent<SpriteSheet>(m_SelectedEntity))
				auto& sheet = scene->m_Registry.GetComponent<SpriteSheet>(m_SelectedEntity);
			
			ImGui::Text("Sprite");

		
			ImGui::DragInt("Row", &row);
			ImGui::DragInt("Column", &column);
		}


		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}
		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			if (ImGui::MenuItem("Transform"))
			{
				if (!scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
				{
					scene->m_Registry.AddComponent(m_SelectedEntity, Transform{});
				}
			}
			if (ImGui::MenuItem("Camera"))
			{
				if (!scene->m_Registry.HasComponent<Camera>(m_SelectedEntity))
				{
					Camera camera;
					scene->m_Registry.AddComponent(m_SelectedEntity, camera);
				}
			}

			if (ImGui::MenuItem("Camera Controller"))
			{
				if (!scene->m_Registry.HasComponent<CameraController>(m_SelectedEntity))
				{
					CameraController cameracontroller;
					scene->m_Registry.AddComponent(m_SelectedEntity, cameracontroller);
				}
			}

			if (ImGui::MenuItem("Sprite"))
			{
				if (!scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity) && !scene->m_Registry.HasComponent<SpriteSheet>(m_SelectedEntity))
				{
					Sprite sprite;
					if (!SelectedAsset.empty())
					{
						std::string path = SelectedAsset;

						XT_CORE_INFO("Selected Asset Path: {0}", path);

						sprite.texture = XTEngine2d::AssetManager::LoadTexture(path);
						XTEngine2d::SpriteSheet sheet(sprite.texture.get(), sprite.texture->GetWidth(), sprite.texture->GetHeight());
						sprite.uv = sheet.GetUV(1, 1);
						scene->m_Registry.AddComponent(m_SelectedEntity, sprite);
						scene->m_Registry.AddComponent(m_SelectedEntity, sheet);

					}
				}
			}

			if (ImGui::MenuItem("Player Controller"))
			{
				if (!scene->m_Registry.HasComponent<PlayerController>(m_SelectedEntity))
				{
					PlayerController playercontroller;
					scene->m_Registry.AddComponent(m_SelectedEntity, playercontroller);
				}
			}

			ImGui::EndPopup();
		}


		if (ImGui::Button("Remove Component"))
		{
			ImGui::OpenPopup("RemoveComponentPopup");
		}
		if (ImGui::BeginPopup("RemoveComponentPopup"))
		{
			if (ImGui::MenuItem("Transform"))
			{
				if (scene->m_Registry.HasComponent<Transform>(m_SelectedEntity))
				{
					scene->m_Registry.RemoveComponent<Transform>(m_SelectedEntity);
				}
			}

			if (ImGui::MenuItem("Camera"))
			{
				if (scene->m_Registry.HasComponent<Camera>(m_SelectedEntity))
				{
					scene->m_Registry.RemoveComponent<Camera>(m_SelectedEntity);
				}
			}

			if (ImGui::MenuItem("Camera Controller"))
			{
				if (scene->m_Registry.HasComponent<CameraController>(m_SelectedEntity))
				{
					scene->m_Registry.RemoveComponent <CameraController>(m_SelectedEntity);
				}
			}

			if (ImGui::MenuItem("Sprite"))
			{
				if (scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity))
				{
					scene->m_Registry.RemoveComponent<Sprite>(m_SelectedEntity);
				}
			}
			if (ImGui::MenuItem("Player Controller"))
			{
				if (scene->m_Registry.HasComponent<PlayerController>(m_SelectedEntity))
				{
					scene->m_Registry.RemoveComponent<PlayerController>(m_SelectedEntity);
				}
			}
			ImGui::EndPopup();
		}
		if (scene->m_Registry.HasComponent<Tag>(m_SelectedEntity))
		{
			auto& tag = scene->m_Registry.GetComponent<Tag>(m_SelectedEntity);

			static char buffer[256];

			strcpy_s(buffer, tag.name.c_str());
			
			if (ImGui::InputText(
				"##Tag",
				buffer,
				256))
			{
				tag.name = buffer;
			}
		}
		if (scene->m_Registry.HasComponent<Camera>(m_SelectedEntity))
		{
			scene->CheckPrimaryCameraAvailability();
			if (!scene->m_PrimaryCamera)
			{
				auto& primecam = scene->m_Registry.GetComponent<Camera>(m_SelectedEntity);
				if (ImGui::Checkbox("Primary Camera", &primecam.primary));
				ImGui::DragFloat("Zoom", &primecam.zoom);
			}
		}
	}
	ImGui::End();
}
