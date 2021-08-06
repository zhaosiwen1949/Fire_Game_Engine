#include "Label.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H
#include "Runtime/2D/ImageSprite.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Graphics/ImageUtils.h"
#include "Runtime/Render/RenderMisc.h"
#include "Runtime/Math/Color.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Font/DynamicFont.h"
#include "Runtime/Scene/RendererManager.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Render/RenderPass.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"

namespace Alice
{
	Label::Label(int maxCharCount){
		mLabelColor = Color4B::WHITE;
		mDisplayText = nullptr;
		mEnd = nullptr;
		mCharCount = 0;
		mTextStyle = kCharStyleNormal;
		mCharSizeInPoint = 12;
		mBoldStrength = 1;
		mOutlineSize = 0;
		mLabelLenInPixel = 0;
		mbPositionChanged = false;
		mbLabelColorChanged = false;
		mbLocalTransformChanged = false;
		mbUseStencilMask = false;
		mShadowDistanceX = 0;
		mShadowDistanceY = 0;
		mAligning = kLabelAligningLeft;
		mMaxCharCount = maxCharCount;
		mFontName.Resize(128);
		mFontName = "Arial";
		mOutlineColor.Set(255, 0, 0, 255);
		mShadowColor.Set(0, 255, 0, 255);
		mMaterial = ExportedMaterial::Clone("builtin/Material/Font");
		mFont = nullptr;
	}

	Label::~Label(){
		char szBuffer[2048] = { 0 };
		int len = 0;
		GetUTF8Text(szBuffer, len);
		//Debug("delete label %s",szBuffer);
		ClearChars();
	}

	void Label::Initialize(int maxCharCount/* =128 */) {
		mMaxCharCount = maxCharCount;
		mLocalData.SetBufferSize(mMaxCharCount * 4);//utf8 local data
		mGlobalData.SetBufferSize(mMaxCharCount * 4);//utf8 data for draw
		mVBO.Init(mMaxCharCount * 4);//vbo for utf8 data
		mEBO.Init(6 * mMaxCharCount, GL_STATIC_DRAW);
		mEBO.mIndexCountForDraw = 0;
	}

	void Label::InitWithDefaultText(){
		/*UTF32Char temp[6];
		temp[0].mCharCode = 'L';
		temp[1].mCharCode = 'a';
		temp[2].mCharCode = 'b';
		temp[3].mCharCode = 'e';
		temp[4].mCharCode = 'l';

		temp[0].mByteCount = 1;
		temp[1].mByteCount = 1;
		temp[2].mByteCount = 1;
		temp[3].mByteCount = 1;
		temp[4].mByteCount = 1;
		SetText(temp, 5);*/
	}

#if ALICE_EDITOR
	void Label::Serialize(Serializer::Label*labelData){
		labelData->set_font(mFontName.mText);
		labelData->set_outlinesize(mOutlineSize);
		labelData->set_charsize(mCharSizeInPoint);
		labelData->set_style(mTextStyle);
		labelData->set_aligning(mAligning);
		labelData->set_shadowx(mShadowDistanceX);
		labelData->set_shadowy(mShadowDistanceY);

		Serializer::Color4B*textColor=labelData->mutable_textcolor();
		textColor->set_r(mLabelColor.r);
		textColor->set_g(mLabelColor.g);
		textColor->set_b(mLabelColor.b);
		textColor->set_a(mLabelColor.a);
		textColor = labelData->mutable_outlinecolor();
		textColor->set_r(mOutlineColor.r);
		textColor->set_g(mOutlineColor.g);
		textColor->set_b(mOutlineColor.b);
		textColor->set_a(mOutlineColor.a);
		textColor = labelData->mutable_shadowcolor();
		textColor->set_r(mShadowColor.r);
		textColor->set_g(mShadowColor.g);
		textColor->set_b(mShadowColor.b);
		textColor->set_a(mShadowColor.a);

		DisplayChar*nextChar = mDisplayText;
		while (nextChar != nullptr){
			labelData->mutable_text()->Add(nextChar->mCharInfo->mCharCode);
			nextChar = nextChar->Next<DisplayChar>();
		}
		Debug("my label count is %d",labelData->text_size());
	}
#endif
	void Label::InitWithSerializedData(const Serializer::Label&labelData)
	{
		mCharSizeInPoint = labelData.charsize();
		mFontName = labelData.font();
		mAligning = (LabelAligning)labelData.aligning();
		mTextStyle = (CharStyle)labelData.style();
		mOutlineSize = labelData.outlinesize();
		mShadowDistanceX = labelData.shadowx();
		mShadowDistanceY = labelData.shadowy();

		const Serializer::Color4B &color = labelData.textcolor();
		mLabelColor.r = color.r();
		mLabelColor.g = color.g();
		mLabelColor.b = color.b();
		mLabelColor.a = color.a();
		const Serializer::Color4B&outlineColor = labelData.outlinecolor();
		mOutlineColor.Set(outlineColor.r(), outlineColor.g(), outlineColor.b(), outlineColor.a());
		const Serializer::Color4B&shadowColor = labelData.shadowcolor();
		mShadowColor.Set(shadowColor.r(), shadowColor.g(), shadowColor.b(), shadowColor.a());

		AliceUInt32 temp[1024];
		memset(temp, 0, 1024 * sizeof(AliceUInt32));
		for (int i=0;i<labelData.text_size();i++)
		{
			temp[i] = labelData.text(i);
		}
	}

