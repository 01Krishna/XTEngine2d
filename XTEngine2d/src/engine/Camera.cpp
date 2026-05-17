#include "Camera.h"

namespace XTEngine2d
{
	Camera::Camera() {}
	
	void Camera::SetViewPortSize(float width, float height)
	{
		viewPortWidth = width;
		viewPortHeight = height;
	}
	glm::mat4 Camera::GetViewMatrix(Transform& transform) const
	{
		glm::mat4 view = glm::mat4(1.0f);

		return glm::translate(view, glm::vec3(-transform.position, 0.0f));
	}

	glm::mat4 Camera::GetProjection() const
	{

		return glm::ortho(0.0f, (float)viewPortWidth * zoom, (float)viewPortHeight * zoom, 0.0f, -1.0f, 1.0f);
	}
}