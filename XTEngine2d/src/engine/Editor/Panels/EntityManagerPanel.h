#pragma once
#include "portable-file-dialogs.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../Utility.h"
#include "../../Scene.h"



class EntityManagerPanel
{
public:
	EntityManagerPanel();
	~EntityManagerPanel();

public:
	void OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity);
};

