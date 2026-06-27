#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"
#include "../Log.h"
namespace XTEngine2d
{
	template <typename T>
	class AddComponentCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		Entity m_Entity = 0;
		T m_ComponentData;	

	public:
		AddComponentCommand(Scene* scene, Entity entity, T& component)
		{
			m_Scene = scene;
			m_Entity = entity;
			m_ComponentData = component;
			CommandName = "AddComponentCommand";
		};
		void Execute() override
		{
			if (!m_Scene->m_Registry.HasComponent<T>(m_Entity))
			{
				m_Scene->m_Registry.AddComponent(m_Entity, m_ComponentData);
			}

		}
			
		void Undo() override
		{
			if (m_Scene->m_Registry.HasComponent<T>(m_Entity))
			{
				m_Scene->m_Registry.RemoveComponent<T>(m_Entity);
			}
		}
		Entity GetCreatedEntity() { return m_Entity; };

	};

}