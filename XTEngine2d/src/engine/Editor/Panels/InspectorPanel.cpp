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
			ImGui::DragFloat2("Position", &transform.localPosition.x);
			ImGui::DragFloat2("Size", &transform.size.x);
			ImGui::DragFloat("Rotation", &transform.localRotation);
			ImGui::DragFloat2("Scale", &transform.localScale.x);
		}

		if (scene->m_Registry.HasComponent<Sprite>(m_SelectedEntity))
		{
			auto& sprite = scene->m_Registry.GetComponent<Sprite>(m_SelectedEntity);
			auto& sheet = scene->m_Registry.GetComponent<SpriteSheet>(m_SelectedEntity);


			ImGui::Text("Sprite");
			ImGui::DragInt("Sprite Width", &sheet.m_SpriteWidth);
			ImGui::DragInt("Sprite Height", &sheet.m_SpriteHeight);

			sheet.SetSpriteDimension(sheet.m_SpriteWidth, sheet.m_SpriteHeight);

			ImGui::DragInt("Row", &sheet.m_SelectedRow);
			ImGui::DragInt("Columns", &sheet.m_SelectedColumn);

			sprite.uv = sheet.GetUV(sheet.m_SelectedColumn, sheet.m_SelectedRow);
		}

		if (scene->m_Registry.HasComponent<PlayerController>(m_SelectedEntity))
		{
			auto& player = scene->m_Registry.GetComponent<PlayerController>(m_SelectedEntity);
			ImGui::Checkbox("Player", &player.player);
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

			if (ImGui::MenuItem("Create Child"))
			{
				Entity parent = m_SelectedEntity;

				Entity child = scene->m_Registry.CreateEntity();

				Tag tag;
				tag.name = "Child";

				Transform transform;

				Hierarchy childHierarchy;
				childHierarchy.parent = parent;

				scene->m_Registry.AddComponent(child, tag);
				scene->m_Registry.AddComponent(child, transform);
				scene->m_Registry.AddComponent(child, childHierarchy);

				// Parent needs hierarchy too
				if (!scene->m_Registry.HasComponent<Hierarchy>(parent))
				{
					scene->m_Registry.AddComponent(parent, Hierarchy{});
				}

				scene->m_Registry
					.GetComponent<Hierarchy>(parent)
					.children
					.push_back(child);
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

		if (ImGui::Button("Save Prefab"))
		{
			if (m_SelectedEntity)
			{
				if (scene->m_Registry.HasComponent<Tag>(m_SelectedEntity))
				{
					Tag tag = scene->m_Registry.GetComponent<Tag>(m_SelectedEntity);
					PrefabSerializer serializer;
					serializer.SerializeEntity(m_SelectedEntity, scene, "Assets//prefabs//" + tag.name + ".prefab");
				}
			}
		}
	}
	ImGui::End();
}