	Label::Label(const char*fontName,const AliceUInt32 *text, int textLen, CharStyle chStyle,int charSize,int outLineSize, Color4B color)
	{
		mDisplayText = nullptr;
		mEnd = nullptr;
		mLabelColor = color;
		mFontName = fontName;
		mTextStyle = chStyle;
		mCharSizeInPoint = charSize;
		mOutlineSize = outLineSize;
		mLabelLenInPixel = 0;
		mbPositionChanged = false;
		mbLabelColorChanged = false;
		mBoldStrength = 1;
	}

	VertexDataFull*Label::GetCharVertexData(int nIndex){
		return mLocalData.GetBuffer<VertexDataFull>() + nIndex * 4;
	}

	VertexDataFull*Label::GetCharGlobalVertexData(int nIndex){
		return mGlobalData.GetBuffer<VertexDataFull>() + nIndex * 4;
	}

	void Label::SetText(const UTF32Char*text, AliceSInt32 len){
		ClearChars();
		if (len==0){
			return;
		}
		if (len>mMaxCharCount){
			Error("label error:out of range %d,max len %d",len,mMaxCharCount);
			return;
		}
		for (int i = 0; i < len;i++){
			AppendChar(text[i]);
		}
		UpdateTextPositon();
	}
	DisplayChar*Label::GetCharInfo(const UTF32Char &utf32Char, AliceUInt8 r /* = 255 */, AliceUInt8 g /* = 255 */, AliceUInt8 b /* = 255 */, AliceUInt8 a /* = 255 */) {
		DisplayChar*displayChar = new (kMemFontId)DisplayChar;
		displayChar->mbCustomColor = true;
		displayChar->mColor.Set(r, g, b, a);
		if (mFont == nullptr) {
			if (!displayChar->InitCharInfoWithFontName(mFontName.mText, utf32Char, mTextStyle, mCharSizeInPoint, mOutlineSize, mShadowDistanceX, mShadowDistanceY)) {
				delete displayChar;
				displayChar = nullptr;
			}
		}
		else {
			if (!displayChar->InitCharInfoWithFont(mFont, utf32Char, mTextStyle, mCharSizeInPoint, mOutlineSize, mShadowDistanceX, mShadowDistanceY)) {
				delete displayChar;
				displayChar = nullptr;
			}
		}
		return displayChar;
	}
	void Label::InsertCharAtIndex(int nIndex, DisplayChar*displayChar) {
		DisplayChar*charAtInsertPos = GetCharAt(nIndex);
		if (charAtInsertPos==nullptr){
			if (nIndex==0){//insert at head,and head is null
				mDisplayText = displayChar;
			}else {//insert at end,but out of range
				mDisplayText->Append(displayChar);
			}
			displayChar->mIndex = mCharCount;
		}else {
			displayChar->InsertBefore(charAtInsertPos);
			displayChar->mIndex = nIndex;
		}
	}
	void Label::AppendCharAtEnd(DisplayChar*displayChar) {
		if (mDisplayText == nullptr) {//the head is null
			mDisplayText = displayChar;
		}
		else {
			mDisplayText->Append(displayChar);//head is not null
		}
		displayChar->mIndex = mCharCount;
		mEnd = displayChar;
	}
	void Label::ShiftChar(int nStart, int nOffset) {

	}
	void Label::ShiftDisplayChar(DisplayChar*displayChar, float offset) {
		DisplayChar*nextChar = displayChar;
		int copyCount = 0;
		while (nextChar != nullptr) {
			MoveChar(nextChar->mIndex, offset, 0);
			if (offset > 0.0f) {
				nextChar->mIndex += 1 ;
			}
			else {
				nextChar->mIndex -= 1;
			}
			nextChar = nextChar->Next<DisplayChar>();
			++copyCount;
		}
		if (copyCount > 0) {
			mbGlobalDataChanged = true;
			if (offset > 0.0f) {
				ShiftData(displayChar->mIndex, displayChar->mIndex - 1, copyCount);
			}else{
				ShiftData(displayChar->mIndex, displayChar->mIndex + 1, copyCount);
			}
		}
	}
	void Label::InsertCharAt(int nIndex, const UTF32Char &utf32Char, AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a, bool customColor) {
		if (nIndex<0||nIndex>mCharCount){
			Error("insert char out of range(0~%d) %d",mCharCount,nIndex);
			return;
		}
		DisplayChar*insertAtChar = GetCharAt(nIndex);
		if (insertAtChar == nullptr) {//insert point must be equal to mCharCount
			if (mCharCount==nIndex){
				AppendChar(utf32Char, r, g, b, a,customColor);
				UpdateTextPositon();
			} else {
				Error("insert char exception range(0~%d) %d", mCharCount, nIndex);
			}
		}
		else {//insertAtChar the the chars after this shift right ->
			DisplayChar*currentChar = GetCharInfo(utf32Char, r, g, b, a);
			if (currentChar == nullptr) {
				return;//create char info fail
			}
			currentChar->mbCustomColor = customColor;
			currentChar->mIndex = nIndex;
			ShiftDisplayChar(insertAtChar, currentChar->GetFrameWidth());
			InitCharRenderableData(currentChar, GetLabelWidth(nIndex - 1));
			currentChar->InsertBefore(insertAtChar);
			if (insertAtChar == mDisplayText) {//insert at head
				mDisplayText = currentChar;
			}
			UpdateLocalBoundingBox(currentChar);
			AppendCharDrawCommandAtEnd();
			mLabelLenInPixel += currentChar->GetFrameWidth();
			mCharCount++;
			UpdateTextPositon();
		}
	}
	void Label::AppendChar(const UTF32Char &utf32Char, AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a, bool customColor) {
		DisplayChar*currentChar = GetCharInfo(utf32Char, r, g, b, a);
		if (currentChar == nullptr) {
			return;
		}
		currentChar->mbCustomColor = customColor;
		AppendCharAtEnd(currentChar);//append char into the label letter chain
		InitCharRenderableData(currentChar,mLabelLenInPixel);//
		AppendCharDrawCommandAtEnd();//append draw command for the char : 6 unsigned short
		UpdateLocalBoundingBox(currentChar);
		mLabelLenInPixel += currentChar->GetFrameWidth();
		mCharCount++;
	}
	void Label::UpdateDisplayCharVertex(VertexDataFull*vertex, float l, float b, float r, float t) {
		vertex[0].mVertex = Vector3f(l, b, 0);
		vertex[1].mVertex = Vector3f(r, b, 0);
		vertex[2].mVertex = Vector3f(r, t, 0);
		vertex[3].mVertex = Vector3f(l, t, 0);
	}
	void Label::UpdateDisplayCharTexcoord(VertexDataFull*vertex, float l, float b, float r, float t) {
		vertex[0].mTexCoord0 = Vector2f(l, b);
		vertex[1].mTexCoord0 = Vector2f(r, b);
		vertex[2].mTexCoord0 = Vector2f(r, t);
		vertex[3].mTexCoord0 = Vector2f(l, t);
	}
	void Label::UpdateDisplayChar(DisplayChar*displayChar,VertexDataFull*vertex) {
		for (int i = 0; i < 4; ++i) {
			vertex[i].mNormal.Set((float)mOutlineColor.r / 255.0f, (float)mOutlineColor.g / 255.0f, (float)mOutlineColor.b / 255.0f, (float)mOutlineColor.a / 255.0f);
			if (displayChar->mbCustomColor) {
				vertex[i].mTangent.Set((float)displayChar->mColor.r / 255.0f, (float)displayChar->mColor.g / 255.0f, (float)displayChar->mColor.b / 255.0f, (float)displayChar->mColor.a / 255.0f);
			}
			else {
				vertex[i].mTangent.Set((float)mLabelColor.r / 255.0f, (float)mLabelColor.g / 255.0f, (float)mLabelColor.b / 255.0f, (float)mLabelColor.a / 255.0f);
			}
			vertex[i].mTexCoord1.Set((float)mShadowColor.r / 255.0f, (float)mShadowColor.g / 255.0f, (float)mShadowColor.b / 255.0f, (float)mShadowColor.a / 255.0f);
		}
	}
	void Label::AppendCharDrawCommandAtEnd() {
		AliceUInt16 offset = mCharCount * 4;
		AliceUInt16 indexes[] = {
			offset,
			static_cast<AliceUInt16>(offset + 1),
			static_cast<AliceUInt16>(offset + 2),
			offset,
			static_cast<AliceUInt16>(offset + 2),
			static_cast<AliceUInt16>(offset + 3)
		};
		mEBO.SubData(indexes, sizeof(AliceUInt16) * 6, mCharCount * sizeof(AliceUInt16) * 6);
		mEBO.mIndexCountForDraw += 6;
	}
	void Label::InitCharRenderableData(DisplayChar*displayChar, float offset) {
		VertexDataFull*vertex = GetCharVertexData(displayChar->mIndex);
		VertexDataFull*gVertex = GetCharGlobalVertexData(displayChar->mIndex);
		CharInfo *charInfo = displayChar->mCharInfo;
		float posY = charInfo->mHoriBearingY - float(charInfo->mAtlasRectangle->mRect.mHeight);
		float bottom = posY - charInfo->mBaselineOffset/2.0f;
		float startX = displayChar->GetCharLeft(offset);
		float charRight = displayChar->GetCharRight(startX);
		//Debug("%c : %f(%f),%d",charInfo->mCharCode, charInfo->mBaselineOffset,GetHeight(), charInfo->mAtlasRectangle->mRect.mHeight);
		UpdateDisplayCharVertex(vertex,startX, bottom, charRight, charInfo->mAtlasRectangle->mRect.mHeight+bottom);
		UpdateDisplayCharTexcoord(vertex,
			charInfo->mAtlasRectangle->mTexCoord.x, charInfo->mAtlasRectangle->mTexCoord.y,
			charInfo->mAtlasRectangle->mTexCoord.z, charInfo->mAtlasRectangle->mTexCoord.w);
		UpdateDisplayChar(displayChar, vertex);
		memcpy(gVertex, vertex, sizeof(VertexDataFull) * 4);//sync data to g buffer for draw
	}

