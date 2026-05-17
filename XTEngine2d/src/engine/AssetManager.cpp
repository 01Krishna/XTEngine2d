	#include "AssetManager.h"

	namespace XTEngine2d
	{


		std::unordered_map<std::string, std::shared_ptr<XTEngine2d::Texture>> XTEngine2d::AssetManager::s_Textures;

		AssetManager::AssetManager()
		{
		}

		AssetManager::~AssetManager()
		{
		}

		std::shared_ptr<XTEngine2d::Texture>AssetManager::LoadTexture(std::string fileName)
		{
			if (s_Textures.find(fileName) != s_Textures.end())
				return s_Textures[fileName];	

			std::shared_ptr<XTEngine2d::Texture> texture = std::make_shared<XTEngine2d::Texture>();
			texture->LoadTexture(fileName.c_str());
			s_Textures[fileName] = texture;

			return texture;
		}
	}