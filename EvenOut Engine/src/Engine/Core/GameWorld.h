#pragma once
#include "Engine/Core/Object.h"
#include "Engine/LogSystem/Log.h"
#include "UIObject.h"

namespace Engine {

	struct SortingLayer {
		SortingLayer(std::shared_ptr<Object> p, int l) : obj(p), layer(l) {}
		~SortingLayer() {};

		std::shared_ptr<Object> obj;
		int layer;
	};

	class GameWorld{
	private:
		std::vector<SortingLayer> layers;
	protected:
		std::vector<std::shared_ptr<Object>> m_ObjectsInScene;

	public:
		GameWorld() {};
		virtual ~GameWorld() { std::cout << "Destroyed!" << std::endl; };

		void Start();

		void Update(float deltaTime);


		void Draw();

		void Refresh();

		std::vector<std::shared_ptr<Object>> GetObjects() { return m_ObjectsInScene; }

		template <typename T, typename... TArgs>
		T* CreateActor(TArgs&& ...mArgs)
		{
			T* obj(new T(std::forward<TArgs>(mArgs)...));
			LOG_CORE("Created Actor" + obj->GetTag(), LOG_INFO);
			std::shared_ptr<Object> uPtr{ obj };

			m_ObjectsInScene.emplace_back(std::move(uPtr));

			obj->Start();

			return obj;
		}

		template <typename T>
		T& GetActorOfClass()
		{
			for (int i = 0; i < m_ObjectsInScene.size(); i++)
			{
				if (auto derivedObj = std::dynamic_pointer_cast<T>(m_ObjectsInScene[i])) {
					return *derivedObj;
				}
			}
		}


	};
}