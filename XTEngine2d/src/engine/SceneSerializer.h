#pragma once
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

namespace XTEngine2d
{
	class Scene;
	class SceneSerializer
	{
	private:
		nlohmann::json data;
		std::string sceneName;
	public:
		SceneSerializer();
		~SceneSerializer();
		void Serialize(const std::string& filePath, const std::string& sceneName, Scene* scene);
		void Deserialize(const std::string& filePath, const std::string& sceneName, Scene* scene);

	};
}
