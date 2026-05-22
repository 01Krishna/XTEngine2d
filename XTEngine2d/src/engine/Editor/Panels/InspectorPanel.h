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

using namespace XTEngine2d;

class InspectorPanel
{
public:
	InspectorPanel();
	~InspectorPanel();

private:
	int row = 1;
	int column = 1;


public:
	void OnImGuiRender(Scene* scene, Entity &m_SelectedEntity, std::string SelectedAsset);

};

