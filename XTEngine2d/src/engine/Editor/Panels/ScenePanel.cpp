#include "ScenePanel.h"

ScenePanel::ScenePanel()
{

}

ScenePanel::~ScenePanel()
{	

}

void ScenePanel::OnImGuiRender(XTEngine2d::Scene* scene , XTEngine2d::SceneState& sceneState)
{
	ImGui::Begin("Scene Controls");
	static char buffer[256];

	strcpy_s(buffer, scene->m_SceneName.c_str());

	if (ImGui::InputText(
		"##sceneName",
		buffer,
		256))
	{
		scene->m_SceneName = buffer;
	}

	if (ImGui::Button("Save Scene", ImVec2(0, 0)))
	{
		XTEngine2d::SceneSerializer serializer;

		serializer.Serialize("Assets\\scenes\\" + scene->m_SceneName + ".json", scene->m_SceneName, scene);
	}
	if (ImGui::Button("Load Scene", ImVec2(0, 0)))
	{
		XTEngine2d::SceneSerializer serializer;

		serializer.Deserialize("Assets\\scenes\\" + scene->m_SceneName + ".json", scene->m_SceneName, scene);
		scene->CheckPrimaryCameraAvailability();
		if (!scene->HasPrimaryCamera() || !scene->m_Registry.ValidateEntity(scene->GetPrimaryCamera()))
			CreateDefaultCamera(scene);

	}

	if (sceneState == XTEngine2d::SceneState::Edit)
	{
		if (ImGui::Button("Play"))
		{
			sceneState = XTEngine2d::SceneState::Play;
		}
		if (ImGui::Button("Clear Scene", ImVec2(0, 0)))
		{
			if (scene)
				scene->Clear();
		}
	}
	else
	{
		if (ImGui::Button("Stop"))
		{
			sceneState = XTEngine2d::SceneState::Edit;
		}
	}
	ImGui::End();
}

void ScenePanel::CreateDefaultCamera(XTEngine2d::Scene* scene)
{
	Entity cameraEntity = scene->m_Registry.CreateEntity();
	XTEngine2d::Tag tag;
	tag.name = "Default Camera";
	scene->m_Registry.AddComponent<XTEngine2d::Tag>(cameraEntity, tag);
	XTEngine2d::Camera cam;
	cam.primary = true;
	cam.zoom = 1.0f;
	scene->m_Registry.AddComponent<XTEngine2d::Camera>(cameraEntity, cam);
	XTEngine2d::Transform transform;
	transform.localPosition = { 0.0f, 0.0f };
	transform.localRotation = 0.f;
	transform.size = { 1.0f, 1.0f };
	scene->m_Registry.AddComponent<XTEngine2d::Transform>(cameraEntity, transform);
}
