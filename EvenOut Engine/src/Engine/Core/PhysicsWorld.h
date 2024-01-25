 #pragma once
#include "epch.h"
#include "Engine/Core/GameWorld.h"

namespace Engine {


	class PhysicsWorld : public b2ContactListener
	{
	public:
		PhysicsWorld();
		~PhysicsWorld() {};
		void Update(float deltaTime);

		void DestroyBody(b2Body* body);


		b2Body* CreateBody(b2BodyDef body);

		void QueueCreateBody(b2Body*& body, const b2BodyDef& bodyDef);
		void QueueFixtureCreation(b2Body*& body, const b2FixtureDef& fixtureDef, const b2PolygonShape& shape);
		void ProcessPendingCreations();


		static PhysicsWorld& GetPhysicsWorld() { return *m_Instance; }
		GameWorld& GetWorld() { return *m_World; }

		void SetWorld(GameWorld* world);


		void BeginContact(b2Contact* contact) override;


		void EndContact(b2Contact* contact) override;


		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;


		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	private:
		static PhysicsWorld* m_Instance;

		GameWorld* m_World;

		b2Vec2 m_Gravity = b2Vec2(0.0f, -9.8f);
		b2World* m_WorldLevel;

		std::vector<b2BodyDef> pendingBodyAssigns;
		std::vector<b2Body**> pendingBodies;		
		
		std::vector<b2FixtureDef> pendingFixturesCreations;
		std::vector<b2PolygonShape> pendingShapeCreations;
		std::vector<b2Body**> pendingFixtureAssings;

	};
}

