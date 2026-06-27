#pragma once
#include "../ICommand.h"
#include "../Hierarchy.h"
#include "../Editor/Panels/PanelsHelper.h"


namespace XTEngine2d
{
   inline void ReparentEntity(Entity child, Entity newParent, XTEngine2d::Scene* scene)
    {
        if (child == newParent)
            return;

        // Child hierarchy
        if (!scene->m_Registry.HasComponent<Hierarchy>(child))
        {
            scene->m_Registry.AddComponent(
                child,
                Hierarchy{});
        }

        auto& childHierarchy =
            scene->m_Registry.GetComponent<Hierarchy>(child);

        // Remove from old parent
        if (childHierarchy.parent != 0)
        {
            Entity oldParent =
                childHierarchy.parent;

            if (scene->m_Registry.HasComponent<Hierarchy>(oldParent))
            {
                auto& oldHierarchy =
                    scene->m_Registry.GetComponent<Hierarchy>(oldParent);

                auto& children =
                    oldHierarchy.children;

                children.erase(
                    std::remove(
                        children.begin(),
                        children.end(),
                        child),
                    children.end());
            }
        }

        // Assign new parent
        childHierarchy.parent = newParent;

        // Ensure parent hierarchy exists
        if (!scene->m_Registry.HasComponent<Hierarchy>(newParent))
        {
            scene->m_Registry.AddComponent(
                newParent,
                Hierarchy{});
        }

        // Add child to new parent
        scene->m_Registry
            .GetComponent<Hierarchy>(newParent)
            .children
            .push_back(child);
    }

   inline bool IsChildOf(Entity entity, Entity possibleParent, XTEngine2d::Scene* scene)
    {
        if (!scene->m_Registry.HasComponent<Hierarchy>(entity))
            return false;

        auto& hierarchy =
            scene->m_Registry.GetComponent<Hierarchy>(entity);

        if (hierarchy.parent == possibleParent)
            return true;

        if (hierarchy.parent == 0)
            return false;

        return IsChildOf(
            hierarchy.parent,
            possibleParent,
            scene);
    };

	class ReparentEntityCommand : public ICommand
	{
	private:
		Scene* m_Scene;
		Entity m_ChildEntity = 0;
		Entity m_OldParentEntity = 0;
		Entity m_NewParentEntity = 0;

	public:
		ReparentEntityCommand(Entity child, Entity newParent, Scene* scene)
		{
			m_Scene = scene;
			m_ChildEntity = child;
			m_NewParentEntity = newParent;
            CommandName = "ReparentEntityCommand";
		};
		void Execute() override
		{
            if (m_Scene->m_Registry.HasComponent<Hierarchy>(m_ChildEntity))
            {
                m_OldParentEntity = m_Scene->m_Registry.GetComponent<Hierarchy>(m_ChildEntity).parent;
            }
            else
            {
                m_OldParentEntity = 0; // No previous parent
            }

			XTEngine2d::ReparentEntity(m_ChildEntity, m_NewParentEntity, m_Scene);
		};
		void Undo() override
		{
            XTEngine2d::ReparentEntity(m_ChildEntity, m_OldParentEntity, m_Scene);
		};
	};
}
