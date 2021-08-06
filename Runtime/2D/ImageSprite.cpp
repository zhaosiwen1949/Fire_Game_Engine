#include "Runtime/2D/ImageSprite.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/Scene/RendererManager.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Math/Matrix3x3.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/IO/AliceResource.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Render/GL20/ShaderProperties.h"
#include "Runtime/Render/DrawCall.h"
#include "Runtime/Render/Camera.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"

namespace Alice {
#define MarkGeometryDirty(v) v|=_00000000_00000000_00000000_00000001
#define MarkColorDirty(v) v|=_00000000_00000000_00000000_00000010
#define MarkTexcoordDirty(v) v|=_00000000_00000000_00000000_00000100

#define GeometryMask(v) (v&_00000000_00000000_00000000_00000001)
#define ColorMask(v) (v&_00000000_00000000_00000000_00000010)
#define TexcoordMask(v) (v&_00000000_00000000_00000000_00000100)
	ImageSprite::ImageSprite():mMaterial(nullptr),mVBO(4){
		mImage.Resize(64);
		mImage = "builtin/Image/Error";
		mMaterial = ExportedMaterial::Clone("builtin/Material/Texture2D");
		mMaterial->SetPrimitive(GL_TRIANGLE_STRIP);
		mWidth = 64.0f;
		mHeight = 64.0f;
		mDirtyMask = 0;
		mVertexData.SetBufferSize(4);
		mAddtionalColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		SetColor(1.0f,1.0f,1.0f,1.0f);
	}

	ImageSprite::~ImageSprite(){
		//default builtin material will not be destroied
		//release texture ref
		//release vbo
	}

	bool ImageSprite::SetMaterial(const char*matName){
		const char*guid = Resource::GetResourceGUIDViaUserName(matName);
		if (guid!=nullptr){
			Material*mat = Material::LoadMaterial(guid);
			if (mat!=nullptr){
				SetMaterial(mat);
				return true;
			}
		}
		return false;
	}

	bool ImageSprite::SetMaterial(Material*material) {
		ResetToStandardTexcoord();
		MarkColorDirty(mDirtyMask);
		mSpriteName.clear();
		mMaterial = material;
		mMaterial->SetPrimitive(GL_TRIANGLE_STRIP);
		return true;
	}
	bool ImageSprite::SetSpriteByName(const char*name){
		TextureUnitProperty*texture_unit_property = mMaterial->GetTextureProperty("U_MainTexture");
		if (texture_unit_property != nullptr) {
			Texture2D* texture = (Texture2D*)texture_unit_property->mValue.mPtr;
			Serializer::Sprite*sprite = texture->GetSprite(name);
			if (sprite!=nullptr){
				SetSprite(sprite);
				return true;
			}
		}
		return false;
	}
	void ImageSprite::SetSprite(Serializer::Sprite*sprite){
		mSpriteName = sprite->name();
		mWidth = sprite->size(0);
		mHeight = sprite->size(1);
		SetSize(sprite->size(0), sprite->size(1));
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[0].mTexCoord0.Set(sprite->texcoords(0), sprite->texcoords(1), 0, 0);
		vertexes[1].mTexCoord0.Set(sprite->texcoords(2), sprite->texcoords(1), 0, 0);
		vertexes[2].mTexCoord0.Set(sprite->texcoords(0), sprite->texcoords(3), 0, 0);
		vertexes[3].mTexCoord0.Set(sprite->texcoords(2), sprite->texcoords(3), 0, 0);
		MarkGeometryDirty(mDirtyMask);
	}

	bool ImageSprite::SetTexture(const char*uniformName, const char*image){
		Texture2D*texture = Texture2D::LoadTexture2D(image);
		if (texture!=nullptr){
			mImage = image;
			return SetTexture(uniformName,texture);
		}
		return false;
	}

