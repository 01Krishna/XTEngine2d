#pragma once
#include "../ICommand.h"


class MoveEntityCommand :public ICommand
{
public:

	void Execute() override;
	void Undo() override;

};

