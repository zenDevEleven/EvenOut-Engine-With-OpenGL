#pragma once
#include "epch.h"
#include "Engine/Core/Component.h"
#include "Engine/LogSystem/Log.h"

namespace Engine {

	class Object
	{
	public:

		Object() = default;

		virtual ~Object() {}

		virtual void Start();

		virtual void Update(float deltaTime);

		virtual void Draw();

		virtual void OnContactEvent(Object* other) {};
		virtual void OnEndContactEvent(Object* other) {};

		void AddTag(const std::string& tag) {
			Tag = tag;
		}

		std::string GetTag() const {
			return Tag;
		}

		bool HasTag(const std::string& tag){
			if (tag == Tag)
				return true;

			return false;
		}

		bool IsActive() const { return m_Active; }

		template<typename T>
		bool HasComponent() const{
			return m_ComponentBitSet[GetComponentTypeID<T>()];
		}

		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... Margs) 
		{
			T* component(new T(std::forward<TArgs>(Margs)...));

			component->m_Actor = this;

			std::unique_ptr<Component>uPtr{ component };

			m_Components.emplace_back(std::move(uPtr));

			m_ComponentArray[GetComponentTypeID<T>()] = component;
			m_ComponentBitSet[GetComponentTypeID<T>()] = true;

			component->Start();
			return *component;
		}

		template<typename T> 
		T& GetComponent() const{
			auto ptr(m_ComponentArray[GetComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}

		void Destroy()
		{
			m_Active = false;
		}
	private:
		bool m_Active = true;
		std::string Tag = "Default";

		std::vector<std::unique_ptr<Component>> m_Components;

		ComponentArray m_ComponentArray;
		ComponentBitSet m_ComponentBitSet;
	};
}

