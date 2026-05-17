#include "AssetsBrowserPanel.h"

AssetsBrowserPanel::AssetsBrowserPanel()
{

}

AssetsBrowserPanel::~AssetsBrowserPanel()
{

}

void AssetsBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Assets Browser");

	int count = 1;

	float static padding = 16.f;
	float static thumbnailSize = 64.f;

	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;

	int columnCount = (int)(panelWidth / cellSize);

	if (columnCount < 1)
		columnCount = 1;
	
	
	ImGui::Columns(columnCount, 0, false);

	for (const auto& entry : std::filesystem::directory_iterator(assetPath))
	{
		ImGui::PushID(count++); 

		std::shared_ptr<XTEngine2d::Texture> IconTexture;

		if (entry.is_directory())
		{
			IconTexture = m_FolderIcon;
		}
		else
		{
			std::string extension = entry.path().extension().string();
			
			if (extension == ".png" || extension == ".jpg")
			{
				selectedAssetFullPath = assetPath / entry.path().filename();

				if (!XTEngine2d::AssetManager::GetTexture(selectedAssetFullPath.string()))
				{
					XTEngine2d::AssetManager::LoadTexture(selectedAssetFullPath.string());
				}

				IconTexture = XTEngine2d::AssetManager::GetTexture(selectedAssetFullPath.string());
			}
			else if (extension == ".json")
			{
				IconTexture = m_SceneIcon;
			}
			else
			{
				IconTexture = m_FileIcon;
			}
		}
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		ImGui::ImageButton("AssetButton",(ImTextureID)(uintptr_t)IconTexture->GetId(),ImVec2(thumbnailSize, thumbnailSize));

		ImGui::PopStyleColor();

		if (ImGui::BeginDragDropSource())
		{
			std::string fullPath = entry.path().string();

			ImGui::SetDragDropPayload(
				"ASSET_PATH",
				fullPath.c_str(),
				fullPath.size() + 1
			);

			ImGui::Text("%s", entry.path().filename().string().c_str());

			ImGui::EndDragDropSource();
		}	


		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			if (entry.is_directory())
			{
				assetPath /= entry.path().filename();
			}
			else
			{
				selectedAsset = entry.path().filename().string();
			}
		}

		ImGui::TextWrapped(entry.path().filename().string().c_str());

		ImGui::PopID();
		ImGui::NextColumn();
	}
	ImGui::Columns(1);
	ImGui::Separator();
	if (ImGui::Button("Back", ImVec2(0, 0)))
	{
		if (assetPath.has_parent_path())
			assetPath = assetPath.parent_path();
	}
	ImGui::End();
}

