#include "Scene.h"

namespace XTEngine2d
{
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}

	void Scene::Update(float deltaTime, InputManager& Input)
	{
		MovementSystem(*this, deltaTime,Input);
	}

	void Scene::Render(Renderer& renderer, glm::mat4 viewProjection)
	{
		RenderSystem(*this, renderer, viewProjection);
	}

	bool Scene::HasPrimaryCamera()
	{
		if(m_PrimaryCamera && m_Registry.ValidateEntity(m_PrimaryCamera))
			return true;

		return false;
	}

	void Scene::CheckPrimaryCameraAvailability()
	{

		auto cameras = m_Registry.View<Camera, Transform>();
		
		for (auto& camera : cameras)
		{
			auto& primaryCamera = m_Registry.GetComponent<Camera>(camera);

			if (primaryCamera.primary)
			{
				m_PrimaryCamera = camera;
			}
			else
			{
				m_PrimaryCamera = 0;
			}
		}
	}

	Entity Scene::GetPrimaryCamera()
	{
		return m_PrimaryCamera;
	}
	void Scene::Clear()
	{
		m_Registry.Clear();
	}
}