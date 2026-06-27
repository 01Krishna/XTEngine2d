#pragma once

#include "../../vendor/glm/glm.hpp"
#include "../../FrameBuffer.h"
#include "../../Renderer.h"
#include "../../Window.h"
#include "../../LineRenderer.h"
#include "../../Scene.h"
#include "../../AssetManager.h"
#include "portable-file-dialogs.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../PrefabSerializer.h"
#include "../../Commands/MoveEntityCommand.h"
#include "../../Commands/AddComponentCommand.h"
#include "../../Commands/RemoveComponentCommand.h"
#include "../../CommandHistory.h"
#include "../../Animation.h"
#include "../../AnimationStateMachine.h"
#include "../../TileMap.h"


using namespace XTEngine2d;

class InspectorPanel
{
public:
	InspectorPanel();
	~InspectorPanel();

private:
	int row = 1;
	int column = 1;
	int	selectedTile = 0;
	std::string temp_state_name;
	CommandHistory* CMDHISTORY;


public:
	
	void Init(CommandHistory* cmdHistory);

	void OnImGuiRender(Scene* scene, Entity &m_SelectedEntity, std::string SelectedAsset);

	int getSelectedTile() {
		return selectedTile;
	};

};

