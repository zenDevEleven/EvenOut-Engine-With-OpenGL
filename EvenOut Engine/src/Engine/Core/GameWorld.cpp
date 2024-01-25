#include "epch.h"
#include "GameWorld.h"
#include "Components.h"

namespace Engine 
{
	bool cmp(std::pair<std::shared_ptr<Object>, int>& a,
		std::pair<std::shared_ptr<Object>, int>& b)
	{
		return a.second < b.second;
	}

	std::vector<std::pair<std::shared_ptr<Object>, int>> sort(std::unordered_map<std::shared_ptr<Object>, int>& M)
	{

		std::vector<std::pair<std::shared_ptr<Object>, int> > A;

		for (auto& it : M) {
			A.push_back(it);
		}

		std::sort(A.begin(), A.end(), cmp);

		return A;
	}


	void GameWorld::Start()
	{
		for (auto& object : m_ObjectsInScene) object->Start();
	}

	void GameWorld::Update(float deltaTime)
	{
		for (size_t i = 0; i < m_ObjectsInScene.size(); i++)
		{
			m_ObjectsInScene[i]->Update(deltaTime);
		}
	}

	void GameWorld::Draw()
	{
		layers.clear();
		for (int i = 0; i < m_ObjectsInScene.size(); ++i)
		{
			if (m_ObjectsInScene[i]->HasComponent<SpriteRenderer2D>())
			{
				SpriteRenderer2D* renderer = &m_ObjectsInScene[i]->GetComponent<SpriteRenderer2D>();
				layers.push_back(SortingLayer(m_ObjectsInScene[i], renderer->m_SortingLayer));
			}
		}

		std::sort(layers.begin(), layers.end(), [](SortingLayer a, SortingLayer b) { return a.layer < b.layer; });

		for (SortingLayer layer : layers)
		{
			layer.obj->Draw();
		}

		for (int i = 0; i < m_ObjectsInScene.size(); ++i)
		{
			if (std::dynamic_pointer_cast<UIObject>(m_ObjectsInScene[i])) {
				m_ObjectsInScene[i]->Draw();
			}
		}

	}

	void GameWorld::Refresh()
	{

		m_ObjectsInScene.erase(std::remove_if(std::begin(m_ObjectsInScene), std::end(m_ObjectsInScene),
			[](const std::shared_ptr<Object>& mObject)
			{
				return !mObject->IsActive();
			}), std::end(m_ObjectsInScene));

	}

}