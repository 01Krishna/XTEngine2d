#pragma once
#include "../../vendor/glm/glm.hpp"
#include "../../FrameBuffer.h"
#include "../../AssetManager.h"
#include "portable-file-dialogs.h"
#include "../../SceneSerializer.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../Utility.h"
#include "../../Scene.h"



class ScenePanel
{
public:
	ScenePanel();
   ~ScenePanel();

public:
	void OnImGuiRender(XTEngine2d::Scene* scene, XTEngine2d::SceneState& sceneState);

private:
	std::string sceneName = "New Scene";
private:
	void CreateDefaultCamera(XTEngine2d::Scene* scene);
};

