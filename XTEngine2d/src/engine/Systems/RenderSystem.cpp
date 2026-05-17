#include "../Scene.h"
#include "../Renderer.h"
#include "../Camera.h"
#include "RenderSystem.h"

namespace XTEngine2d
{
	void RenderSystem(XTEngine2d::Scene& scene, XTEngine2d::Renderer& renderer, glm::mat4 viewProjection)
	{
		
		const auto& entities = scene.m_Registry.View<XTEngine2d::Transform, XTEngine2d::Sprite>();

		renderer.BeginScene(viewProjection);
		
		for (auto& entity : entities)
		{		
			if (scene.m_Registry.HasComponent<Transform>(entity))
			{
				auto& transform = scene.m_Registry.GetComponent<Transform>(entity);
				if (scene.m_Registry.HasComponent<Sprite>(entity))
				{
					auto& sprite = scene.m_Registry.GetComponent<Sprite>(entity);
					renderer.Draw(scene.m_Shader, sprite, transform);
				}
			}
				
		}
	}	
};