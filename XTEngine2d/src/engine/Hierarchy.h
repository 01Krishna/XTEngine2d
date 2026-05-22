#pragma once
#include <vector>
#include "Scene.h"

class Hierarchy
{
public:
	Hierarchy();
	~Hierarchy();

public:
	Entity parent = 0;
	std::vector<Entity> children;
};

