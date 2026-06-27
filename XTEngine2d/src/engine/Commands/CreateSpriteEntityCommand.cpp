#include "CreateSpriteEntityCommand.h"

void XTEngine2d::CreateSpriteEntityCommand::Execute()
{
	m_Entity = m_Scene->m_Registry.CreateEntity();

	Transform transform;
	Sprite sprite;
	SpriteSheet sheet;

	Tag tag;
	tag.name = "Entity";

	sprite.texture = AssetManager::LoadTexture(m_Path);
	sheet.SetTexture(sprite.texture.get(), sprite.texture->GetWidth(), sprite.texture->GetHeight());
	sprite.uv = sheet.GetUV(1, 1);
	m_Scene->m_Registry.AddComponent(m_Entity, transform);
	m_Scene->m_Registry.AddComponent(m_Entity, sprite);
	m_Scene->m_Registry.AddComponent(m_Entity, sheet);
	m_Scene->m_Registry.AddComponent(m_Entity, tag);

	m_EntitySnapshot.TakeEntitySnapshot(m_Entity,m_Scene);
}

void XTEngine2d::CreateSpriteEntityCommand::Undo()
{
	m_Scene->m_Registry.DestroyEntity(m_Entity);
}
