#include "epch.h"
#include "Actor.h"

namespace Engine {
	Actor::Actor()
	{
	}
	Actor::~Actor()
	{
	}
	void Actor::Start()
	{
		__super::Start();
	}

	void Actor::Update(float deltaTime)
	{
		__super::Update(deltaTime);
	}

	void Actor::OnContactEvent(Object* other)
	{
		__super::OnContactEvent(other);
	}

	void Actor::OnEndContactEvent(Object* other)
	{
		__super::OnEndContactEvent(other);
	}

}
