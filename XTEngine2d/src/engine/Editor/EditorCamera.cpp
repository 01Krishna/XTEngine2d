#include "EditorCamera.h"

namespace XTEngine2d
{
	EditorCamera::EditorCamera() {}
	
	void EditorCamera::SetViewPortSize(float width, float height)
	{
		viewPortWidth = width;
		viewPortHeight = height;
	}
	glm::mat4 EditorCamera::GetViewMatrix(glm::vec2 position) const
	{
		glm::mat4 view = glm::mat4(1.0f);

		return glm::translate(view, glm::vec3(-position, 0.0f));
	}

	void EditorCamera::UpdateCamera()
	{

	}

	glm::mat4 EditorCamera::GetProjection() const
	{

		return glm::ortho(0.0f, (float)viewPortWidth * zoom, (float)viewPortHeight * zoom, 0.0f, -1.0f, 1.0f);
	}
}