	void Label::UpdateLocalBoundingBox(DisplayChar*currentChar) {
		VertexDataFull*vertexes = GetCharVertexData(currentChar->mIndex);
		mBottomY = mBottomY > vertexes[0].mVertex.y ? vertexes[0].mVertex.y : mBottomY;
		mLeftBottom.Set(ALICE_MIN(vertexes[0].mVertex.x, mLeftBottom.x),
			ALICE_MIN(vertexes[0].mVertex.y, mLeftBottom.y),
			ALICE_MIN(vertexes[0].mVertex.z, mLeftBottom.z));
		mRightTop.Set(ALICE_MAX(vertexes[2].mVertex.x, mRightTop.x),
			ALICE_MAX(vertexes[2].mVertex.y, mRightTop.y),
			ALICE_MAX(vertexes[2].mVertex.z, mRightTop.z));
	}

	void Label::UpdateBoundingBox(){
		memset(mBox.mPoints, 0, sizeof(Vector3f)*4);
		mBox.mPoints[0].Set(mLeftBottom.v);
		mBox.mPoints[1].Set(mRightTop.x, mLeftBottom.y, 0.0f);
		mBox.mPoints[2].Set(mRightTop.v);
		mBox.mPoints[3].Set(mLeftBottom.x, mRightTop.y, 0.0f);
		if (mOwner!=nullptr){
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			Matrix4x4 label_world_matrix;
			Matrix4x4MultiplyMatrix4x4(world_matrix.mData,mLocalShiftMatrix.mData, label_world_matrix.mData);
			for (int i = 0; i < 4; i++){
				mBox.mPoints[i] = label_world_matrix *Vector4f(mBox.mPoints[i]);
			}
		}
	}

