#include "CommandHistory.h"


namespace XTEngine2d
{
	void CommandHistory::ExecuteCommand(std::shared_ptr<ICommand> command)
	{
		command->Execute();
		
		m_UndoStack.push(command);

		while (!m_RedoStack.empty())
			m_RedoStack.pop();

	}
	void CommandHistory::Undo()
	{
		if (m_UndoStack.empty())
		{
			return;
		}

		auto cmd = m_UndoStack.top();

		m_UndoStack.pop();

		cmd->Undo();


		m_RedoStack.push(cmd);

	}

	void CommandHistory::Redo()
	{
		if (m_RedoStack.empty())
			return;

		auto cmd = m_RedoStack.top();
		m_RedoStack.pop();

		cmd->Execute();

		m_UndoStack.push(cmd);
	}

}