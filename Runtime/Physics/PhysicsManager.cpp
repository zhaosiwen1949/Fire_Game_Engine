#include "PhysicsManager.h"
#include "Physics2DComponent.h"
#include "Physics3DComponent.h"
#include "Runtime/Scene/GameObject.h"
#include <unordered_map>

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxBroadPhase.h"
#pragma comment(lib,"PxFoundation_x86.lib")
#pragma comment(lib,"PhysX3_x86.lib")
#pragma comment(lib,"PhysX3Common_x86.lib")
#pragma comment(lib,"PhysX3Extensions.lib")
#pragma comment(lib,"PhysX3Cooking_x86.lib")
#if NDEBUG
#pragma comment(lib,"LowLevel.lib")
#pragma comment(lib,"LowLevelAABB.lib")
#pragma comment(lib,"LowLevelCloth.lib")
#pragma comment(lib,"LowLevelDynamics.lib")
#pragma comment(lib,"LowLevelParticles.lib")
#pragma comment(lib,"PxTask_x86.lib")
#pragma comment(lib,"PsFastXml_x86.lib")
#pragma comment(lib,"PhysX3Vehicle.lib")
#pragma comment(lib,"SceneQuery.lib")
#pragma comment(lib,"SimulationController.lib")
#endif
#if ALICE_PLATFORM_WIN && _DEBUG
#define NDEBUG 1
#endif
#if NDEBUG && ALICE_PLATFORM_WIN && _DEBUG
#pragma comment(lib,"PxPvdSDK_x86.lib")
physx::PxPvd*  pvd = nullptr;
physx::PxPvdTransport* transport = nullptr;
#endif
static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;
static Alice::PhysicsManager sPhysicsManager;
static std::unordered_map<AliceUInt32, AliceUInt32> sPhysicsMasks;
namespace Alice{
	physx::PxPhysics*PhysicsManager::mPhysxEngine = nullptr;
	physx::PxFoundation *PhysicsManager::mPhysxEngineFoundation = nullptr;
	physx::PxCooking *PhysicsManager::mPhysxEngineCooking = nullptr;
	physx::PxScene*PhysicsManager::mPhysxScence = nullptr;
	float PhysicsManager::mAccTimeEscapted = 0.0f;
	float PhysicsManager::mFixedUpdateTime = 0.0f;
	bool PhysicsManager::FixedUpdate(float deltaTime){
		mAccTimeEscapted += deltaTime;
		if (mAccTimeEscapted>=mFixedUpdateTime&&mAccTimeEscapted>0.0f){
			//process physics world
			m2DPhysicWorld->Step(mAccTimeEscapted, 8, 3);
			//change world object pos by physics world
			if (mPhysxScence) {
				mPhysxScence->simulate(mAccTimeEscapted);
				mPhysxScence->fetchResults(true);
				// retrieve array of actors that moved
				physx::PxU32 nbActiveActors;
				physx::PxActor** activeActors = mPhysxScence->getActiveActors(nbActiveActors);
				// update each render object with the new transform
				for (physx::PxU32 i = 0; i < nbActiveActors; ++i) {
					if (activeActors[i]->userData != nullptr) {
						PhysicsComponent* component = static_cast<PhysicsComponent*>(activeActors[i]->userData);
						component->SyncDataToLogical();
					}
				}
			}
			mAccTimeEscapted = 0.0f;
			return true;
		}
		return false;
	}