	void Label::Update(float deltaTime){
		bool bDataChanged = false;
		if (mbPositionChanged || mbLocalTransformChanged) {
			//Debug("update mbPositionChanged(%d) mbLocalTransformChanged(%d)",mbPositionChanged,mbLocalTransformChanged);
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			Matrix4x4 label_world_matrix;
			Matrix4x4MultiplyMatrix4x4(world_matrix.mData,mLocalShiftMatrix.mData, label_world_matrix.mData);
			DisplayChar*nextChar = mDisplayText;
			bDataChanged = true;
			while (nextChar != nullptr){
				VertexDataFull*localData = GetCharVertexData(nextChar->mIndex);
				VertexDataFull*globalData = GetCharGlobalVertexData(nextChar->mIndex);
				for (int i=0;i<4;i++){
					globalData[i].mVertex = label_world_matrix *Vector4f(localData[i].mVertex);
				}
				nextChar = nextChar->Next<DisplayChar>();
			}
			UpdateBoundingBox();
			mbPositionChanged = false;
			mbLocalTransformChanged = false;
		}
		if (mbLabelColorChanged){
			DisplayChar*nextChar = mDisplayText;
			bDataChanged = true;
			while (nextChar != nullptr){
				VertexDataFull*globalData = mGlobalData.GetBuffer<VertexDataFull>() +nextChar->mIndex*4;
				for (int i = 0; i < 4; ++i) {
					globalData[i].mNormal.Set((float)mOutlineColor.r / 255.0f, (float)mOutlineColor.g / 255.0f, (float)mOutlineColor.b / 255.0f, (float)mOutlineColor.a / 255.0f);
					if (nextChar->mbCustomColor) {
						globalData[i].mTangent.Set((float)nextChar->mColor.r / 255.0f, (float)nextChar->mColor.g / 255.0f, (float)nextChar->mColor.b / 255.0f, (float)nextChar->mColor.a / 255.0f);
					}
					else {
						globalData[i].mTangent.Set((float)mLabelColor.r / 255.0f, (float)mLabelColor.g / 255.0f, (float)mLabelColor.b / 255.0f, (float)mLabelColor.a / 255.0f);
					}
					globalData[i].mTexCoord1.Set((float)mShadowColor.r / 255.0f, (float)mShadowColor.g / 255.0f, (float)mShadowColor.b / 255.0f, (float)mShadowColor.a / 255.0f);
				}
				nextChar = nextChar->Next<DisplayChar>();
			}
			mbLabelColorChanged = false;
		}
		if (bDataChanged||mbGlobalDataChanged)
		{
			mbGlobalDataChanged = false;
			mVBO.SubData(mGlobalData.mData, mGlobalData.mBufferSize);
		}
	}

