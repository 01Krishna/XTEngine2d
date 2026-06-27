#include "InspectorPanel.h"

InspectorPanel::InspectorPanel()
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Init(CommandHistory* cmdHistory)
{
	CMDHISTORY = cmdHistory;
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
			if (scene->m_Registry.HasComponent<SpriteSheet>(m_SelectedEntity))
			{

				auto& sheet = scene->m_Registry.GetComponent<SpriteSheet>(m_SelectedEntity);

				ImGui::Text("Sprite");
				ImGui::DragInt("Sprite Width", &sheet.m_SpriteWidth);
				ImGui::DragInt("Sprite Height", &sheet.m_SpriteHeight);

				sheet.SetSpriteDimension(sheet.m_SpriteWidth, sheet.m_SpriteHeight);

				bool hasAnimation = scene->m_Registry.HasComponent<Animation>(m_SelectedEntity);

				if (!hasAnimation)
				{
					bool changed1 = true;
					bool changed2 = true;

					changed1 |= ImGui::DragInt("Row", &sheet.m_SelectedRow);

					changed2 |= ImGui::DragInt("Column", &sheet.m_SelectedColumn);

					if (changed1 || changed2)
					{
						sheet.m_SelectedColumn = glm::clamp(sheet.m_SelectedColumn, 0, sheet.m_Columns - 1);
						sheet.m_SelectedRow = glm::clamp(sheet.m_SelectedRow, 0, sheet.m_Rows - 1);
						sprite.uv = sheet.GetUV(sheet.m_SelectedColumn, sheet.m_SelectedRow);
					}
				}
			}
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
				Transform t;
				auto cmd = std::make_shared<XTEngine2d::AddComponentCommand<Transform>>(scene, m_SelectedEntity,t);
				CMDHISTORY->ExecuteCommand(cmd);
			}
			if (ImGui::MenuItem("Camera"))
			{
				Camera camera;
				auto cmd = std::make_shared<AddComponentCommand<Camera>>(scene, m_SelectedEntity, camera);
				CMDHISTORY->ExecuteCommand(cmd);
			}

			if (ImGui::MenuItem("Camera Controller"))
			{

				CameraController cameracontroller;
				auto cmd = std::make_shared<AddComponentCommand<CameraController>>(scene, m_SelectedEntity, cameracontroller);
				CMDHISTORY->ExecuteCommand(cmd);
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

						auto cmd = std::make_shared<AddComponentCommand<Sprite>>(scene, m_SelectedEntity, sprite);
						CMDHISTORY->ExecuteCommand(cmd);

						auto cmd2 = std::make_shared<AddComponentCommand<SpriteSheet>>(scene, m_SelectedEntity, sheet);
						CMDHISTORY->ExecuteCommand(cmd2);
					}
				}
			}

			if (ImGui::MenuItem("Player Controller"))
			{
				PlayerController playercontroller;
				auto cmd = std::make_shared<AddComponentCommand<PlayerController>>(scene, m_SelectedEntity, playercontroller);
				CMDHISTORY->ExecuteCommand(cmd);

			}

			if (ImGui::MenuItem("Animation"))
			{
				Animation animation;
				auto cmd = std::make_shared<AddComponentCommand<Animation>>(scene, m_SelectedEntity, animation);
				CMDHISTORY->ExecuteCommand(cmd);
			}

			if (ImGui::MenuItem("Animation States"))
			{
				AnimationStateMachine StateMachine;
				auto cmd = std::make_shared<AddComponentCommand<AnimationStateMachine>>(scene, m_SelectedEntity, StateMachine);
				CMDHISTORY->ExecuteCommand(cmd);
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

			if (ImGui::MenuItem("TileMap"))
			{
				TileMap tilemap;
				if (!SelectedAsset.empty())
				{
					std::string path = SelectedAsset;
					tilemap.texture = AssetManager::GetTexture(path);
				}

				tilemap.tiles.resize(tilemap.width * tilemap.height, -1);
				scene->m_Registry.AddComponent<TileMap>(m_SelectedEntity, tilemap);
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
				auto cmd = std::make_shared<XTEngine2d::RemoveComponentCommand<Transform>>(scene, m_SelectedEntity);

				CMDHISTORY->ExecuteCommand(cmd);
			}

			if (ImGui::MenuItem("Camera"))
			{
				auto cmd = std::make_shared<XTEngine2d::RemoveComponentCommand<Camera>>(scene, m_SelectedEntity);
				CMDHISTORY->ExecuteCommand(cmd);
			}

			if (ImGui::MenuItem("Camera Controller"))
			{
				auto cmd = std::make_shared<XTEngine2d::RemoveComponentCommand<CameraController>>(scene, m_SelectedEntity);
				CMDHISTORY->ExecuteCommand(cmd);
			}

			if (ImGui::MenuItem("Sprite"))
			{
				auto cmd = std::make_shared<XTEngine2d::RemoveComponentCommand<Sprite>>(scene, m_SelectedEntity);
				CMDHISTORY->ExecuteCommand(cmd);

				auto cmd2 = std::make_shared<XTEngine2d::RemoveComponentCommand<SpriteSheet>>(scene, m_SelectedEntity);
				CMDHISTORY->ExecuteCommand(cmd2);

			}

			if (ImGui::MenuItem("Animation"))
			{
				auto cmd = std::make_shared<XTEngine2d::RemoveComponentCommand<Animation>>(scene, m_SelectedEntity);
				CMDHISTORY->ExecuteCommand(cmd);
			}

			if (ImGui::MenuItem("Animation States"))
			{
				auto cmd = std::make_shared<XTEngine2d::RemoveComponentCommand<AnimationStateMachine>>(scene, m_SelectedEntity);
				CMDHISTORY->ExecuteCommand(cmd);
			}

			if (ImGui::MenuItem("Player Controller"))
			{
				auto cmd = std::make_shared<XTEngine2d::RemoveComponentCommand<PlayerController>>(scene, m_SelectedEntity);
				CMDHISTORY->ExecuteCommand(cmd);
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
			
			auto& primecam = scene->m_Registry.GetComponent<Camera>(m_SelectedEntity);
			
			if (!scene->m_PrimaryCamera)
			{
				if (ImGui::Checkbox("Primary Camera", &primecam.primary));
			}
			ImGui::DragFloat("Zoom", &primecam.zoom);
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


		if (scene->m_Registry.HasComponent<Animation>(m_SelectedEntity))
		{
			auto& animation = scene->m_Registry.GetComponent<Animation>(m_SelectedEntity);

			ImGui::Text("Animation");
			ImGui::DragInt("Start", &animation.m_StartFrame);
			ImGui::DragInt("End", &animation.m_EndFrame);
			ImGui::DragInt("Starting Row", &animation.m_StartingRow);
			ImGui::DragInt("Starting Column", &animation.m_StartingColumn);
			ImGui::DragFloat("Fps", &animation.m_Fps);
			ImGui::Checkbox("Row(T)/Column(F)", &animation.m_RowOrColumnAnchor);
			ImGui::Checkbox("Loop", &animation.m_Looping);
			ImGui::Checkbox("Playing", &animation.m_Playing);
		}

		if (scene->m_Registry.HasComponent<AnimationStateMachine>(m_SelectedEntity))
		{
			auto& stateMachine = scene->m_Registry.GetComponent<AnimationStateMachine>(m_SelectedEntity);
			std::unordered_map<std::string, AnimationState>& stateMap = stateMachine.GetAllStates();

			ImGui::Separator();
			ImGui::Text("Animation State Machine");
			ImGui::Text("Total States: %d", stateMachine.m_TotalStates);
			ImGui::Text("Current State: %s", stateMachine.m_CurrentState.first.c_str());

			static char buffer[256];

			strcpy_s(buffer, temp_state_name.c_str());

			if (ImGui::InputText(
				"##StateTag",
				buffer,
				256))
			{
				temp_state_name = buffer;
			}

			if(ImGui::Button("Add State"))
			{
				stateMachine.AddState(temp_state_name, 0.0f, false, 0);
			}

			for (auto& state : stateMap)
			{
				ImGui::PushID(state.first.c_str());
				ImGui::Text(state.first.c_str());
				ImGui::Checkbox("Looping", &state.second.looping);
				ImGui::DragInt("Frame", &state.second.startingFrame);
				ImGui::PopID();
			}
		}

		if (scene->m_Registry.HasComponent<TileMap>(m_SelectedEntity))
		{
			auto& tilemap = scene->m_Registry.GetComponent<TileMap>(m_SelectedEntity);

			ImGui::Text("TileMap");
			bool changed = false;
			bool changed2 = false;
			bool changed3 = false;
			changed2 |= ImGui::DragInt("Width", &tilemap.width);
			changed3 |= ImGui::DragInt("Height", &tilemap.height);

			changed |= ImGui::DragInt("GridSize", &tilemap.gridSize);

			if(changed || changed2 || changed3)
				tilemap.tiles.resize(tilemap.width * tilemap.height, -1);

			ImGui::Checkbox("Animated", &tilemap.isAnimated);
			ImGui::DragInt("Frame Count", &tilemap.m_FrameCount);
			ImGui::DragFloat("Animation Speed", &tilemap.animationSpeed);
			
			ImGui::Begin("Tile Palette");

			if (tilemap.texture)
			{
				ImVec2 imageSize(256, 256);

				ImGui::Image((ImTextureID)(uintptr_t)tilemap.texture->GetId(),imageSize);

				ImVec2 imageMin = ImGui::GetItemRectMin();
				ImVec2 imageMax = ImGui::GetItemRectMax();
				
				SpriteSheet sheet(tilemap.texture.get(), tilemap.gridSize, tilemap.gridSize);
		
				ImDrawList* drawList =	ImGui::GetWindowDrawList();
			
				float cellWidth = imageSize.x / sheet.m_Columns;

				float cellHeight = imageSize.y / sheet.m_Rows;
				
				for (int x = 0; x <= sheet.m_Columns; x++)
				{
					float xpos = imageMin.x + x * cellWidth;

					drawList->AddLine(ImVec2(xpos, imageMin.y),	ImVec2(xpos, imageMax.y),IM_COL32(255, 255, 255, 100));
				}

				for (int y = 0; y <= sheet.m_Rows; y++)
				{
					float ypos = imageMin.y + y * cellHeight;

					drawList->AddLine(ImVec2(imageMin.x, ypos),	ImVec2(imageMax.x, ypos),IM_COL32(255, 255, 255, 100));
				}
				
				if (ImGui::IsItemHovered() &&
					ImGui::IsMouseClicked(0))
				{
					ImVec2 mouse = ImGui::GetMousePos();

					int column = (mouse.x - imageMin.x) / cellWidth;

					int row = (mouse.y - imageMin.y) / cellHeight;

					selectedTile = row * sheet.m_Columns + column;

				}
				int selectedColumn;
				int selectedRow;
					
				if(sheet.m_Columns != 0)
				{
					selectedColumn = selectedTile % sheet.m_Columns;
					selectedRow = selectedTile / sheet.m_Columns;
				}


				ImVec2 rectMin(imageMin.x + selectedColumn * cellWidth,imageMin.y + selectedRow * cellHeight);

				ImVec2 rectMax(rectMin.x + cellWidth,rectMin.y + cellHeight);

				drawList->AddRect(rectMin,rectMax,IM_COL32(255, 255, 0, 255),0.0f,0,3.0F);
			}

			ImGui::End();
			
		}
	}
	ImGui::End();
}
