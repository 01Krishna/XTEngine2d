#pragma once
#include <stack>
#include "ICommand.h"
#include <memory>

namespace XTEngine2d
{
	class CommandHistory
	{
	public:
		std::stack<std::shared_ptr<ICommand>> m_UndoStack;
		std::stack<std::shared_ptr<ICommand>> m_RedoStack;

	public:
		void ExecuteCommand(std::shared_ptr<ICommand> command);

		void Undo();
		void Redo();


		void ShowUndoStack(std::stack<std::shared_ptr<ICommand>> tempStack, std::stack<std::string> &forRetrival)
		{
			while (!tempStack.empty())
			{
				forRetrival.push( tempStack.top()->CommandName);
				tempStack.pop();
			}

		};
		void ShowRedoStack(std::stack<std::shared_ptr<ICommand>> tempStack, std::stack<std::string > &forRetrival)
		{
			while (!tempStack.empty())
			{
				forRetrival.push( tempStack.top()->CommandName);
				tempStack.pop();
			}
		};
	};
}
