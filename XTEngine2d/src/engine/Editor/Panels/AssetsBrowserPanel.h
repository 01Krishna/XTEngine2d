#pragma once

#include <filesystem>
#include "../../vendor/glm/glm.hpp"
#include "../../FrameBuffer.h"
#include "../../AssetManager.h"
#include "portable-file-dialogs.h"
#include "../../SceneSerializer.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../Utility.h"
#include "../../Scene.h"


class AssetsBrowserPanel
{
public:
	AssetsBrowserPanel();	
	~AssetsBrowserPanel();

	std::string assetDirectory = "Assets";
	std::filesystem::path assetPath = assetDirectory;
	std::filesystem::path selectedAssetFullPath;
	std::string selectedAsset;

	std::shared_ptr<XTEngine2d::Texture> m_FolderIcon;
	std::shared_ptr<XTEngine2d::Texture> m_FileIcon;
	std::shared_ptr<XTEngine2d::Texture> m_SceneIcon;
	std::shared_ptr<XTEngine2d::Texture> m_BackIcon;


public:
	void OnImGuiRender();

	std::string GetSelectedAsset()
	{
		return selectedAssetFullPath.string();
	}

	void LoadIcons()
	{
		m_FolderIcon = XTEngine2d::AssetManager::LoadTexture("Assets\\editor\\folder.png");
		m_FileIcon = XTEngine2d::AssetManager::LoadTexture("Assets\\editor\\file.png");
		m_SceneIcon = XTEngine2d::AssetManager::LoadTexture("Assets\\editor\\scene.png");
		m_BackIcon = XTEngine2d::AssetManager::LoadTexture("Assets\\editor\\back.png");
	}
};

