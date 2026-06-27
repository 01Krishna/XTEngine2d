#include "EntityManagerPanel.h"

void EntityManagerPanel::OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity)
{
	ImGui::Begin("Entity Manager");
	if (ImGui::Button("Delete Entity"))
	{
		auto cmd = std::make_shared<XTEngine2d::DeleteEntityCommand>(scene, m_SelectedEntity);

		CMDHISTORY->ExecuteCommand(cmd);
		
		scene->CheckPrimaryCameraAvailability();

		m_SelectedEntity = 0;
	}

	if(ImGui::Button("Create Entity"))
	{
		m_SelectedEntity = 0;
		Entity entity = 0;

		auto cmd = std::make_shared<XTEngine2d::CreateEntityCommand>(scene);

		CMDHISTORY->ExecuteCommand(cmd);
		entity = cmd->GetCreatedEntity();

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

void EntityManagerPanel::Init(XTEngine2d::CommandHistory* cmdHistory)
{
	CMDHISTORY = cmdHistory;
}
