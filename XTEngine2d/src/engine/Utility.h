#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Textures.h"
#include "Sprite.h"
#include "Shader.h" 
#include "SpriteSheet.h"
#include "KeyboardInputHandle.h"
#include "MouseInputHandle.h"
#include "Log.h"
#include <string>

namespace XTEngine2d
{
	struct Transform
	{
		Transform(glm::vec2 Pos, glm::vec2 Size, float Rotation)
			:localPosition(Pos),size(Size),localRotation(Rotation)
		{ }
		Transform() = default;

		glm::vec2 localPosition = { 10,10 };
		glm::vec2 localScale = { 1,1 };
		float localRotation = 0.f;


		glm::vec2 worldPosition = { 0,0 };
		glm::vec2 worldScale = { 1,1 };
		float worldRotation = 0.f;

		glm::vec2 size = { 100,100 };
	};


	struct Velocity
	{
		glm::vec2 velocity;
		Velocity(glm::vec2 vel)
			:velocity(vel){ }

		Velocity() { velocity.x = 0; velocity.y = 0; }
	};

	struct Tag
	{
		Tag(std::string Name):name(Name){}
		Tag(){}
		std::string name = "Entity";
	};
	
	enum class SceneState
	{
		Edit,
		Play,
		Pause
	};

};
