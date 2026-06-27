#pragma once
#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"
#include "../Log.h"
namespace XTEngine2d
{
	template <typename T>
	class RemoveComponentCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		Entity m_Entity = 0;
		T m_ComponentData;


	public:
		RemoveComponentCommand(Scene* scene, Entity entity)
		{
			m_Scene = scene;
			m_Entity = entity;
			CommandName = "RemoveComponentCommand";

		};
		void Execute() override
		{
			if (m_Scene->m_Registry.HasComponent<T>(m_Entity))
			{
				m_ComponentData = m_Scene->m_Registry.GetComponent<T>(m_Entity);
				m_Scene->m_Registry.RemoveComponent<T>(m_Entity);
			}

		}

		void Undo() override
		{
			if (!m_Scene->m_Registry.HasComponent<T>(m_Entity))
			{
				m_Scene->m_Registry.AddComponent(m_Entity, m_ComponentData);
			}
		}
		Entity GetCreatedEntity() { return m_Entity; };

	};

}