#include "RenderState.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/Render/AliceGL.h"
namespace Alice {
	RenderState GlobalRenderState::mRenderState;
	float GlobalRenderState::mClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	bool GlobalRenderState::mEnableAttributes[16];
	RenderState::RenderState() {
		mPrimitiveType = GL_TRIANGLES;
		mbBlend = false;
		mbZTest = true;
		mbAlphaTest = false;
		mZTestFunction = GL_LEQUAL;
		mbWriteR = true;
		mbWriteG = true;
		mbWriteB = true;
		mbWriteZ = true;
		mbWriteStencil = true;
		mbCull = true;
		mbWriteStencil = false;
		mCullFace = GL_BACK;
		mbEnablePointSprite = true;
		mAlphaTestFunction = GL_GREATER;
		mAlphaTestValue = 0.0f;
		mOffsetFactor = 0.0f;
		mOffsetUnit = 0.0f;
		mSRCBlendFunc = GL_SRC_ALPHA;
		mDSTBlendFunc = GL_ONE_MINUS_SRC_ALPHA;
		mClearStencilValue = -1;
#ifdef ALICE_IPHONE
#elif ALICE_ANDROID
#else
		mPolygonMode = GL_FILL;
#endif
	}
	void RenderState::operator=(const RenderState&rs) {
		memcpy(this, &rs, sizeof(RenderState));
	}
	void GlobalRenderState::Init() {
		OGL_CALL(glDisable(GL_BLEND));
		OGL_CALL(glBlendFunc(mRenderState.mSRCBlendFunc,mRenderState.mDSTBlendFunc));
#if ALICE_WIN_PLAYER | ALICE_OSX_PLAYER
		OGL_CALL(glDisable(GL_ALPHA_TEST));
#endif
		OGL_CALL(glEnable(GL_DEPTH_TEST));
		OGL_CALL(glDepthFunc(GL_LEQUAL));
		OGL_CALL(glDepthMask(true));
		OGL_CALL(glColorMask(true,true,true,true));
		OGL_CALL(glStencilMask(true));
		OGL_CALL(glEnable(GL_CULL_FACE));
		OGL_CALL(glCullFace(mRenderState.mCullFace));
		OGL_CALL(glPolygonOffset(mRenderState.mOffsetFactor,mRenderState.mOffsetUnit));
#if ALICE_WIN_PLAYER
		OGL_CALL(glEnable(GL_POINT_SPRITE));
		OGL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
#elif ALICE_OSX_PLAYER
		OGL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
#else //mobile don't need to enable anything
#endif
#ifdef ALICE_IPHONE
#elif ALICE_ANDROID
#else
		glPolygonMode(GL_FRONT_AND_BACK, mRenderState.mPolygonMode);
#endif
		for (int i = 0; i < 16; ++i) {
			mEnableAttributes[i] = false;
		}
	}
	void GlobalRenderState::BlendFunc(unsigned int src, unsigned int dst) {
		if (mRenderState.mSRCBlendFunc!=src|| mRenderState.mDSTBlendFunc!=dst){
			mRenderState.mSRCBlendFunc = src;
			mRenderState.mDSTBlendFunc = dst;
			OGL_CALL(glBlendFunc(mRenderState.mSRCBlendFunc, mRenderState.mDSTBlendFunc));
		}
	}
	void GlobalRenderState::EnableBlend(bool enable) {
		if (mRenderState.mbBlend!=enable){
			mRenderState.mbBlend = enable;
			if (mRenderState.mbBlend){
				OGL_CALL(glEnable(GL_BLEND));
			}
			else {
				OGL_CALL(glDisable(GL_BLEND));
			}
		}
	}
	void GlobalRenderState::EnableDepthTest(bool enable) {
		if (mRenderState.mbZTest != enable) {
			mRenderState.mbZTest = enable;
			if (mRenderState.mbZTest) {
				OGL_CALL(glEnable(GL_DEPTH_TEST));
			}
			else {
				OGL_CALL(glDisable(GL_DEPTH_TEST));
			}
		}
	}
	void GlobalRenderState::EnablePointSprite(bool enable) {
		if (mRenderState.mbEnablePointSprite != enable) {
			mRenderState.mbEnablePointSprite = enable;
			if (mRenderState.mbEnablePointSprite) {
#if ALICE_WIN_PLAYER
				OGL_CALL(glEnable(GL_POINT_SPRITE));
				OGL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
#elif ALICE_OSX_PLAYER
				OGL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
#endif
			}
			else {
#if ALICE_WIN_PLAYER
				OGL_CALL(glDisable(GL_POINT_SPRITE));
				OGL_CALL(glDisable(GL_PROGRAM_POINT_SIZE));
#elif ALICE_OSX_PLAYER
				OGL_CALL(glDisable(GL_PROGRAM_POINT_SIZE));
#endif
			}
		}
	}
	void GlobalRenderState::EnableRenderTwoSide(bool enable) {
		if (mRenderState.mbCull != enable) {
			mRenderState.mbCull = enable;
			if (mRenderState.mbCull) {
				OGL_CALL(glDisable(GL_CULL_FACE));
			}
			else {
				OGL_CALL(glEnable(GL_CULL_FACE));
			}
		}
	}
	void GlobalRenderState::EnableCull(bool enable) {
		if (mRenderState.mbCull != enable) {
			mRenderState.mbCull = enable;
			if (mRenderState.mbCull) {
				OGL_CALL(glEnable(GL_CULL_FACE));
			}
			else {
				OGL_CALL(glDisable(GL_CULL_FACE));
			}
		}
	}
	void GlobalRenderState::CullFace(int face) {
		if (mRenderState.mCullFace != face) {
			mRenderState.mCullFace = face;
			OGL_CALL(glCullFace(mRenderState.mCullFace));
		}
	}
	void GlobalRenderState::PolygonMode(int mode) {
		if (mRenderState.mPolygonMode != mode) {
			mRenderState.mPolygonMode = mode;
#ifdef ALICE_IPHONE
#elif ALICE_ANDROID
#else
			glPolygonMode(GL_FRONT_AND_BACK, mRenderState.mPolygonMode);
#endif
		}
	}
	bool GlobalRenderState::EnableZWrite(bool enable) {
		bool original = mRenderState.mbWriteZ;
		if (mRenderState.mbWriteZ != enable) {
			mRenderState.mbWriteZ = enable;
			OGL_CALL(glDepthMask(mRenderState.mbWriteZ));
		}
		return original;
	}
	void GlobalRenderState::ColorMask(bool r,bool g,bool b,bool a){
		if (mRenderState.mbWriteR != r || mRenderState.mbWriteG!=g || mRenderState.mbWriteB!=b || mRenderState.mbWriteA!=a) {
			mRenderState.mbWriteR = r;
			mRenderState.mbWriteG = g;
			mRenderState.mbWriteB = b;
			mRenderState.mbWriteA = a;
			OGL_CALL(glColorMask(r,g,b,a));
		}
	}
	void GlobalRenderState::StencilMask(bool write) {
		if (mRenderState.mbWriteStencil != write) {
			mRenderState.mbWriteStencil = write;
			OGL_CALL(glStencilMask(write));
		}
	}
	void GlobalRenderState::ZTest(int func) {
		if (mRenderState.mZTestFunction != func) {
			mRenderState.mZTestFunction = func;
			OGL_CALL(glDepthFunc(mRenderState.mZTestFunction));
		}
	}
	void GlobalRenderState::ZOffset(float factor, float unit) {
		if (mRenderState.mOffsetFactor != factor || mRenderState.mOffsetUnit != unit) {
			mRenderState.mOffsetFactor = factor;
			mRenderState.mOffsetUnit = unit;
			OGL_CALL(glPolygonOffset(mRenderState.mOffsetFactor, mRenderState.mOffsetUnit));
		}
	}
	void GlobalRenderState::AlphaTest(int func, float v) {
		if (mRenderState.mAlphaTestFunction!=func||mRenderState.mAlphaTestValue!=v){
			mRenderState.mAlphaTestFunction = func;
			mRenderState.mAlphaTestValue = v;
#if ALICE_WIN_PLAYER | ALICE_OSX_PLAYER
			OGL_CALL(glAlphaFunc(func, v));
#endif
		}
	}
	bool GlobalRenderState::EnableAlphaTest(bool enable) {
		bool old = mRenderState.mbAlphaTest;
		if (mRenderState.mbAlphaTest!=enable){
			mRenderState.mbAlphaTest = enable;
#if ALICE_WIN_PLAYER | ALICE_OSX_PLAYER
			if (enable){
				OGL_CALL(glEnable(GL_ALPHA_TEST));
			}
			else {
				OGL_CALL(glDisable(GL_ALPHA_TEST));
			}
#endif
		}
		return old;
	}
	void GlobalRenderState::ClearColor(float r, float g, float b, float a, int color_buffer_index) {
		if (mClearColor[0]!=r||mClearColor[1]!=g||mClearColor[2]!=b||mClearColor[3]!=a){
			mClearColor[0] = r;
			mClearColor[1] = g;
			mClearColor[2] = b;
			mClearColor[3] = a;
			OGL_CALL(glClearBufferfv(GL_COLOR, color_buffer_index,mClearColor));
		}
	}
	void GlobalRenderState::ClearStencil(int value) {
		if (mRenderState.mClearStencilValue != value) {
			mRenderState.mClearStencilValue = value;
			OGL_CALL(glClearStencil(mRenderState.mClearStencilValue));
		}
	}
	void GlobalRenderState::EnableAttribute(int index, bool enable) {
		if (mEnableAttributes[index] != enable) {
			mEnableAttributes[index] = enable;
			if (enable) {
				OGL_CALL(glEnableVertexAttribArray(index));
			}
			else {
				OGL_CALL(glDisableVertexAttribArray(index));
			}
		}
	}
	void GlobalRenderState::EnableTextureUnit(int index, bool enable) {
		OGL_CALL(glActiveTexture(GL_TEXTURE0 + index));
	}
	void GlobalRenderState::BindTexture(int texture_type, unsigned int texture) {
		OGL_CALL(glBindTexture(texture_type, texture));
	}
}