#pragma once

#include "Utility.h"

namespace XTEngine2d
{
	class Camera
	{
	private:
		float viewPortWidth = 800;
		float viewPortHeight = 600;
	public:
		float zoom = 1.0f;
		bool primary = false;
	public:
		Camera();
		glm::mat4 GetProjection() const;
		void SetViewPortSize(float width, float height);
		glm::mat4 GetViewMatrix(Transform& transform) const;
	};
}