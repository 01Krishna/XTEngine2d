#include "DeleteEntityCommand.h"

void XTEngine2d::DeleteEntityCommand::Execute()
{
	m_Scene->m_Registry.DestroyEntity(m_Entity);
}


void XTEngine2d::DeleteEntityCommand::Undo()
{
	m_EntitySnapshot.RetriveSnapshotData(m_Entity, m_Scene);
}
