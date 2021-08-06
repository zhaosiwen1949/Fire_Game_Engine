#pragma once
#include "Runtime/Base/Object.h"
#include "Runtime/Utils/LinkedList.h"
#include "Runtime/Font/DynamicFont.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/Render/DrawCall.h"
#include "Runtime/Scene/RendererManager.h"
#include "Runtime/Font/CharInfo.h"
#include "Runtime/String/StringUtils.h"

namespace Alice{
	class DisplayChar :public DoubleLinkedList{
	public:
		CharInfo*mCharInfo;
		AliceUInt32 mCharCode;
		bool mbCustomColor;
		float mFrameWith;
		Color4B mColor;
		AliceUInt16 mIndex;
		DisplayChar();
		virtual ~DisplayChar();
		float GetFrameWidth();

		float GetCharLeft(float startPoint);
		float GetCharRight(float startPoint);

		bool InitCharInfoWithFontName(const char*fontName, const UTF32Char &charCode, CharStyle style, AliceSInt32 size, AliceSInt32 outlineSize, AliceSInt32 shadowX, AliceSInt32 shadowY);
		bool InitCharInfoWithFont(DynamicFont*font, const UTF32Char &charCode, CharStyle style, AliceSInt32 size, AliceSInt32 outlineSize, AliceSInt32 shadowX, AliceSInt32 shadowY);
		void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
	};
}
