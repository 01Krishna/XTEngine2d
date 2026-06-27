#include "ComponentSerializer.h"
#include "AssetManager.h"
#include <string>

json ComponentSerializer::SerializeTransform(const XTEngine2d::Transform& transform)
{
	json entityJson;

	entityJson["x"] = transform.localPosition.x;
	entityJson["y"] = transform.localPosition.y;

	entityJson["width"] = transform.size.x;
	entityJson["height"] = transform.size.y;

	entityJson["rotation"] = transform.localRotation;

	return entityJson;
}

XTEngine2d::Transform ComponentSerializer::DeserializeTransform(const json& transformJson)
{
	XTEngine2d::Transform transform;

	transform.localPosition.x = transformJson["x"];
	transform.localPosition.y = transformJson["y"];
	transform.size.x = transformJson["width"];
	transform.size.y = transformJson["height"];
	transform.localRotation = transformJson["rotation"];

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

	entityJson["SpriteRow"] = sheet.m_SelectedRow;
	entityJson["SpriteColumn"] = sheet.m_SelectedColumn;

	return entityJson;
}

XTEngine2d::SpriteSheet ComponentSerializer::DeserializeSpriteSheet(const json& sheetJson)
{
	XTEngine2d::SpriteSheet sheet;

	 sheet.m_SpriteWidth = sheetJson["SpriteWidth"];
	 sheet.m_SpriteHeight = sheetJson["SpriteHeight"];
	 sheet.m_SelectedRow = sheetJson["SpriteRow"];
	 sheet.m_SelectedColumn = sheetJson["SpriteColumn"];

	return sheet;
}

json ComponentSerializer::SerializePlayerController(const XTEngine2d::PlayerController& playerController)
{
	json entityJson;

	entityJson["Player"] = playerController.player;

	entityJson["VelocityX"] = playerController.vel.velocity.x;
	entityJson["VelocityY"] = playerController.vel.velocity.y;

	return entityJson;
}

XTEngine2d::PlayerController ComponentSerializer::DeSerializePlayerController(const json& playercontrollerJson)
{
	XTEngine2d::PlayerController player;

	player.player = playercontrollerJson["Player"];

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

json ComponentSerializer::SerializeAnimation(const XTEngine2d::Animation& animation)
{	
	json entityJson;

	entityJson["StartFrame"] = animation.m_StartFrame;
	entityJson["EndFrame"] = animation.m_EndFrame;
	entityJson["Fps"] = animation.m_Fps;
	entityJson["Looping"] = animation.m_Looping;
	entityJson["Playing"] = animation.m_Playing;

	return entityJson;
}

XTEngine2d::Animation ComponentSerializer::DeserializeAnimation(const json& animationJson)
{
	XTEngine2d::Animation animation;

	animation.m_StartFrame = animationJson["StartFrame"];
	animation.m_EndFrame = animationJson["EndFrame"];
	animation.m_Fps = animationJson["Fps"];
	animation.m_Looping = animationJson["Looping"];
	animation.m_Playing = animationJson["Playing"];

	return animation;
}

json ComponentSerializer::SerializeTileMap(const XTEngine2d::TileMap& tilemap)
{
	json entityJson;
	entityJson["Height"] = tilemap.height;
	entityJson["Width"] = tilemap.width;
	entityJson["GridSize"] = tilemap.gridSize;

	for (int i = 0; i < tilemap.tiles.size(); i++)
	{
		entityJson["Tiles"][i] = tilemap.tiles[i];
	}
	entityJson["Texture"] = tilemap.texture ? tilemap.texture->GetPath() : "";

	return entityJson;
}

XTEngine2d::TileMap ComponentSerializer::DeserializeTileMap(const json& tilemapJson)
{
	XTEngine2d::TileMap tilemap;

	tilemap.height = tilemapJson["Height"];
	tilemap.width = tilemapJson["Width"];
	tilemap.gridSize = tilemapJson["GridSize"];

	tilemap.tiles.resize(tilemapJson["Tiles"].size());

	for (int i = 0; i < tilemapJson["Tiles"].size(); i++)
	{
		tilemap.tiles[i] = tilemapJson["Tiles"][i];
	}
	std::string texturePath = tilemapJson.value("Texture", std::string());
	tilemap.texture = XTEngine2d::AssetManager::LoadTexture(texturePath);
	return tilemap;
}

json ComponentSerializer::SerializeAnimationStateMachine(const XTEngine2d::AnimationStateMachine& State)
{
	json entityJson;
	
	entityJson["Name"] = State.m_States.begin()->first;

	entityJson["Looping"] = State.m_States.begin()->second.looping;
	entityJson["Value"] = State.m_States.begin()->second.value;
	entityJson["Frame"] = State.m_States.begin()->second.startingFrame;

	return entityJson;
}

XTEngine2d::AnimationStateMachine ComponentSerializer::DeserializeAnimationStateMachine(const json& stateJson)
{
	XTEngine2d::AnimationStateMachine StateMachine;

	XTEngine2d::AnimationState temp;


	temp.looping = stateJson["Looping"];
	temp.value = stateJson["Value"];
	temp.startingFrame = stateJson["Frame"];

	StateMachine.m_States[stateJson["Name"]] = temp;
	
	return StateMachine;
}