	Physics2DComponent*PhysicsManager::CreatePhysics2DComponent(){
		/*lua_State*L = LuaEngine::Store();
		ExportedPhysics2DComponent::New_ExportedAliceType(L);
		Physics2DComponent*p2d = TO_USERDATA(L, ExportedPhysics2DComponent, -1);
		LuaEngine::Restore();

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 0.0f);
		bodyDef.userData = p2d;
		p2d->mBody = sPhysicsManager.m2DPhysicWorld->CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(0.5f, 0.5f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		p2d->mBody->CreateFixture(&fixtureDef);

		return p2d;*/
		return nullptr;
	}
	physx::PxFilterFlags filter(physx::PxFilterObjectAttributes	attributes0,
		physx::PxFilterData				filterData0,
		physx::PxFilterObjectAttributes	attributes1,
		physx::PxFilterData				filterData1,
		physx::PxPairFlags&				pairFlags,
		const void*					constantBlock,
		physx::PxU32						constantBlockSize)
	{
		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

		return physx::PxFilterFlags();
	}
	void PhysicsManager::Init(void*data){
		if (data!=nullptr){
		}
		b2Vec2 gravity(0.0f,-9.8f*0.2f);
		m2DPhysicWorld = new b2World(gravity);
		m2DPhysicWorld->SetContactListener(this);
		//init physx
		mPhysxEngineFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback,
			gDefaultErrorCallback);
		if (!mPhysxEngineFoundation)
			Error("PxCreateFoundation failed!");
#if NDEBUG && ALICE_PLATFORM_WIN && _DEBUG
		pvd = PxCreatePvd(*mPhysxEngineFoundation);
		transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif
		mPhysxEngine = PxCreatePhysics(PX_PHYSICS_VERSION, *mPhysxEngineFoundation,
			physx::PxTolerancesScale()
#if NDEBUG && ALICE_PLATFORM_WIN && _DEBUG
			,true,pvd
#endif
		);
		if (!mPhysxEngine)
			Error("PxCreatePhysics failed!");
		physx::PxSceneDesc description(mPhysxEngine->getTolerancesScale());
		description.gravity = physx::PxVec3(0.0, -9.81f, 0.0f);
		description.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		description.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
		description.filterShader = filter;
;		mPhysxScence = mPhysxEngine->createScene(description);
		if (!mPhysxScence){
			Error("create phsx 3d scene failed!");
		}
		mPhysxEngineCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mPhysxEngineFoundation, physx::PxCookingParams(mPhysxEngine->getTolerancesScale()));
		if (!mPhysxEngineCooking)
			Error("PxCreateCooking failed!");
		Physics3DComponent::mDefaultMaterial = mPhysxEngine->createMaterial(0.0f, 0.0f, 0.0f);
	}

	void PhysicsManager::ShutDown() {
		mPhysxEngine->release();
#if NDEBUG && ALICE_PLATFORM_WIN && _DEBUG
		pvd->release();
		transport->release();
#endif
        mPhysxEngineFoundation->release();
	}

	void PhysicsManager::AddPhysicsMask(AliceUInt32 layerID, AliceUInt32 mask){
		auto iter = sPhysicsMasks.find(layerID);
		if (iter == sPhysicsMasks.end()){
			sPhysicsMasks.insert(std::pair<AliceUInt32, AliceUInt32>(layerID, mask));
		}else{
			iter->second = mask;
		}
	}
	void PhysicsManager::DeletePhysicsMask(AliceUInt32 layerID){
		auto iter = sPhysicsMasks.find(layerID);
		if (iter !=sPhysicsMasks.end()){
			sPhysicsMasks.erase(iter);
		}
	}
	AliceUInt16 PhysicsManager::GetMaskByLayerID(AliceUInt32 layerID){
		auto iter = sPhysicsMasks.find(layerID);
		if (iter!=sPhysicsMasks.end()){
			return (AliceUInt16)iter->second;
		}
		return 0xFFFF;
	}
	void PhysicsManager::ClearMasks(){
		sPhysicsMasks.clear();
	}
	void PhysicsManager::BeginContact(b2Contact* contact){
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();
		Physics2DComponent*p2dA = (Physics2DComponent*)a->GetBody()->GetUserData();
		Physics2DComponent*p2dB = (Physics2DComponent*)b->GetBody()->GetUserData();
		p2dA->OnCollideBegin(p2dB);
		p2dB->OnCollideBegin(p2dA);
	}
	void PhysicsManager::EndContact(b2Contact* contact){
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();
		Physics2DComponent*p2dA = (Physics2DComponent*)a->GetBody()->GetUserData();
		Physics2DComponent*p2dB = (Physics2DComponent*)b->GetBody()->GetUserData();
		p2dA->OnCollideEnd(p2dB);
		p2dB->OnCollideEnd(p2dA);
	}
}
Alice::PhysicsManager&GetPhysicsManager(){
	return sPhysicsManager;
}