	bool ImageSprite::SetTexture(const char*uniformName, Texture2D* texture){
		mMaterial.mPtr = (Material*)PrepareUpdateMaterial(mMaterial.mPtr);
		mSpriteName.clear();
		mMaterial->SetTextureProperty(uniformName, texture);
		ResetToStandardTexcoord();//maybe prev sprite is a atlas sprite,so need to reset texcoord
		mWidth = texture->mWidth;
		mHeight = texture->mHeight;
		mImage = texture->mName;
		MarkGeometryDirty(mDirtyMask);
		return true;
	}

	void ImageSprite::Update(float deltaTime){
		bool bDataChanged = false;
		bool needUpdateBoundingBox = false;
		if (mDirtyMask>0){
			if (GeometryMask(mDirtyMask) > 0) {
				bDataChanged = true;
				needUpdateBoundingBox = true;
				mbPositionChanged = false;
				UpdateVertexData();
			}
			if (ColorMask(mDirtyMask) > 0) {
				bDataChanged = true;
			}
			mDirtyMask = 0;
		}
		if (mbPositionChanged) {
			bDataChanged = true;
			needUpdateBoundingBox = true;
			mbPositionChanged = false;
			UpdateVertexData();
		}
		if (needUpdateBoundingBox) {
			UpdateBoundingBox();
		}
		if (bDataChanged){
			mVBO.SubData(mVertexData.mData, mVertexData.mBufferSize);
		}
	}

