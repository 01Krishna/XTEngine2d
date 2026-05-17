#pragma once


namespace XTEngine2d
{
	class Renderer;
	class Scene;
	void RenderSystem(XTEngine2d::Scene& scene, XTEngine2d::Renderer &renderer, glm::mat4 viewProjection);
};