	int Label::GetText(AliceUInt32*bufferOut)
	{
		DisplayChar*nextChar = mDisplayText;
		int i = 0;
		while (nextChar != nullptr)
		{
			bufferOut[i++] = nextChar->mCharInfo->mCharCode;
			nextChar = nextChar->Next<DisplayChar>();
		}
		return i;
	}

	void Label::GetUTF8Text(char*bufferOut,int &bufferLen)
	{
		DisplayChar *nextChar = mDisplayText;
		bufferLen = 0;
		while (nextChar!=nullptr){
			int len = -1;
			unsigned char buffer[4] = {0};
			if (StringUtils::UTF32CharToUTF8(nextChar->mCharCode,buffer,len)){
				memcpy(bufferOut+bufferLen,buffer,len);
				bufferLen += len;
			}
			nextChar = nextChar->Next<DisplayChar>();
		}
	}
	void Label::GetUTF8CharAt(int nIndex, char*bufferOut, int &bufferLen)
	{
		DisplayChar *charToFind=GetCharAt(nIndex);
		if (charToFind==nullptr){
			return;
		}
		StringUtils::UTF32CharToUTF8(charToFind->mCharInfo->mCharCode, (unsigned char*)bufferOut, bufferLen);
	}

	void Label::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	)
	{
		if (mCharCount==0){
			return;
		}
		Rect<float> view_rect;
		if (rq->mCamera->GetViewRect(view_rect)) {
			Rect<float> sprite_rect;
			GetRect(sprite_rect);
			if (false == view_rect.IsOverlappedWith(sprite_rect)) {
				return;
			}
		}
		DrawCall*newDC =new (kMemRendererId)DrawCall;
		if (mMaterial != nullptr) {
			newDC->mMaterial = mMaterial.mPtr;
			if (mMaterial->GetTextureProperty("U_MainTexture")->mValue == nullptr) {
				mMaterial->SetTextureProperty("U_MainTexture", mDisplayText->mCharInfo->mAtlas->mTexture);
			}
		} else {
			newDC->mMaterial = mDisplayText->mCharInfo->mAtlas->mMaterial.mPtr;
		}
		newDC->mMaterial->mRenderPasses.front()->mbUseStencilMask = mbUseStencilMask;
		newDC->mVBO = &mVBO;
        newDC->mEBO = &mEBO;
		newDC->mGameObject = mOwner;
		rq->AppendUIDrawCall(newDC);
	}
	///
	/*
	mEnd=null,mDisplayText=null,mLabelLenInPixel=0,mCharCount=0,mUTF8Str="",mIBO=0,
	*/
	void Label::ClearChars(){
		if (mDisplayText != nullptr){
			while (mCharCount!=0){
				TrimEnd();
			}
		}
		mEnd = nullptr;
		mLabelLenInPixel = 0.0f;
		mBox.mPoints[0].Set(0.0f,0.0f,0.0f);
		mBox.mPoints[1].Set(0.0f, 0.0f, 0.0f);
		mBox.mPoints[2].Set(0.0f, 0.0f, 0.0f);
		mBox.mPoints[3].Set(0.0f, 0.0f, 0.0f);
	}

	void Label::RecreateChars(){
		char szBuffer[2048] = {0};
		int len = 0;
		GetUTF8Text(szBuffer, len);
		if (len>0){
			SetText(szBuffer);
		}
	}

	void Label::MoveChar(int nIndex, float offsetX, float offsetY) {
		if (nIndex<0||nIndex>=mCharCount){
			Error("move char out of range(0~%d) %d",mCharCount-1,nIndex);
			return;
		}
		VertexDataFull*vertexOld = GetCharVertexData(nIndex);
		VertexDataFull*gVertexOld = GetCharGlobalVertexData(nIndex);
		vertexOld[0].mVertex.x += offsetX; vertexOld[0].mVertex.y += offsetY;
		vertexOld[1].mVertex.x += offsetX; vertexOld[1].mVertex.y += offsetY;
		vertexOld[2].mVertex.x += offsetX; vertexOld[2].mVertex.y += offsetY;
		vertexOld[3].mVertex.x += offsetX; vertexOld[3].mVertex.y += offsetY;
		gVertexOld[0].mVertex.x += offsetX; gVertexOld[0].mVertex.y += offsetY;
		gVertexOld[1].mVertex.x += offsetX; gVertexOld[1].mVertex.y += offsetY;
		gVertexOld[2].mVertex.x += offsetX; gVertexOld[2].mVertex.y += offsetY;
		gVertexOld[3].mVertex.x += offsetX; gVertexOld[3].mVertex.y += offsetY;
	}
	void Label::ShiftData(int nDst, int nSrc, int nCount) {
		VertexDataFull*dst = GetCharVertexData(nDst);
		VertexDataFull*dstG = GetCharGlobalVertexData(nDst);
		VertexDataFull*src = GetCharVertexData(nSrc);
		VertexDataFull*srcG = GetCharGlobalVertexData(nSrc);
		VertexDataFull*temp = new VertexDataFull[nCount*4];
		memcpy(temp,src, sizeof(VertexDataFull)*nCount * 4);
		memcpy(dst, temp, sizeof(VertexDataFull)*nCount * 4);

		memcpy(temp, srcG, sizeof(VertexDataFull)*nCount * 4);
		memcpy(dstG, temp, sizeof(VertexDataFull)*nCount * 4);
		delete[] temp;
	}
	DisplayChar*Label::GetCharAt(int nIndex) {
		DisplayChar*charToFind = mDisplayText; 
		int nCurrentIndex = 0;
		while (charToFind != nullptr) {
			if (nIndex == nCurrentIndex) {
				break;
			}
			nCurrentIndex++;
			charToFind = charToFind->Next<DisplayChar>();
		}
		return charToFind;
	}
	void Label::DeleteCharAt(int nIndex){
		DisplayChar*charToDelete = GetCharAt(nIndex);
		if (charToDelete==nullptr){
			return;
		}
		if (charToDelete == mDisplayText) {
			mDisplayText = mDisplayText->Next<DisplayChar>();
		}
		DisplayChar*nextChar = charToDelete->Next<DisplayChar>();
		float offset = charToDelete->GetFrameWidth();
		if (nextChar != nullptr) {
			ShiftDisplayChar(nextChar, -offset);
		}
		if (charToDelete==mEnd){
			mEnd = charToDelete->Prev<DisplayChar>();
		}
		charToDelete->LeaveList();
		mLabelLenInPixel -= offset;
		mEBO.mIndexCountForDraw -= 6;
		mCharCount--;
		delete charToDelete;
		if (mAligning==kLabelAligningMiddleH){
			UpdateTextPositionMiddleAligning();
		}
	}
	void Label::TrimEnd(){
		if (mCharCount==0){
			return;
		}
		DisplayChar*charToDelete = mEnd;
		if (mDisplayText==charToDelete){
			mDisplayText = nullptr;
		}
		mEnd = mEnd->Prev<DisplayChar>();
		mEBO.mIndexCountForDraw -= 6;
		mLabelLenInPixel -= charToDelete->GetFrameWidth();
		delete charToDelete;
		mCharCount--;
	}

	float Label::GetCharWidthInPixel(int nIndex) {
		DisplayChar*charToFind = GetCharAt(nIndex);
		if (charToFind == nullptr) {
			return 0.0f;
		}
		return charToFind->GetFrameWidth();
	}

	float Label::GetLabelWidth(int nEnd) {
		if (nEnd==-1){
			return 0.0f;
		}
		float labelLen = mLabelLenInPixel;
		DisplayChar*charToFind = mEnd;
		while (charToFind!=nullptr){
			if (charToFind->mIndex<=nEnd){
				break;
			}
			labelLen -= charToFind->GetFrameWidth();
			charToFind = charToFind->Prev<DisplayChar>();
		}
		return labelLen;
	}
	float Label::GetHeight() {
		DynamicFont*df = mFont;
		if (df == nullptr) {
			df = DynamicFont::GetFont(mFontName.mText);
		}
		if (df==nullptr){
			return 0.0f;
		}
		return df->mFace->GetLineHeight(mCharSizeInPoint);
	}
	void Label::UpdateTextPositon(){
		if (mAligning==kLabelAligningLeft){
			UpdateTextPositionLeftAligning();
		}else if(mAligning==kLabelAligningMiddleH){
			UpdateTextPositionMiddleAligning();
		}else if(mAligning==kLabelAligningRight){
			UpdateTextPositionRightAligning();
		}
	}

	void Label::UpdateTextPositionLeftAligning() {
		mLocalShiftMatrix.mData[12] = 0.0f;
		mbLocalTransformChanged = true;
		//Debug("UpdateTextPositionLeftAligning");
	}

	void Label::UpdateTextPositionMiddleAligning() {
		float leftShift = mLabelLenInPixel / 2.0f;
		mLocalShiftMatrix.mData[12] = -leftShift;
		mbLocalTransformChanged = true;
		//Debug("UpdateTextPositionMiddleAligning");
	}

	void Label::UpdateTextPositionRightAligning() {
		float leftShift = mLabelLenInPixel;
		mLocalShiftMatrix.mData[12] = -leftShift;
		mbLocalTransformChanged = true;
		//Debug("UpdateTextPositionRightAligning");
	}

	AliceSInt32 Label::GetLenCharCount()
	{
		return mCharCount;
	}

	float Label::GetLabelLenInPixel()
	{
		return mLabelLenInPixel;
	}
	//api
	void Label::SetAligning(LabelAligning aligning){
		mAligning = aligning;
		if (mDisplayText==nullptr){
			return;
		}
		UpdateTextPositon();
	}

	void Label::SetCharSize(int nCharSize){
		mCharSizeInPoint = nCharSize;
		RecreateChars();
	}

	void Label::SetStyle(CharStyle style)
	{
		mTextStyle = style;
		RecreateChars();
	}

	void Label::SetColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a)
	{
		mLabelColor.Set(r, g, b, a);
		mbLabelColorChanged = true;
	}

	void Label::SetText(const char*utf8Str){
		if (strlen(utf8Str) > 0){
			UTF32Char charCode[1024] = {0};
			int nCount = StringUtils::UTF8ToUTF32((const unsigned char*)utf8Str, strlen(utf8Str), charCode);
			if (nCount == 0){
				errorC("label set text must be utf8 encoded");
				return ;
			}
			SetText(charCode, nCount);
		}else{
			ClearChars();
		}
	}

	void Label::AppendText(const char*utf8Str){
		if (strlen(utf8Str) > 0){
			UTF32Char charCode[1024] = { 0 };
			int nCount = StringUtils::UTF8ToUTF32((const unsigned char*)utf8Str, strlen(utf8Str), charCode);
			if (nCount == 0){
				errorC("label append text must be utf8 encoded");
				return;
			}
			for (int i = 0; i < nCount; i++){
				AppendChar(charCode[i]);
			}
			UpdateTextPositon();
		}
	}

	void Label::AppendText(const char*utf8Str, AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a){
		if (strlen(utf8Str) > 0){
			UTF32Char charCode[1024] = { 0 };
			int nCount = StringUtils::UTF8ToUTF32((const unsigned char*)utf8Str, strlen(utf8Str), charCode);
			if (nCount == 0){
				errorC("label append text must be utf8 encoded");
				return;
			}
			if ((mCharCount+nCount)>mMaxCharCount){
				Error("cannot append more text,out of range %d,%d",mCharCount,nCount);
				return;
			}
			for (int i = 0; i < nCount; i++){
				AppendChar(charCode[i],r,g,b,a,true);
			}
			UpdateTextPositon();
		}
	}

	bool Label::SetFont(const char*fontName)
	{
		const char*guid = Resource::GetResourceGUIDViaUserName(fontName);
		if (guid!=nullptr)
		{
			mFontName = guid;
			RecreateChars();
			return true;
		}
		return false;
	}

	void Label::SetOutlineColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a)
	{
		mOutlineColor.Set(r, g, b, a);
		mbLabelColorChanged = true;
	}

	void Label::SetOutlineSize(int outlineSize)
	{
		mOutlineSize = outlineSize;
		RecreateChars();
	}

	void Label::SetShadow(int x, int y)
	{
		mShadowDistanceX = x;
		mShadowDistanceY = y;
		RecreateChars();
	}

	void Label::SetShadowColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a)
	{
		mShadowColor.Set(r, g, b, a);
		mbLabelColorChanged = true;
	}
}
