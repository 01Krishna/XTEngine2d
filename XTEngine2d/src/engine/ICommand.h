#pragma once
#include "Utility.h"

namespace XTEngine2d {

    class Scene;

    class ICommand
    {
    public:
        virtual ~ICommand() = default;

        virtual void Execute() = 0;
        virtual void Undo() = 0;

        std::string CommandName = "None";
    };
}