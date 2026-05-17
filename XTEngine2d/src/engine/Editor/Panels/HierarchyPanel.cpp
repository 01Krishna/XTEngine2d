#include "HierarchyPanel.h"

HierarchyPanel::HierarchyPanel()
{
}

HierarchyPanel::~HierarchyPanel()
{
}

void HierarchyPanel::OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity)
{
	auto entities = scene->m_Registry.GetAllEntities();

	ImGui::Begin("Hierarchy");
	for (auto& ent : entities)
	{
		if (scene->m_Registry.HasComponent<XTEngine2d::Tag>(ent))
		{
			XTEngine2d::Tag tag = scene->m_Registry.GetComponent<XTEngine2d::Tag>(ent);

			std::string label = tag.name + "##" + std::to_string(ent);

			if (ImGui::Selectable(label.c_str(),
				m_SelectedEntity == ent))
			{
				m_SelectedEntity = ent;

			}
		}
	}
	ImGui::End();
}
