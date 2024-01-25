#include "epch.h"
#include "PhysicsWorld.h"
#include "Engine/Core/Components.h"

namespace Engine {

	PhysicsWorld* PhysicsWorld::m_Instance;

	PhysicsWorld::PhysicsWorld() {
		m_Instance = this;
		m_WorldLevel = new b2World(m_Gravity);

		m_WorldLevel->SetContactListener(this);
		m_WorldLevel->SetContinuousPhysics(true);
		m_WorldLevel->SetAllowSleeping(false);

	}

	void PhysicsWorld::Update(float deltaTime)
	{
		m_WorldLevel->Step(deltaTime, 12, 8);

		ProcessPendingCreations();
	}

	void PhysicsWorld::QueueCreateBody(b2Body*& body, const b2BodyDef& bodyDef) {
		pendingBodyAssigns.push_back(bodyDef);
		pendingBodies.push_back(&body);
	}
	
	void PhysicsWorld::QueueFixtureCreation(b2Body*& body, const b2FixtureDef& fixtureDef, const b2PolygonShape& shapeDef) 
	{
		pendingShapeCreations.push_back(shapeDef);
		pendingFixtureAssings.push_back(&body);
		pendingFixturesCreations.push_back(fixtureDef);
	}

	void PhysicsWorld::ProcessPendingCreations() {
		for (int i = 0; i < pendingBodyAssigns.size(); ++i)
		{

			b2Body* newBody = CreateBody(pendingBodyAssigns[i]);
			*(pendingBodies[i]) = newBody;

			for (int j = 0; j < pendingFixturesCreations.size(); ++j)
			{
				if (pendingBodies[i] == pendingFixtureAssings[j])
				{
					b2FixtureDef newFix;
					newFix.shape = &pendingShapeCreations[j];
					newFix.density = pendingFixturesCreations[j].density;
					newFix.friction = pendingFixturesCreations[j].friction;
					newFix.restitution = pendingFixturesCreations[j].restitution;
					newFix.restitutionThreshold = pendingFixturesCreations[j].restitutionThreshold;

					newFix.userData.pointer = pendingFixturesCreations[j].userData.pointer;
					newFix.isSensor = pendingFixturesCreations[j].isSensor;

					newBody->CreateFixture(&newFix);
				}
			}
		}

		pendingBodyAssigns.clear();
		pendingBodies.clear();
		pendingFixturesCreations.clear();
		pendingFixtureAssings.clear();
	}

	void PhysicsWorld::DestroyBody(b2Body* body)
	{
		m_WorldLevel->DestroyBody(body);
	}

	b2Body* PhysicsWorld::CreateBody(b2BodyDef body)
	{
		LOG_CORE("Created Physics Body", LOG_INFO);
		return m_WorldLevel->CreateBody(&body);
	}

	void PhysicsWorld::SetWorld(GameWorld* world)
	{
		m_World = world;
	}

	void PhysicsWorld::BeginContact(b2Contact* contact)
	{
		Object* actorA = ((Object*)contact->GetFixtureA()->GetUserData().pointer);
		Object* actorB = ((Object*)contact->GetFixtureB()->GetUserData().pointer);


		if (actorA != nullptr && actorB != nullptr) {
			actorA->OnContactEvent(actorB);
			actorB->OnContactEvent(actorA);
		}

	}

	void PhysicsWorld::EndContact(b2Contact* contact)
	{
		Object* actorA = ((Object*)contact->GetFixtureA()->GetUserData().pointer);
		Object* actorB = ((Object*)contact->GetFixtureB()->GetUserData().pointer);

		if (actorA != nullptr && actorB != nullptr) {
			actorA->OnEndContactEvent(actorB);
			actorB->OnEndContactEvent(actorA);
		}

	}

	void PhysicsWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		
	}

	void PhysicsWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		
	}

}
