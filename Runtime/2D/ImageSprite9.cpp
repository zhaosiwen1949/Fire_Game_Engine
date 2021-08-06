#include "Runtime/2D/ImageSprite9.h"
#include "Runtime/Scene/RendererManager.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Render/RenderPass.h"
#include "Runtime/Render/DrawCall.h"
#include "Runtime/Render/Camera.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"

namespace Alice
{
#define MarkGeometryDirty(v) v|=_00000000_00000000_00000000_00000001
#define MarkColorDirty(v) v|=_00000000_00000000_00000000_00000010
#define MarkTexcoordDirty(v) v|=_00000000_00000000_00000000_00000100

#define GeometryMask(v) (v&_00000000_00000000_00000000_00000001)
#define ColorMask(v) (v&_00000000_00000000_00000000_00000010)
#define TexcoordMask(v) (v&_00000000_00000000_00000000_00000100)
	ImageSprite9::ImageSprite9() :
		mMaterial(nullptr),
		mbRotateTexcoord(false),
		mDirtyMask(0),
		mEBO(54, GL_STATIC_DRAW),
		mVBO(16) {
		static AliceUInt16 indexes[] ={
			0,1,5,0,5,4,//第一个框框
			1,2,6,1,6,5,//第二个框框
			2,3,7,2,7,6,//第三个框框
			4,5,9,4,9,8,//第四个框框
			5,6,10,5,10,9,//第五个框框
			6,7,11,6,11,10,//第六个框框
			8,9,13,8,13,12,//第七个框框
			9,10,14,9,14,13,//第八个框框
			10,11,15,10,15,14,//第九个框框
		};
		mImage.Resize(64);
		mImage="builtin/Image/Error";
		mVertexData.SetBufferSize(16);
		mEBO.SubData(indexes, sizeof(AliceUInt16) * 54);
		mMaterial = ExportedMaterial::Clone("builtin/Material/Texture2D");
		//初始化九宫格，上下左右边距分别是10%
		mLeftMargin = 0.0f;
		mRightMargin = 0.0f;
		mTopMargin = 0.0f;
		mBottomMargin = 0.0f;
		mWidth = 64;
		mHeight = 64;
		mOriginalWidth = 64;
		mOriginalHeight = 64;
		mSpriteTexcoordU.Set(0, 1);
		mSpriteTexcoordV.Set(0, 1);
		mAddtionalColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void ImageSprite9::SetTexture(const char*uniformName, Texture2D*texture) {
		TextureUnitProperty*texture_unit_property = mMaterial->GetTextureProperty(uniformName);
		if (texture_unit_property == nullptr) {//no texture2d sampler property
			return;
		}
		if (texture_unit_property->mValue != nullptr&&texture_unit_property->mValue->mName == texture->mName) {
			UpdateByTexture(texture);
		}
		else {
			mMaterial.mPtr = (Material*)PrepareUpdateMaterial(mMaterial.mPtr);
			mMaterial->SetTextureProperty(uniformName, texture);
			UpdateByTexture(texture);
		}
	}
	void ImageSprite9::SetTexture(const char*uniformName, const char*path) {
		TextureUnitProperty*texture_unit_property = mMaterial->GetTextureProperty(uniformName);
		Texture2D*texture = nullptr;
		if (texture_unit_property == nullptr) {//no texture2d sampler property
			return;
		}
		if (false==(texture_unit_property->mValue != nullptr&&texture_unit_property->mValue->mName == path)){
			texture = Texture2D::LoadTexture2D(path);
		}
		if (texture == nullptr){//new image is the same with exist one
			UpdateByTexture((Texture2D*)texture_unit_property->mValue.mPtr);
		} else {//need to update material property
			mMaterial.mPtr = (Material*)PrepareUpdateMaterial(mMaterial.mPtr);
			mMaterial->SetTextureProperty(uniformName, texture);
			UpdateByTexture(texture);
		}
	}
	void ImageSprite9::UpdateByTexture(Texture2D*texture) {
		mWidth = texture->mWidth;
		mHeight = texture->mHeight;
		mOriginalWidth = mWidth;
		mOriginalHeight = mHeight;

		mSpriteName.clear();
		mSpriteTexcoordU.Set(0, 1);
		mSpriteTexcoordV.Set(0, 1);
		mImage = texture->mName;
		MarkGeometryDirty(mDirtyMask);
		MarkTexcoordDirty(mDirtyMask);
	}
	void ImageSprite9::SetColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a){
		SetColor((float)r/ 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
	}
	void ImageSprite9::SetColor(float r, float g, float b, float a) {
		mColor.Set(ALICE_MIN(r, 1.0f), ALICE_MIN(g, 1.0f), ALICE_MIN(b, 1.0f), ALICE_MIN(a, 1.0f));
		for (int i=0;i<16;++i){
			UpdateVertexTangent(i, mColor.v[0], mColor.v[1], mColor.v[2], mColor.v[3]);
		}
		//this code will be delete in the next version
		UniformVector4Property*v4prop = mMaterial->GetVec4Property("U_Color");
		if (v4prop != nullptr) {
			mMaterial.mPtr = (Material*)PrepareUpdateMaterial(mMaterial.mPtr);
			mMaterial->SetVec4Property("U_Color", mColor.v);
		}
	}
	void ImageSprite9::SetAdditionalColor(float r, float g, float b, float a) {
		mAddtionalColor.Set(ALICE_MIN(r, 1.0f), ALICE_MIN(g, 1.0f), ALICE_MIN(b, 1.0f), ALICE_MIN(a, 1.0f));
		for (int i = 0; i < 16; ++i) {
			UpdateVertexTangent(i, mColor.v[0], mColor.v[1], mColor.v[2], mColor.v[3]);
		}
	}
	void ImageSprite9::SetAlpha(AliceUInt8 alpha){
		mColor.v[3] = (float)ALICE_MIN(alpha, 255) / 255.0f;
		for (int i = 0; i < 16; ++i) {
			UpdateVertexTangent(i, mColor.v[0], mColor.v[1], mColor.v[2], mColor.v[3]);
		}
		//this code will be delete in the next version
		UniformVector4Property*v4prop = mMaterial->GetVec4Property("U_Color");
		if (v4prop != nullptr) {
			mMaterial.mPtr = (Material*)PrepareUpdateMaterial(mMaterial.mPtr);
			mMaterial->SetVec4Property("U_Color", mColor.v);
		}
	}
	void ImageSprite9::SetAtlas(std::string atlasName){
	}
	void ImageSprite9::SetOriginalSize(float width, float height){
		mOriginalWidth = width;
		mOriginalHeight = height;
	}
	void ImageSprite9::SetWidth(float value){
		if (value != mWidth) {
			mWidth = value;
			MarkGeometryDirty(mDirtyMask);
		}
	}

	void ImageSprite9::SetHeight(float value) {
		if (value != mHeight) {
			mHeight = value;
			MarkGeometryDirty(mDirtyMask);
		}
	}

	void ImageSprite9::SetSize(float w, float h) {
		if (mWidth != w || mHeight != h) {
			mWidth = w;
			mHeight = h;
			MarkGeometryDirty(mDirtyMask);
		}
	}
	bool ImageSprite9::SetSpriteByName(const char*name){
		TextureUnitProperty*texture_unit_property = mMaterial->GetTextureProperty("U_MainTexture");
		if (texture_unit_property != nullptr) {
			Texture2D* texture = (Texture2D*)texture_unit_property->mValue.mPtr;
			Serializer::Sprite*sprite = texture->GetSprite(name);
			if (sprite != nullptr) {
				SetSprite(sprite);
				return true;
			}
		}
		return false;
	}
	void ImageSprite9::BlendFunc(GLenum src, GLenum dst) {
	}

	void ImageSprite9::SetSprite(Serializer::Sprite*sprite){
		mSpriteName = sprite->name();
		mOriginalWidth = sprite->size(0);
		mOriginalHeight = sprite->size(1);
		mSpriteTexcoordU.Set(sprite->texcoords(0), sprite->texcoords(2));
		mSpriteTexcoordV.Set(sprite->texcoords(1), sprite->texcoords(3));
		mbRotateTexcoord = sprite->rotate();
		if (mWidth==0.0f||mHeight==0.0f){
			mWidth = mOriginalWidth;
			mHeight = mOriginalHeight;
		}
		if (sprite->split_size() > 0) {
			SetSplit(sprite->split(0), sprite->split(1), sprite->split(2), sprite->split(3));
		}
		MarkGeometryDirty(mDirtyMask);
		MarkTexcoordDirty(mDirtyMask);
	}
	void ImageSprite9::SetSplit(float top, float right, float bottom, float left) {
		mLeftMargin = left;
		mRightMargin = right;
		mTopMargin = top;
		mBottomMargin = bottom;
		MarkTexcoordDirty(mDirtyMask);
	}

	bool ImageSprite9::SetMaterial(const char*matName)
	{
		const char*guid = Resource::GetResourceGUIDViaUserName(matName);
		if (guid != nullptr)
		{
			Material*mat = Material::LoadMaterial(guid);
			if (mat != nullptr)
			{
				SetMaterial(mat);
				return true;
			}
		}
		return false;
	}

	bool ImageSprite9::SetMaterial(Material*material)
	{
		mMaterial = material;
		mSpriteTexcoordU.Set(0, 1);
		mSpriteTexcoordV.Set(0, 1);
		MarkGeometryDirty(mDirtyMask);
		MarkTexcoordDirty(mDirtyMask);
		return true;
	}

	void ImageSprite9::UpdateVertexData() {
		VertexDataFull*vertexDatas = mVertexData.GetBuffer<VertexDataFull>();
		if (mWidth==0.0f||mHeight==0.0f){
			for (int i = 0; i < 16; ++i) {
				vertexDatas[i].mVertex.Set(0.0f, 0.0f, 0.0f);
			}
			return;
		}
		float halfWidth = mWidth / 2.0f;
		float halfHeight = mHeight / 2.0f;
		//从左往右，从下往上生成所有定点信息
		//下面依次是所有点的坐标初始化
		//  12---13----14----15
		//  |     |     |     |
		//  |	  |		|	  |
		//  8-----9----10-----11
		//  |     |     |     |
		//  |	  |		|	  |
		//  4-----5-----6-----7
		//  |     |     |     |
		//  |     |		|	  |
		//  0-----1-----2-----3
		const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
		Vector3f vertexes[16];
		//第一排
		vertexes[0] = world_matrix * Vector3f(-halfWidth, -halfHeight, 0);
		vertexes[1] = world_matrix * Vector3f(-halfWidth + mLeftMargin, -halfHeight, 0);
		vertexes[2] = world_matrix * Vector3f(halfWidth - mRightMargin, -halfHeight, 0);
		vertexes[3] = world_matrix * Vector3f(halfWidth, -halfHeight, 0);
		//第二排
		vertexes[4] = world_matrix * Vector3f(-halfWidth, -halfHeight + mBottomMargin, 0);
		vertexes[5] = world_matrix * Vector3f(-halfWidth + mLeftMargin, -halfHeight + mBottomMargin, 0);
		vertexes[6] = world_matrix * Vector3f(halfWidth - mRightMargin, -halfHeight + mBottomMargin, 0);
		vertexes[7] = world_matrix * Vector3f(halfWidth, -halfHeight + mBottomMargin, 0);
		//第三排
		vertexes[8] = world_matrix * Vector3f(-halfWidth, halfHeight - mTopMargin, 0);
		vertexes[9] = world_matrix * Vector3f(-halfWidth + mLeftMargin, halfHeight - mTopMargin, 0);
		vertexes[10] = world_matrix*Vector3f(halfWidth - mRightMargin, halfHeight - mTopMargin, 0);
		vertexes[11] = world_matrix*Vector3f(halfWidth, halfHeight - mTopMargin, 0);
		//第四排
		vertexes[12] = world_matrix * Vector3f(-halfWidth, halfHeight, 0);
		vertexes[13] = world_matrix * Vector3f(-halfWidth + mLeftMargin, halfHeight, 0);
		vertexes[14] = world_matrix * Vector3f(halfWidth - mRightMargin, halfHeight, 0);
		vertexes[15] = world_matrix * Vector3f(halfWidth, halfHeight, 0);

		for (int i = 0; i < 16; ++i) {
			vertexDatas[i].mVertex.Set(vertexes[i].v[0], vertexes[i].v[1], vertexes[i].v[2]);
		}
	}
	void ImageSprite9::UpdateTexcoordData(){
		if (mbRotateTexcoord==false){
			float left = mLeftMargin;
			float right = mRightMargin;
			VertexDataFull*vertexDatas = mVertexData.GetBuffer<VertexDataFull>();
			//第一排
			float uRange = mSpriteTexcoordU.y - mSpriteTexcoordU.x;
			float vRange = mSpriteTexcoordV.y - mSpriteTexcoordV.x;
			float texcoordc2 = mSpriteTexcoordU.x + uRange * (left / mOriginalWidth);
			float texcoordc3 = mSpriteTexcoordU.x + uRange * (mOriginalWidth - right) / mOriginalWidth;
			float texcoordr2 = mSpriteTexcoordV.x + vRange * (mBottomMargin / mOriginalHeight);
			float texcoordr3 = mSpriteTexcoordV.x + vRange * ((mOriginalHeight - mTopMargin) / mOriginalHeight);

			vertexDatas[0].mTexCoord0.Set(mSpriteTexcoordU.x, mSpriteTexcoordV.x, 0, 0);
			vertexDatas[1].mTexCoord0.Set(texcoordc2, mSpriteTexcoordV.x, 0, 0);
			vertexDatas[2].mTexCoord0.Set(texcoordc3, mSpriteTexcoordV.x, 0, 0);
			vertexDatas[3].mTexCoord0.Set(mSpriteTexcoordU.y, mSpriteTexcoordV.x, 0, 0);
			//第二排
			vertexDatas[4].mTexCoord0.Set(mSpriteTexcoordU.x, texcoordr2, 0, 0);
			vertexDatas[5].mTexCoord0.Set(texcoordc2, texcoordr2, 0, 0);
			vertexDatas[6].mTexCoord0.Set(texcoordc3, texcoordr2, 0, 0);
			vertexDatas[7].mTexCoord0.Set(mSpriteTexcoordU.y, texcoordr2, 0, 0);
			//第三排
			vertexDatas[8].mTexCoord0.Set(mSpriteTexcoordU.x, texcoordr3, 0, 0);
			vertexDatas[9].mTexCoord0.Set(texcoordc2, texcoordr3, 0, 0);
			vertexDatas[10].mTexCoord0.Set(texcoordc3, texcoordr3, 0, 0);
			vertexDatas[11].mTexCoord0.Set(mSpriteTexcoordU.y, texcoordr3, 0, 0);
			//第四排
			vertexDatas[12].mTexCoord0.Set(mSpriteTexcoordU.x, mSpriteTexcoordV.y, 0, 0);
			vertexDatas[13].mTexCoord0.Set(texcoordc2, mSpriteTexcoordV.y, 0, 0);
			vertexDatas[14].mTexCoord0.Set(texcoordc3, mSpriteTexcoordV.y, 0, 0);
			vertexDatas[15].mTexCoord0.Set(mSpriteTexcoordU.y, mSpriteTexcoordV.y, 0, 0);
		}
		else {
			float left = mLeftMargin;
			float right = mRightMargin;
			VertexDataFull*vertexDatas = mVertexData.GetBuffer<VertexDataFull>();
			//第一排
			float uRange = mSpriteTexcoordU.y - mSpriteTexcoordU.x;//delta x
			float vRange = mSpriteTexcoordV.y - mSpriteTexcoordV.x;//delta y
			float texcoordc2 = mSpriteTexcoordU.x + uRange * (mBottomMargin / mOriginalHeight);
			float texcoordc3 = mSpriteTexcoordU.x + uRange * (mOriginalHeight - mTopMargin) / mOriginalHeight;
			float texcoordr2 = mSpriteTexcoordV.x + vRange * (mLeftMargin / mOriginalWidth);
			float texcoordr3 = mSpriteTexcoordV.x + vRange * ((mOriginalWidth - mRightMargin) / mOriginalWidth);
			vertexDatas[0].mTexCoord0.Set(mSpriteTexcoordU.x, mSpriteTexcoordV.x, 0, 0);
			vertexDatas[1].mTexCoord0.Set(mSpriteTexcoordU.x, texcoordr2, 0, 0);
			vertexDatas[2].mTexCoord0.Set(mSpriteTexcoordU.x, texcoordr3, 0, 0);
			vertexDatas[3].mTexCoord0.Set(mSpriteTexcoordU.x, mSpriteTexcoordV.y, 0, 0);
			//第二排
			vertexDatas[4].mTexCoord0.Set(texcoordc2, mSpriteTexcoordV.x, 0, 0);
			vertexDatas[5].mTexCoord0.Set(texcoordc2, texcoordr2, 0, 0);
			vertexDatas[6].mTexCoord0.Set(texcoordc2, texcoordr3, 0, 0);
			vertexDatas[7].mTexCoord0.Set(texcoordc2, mSpriteTexcoordV.y, 0, 0);

			//第三排
			vertexDatas[8].mTexCoord0.Set(texcoordc3, mSpriteTexcoordV.x, 0, 0);
			vertexDatas[9].mTexCoord0.Set(texcoordc3, texcoordr2, 0, 0);
			vertexDatas[10].mTexCoord0.Set(texcoordc3, texcoordr3, 0, 0);
			vertexDatas[11].mTexCoord0.Set(texcoordc3, mSpriteTexcoordV.y, 0, 0);

			//第四排
			vertexDatas[12].mTexCoord0.Set(mSpriteTexcoordU.y, mSpriteTexcoordV.x, 0, 0);
			vertexDatas[13].mTexCoord0.Set(mSpriteTexcoordU.y, texcoordr2, 0, 0);
			vertexDatas[14].mTexCoord0.Set(mSpriteTexcoordU.y, texcoordr3, 0, 0);
			vertexDatas[15].mTexCoord0.Set(mSpriteTexcoordU.y, mSpriteTexcoordV.y, 0, 0);
		}
	}

	void ImageSprite9::UpdateBoundingBox(){
		VertexDataFull*vertexes = (VertexDataFull*)mVertexData.mData;
		mBox.mPoints[0].Set(vertexes[0].mVertex.v[0], vertexes[0].mVertex.v[1], vertexes[0].mVertex.v[2]);
		mBox.mPoints[1].Set(vertexes[3].mVertex.v[0], vertexes[3].mVertex.v[1], vertexes[3].mVertex.v[2]);
		mBox.mPoints[2].Set(vertexes[15].mVertex.v[0], vertexes[15].mVertex.v[1], vertexes[15].mVertex.v[2]);
		mBox.mPoints[3].Set(vertexes[12].mVertex.v[0], vertexes[12].mVertex.v[1], vertexes[12].mVertex.v[2]);
	}

	void ImageSprite9::Update(float deltaTime){
		bool bDataChanged = false, needUpdateBoundingBox = false;
		if (mDirtyMask>0){
			if (GeometryMask(mDirtyMask) > 0) {
				bDataChanged = true;
				needUpdateBoundingBox = true;
				mbPositionChanged = false;
				UpdateVertexData();
			}
			if (TexcoordMask(mDirtyMask) > 0) {
				bDataChanged = true;
				UpdateTexcoordData();
			}
			if (ColorMask(mDirtyMask)>0){
				bDataChanged = true;
			}
			mDirtyMask = 0;
		}
		if (mbPositionChanged){
			UpdateVertexData();
			bDataChanged = true;
			mbPositionChanged = false;
			needUpdateBoundingBox = true;
		}
		if (needUpdateBoundingBox) {
			UpdateBoundingBox();
		}
		if (bDataChanged) {
			mVBO.SubData(mVertexData.mData, mVertexData.mBufferSize);
		}
	}

	void ImageSprite9::Render(RenderQueue*rq
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
		newDC->mEBO = &mEBO;
		newDC->mGameObject = mOwner;
		rq->AppendUIDrawCall(newDC);
	}

	ImageSprite9::~ImageSprite9(){
		//default builtin material will not be destroied
		//release texture ref
		//release vbo
	}

	void ImageSprite9::UpdateVertexNormal(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mNormal.Set(x, y, z, w);
		MarkColorDirty(mDirtyMask);
	}

	void ImageSprite9::UpdateVertexTangent(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mTangent.Set(x*mAddtionalColor.v[0], y*mAddtionalColor.v[1], z*mAddtionalColor.v[2], w*mAddtionalColor.v[3]);
		MarkColorDirty(mDirtyMask);
	}

	void ImageSprite9::UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		vertexes[nIndex].mTexCoord1.Set(x, y, z, w);
		MarkColorDirty(mDirtyMask);
	}
}
