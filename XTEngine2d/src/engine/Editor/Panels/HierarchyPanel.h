#pragma once
#include "../../../../vendor/imgui/imgui.h"
#include "../../Utility.h"
#include "../../Scene.h"

class HierarchyPanel
{
public:
	HierarchyPanel();
	~HierarchyPanel();

public:
	void OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity);

	void DrawEntityNode(Entity entity, XTEngine2d::Scene* scene, Entity& m_SelectedEntity);

	void DrawHierarchy(std::vector<Entity>& entities, XTEngine2d::Scene* scene, Entity& m_SelectedEntity);
	
	void ReparentEntity(Entity child, Entity parent, XTEngine2d::Scene* scene);

	bool IsChildOf(Entity entity, Entity possibleParent, XTEngine2d::Scene* scene);

};

