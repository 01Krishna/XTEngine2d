#pragma once
#include "Utility.h"

namespace XTEngine2d
{
	class PlayerController
	{
	public:
		XTEngine2d::Velocity vel;
		bool player = false;
	public:
		void Update(XTEngine2d::Transform& transform, Input_Keys Key)
		{
		}
	};
}