#pragma once
#include "Scene.h"
#include "Registry.h"

namespace XTEngine2d
{

	class EntitySnapshot
	{
	public:
		Entity m_Entity;
		Registry m_Registry;
	public:
		EntitySnapshot();
		~EntitySnapshot();

		void TakeEntitySnapshot(Entity entity, Scene* scene);
		void RetriveSnapshotData(Entity entity, Scene* scene);

	};

}