	void ImageSprite::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	)
	{
		Rect<float> view_rect;
		if (rq->mCamera->GetViewRect(view_rect)) {
			Rect<float> sprite_rect;
			GetRect(sprite_rect);
			if (false == view_rect.IsOverlappedWith(sprite_rect)) {
				return;
			}
		}
		DrawCall*newDC = new (kMemRendererId)DrawCall;
		newDC->mMaterial = mMaterial.mPtr;
		newDC->mVBO = &mVBO;
		newDC->mEBO = nullptr;
		newDC->mGameObject = mOwner;
		rq->AppendUIDrawCall(newDC);
	}
	void ImageSprite::SetColor(Color4B&color){
		SetColor(color.r, color.g, color.b, color.a);
	}
	void ImageSprite::SetColor(Color4f&color) {
		SetColor(color.v[0], color.v[1], color.v[2], color.v[3]);
	}
	void ImageSprite::SetColor(AliceUInt8 r,AliceUInt8 g,AliceUInt8 b,AliceUInt8 a){
		SetColor((float)r/255.0f,(float)g/ 255.0f,(float)b / 255.0f,(float)a/ 255.0f);
	}
	void ImageSprite::SetColor(float r, float g, float b, float a) {
		mColor.Set(ALICE_MIN(r, 1.0f), ALICE_MIN(g, 1.0f), ALICE_MIN(b, 1.0f), ALICE_MIN(a, 1.0f));
		for (int i = 0; i < 4; ++i) {
			UpdateVertexTangent(i, mColor.v[0], mColor.v[1], mColor.v[2], mColor.v[3]);
		}
		//this code will be delete in the next version
		UniformVector4Property*v4prop = mMaterial->GetVec4Property("U_Color");
		if (v4prop != nullptr) {
			mMaterial.mPtr = (Material*)PrepareUpdateMaterial(mMaterial.mPtr);
			mMaterial->SetVec4Property("U_Color", mColor.v);
		}
	}
	void ImageSprite::SetAdditionalColor(float r, float g, float b, float a) {
		mAddtionalColor.Set(ALICE_MIN(r, 1.0f), ALICE_MIN(g, 1.0f), ALICE_MIN(b, 1.0f), ALICE_MIN(a, 1.0f));
		for (int i = 0; i < 4; ++i) {
			UpdateVertexTangent(i, mColor.v[0], mColor.v[1], mColor.v[2], mColor.v[3]);
		}
	}
	void ImageSprite::SetWidth(float value){
		if (value != mWidth) {
			mWidth = value;
			MarkGeometryDirty(mDirtyMask);
		}
	}

	void ImageSprite::SetHeight(float value){
		if (value != mHeight) {
			mHeight = value;
			MarkGeometryDirty(mDirtyMask);
		}
	}

	void ImageSprite::SetSize(float w, float h) {
		if (mWidth!=w||mHeight!=h){
			mWidth = w;
			mHeight = h;
			MarkGeometryDirty(mDirtyMask);
		}
	}

	void ImageSprite::SetAlpha(AliceUInt8 alpha){
		mColor.v[3] = (float)ALICE_MIN(alpha, 255) / 255.0f;
		for (int i = 0; i < 4; ++i) {
			UpdateVertexTangent(i, mColor.v[0], mColor.v[1], mColor.v[2], mColor.v[3]);
		}
		//this code will be delete in the next version
		UniformVector4Property*v4prop = mMaterial->GetVec4Property("U_Color");
		if (v4prop != nullptr) {
			mMaterial.mPtr = (Material*)PrepareUpdateMaterial(mMaterial.mPtr);
			mMaterial->SetVec4Property("U_Color", mColor.v);
		}
	}

	Component*ImageSprite::Clone(){
		ImageSprite*is = nullptr;
		is->mMaterial = mMaterial;
		is->SetWidth(mWidth);
		is->SetHeight(mHeight);
		is->SetColor(mColor.v[0],mColor.v[1],mColor.v[2],mColor.v[3]);
		return is;
	}
	void ImageSprite::SetSprite(Serializer::ImageTile*imageTile) {
	}
	void ImageSprite::UpdateVertexData(){
		float halfWidth = mWidth / 2.0f;
		float halfHeight = mHeight / 2.0f;
		Vector3f v[4];
		const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
		v[0] = world_matrix*Vector3f(-halfWidth, -halfHeight, 0);
		v[1] = world_matrix*Vector3f(halfWidth, -halfHeight, 0);
		v[2] = world_matrix*Vector3f(-halfWidth, halfHeight, 0);
		v[3] = world_matrix*Vector3f(halfWidth, halfHeight, 0);
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[0].mVertex.Set(v[0].x, v[0].y, v[0].z);
		vertexes[1].mVertex.Set(v[1].x, v[1].y, v[1].z);
		vertexes[2].mVertex.Set(v[2].x, v[2].y, v[2].z);
		vertexes[3].mVertex.Set(v[3].x, v[3].y, v[3].z);
	}
	void ImageSprite::ResetToStandardTexcoord() {
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[0].mTexCoord0.Set(0, 0, 0, 0);
		vertexes[1].mTexCoord0.Set(1, 0, 0, 0);
		vertexes[2].mTexCoord0.Set(0, 1, 0, 0);
		vertexes[3].mTexCoord0.Set(1, 1, 0, 0);
	}

	void ImageSprite::SetTexcoordData(Vector4f*texcoords){
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		for (int i = 0; i < 4;i++){
			vertexes[i].mTexCoord0 = texcoords[i];
		}
	}
	void ImageSprite::UpdateBoundingBox(){
		VertexDataFull*vertexes = (VertexDataFull*)mVertexData.mData;
		mBox.mPoints[0].Set(vertexes[0].mVertex.x, vertexes[0].mVertex.y, vertexes[0].mVertex.z);
		mBox.mPoints[1].Set(vertexes[1].mVertex.x, vertexes[1].mVertex.y, vertexes[1].mVertex.z);
		mBox.mPoints[2].Set(vertexes[3].mVertex.x, vertexes[3].mVertex.y, vertexes[3].mVertex.z);
		mBox.mPoints[3].Set(vertexes[2].mVertex.x, vertexes[2].mVertex.y, vertexes[2].mVertex.z);
	}

	void ImageSprite::UpdateVertexNormal(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mNormal.Set(x, y, z, w);
		MarkColorDirty(mDirtyMask);
	}

	void ImageSprite::UpdateVertexTangent(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mTangent.Set(x*mAddtionalColor.v[0], y*mAddtionalColor.v[1], z*mAddtionalColor.v[2], w*mAddtionalColor.v[3]);
		MarkColorDirty(mDirtyMask);
	}

	void ImageSprite::UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mTexCoord1.Set(x, y, z, w);
		MarkColorDirty(mDirtyMask);
	}
}
