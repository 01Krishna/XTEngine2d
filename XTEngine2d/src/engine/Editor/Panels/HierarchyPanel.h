#pragma once
#include "../../../../vendor/imgui/imgui.h"
#include "../../Utility.h"
#include "../../Scene.h"
#include "../../Commands/ReparentEntityCommand.h"
#include "../../CommandHistory.h"
#include "../../InputManager.h"


class HierarchyPanel
{
public:
	HierarchyPanel();
	~HierarchyPanel();

	void Init(XTEngine2d::CommandHistory* cmdHistory)
	{
		CMDHISTORY = cmdHistory;
	}

private:
	XTEngine2d::CommandHistory* CMDHISTORY;

public:
	void OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity);

	void DrawEntityNode(Entity entity, XTEngine2d::Scene* scene, Entity& m_SelectedEntity);

	void DrawHierarchy(std::vector<Entity>& entities, XTEngine2d::Scene* scene, Entity& m_SelectedEntity);
	

};

