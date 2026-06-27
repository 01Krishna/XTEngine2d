#include "MoveEntityCommand.h"

namespace XTEngine2d
{


    void MoveEntityCommand::Execute()
    {

        auto& transform =
            m_Scene->m_Registry.GetComponent<Transform>(m_Entity);

        transform.localPosition = m_NewPosition;
    }

    void MoveEntityCommand::Undo()
    {
        auto& transform =
            m_Scene->m_Registry.GetComponent<Transform>(m_Entity);

        transform.localPosition = m_OldPosition;
    }

}
