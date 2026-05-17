#include "ComponentSerializer.h"
#include "AssetManager.h"
#include <string>

json ComponentSerializer::SerializeTransform(const XTEngine2d::Transform& transform)
{
	json entityJson;

	entityJson["x"] = transform.position.x;
	entityJson["y"] = transform.position.y;

	entityJson["width"] = transform.size.x;
	entityJson["height"] = transform.size.y;

	entityJson["rotationX"] = transform.rotation.x;
	entityJson["rotationY"] = transform.rotation.y;
	entityJson["rotationZ"] = transform.rotation.z;



	return entityJson;
}

XTEngine2d::Transform ComponentSerializer::DeserializeTransform(const json& transformJson)
{
	XTEngine2d::Transform transform;

	transform.position.x = transformJson["x"];
	transform.position.y = transformJson["y"];
	transform.size.x = transformJson["width"];
	transform.size.y = transformJson["height"];
	transform.rotation.x = transformJson["rotationX"];
	transform.rotation.y = transformJson["rotationY"];
	transform.rotation.z = transformJson["rotationZ"];

	return transform;
}

json ComponentSerializer::SerializeSprite(const XTEngine2d::Sprite& sprite)
{
	json entityJson;
	if (sprite.texture)
		entityJson["texture"] = sprite.texture->GetPath();
	else
		entityJson["texture"] = "";

	entityJson["uv"]["u1"] = sprite.uv.x;
	entityJson["uv"]["v1"] = sprite.uv.y;
	entityJson["uv"]["u2"] = sprite.uv.w;
	entityJson["uv"]["v2"] = sprite.uv.z;
	return entityJson;
}

XTEngine2d::Sprite ComponentSerializer::DeserializeSprite(const json& spriteJson)
{
	XTEngine2d::Sprite sprite;

	std::string texturePath = spriteJson.value("texture", std::string());

	sprite.uv.x = spriteJson["uv"]["u1"];
	sprite.uv.y = spriteJson["uv"]["v1"];
	sprite.uv.w = spriteJson["uv"]["u2"];
	sprite.uv.z = spriteJson["uv"]["v2"];

	sprite.texture = XTEngine2d::AssetManager::LoadTexture(texturePath);

	return sprite;
}

json ComponentSerializer::SerializeSpriteSheet(const XTEngine2d::SpriteSheet& sheet)
{
	json entityJson;

	entityJson["SpriteWidth"] = sheet.m_SpriteWidth;
	entityJson["SpriteHeight"] = sheet.m_SpriteHeight;

	return entityJson;
}

XTEngine2d::SpriteSheet ComponentSerializer::DeserializeSpriteSheet(const json& sheetJson)
{
	XTEngine2d::SpriteSheet sheet;

	 sheet.m_SpriteWidth = sheetJson["SpriteWidth"];
	 sheet.m_SpriteHeight = sheetJson["SpriteHeight"];

	return sheet;
}

json ComponentSerializer::SerializePlayerController(const XTEngine2d::PlayerController& playerController)
{
	json entityJson;

	entityJson["VelocityX"] = playerController.vel.velocity.x;
	entityJson["VelocityY"] = playerController.vel.velocity.y;

	return entityJson;
}

XTEngine2d::PlayerController ComponentSerializer::DeSerializePlayerController(const json& playercontrollerJson)
{
	XTEngine2d::PlayerController player;

	player.vel.velocity.x = playercontrollerJson["VelocityX"];
	player.vel.velocity.y = playercontrollerJson["VelocityY"];

	return player;
}

json ComponentSerializer::SerializeCamera(const XTEngine2d::Camera& camera)
{
	json entityJson;

	entityJson["Zoom"] = camera.zoom;
	entityJson["Primary"] = camera.primary;

	return entityJson;
}

XTEngine2d::Camera ComponentSerializer::DeSerializeCamera(const json& cameraJson)
{
	XTEngine2d::Camera camera;

	camera.zoom = cameraJson["Zoom"];
	camera.primary = cameraJson["Primary"];

	return camera;
}

json ComponentSerializer::SerializeCameraController(const XTEngine2d::CameraController& cameraController)
{
	json entityJson;

	entityJson["VelocityX"] = cameraController.vel.velocity.x;
	entityJson["VelocityY"] = cameraController.vel.velocity.y;
	
	return entityJson;
}

XTEngine2d::CameraController ComponentSerializer::DeSerializeCameraController(const json& cameraControllerJson)
{
	XTEngine2d::CameraController cameraController;

	cameraController.vel.velocity.x = cameraControllerJson["VelocityX"];
	cameraController.vel.velocity.y = cameraControllerJson["VelocityY"];

	return cameraController;
}


json ComponentSerializer::SerializeTag(const XTEngine2d::Tag &tag)
{
	json entityJson;
	entityJson = tag.name;
	return entityJson;
}

XTEngine2d::Tag ComponentSerializer::DeserializeTag(const json& tagJson)
{
	XTEngine2d::Tag tag;
	tag.name = tagJson.get<std::string>();
	return tag;
}
