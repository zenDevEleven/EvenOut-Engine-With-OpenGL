#pragma once
#include "Engine/Core/Object.h"

namespace Engine {
	class UIObject : public Object
	{
	public:
		UIObject();
		virtual ~UIObject();


		virtual void Start();
		virtual void Update(float deltaTime);
	};
}