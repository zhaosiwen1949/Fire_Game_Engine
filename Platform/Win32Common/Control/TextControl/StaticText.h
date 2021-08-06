#pragma once
#include "../Control.h"

namespace Editor
{
	enum AligningMode
	{
		AligningModeLeft,
		AligningModeMiddle
	};
	class StaticText:public Control
	{
	protected:
		char mText[128];
		TCHAR mWText[128];
		Rect mBkgColorRect;
		Color mBkgColor;
		bool mbDrawBkgColor;
		Color mTextColor;
		AligningMode mAligning;
	public:
		StaticText();
		void SetAligning(AligningMode aligning);
		void SetText(const char* text);
		const char* GetText();
		LPCTSTR GetWText();
		void SetTextColor(BYTE r,BYTE g,BYTE b);
		Color&GetTextColor();
		void SetBkgColor(BYTE r, BYTE g, BYTE b);
		bool ShowBkgColor(bool bShow);
		bool IsShowBkgColor();
		void SetBkgColorRect(int x,int y,int width,int height);
		void AdjustRect(int x,int y,int width,int height);

		virtual bool TestIntersect(int x, int y);
		bool operator==(StaticText&right);
		bool operator==(const char*text);
		bool operator!=(const char*text);
		void operator=(const char*text);
		void operator=(LPCTSTR text);
	public:
		virtual void Draw(Graphics&painter);
	public:
		static Font *SharedFont;
		static StringFormat*SharedStringFormat;
	};
}