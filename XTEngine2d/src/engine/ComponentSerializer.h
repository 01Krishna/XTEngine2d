#pragma once
#include "nlohmann/json.hpp"
#include "Utility.h"
#include "AssetManager.h"
#include "Camera.h"
#include "PlayerController.h"
#include "CameraController.h"
#include "Sprite.h"
#include "Animation.h"
#include "AnimationStateMachine.h"
#include "TileMap.h"

using json = nlohmann::json;


namespace ComponentSerializer
{

	json SerializeTransform(const XTEngine2d::Transform& transform);
	XTEngine2d::Transform DeserializeTransform(const json& transformJson);

	json SerializeSprite(const XTEngine2d::Sprite& sprite);
	XTEngine2d::Sprite DeserializeSprite(const json& spriteJson);

	json SerializeSpriteSheet(const XTEngine2d::SpriteSheet& sheet);
	XTEngine2d::SpriteSheet DeserializeSpriteSheet(const json& sheetJson);

	json SerializePlayerController(const XTEngine2d::PlayerController& playerController);
	XTEngine2d::PlayerController DeSerializePlayerController(const json& playercontrollerJson);

	json SerializeCamera(const XTEngine2d::Camera& camera);
	XTEngine2d::Camera DeSerializeCamera(const json& cameraJson);

	json SerializeCameraController(const XTEngine2d::CameraController& cameraController);
	XTEngine2d::CameraController DeSerializeCameraController(const json& cameraControllerJson);

	json SerializeTag(const XTEngine2d::Tag& tag);
	XTEngine2d::Tag  DeserializeTag(const json& tagJson);


	json SerializeAnimation(const XTEngine2d::Animation& animation);
	XTEngine2d::Animation DeserializeAnimation(const json& animationjson);

	json SerializeTileMap(const XTEngine2d::TileMap& tilemap);
	XTEngine2d::TileMap DeserializeTileMap(const json& tilemapJson);

	json SerializeAnimationStateMachine(const XTEngine2d::AnimationStateMachine& State);
	XTEngine2d::AnimationStateMachine DeserializeAnimationStateMachine(const json& StateJson);

}