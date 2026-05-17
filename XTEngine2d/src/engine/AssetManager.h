#pragma once
#include "Core.h"
#include <unordered_map>
#include <memory>
#include "Textures.h"
#include <string>

namespace XTEngine2d
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

	public:
		static std::shared_ptr<XTEngine2d::Texture> LoadTexture(std::string	fileName);

		static std::shared_ptr<XTEngine2d::Texture> GetTexture(std::string fileName)
		{
			if (s_Textures.find(fileName) != s_Textures.end())
				return s_Textures[fileName];
			else
				return nullptr;
		}
	private:
		static std::unordered_map<std::string, std::shared_ptr<XTEngine2d::Texture>>s_Textures;

		//static void LoadShader(const char* vertexPath, const char* fragmentPath);
		//static void LoadMesh(const char* fileName);
		//static void LoadAudio(const char* fileName);
		//static void LoadFont(const char* fileName);
		//static void LoadAnimation(const char* fileName);
		//static void LoadPrefab(const char* fileName);
		//static void LoadScene(const char* fileName);
		//static void LoadScript(const char* fileName);
		//static void LoadMaterial(const char* fileName);
		//static void LoadParticleSystem(const char* fileName);
		//static void LoadUIElement(const char* fileName);
		//static void LoadAnimationController(const char* fileName);
		//static void LoadAudioClip(const char* fileName);
		//static void LoadSpriteSheet(const char* fileName);
		//static void LoadTileMap(const char* fileName);
		//static void LoadSkybox(const char* fileName);
		//static void LoadTerrain(const char* fileName);
		//static void LoadCubemap(const char* fileName);
		//static void LoadFramebuffer(const char* fileName);
		//static void LoadRenderTexture(const char* fileName);


	};
}