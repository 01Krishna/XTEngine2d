#include "PrefabSerializer.h"

using namespace XTEngine2d;

void PrefabSerializer::SerializeEntity(Entity selectedEntity, XTEngine2d::Scene* scene, std::string path)
{
	
	if (scene->m_Registry.HasComponent<Transform>(selectedEntity))
	{
		Transform transform = scene->m_Registry.GetComponent<Transform>(selectedEntity);
		data["Transform"] = ComponentSerializer::SerializeTransform(transform);
	}

	if (scene->m_Registry.HasComponent<Sprite>(selectedEntity))
	{
		Sprite sprite = scene->m_Registry.GetComponent<Sprite>(selectedEntity);
		data["Sprite"] = ComponentSerializer::SerializeSprite(sprite);
		if (scene->m_Registry.HasComponent<SpriteSheet>(selectedEntity))
		{
			SpriteSheet sheet = scene->m_Registry.GetComponent<SpriteSheet>(selectedEntity);
			data["SpriteSheet"] = ComponentSerializer::SerializeSpriteSheet(sheet);
		}
	}
	if (scene->m_Registry.HasComponent<PlayerController>(selectedEntity))
	{
		PlayerController playerCont = scene->m_Registry.GetComponent<PlayerController>(selectedEntity);
		data["PlayerController"] = ComponentSerializer::SerializePlayerController(playerCont);
	}

	if (scene->m_Registry.HasComponent<XTEngine2d::Camera>(selectedEntity))
	{
		Camera camera = scene->m_Registry.GetComponent<XTEngine2d::Camera>(selectedEntity);
		data["Camera"] = ComponentSerializer::SerializeCamera(camera);
	}

	if (scene->m_Registry.HasComponent<XTEngine2d::CameraController>(selectedEntity))
	{
		CameraController camCont = scene->m_Registry.GetComponent<CameraController>(selectedEntity);
		data["CameraController"] = ComponentSerializer::SerializeCameraController(camCont);
	}

	if (scene->m_Registry.HasComponent<XTEngine2d::Tag>(selectedEntity))
	{
		Tag tag = scene->m_Registry.GetComponent<Tag>(selectedEntity);
		data["Tag"] = ComponentSerializer::SerializeTag(tag);
	}

	std::ofstream f2(path);
	f2 << data;
}

Entity PrefabSerializer::DeserializeEntity(XTEngine2d::Scene* scene, std::string filePath)
{
	if (!filePath.empty())
	{
		std::ifstream f(filePath);
		f >> data;
	}

	Entity ent = scene->m_Registry.CreateEntity();


	if (data.contains("PlayerController"))
	{
		PlayerController playerCont = ComponentSerializer::DeSerializePlayerController(data["PlayerController"]);
		scene->m_Registry.AddComponent(ent, playerCont);
	}

	if (data.contains("Camera"))
	{
		Camera camera = ComponentSerializer::DeSerializeCamera(data["Camera"]);
		scene->m_Registry.AddComponent(ent, camera);
	}

	if (data.contains("CameraController"))
	{
		CameraController camCont = ComponentSerializer::DeSerializeCameraController(data["CameraController"]);
		scene->m_Registry.AddComponent(ent, camCont);
	}

	if (data.contains("Transform"))
	{
		Transform transform = ComponentSerializer::DeserializeTransform(data["Transform"]);
		scene->m_Registry.AddComponent(ent, transform);
	}

	if (data.contains("Sprite"))
	{
		Sprite sprite = ComponentSerializer::DeserializeSprite(data["Sprite"]);

		SpriteSheet sheet = ComponentSerializer::DeserializeSpriteSheet(data["SpriteSheet"]);

		sheet.SetTexture(sprite.texture.get(), sheet.m_SpriteWidth, sheet.m_SpriteHeight);

		scene->m_Registry.AddComponent(ent, sprite);
		scene->m_Registry.AddComponent(ent, sheet);
	}

	if (data.contains("Tag"))
	{
		Tag tag = ComponentSerializer::DeserializeTag(data["Tag"]);
		scene->m_Registry.AddComponent(ent, tag);
	}

	return ent;
}
