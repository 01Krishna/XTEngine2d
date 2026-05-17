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
};

