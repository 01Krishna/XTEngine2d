#include "EntityManagerPanel.h"

void EntityManagerPanel::OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity)
{
	ImGui::Begin("Entity Manager");
	if (ImGui::Button("Delete Entity"))
	{
		scene->m_Registry.DestroyEntity(m_SelectedEntity);
		scene->CheckPrimaryCameraAvailability();
		m_SelectedEntity = 0;
	}

	if (ImGui::Button("Create Entity"))
	{
		m_SelectedEntity = 0;
		Entity entity = scene->m_Registry.CreateEntity();
		XTEngine2d::Tag tag;
		tag.name = "Entity";

		if (!scene->m_Registry.HasComponent<XTEngine2d::Tag>(entity))
			scene->m_Registry.AddComponent<XTEngine2d::Tag>(entity, tag);
		m_SelectedEntity = entity;
	}
	ImGui::End();
}

EntityManagerPanel::EntityManagerPanel()
{
}

EntityManagerPanel::~EntityManagerPanel()
{
}
