#pragma once
#include "Runtime/Render/RenderPass.h"
namespace Alice{
	class ExportedRenderPass :public RenderPass {
	public:
		static int Export(lua_State*L);
	public:
		DEFINE_LUA_API(New);
		DEFINE_LUA_API(SetType);
		DEFINE_LUA_API(EnableBlend);
		DEFINE_LUA_API(EnableZWrite);
		DEFINE_LUA_API(EnableZTest);
		DEFINE_LUA_API(EnableAlphaTest);
		DEFINE_LUA_API(EnablePointSprite);
		DEFINE_LUA_API(BlendFunc);
		DEFINE_LUA_API(AlphaTest);
		DEFINE_LUA_API(ZTest);
		DEFINE_LUA_API(Offset);
		DEFINE_LUA_API(EnableCull);
		DEFINE_LUA_API(Cull);
		DEFINE_LUA_API(WriteStencilMask);
		DEFINE_LUA_API(UseStencilMask);
		DEFINE_LUA_API(SetFloat);
		DEFINE_LUA_API(SetVec2);
		DEFINE_LUA_API(SetVec3);
		DEFINE_LUA_API(SetVec4);
		DEFINE_LUA_API(SetMat3);
		DEFINE_LUA_API(SetMat4);
		DEFINE_LUA_API(SetTexture2D);
		DEFINE_LUA_API(GetTexture2D);
		DEFINE_LUA_API(SetTextureCube);
		DEFINE_LUA_API(GetTextureCube);
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(ExportedRenderPass, RenderPass)
	};
}