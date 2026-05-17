#pragma once

#include "../Utility.h"

namespace XTEngine2d
{
	class EditorCamera
	{
	private:
		float viewPortWidth = 800;
		float viewPortHeight = 600;
	public:
		float zoom = 1.0f;
		glm::vec2 position = { 0,0 };

	public:
		EditorCamera();
		glm::mat4 GetProjection() const;
		void SetViewPortSize(float width, float height);
		glm::mat4 GetViewMatrix(glm::vec2 position) const;
		void UpdateCamera();
	};
}