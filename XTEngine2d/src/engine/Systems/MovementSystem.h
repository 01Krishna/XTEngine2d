#pragma once
#include "../PlayerController.h"
#include "../CameraController.h"


namespace XTEngine2d
{
	class Scene;
	class InputManager;
	void MovementSystem(XTEngine2d::Scene& scene, float deltaTime, InputManager& Input);
};