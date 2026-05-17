#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <vector>
#include <memory>
#include "Core.h"



using Entity = uint32_t;

inline size_t GetUniqueComponentID()
{
	static size_t lastID = 0;
	return lastID++;
}

template<typename T>
size_t GetComponentTypeID()
{
	static size_t typeID = GetUniqueComponentID();
	return typeID;
}


namespace XTEngine2d
{


	class XT_API Registry
	{
	public:
		Entity m_NextEntity = 1;
	public:
		Registry() = default;

		Registry(const Registry&) = delete;
		Registry& operator=(const Registry&) = delete;

		Registry(Registry&&) = default;
		Registry& operator=(Registry&&) = default;


		struct IComponentStorage
		{
			virtual ~IComponentStorage() = default;

			virtual std::vector<Entity> GetEntities() = 0;
			virtual void RemoveEntity(Entity entity) = 0;
		};

		template <typename T>
		struct ComponentStorage : IComponentStorage
		{
			std::unordered_map<Entity, T> components;

			std::vector<Entity> GetEntities() override
			{
				std::vector<Entity> result;

				for (auto& [entity, component] : components)
					result.push_back(entity);

				return result;
			}

			void RemoveEntity(Entity entity) override
			{
				components.erase(entity);
			}
		};

		std::unordered_map<size_t, std::unique_ptr<IComponentStorage>> m_Storages;

	public:
		Entity CreateEntity()
		{
			return m_NextEntity++;
		}

	private:

		template <typename T>
		ComponentStorage<T>& GetStorage()
		{
			size_t type = GetComponentTypeID<T>();

			auto it = m_Storages.find(type);

			if (it == m_Storages.end())
			{
				auto storage = std::make_unique<ComponentStorage<T>>();
				it = m_Storages.emplace(type, std::move(storage)).first;
			}

			return *static_cast<ComponentStorage<T>*>(it->second.get());
		}

	public:
		template <typename T>
		void AddComponent(Entity entity, const T& component)
		{
			GetStorage<T>().components[entity] = component;
		}

		template <typename T>
		T& GetComponent(Entity entity)
		{
			return GetStorage<T>().components.at(entity);
		}

		template <typename T>
		bool HasComponent(Entity entity)
		{
			auto& storage = GetStorage<T>().components;
			return storage.find(entity) != storage.end();
		}
		
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			auto& storage = GetStorage<T>().components;
			storage.erase(entity);
		}


		void DestroyEntity(Entity entity)
		{
			for (auto& [type, storage] : m_Storages)
			{
				storage->RemoveEntity(entity);
			}
		}

		template <typename T1, typename T2>
		std::vector<Entity> View()
		{
			std::vector<Entity> result;

			auto& storage1 = GetStorage<T1>().components;
			auto& storage2 = GetStorage<T2>().components;

			for (auto& [entity, comp] : storage1)
			{
				if (storage2.find(entity) != storage2.end())
				{
					result.push_back(entity);
				}
			}
			return result;
		}

		std::vector<Entity> GetAllEntities()
		{
			std::unordered_set<Entity> unique;

			for (auto& [type, storage] : m_Storages)
			{
				for (auto& entity : storage->GetEntities())
				{
					unique.insert(entity);
				}
			}

			return std::vector<Entity>(unique.begin(), unique.end());
		}

		bool ValidateEntity(Entity entity)
		{
			for (auto& [type, storage] : m_Storages)
			{
				for (auto& entityInStorage : storage->GetEntities())
				{
					if (entityInStorage == entity)
					{
						return true;
					}
				}
			}
			return false;
		}

		void Clear()
		{
			m_Storages.clear();
			m_NextEntity = 1;
		}
	};
}