#pragma once
#include "Runtime/2D/Component2D.h"
#include "Runtime/Font/DynamicFont.h"
#include "Runtime/Graphics/DynamicTextureAtlas.h"
#include "Runtime/Render/Camera.h"
#include "Runtime/Scene/RendererManager.h"
#include "DisplayChar.h"
#include "Runtime/Serializer/Label.serializer.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Render/AliceGL.h"
namespace Alice{
	enum LabelAligning{
		kLabelAligningLeft=1,
		kLabelAligningMiddleH=2,
		kLabelAligningRight=4,
		kLabelAligningUp=8,
		kLabelAligningBottom=16,
		kLabelAligningMiddleV=32
	};
	class Label:public Component2D
	{
	protected:
		void MoveChar(int nIndex, float offsetX, float offsetY);
		void ShiftData(int nDst,int nSrc, int nCount);
		void ShiftChar(int nStart, int nOffset);
		void ShiftDisplayChar(DisplayChar*displayChar, float offset);
		DisplayChar* GetCharAt(int nIndex);
		DisplayChar* GetCharInfo(const UTF32Char &utf32Char, AliceUInt8 r = 255, AliceUInt8 g = 255, AliceUInt8 b = 255, AliceUInt8 a = 255);
		void InsertCharAtIndex(int nIndex,DisplayChar*displayChar);
		void AppendCharAtEnd(DisplayChar*displayChar);
		void UpdateDisplayCharVertex(VertexDataFull*vertex, float l, float b, float r, float t);
		void UpdateDisplayCharTexcoord(VertexDataFull*vertex, float l, float b, float r, float t);
		void UpdateDisplayChar(DisplayChar*displayChar,VertexDataFull*vertex);
		void AppendCharDrawCommandAtEnd();
		void InitCharRenderableData(DisplayChar*displayChar, float offset);
	public:
		Label(int maxCharCount=128);
		virtual ~Label();
		Label(const char*fontName, const AliceUInt32 *text,int textLen,CharStyle chStyle,int charSize, int outLineSize,Color4B color);
		void Initialize(int maxCharCount=128);
		void InitWithDefaultText();
		void InitWithSerializedData(const Serializer::Label&labelData);
		void SetText(const UTF32Char*text, AliceSInt32 len);
		void AppendChar(const UTF32Char &utf32Char, AliceUInt8 r = 255, AliceUInt8 g = 255, AliceUInt8 b = 255, AliceUInt8 a = 255, bool customColor = false);
		void InsertCharAt(int nIndex, const UTF32Char &utf32Char, AliceUInt8 r = 255, AliceUInt8 g = 255, AliceUInt8 b = 255, AliceUInt8 a = 255, bool customColor = false);
		void UpdateLocalBoundingBox(DisplayChar*currentChar);
		void ClearChars();
		void RecreateChars();
		void DeleteCharAt(int nIndex);
		float GetCharWidthInPixel(int nIndex);
		//到nEnd为止，包含nEnd在内的所有字符加起来的总的像素长度
		float GetLabelWidth(int nEnd);
		float GetHeight();
		void TrimEnd();
		void UpdateTextPositon();
		void UpdateTextPositionLeftAligning();
		void UpdateTextPositionMiddleAligning();
		void UpdateTextPositionRightAligning();
		VertexDataFull*GetCharVertexData(int nIndex);
		VertexDataFull*GetCharGlobalVertexData(int nIndex);
		void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		void UpdateBoundingBox();
		void Update(float deltaTime);
		int GetText(AliceUInt32*bufferOut);
		//get utf8 encoded str
		void GetUTF8Text(char*bufferOut, int&len);
		void GetUTF8CharAt(int nIndex,char*bufferOut, int&len);
		AliceSInt32 GetLenCharCount();
		float GetLabelLenInPixel();
		Vector3f mLeftBottom, mRightTop;
		Matrix4x4 mLocalShiftMatrix;//shift text for aligning
		LabelAligning mAligning;
		DisplayChar*mEnd;
		AliceSInt32 mCharCount;
		AliceSInt32 mMaxCharCount;
		float mLabelLenInPixel;
		FixedString mFontName;
		CharStyle mTextStyle;
		Color4B mLabelColor;
		Color4B mShadowColor;
		Color4B mOutlineColor;
		bool mbLocalTransformChanged;
		bool mbLabelColorChanged;
		bool mbGlobalDataChanged;
		bool mbUseStencilMask;
		int mOutlineSize;
		int mBoldStrength;
		AliceSInt8 mShadowDistanceX, mShadowDistanceY;
		int mBottomY;
		AliceSInt8 mCharSizeInPoint;
		VertexData mLocalData,mGlobalData;
		VBO mVBO;
		EBO mEBO; 
		SmartPtr<Material> mMaterial;
		DynamicFont*mFont;
	public:
		OPT(void)SetAligning(LabelAligning aligning);
		OPT(void)SetStyle(CharStyle style);
		OPT(void)SetColor(AliceUInt8 r, AliceUInt8 g,AliceUInt8 b,AliceUInt8 a);
		OPT(void)SetText(const char*utf8Str);
		OPT(void)AppendText(const char*utf8Str);
		OPT(void)AppendText(const char*utf8Str, AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a);
		OPT(bool)SetFont(const char*fontName);
		OPT(void)SetOutlineColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a);
		OPT(void)SetOutlineSize(int outlineSize);
		OPT(void)SetShadow(int x,int y);
		OPT(void)SetShadowColor(AliceUInt8 r, AliceUInt8 g, AliceUInt8 b, AliceUInt8 a);
		OPT(void)SetCharSize(int nCharSize);
	public:
		DECLEAR_ALICE_CLASS(Label)
	public:
		DisplayChar* mDisplayText;
#if ALICE_EDITOR
		void Serialize(Serializer::Label*labelData);
#endif
	};
}