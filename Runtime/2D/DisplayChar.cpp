#include "DisplayChar.h"
#include "Runtime/Scene/SceneManager.h"

namespace Alice{
	DisplayChar::DisplayChar(){
		mCharInfo = nullptr;
		mbCustomColor = false;
		mCharCode = 0;
	}

	DisplayChar::~DisplayChar(){
	}

	void DisplayChar::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	)
	{
	}
	float DisplayChar::GetFrameWidth() {
		return mFrameWith;
	}

	float DisplayChar::GetCharLeft(float startPoint) {
		return startPoint + mCharInfo->mHoriBearingX;
	}

	float DisplayChar::GetCharRight(float startPoint) {
		return startPoint + mCharInfo->mWidth;
	}

	bool DisplayChar::InitCharInfoWithFontName(const char*fontName, const UTF32Char &charCode, CharStyle style, AliceSInt32 size, AliceSInt32 outlineSize, AliceSInt32 shadowX, AliceSInt32 shadowY){
		if (DynamicFont::CreateDisplayChar(fontName, charCode.mCharCode, style, size, &mCharInfo, outlineSize, shadowX, shadowY)) {
			mCharCode = charCode.mCharCode;
			mCharInfo->mByteCount = charCode.mByteCount;
			if (mCharCode ==9){//tab \t
				mFrameWith = mCharInfo->mAdvanceX * 4;
			}else if (mCharCode ==32){//space
				mFrameWith = mCharInfo->mAdvanceX;
			}else{
				mFrameWith = mCharInfo->mAdvanceX;
			}
			return true;
		}
		return false;
	}

	bool DisplayChar::InitCharInfoWithFont(DynamicFont*font, const UTF32Char &charCode, CharStyle style, AliceSInt32 size, AliceSInt32 outlineSize, AliceSInt32 shadowX, AliceSInt32 shadowY) {
		if (font->CreateChar(charCode.mCharCode, style, size, &mCharInfo, outlineSize, shadowX, shadowY)) {
			mCharCode = charCode.mCharCode;
			mCharInfo->mByteCount = charCode.mByteCount;
			if (mCharCode == 9) {
				mFrameWith = mCharInfo->mAdvanceX * 4;
			}
			else if (mCharCode == 32) {
				mFrameWith = mCharInfo->mAdvanceX;
			}
			else {
				mFrameWith = mCharInfo->mAdvanceX;
			}
			return true;
		}
		return false;
	}
}