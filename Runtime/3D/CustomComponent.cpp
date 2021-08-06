#include "CustomComponent.h"
#include "Runtime/Render/DrawCall.h"
#include "Runtime/Render/Material.h"

namespace Alice
{

	void CustomComponent::Update(float deltaTime)
	{
		if (mUpdater!=LUA_REFNIL)
		{
			lua_State*L = LuaEngine::Store();
			lua_getref(L, mUpdater);
			if (lua_iscfunction(L,-1))
			{
				lua_pushnumber(L, deltaTime);
				if (lua_pcall(L,1,0,0)!=0)
				{
					errorC("CustomComponent update fail : %s",lua_tostring(L,-1));
				}
			}
			LuaEngine::Restore();
		}
	}

	void CustomComponent::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	)
	{
		DrawCall*newDC = new (kMemRendererId)DrawCall;
		newDC->mMaterial = mMaterial.mPtr;
		newDC->mCustomComponent = this;
		if (mMaterial->mRenderingQueue < kRenderingQueueTransparent) {
			rq->AppendOpaqueDrawCall(newDC);
		}
		else {
			rq->AppendTransparentDrawCall(newDC);
		}
	}

	void CustomComponent::Rendering(Camera*camera)
	{
		if (mRenderer != LUA_REFNIL)
		{
			lua_State*L = LuaEngine::Store();
			lua_getref(L, mRenderer);
			if (lua_iscfunction(L, -1))
			{
				lua_getref(L, mRenderingParam);
				RenderingParam*rp = (RenderingParam*)lua_touserdata(L,-1);
				rp->mProjectionMatrix = camera->mProjectionMatrix.mData;
				rp->mViewMatrix = camera->mViewMatrix.mData;
				if (lua_pcall(L, 1, 0, 0) != 0)
				{
					errorC("CustomComponent Rendering fail : %s", lua_tostring(L, -1));
				}
			}
			LuaEngine::Restore();
		}
	}

	CustomComponent::CustomComponent() :mUpdater(LUA_REFNIL), mRenderer(LUA_REFNIL),mMaterial(nullptr)
	{
		mMaterial = new (kMemMaterialId)Material;
		lua_newuserdata(LuaEngine::s_GlobalStatePtr, sizeof(RenderingParam));
		mRenderingParam=luaL_ref(LuaEngine::s_GlobalStatePtr,LUA_REGISTRYINDEX);
	}

	CustomComponent::~CustomComponent()
	{
		if (mUpdater!=LUA_REFNIL)
		{
			lua_unref(LuaEngine::s_GlobalStatePtr, mUpdater);
			mUpdater = LUA_REFNIL;
		}
		if (mRenderer!=LUA_REFNIL)
		{
			lua_unref(LuaEngine::s_GlobalStatePtr, mRenderer);
			mRenderer = LUA_REFNIL;
		}
		lua_unref(LuaEngine::s_GlobalStatePtr, mRenderingParam);
		mRenderingParam = LUA_REFNIL;
	}
}