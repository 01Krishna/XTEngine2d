#pragma once
#include "ComponentSerializer.h"
#include "Scene.h"
#include "Utility.h"

class PrefabSerializer
{
private:
	nlohmann::json data;

public:
	void SerializeEntity(Entity selectedEntity, XTEngine2d::Scene* scene, std::string path);
	Entity DeserializeEntity(XTEngine2d::Scene* scene, std::string filePat);
};

