#include "SceneSerializer.h"
#include "Scene.h"
#include "AssetManager.h"
#include "ComponentSerializer.h"
#include "Camera.h"

using json = nlohmann::json;

namespace XTEngine2d
{
	SceneSerializer::SceneSerializer()
	{
	}
	SceneSerializer::~SceneSerializer()
	{
	}
	void SceneSerializer::Serialize(const std::string& filePath, const std::string& IsceneName, Scene* scene)
	{
		data["Scene"] = IsceneName;
		data["Entities"] = json::array();
		auto entities = scene->m_Registry.GetAllEntities();

		for (auto entity : entities)
		{
			json entityJson;
			if(scene->m_Registry.HasComponent<Transform>(entity))
			{
				Transform transform = scene->m_Registry.GetComponent<Transform>(entity);
				entityJson["Transform"] = ComponentSerializer::SerializeTransform(transform);
			}

			if (scene->m_Registry.HasComponent<Sprite>(entity))
			{
				Sprite sprite = scene->m_Registry.GetComponent<Sprite>(entity);
				entityJson["Sprite"] = ComponentSerializer::SerializeSprite(sprite);
				if (scene->m_Registry.HasComponent<SpriteSheet>(entity))
				{
					SpriteSheet sheet = scene->m_Registry.GetComponent<SpriteSheet>(entity);
					entityJson["SpriteSheet"] = ComponentSerializer::SerializeSpriteSheet(sheet);
				}
			}
			if (scene->m_Registry.HasComponent<PlayerController>(entity))
			{
				PlayerController playerCont = scene->m_Registry.GetComponent<PlayerController>(entity);
				entityJson["PlayerController"] = ComponentSerializer::SerializePlayerController(playerCont);
			}
			
			if (scene->m_Registry.HasComponent<XTEngine2d::Camera>(entity))
			{
				Camera camera = scene->m_Registry.GetComponent<XTEngine2d::Camera>(entity);
				entityJson["Camera"] = ComponentSerializer::SerializeCamera(camera);
			}

			if (scene->m_Registry.HasComponent<XTEngine2d::CameraController>(entity))
			{
				CameraController camCont = scene->m_Registry.GetComponent<CameraController>(entity);
				entityJson["CameraController"] = ComponentSerializer::SerializeCameraController(camCont);
			}

			if (scene->m_Registry.HasComponent<XTEngine2d::Tag>(entity))
			{
				Tag tag = scene->m_Registry.GetComponent<Tag>(entity);
				entityJson["Tag"] = ComponentSerializer::SerializeTag(tag);
			}

			data["Entities"].push_back(entityJson);
		}

		std::ofstream f2(filePath);
		f2 << data;

	}
	void SceneSerializer::Deserialize(const std::string& filePath, const std::string& IsceneName, Scene* scene)
	{
		if (!filePath.empty())
		{
			std::ifstream f(filePath);
			f >> data;
		}

		for (auto& entityJson : data["Entities"])
		{
			Entity ent = scene->m_Registry.CreateEntity();


			if (entityJson.contains("PlayerController"))
			{
				PlayerController playerCont = ComponentSerializer::DeSerializePlayerController(entityJson["PlayerController"]);
				scene->m_Registry.AddComponent(ent, playerCont);
			}

			if (entityJson.contains("Camera"))
			{
				Camera camera = ComponentSerializer::DeSerializeCamera(entityJson["Camera"]);
				scene->m_Registry.AddComponent(ent, camera);
			}
			
			if (entityJson.contains("CameraController"))
			{
				CameraController camCont = ComponentSerializer::DeSerializeCameraController(entityJson["CameraController"]);
				scene->m_Registry.AddComponent(ent, camCont);
			}

			if (entityJson.contains("Transform"))
			{
				Transform transform = ComponentSerializer::DeserializeTransform(entityJson["Transform"]);
				scene->m_Registry.AddComponent(ent, transform);
			}

			if (entityJson.contains("Sprite"))
			{
				Sprite sprite = ComponentSerializer::DeserializeSprite(entityJson["Sprite"]);

				SpriteSheet sheet = ComponentSerializer::DeserializeSpriteSheet(entityJson["SpriteSheet"]);

				sheet.SetTexture(sprite.texture.get(), sheet.m_SpriteWidth, sheet.m_SpriteHeight);

				scene->m_Registry.AddComponent(ent, sprite);
				scene->m_Registry.AddComponent(ent, sheet);
			}

			if (entityJson.contains("Tag"))
			{
				Tag tag = ComponentSerializer::DeserializeTag(entityJson["Tag"]);
				scene->m_Registry.AddComponent(ent, tag);
			}
		}
	}
}