#include "epch.h"
#include "Object.h"

namespace Engine {

	void Object::Start()
	{
		

	}

	void Object::Update(float deltaTime)
	{
		for (auto& c : m_Components) {
			c->UpdateComponent(deltaTime);
		}
	}
	void Object::Draw()
	{
		for (auto& c : m_Components) {
			c->DrawComponent();
		}
	}
}