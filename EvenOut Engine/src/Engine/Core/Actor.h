#pragma once
#include "Engine/Core/Object.h"

namespace Engine {
	class Actor : public Object
	{
	public:
		Actor();
		virtual ~Actor();


		virtual void Start();
		virtual void Update(float deltaTime);

		virtual void OnContactEvent(Object* other);

		virtual void OnEndContactEvent(Object* other);

	};
}