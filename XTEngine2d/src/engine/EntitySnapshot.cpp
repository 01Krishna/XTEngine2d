#include "EntitySnapshot.h"

XTEngine2d::EntitySnapshot::EntitySnapshot()
{
}

XTEngine2d::EntitySnapshot::~EntitySnapshot()
{
}

void XTEngine2d::EntitySnapshot::TakeEntitySnapshot(Entity entity, Scene* scene)
{

	m_Entity = entity;
	
	if(scene->m_Registry.HasComponent<Transform>(entity))
		m_Registry.AddComponent<Transform>(entity, scene->m_Registry.GetComponent<Transform>(entity));

	if (scene->m_Registry.HasComponent<Sprite>(entity))
	{
		m_Registry.AddComponent<Sprite>(entity, scene->m_Registry.GetComponent<Sprite>(entity));
		m_Registry.AddComponent<SpriteSheet>(entity, scene->m_Registry.GetComponent<SpriteSheet>(entity));

	}

	if(scene->m_Registry.HasComponent<Camera>(entity))
		m_Registry.AddComponent<Camera>(entity, scene->m_Registry.GetComponent<Camera>(entity));

	if(scene->m_Registry.HasComponent<CameraController>(entity))
		m_Registry.AddComponent<CameraController>(entity, scene->m_Registry.GetComponent<CameraController>(entity));

	if(scene->m_Registry.HasComponent<PlayerController>(entity))
		m_Registry.AddComponent<PlayerController>(entity, scene->m_Registry.GetComponent<PlayerController>(entity));

	if(scene->m_Registry.HasComponent<Tag>(entity))
		m_Registry.AddComponent<Tag>(entity, scene->m_Registry.GetComponent<Tag>(entity));

}

void XTEngine2d::EntitySnapshot::RetriveSnapshotData(Entity entity, Scene* scene)
{
	if (m_Registry.HasComponent<Transform>(m_Entity))
		scene->m_Registry.AddComponent<Transform>(entity, m_Registry.GetComponent<Transform>(m_Entity));

	if (m_Registry.HasComponent<Sprite>(m_Entity))
	{
		scene->m_Registry.AddComponent<Sprite>(entity, m_Registry.GetComponent<Sprite>(m_Entity));
		scene->m_Registry.AddComponent<SpriteSheet>(entity, m_Registry.GetComponent<SpriteSheet>(m_Entity));

	}

	if (m_Registry.HasComponent<Camera>(m_Entity))
		scene->m_Registry.AddComponent<Camera>(entity, m_Registry.GetComponent<Camera>(m_Entity));

	if (m_Registry.HasComponent<CameraController>(m_Entity))
		scene->m_Registry.AddComponent<CameraController>(entity, m_Registry.GetComponent<CameraController>(m_Entity));

	if (m_Registry.HasComponent<PlayerController>(m_Entity))
		scene->m_Registry.AddComponent<PlayerController>(entity, m_Registry.GetComponent<PlayerController>(m_Entity));

	if (m_Registry.HasComponent<Tag>(m_Entity))
		scene->m_Registry.AddComponent<Tag>(entity, m_Registry.GetComponent<Tag>(m_Entity));

}
