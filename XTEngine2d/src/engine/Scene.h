#pragma once
#include "Core.h"
#include "Registry.h"
#include "Utility.h"
#include "Systems/MovementSystem.h"
#include "Systems//RenderSystem.h"
#include "Camera.h"

namespace XTEngine2d
{

	class Renderer;

	class XT_API Scene
	{
	public:
		Registry m_Registry;
		Shader m_Shader;
		Entity m_PrimaryCamera = 0;


		Scene();	
		~Scene();

		void Update(float deltaTime, InputManager& Input);

		void Render(Renderer& renderer, glm::mat4 viewProjection);

		bool HasPrimaryCamera();

		void CheckPrimaryCameraAvailability();

		Entity GetPrimaryCamera();

		void Clear();

		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		Scene(Scene&&) = delete;
		Scene& operator=(Scene&&) = delete;
	};
}
