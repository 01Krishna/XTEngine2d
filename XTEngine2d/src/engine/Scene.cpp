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
		UpdateHierarchityTransfrom();
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

	void Scene::UpdateHierarchityTransfrom()
	{
		auto allEntities = m_Registry.GetAllEntities();

		for (auto entity : allEntities)
		{
			if (!m_Registry.HasComponent<Transform>(entity))
				continue;


			bool isRoot = true;

			if (m_Registry.HasComponent<Hierarchy>(entity))
			{
				auto& hierarchy = m_Registry.GetComponent<Hierarchy>(entity);

				isRoot = hierarchy.parent == 0;

			}

			if (isRoot)
			{
				UpdateChildTransform(entity);
			}
		}


	}
	void Scene::UpdateChildTransform(Entity entity)
	{

		if (!m_Registry.HasComponent<Transform>(entity))
			return;

		auto& transform = m_Registry.GetComponent<Transform>(entity);


		if (m_Registry.HasComponent<Hierarchy>(entity))
		{
			auto& hierarchy = m_Registry.GetComponent<Hierarchy>(entity);

			if (hierarchy.parent != 0)
			{
				auto& parentTransform = m_Registry.GetComponent<Transform>(hierarchy.parent);

				transform.worldPosition = parentTransform.worldPosition + transform.localPosition;

				transform.worldRotation = parentTransform.worldRotation + transform.localRotation;

				transform.worldScale = parentTransform.worldScale + transform.localScale;

				//XT_CORE_INFO("World POS: {},{}",transform.worldPosition.x, transform.worldPosition.y);
				//XT_CORE_INFO("Local POS: {},{}",transform.localPosition.x,transform.localPosition.y);
			}
			else
			{
				transform.worldPosition = transform.localPosition;
				transform.worldRotation = transform.localRotation;
				transform.worldScale    = transform.localScale;

			}


			for (auto child : hierarchy.children)
			{
				UpdateChildTransform(child);
			}
		}
		else
		{
			transform.worldPosition = transform.localPosition;
			transform.worldRotation = transform.localRotation;
			transform.worldScale    = transform.localScale;
		}

	}
}