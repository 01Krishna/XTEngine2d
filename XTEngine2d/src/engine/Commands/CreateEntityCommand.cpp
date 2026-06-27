#include "CreateEntityCommand.h"

void XTEngine2d::CreateEntityCommand::Execute()
{
	m_Entity = m_Scene->m_Registry.CreateEntity();

	XT_CORE_INFO("Entity: {}", m_Entity);

}


void XTEngine2d::CreateEntityCommand::Undo()
{
	if(m_Scene->m_Registry.ValidateEntity(m_Entity))
		m_Scene->m_Registry.DestroyEntity(m_Entity);
	XT_CORE_INFO("Entity: {}", m_Entity);

}
