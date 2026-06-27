#pragma once
#include "portable-file-dialogs.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../Utility.h"
#include "../../Scene.h"
#include "../../Commands/CreateEntityCommand.h"
#include "../../Commands/DeleteEntityCommand.h"
#include "../../CommandHistory.h"




class EntityManagerPanel
{
public:
	EntityManagerPanel();
	~EntityManagerPanel();

	void Init(XTEngine2d::CommandHistory* cmdHistory);
private:
	XTEngine2d::CommandHistory* CMDHISTORY;

public:
	void OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity);
};

