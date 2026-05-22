#pragma once
#include "MovementSystem.h"
#include "../Scene.h"
#include "../InputManager.h"
#include "../Camera.h"


namespace XTEngine2d
{


	void MovementSystem(XTEngine2d::Scene& scene, float deltaTime, InputManager& Input)
	{

		auto entities = scene.m_Registry.View<XTEngine2d::Transform, XTEngine2d::PlayerController>();

		auto cameraentities = scene.m_Registry.View<XTEngine2d::Transform, XTEngine2d::CameraController>();
			
		for (auto& camera : cameraentities)
		{

			if (scene.m_Registry.HasComponent<XTEngine2d::Transform>(camera) && scene.m_Registry.HasComponent<XTEngine2d::CameraController>(camera) && scene.m_Registry.HasComponent<XTEngine2d::Camera>(camera))
			{
				auto& cam = scene.m_Registry.GetComponent<XTEngine2d::Camera>(camera);
				if (cam.primary)
				{
					auto& cameracontrol = scene.m_Registry.GetComponent<XTEngine2d::CameraController>(camera);
					auto& camtransform = scene.m_Registry.GetComponent<XTEngine2d::Transform>(camera);


					if (Input.IsKeyPressed(GLFW_KEY_LEFT))
					{
						cameracontrol.vel.velocity.x -= 0.5;
					}
					else if (Input.IsKeyPressed(GLFW_KEY_UP))
					{
						cameracontrol.vel.velocity.y -= 0.5;
					}
					else if (Input.IsKeyPressed(GLFW_KEY_RIGHT))
					{
						cameracontrol.vel.velocity.x += 0.5;
					}
					else if (Input.IsKeyPressed(GLFW_KEY_DOWN))
					{
						cameracontrol.vel.velocity.y += 0.5;
					}
					else
					{
						cameracontrol.vel.velocity.x = 0;
						cameracontrol.vel.velocity.y = 0;
					}

					camtransform.localPosition.x += cameracontrol.vel.velocity.x * deltaTime;
					camtransform.localPosition.y += cameracontrol.vel.velocity.y * deltaTime;
				}
			}
			else
			{
				XT_CORE_INFO("NO ENTITY FOUND WITH THE GIVEN COMPONENTS!");
			}

		}

		for (auto& entity : entities)
		{
			if (scene.m_Registry.HasComponent<XTEngine2d::Transform>(entity) && scene.m_Registry.HasComponent<XTEngine2d::PlayerController>(entity))
			{
				auto& transform = scene.m_Registry.GetComponent<XTEngine2d::Transform>(entity);
				auto& playercontrol = scene.m_Registry.GetComponent<XTEngine2d::PlayerController>(entity);

				if (playercontrol.player)
				{

					if (Input.IsKeyPressed(GLFW_KEY_A))
					{
						playercontrol.vel.velocity.x -= 0.5;
					}
					else if (Input.IsKeyPressed(GLFW_KEY_W))
					{
						playercontrol.vel.velocity.y -= 0.5;
					}
					else if (Input.IsKeyPressed(GLFW_KEY_D))
					{
						playercontrol.vel.velocity.x += 0.5;
					}
					else if (Input.IsKeyPressed(GLFW_KEY_S))
					{
						playercontrol.vel.velocity.y += 0.5;
					}
					else
					{
						if (playercontrol.vel.velocity.x > 0)
							playercontrol.vel.velocity.x -= 0.5;

						else if (playercontrol.vel.velocity.x < 0)
							playercontrol.vel.velocity.x += 0.5;

						if (playercontrol.vel.velocity.y > 0)
							playercontrol.vel.velocity.y -= 0.5;
						else if (playercontrol.vel.velocity.y < 0)
							playercontrol.vel.velocity.y += 0.5;
					}

				transform.localPosition.x += playercontrol.vel.velocity.x * deltaTime;
				transform.localPosition.y += playercontrol.vel.velocity.y * deltaTime;

				}
			}	
			else
			{
				XT_CORE_INFO("NO ENTITY FOUND WITH THE GIVEN COMPONENTS!");
			}
		}
	};
};