#include "StaticText.h"
#include "Runtime/String/StringUtils.h"
#pragma warning(disable:4244)
namespace Editor
{
	Font* StaticText::SharedFont = nullptr;
	StringFormat*StaticText::SharedStringFormat = nullptr;
	StaticText::StaticText():mBkgColor(0, 100, 150),mTextColor(200, 200, 200),mbDrawBkgColor(false),mAligning(AligningModeLeft)
	{
		if (SharedFont==nullptr)
		{
			static FontFamily fontfamily(TEXT("宋体"));
			SharedFont = new Font(&fontfamily, 12, FontStyleRegular, UnitPixel);

			SharedStringFormat = new StringFormat;
			SharedStringFormat->SetAlignment(StringAlignmentNear);
			SharedStringFormat->SetTrimming(StringTrimmingEllipsisCharacter);
		}
	}

	void StaticText::SetBkgColor(BYTE r, BYTE g, BYTE b)
	{
		mBkgColor = Color(r,g,b);
	}

	void StaticText::SetBkgColorRect(int x, int y, int width, int height)
	{
		mBkgColorRect = Rect(x,y,width,height);
	}

	bool StaticText::TestIntersect(int x, int y)
	{
		if (mBkgColorRect.Contains(x, y))
		{
			mIntersectPos = ControlIntersectPosMiddle;
			if (y <= mBkgColorRect.Y + 2)
			{
				mIntersectPos = ControlIntersectPosUpper;
			}
			else if (y >= mBkgColorRect.Y + mBkgColorRect.Height - 2)
			{
				mIntersectPos = ControlIntersectPosBottom;
			}
			return true;
		}
		return false;
	}


	void StaticText::AdjustRect(int x, int y, int width, int height)
	{
		StringFormat sf;
		sf.SetAlignment(StringAlignmentNear);

		RectF bound;

	}

	bool StaticText::operator==(StaticText&right)
	{
		return 0 == strcmp(right.GetText(), mText);
	}

	bool StaticText::operator ==(const char*text)
	{
		return strcmp(mText, text) == 0;
	}

	bool StaticText::operator !=(const char*text)
	{
		return strcmp(mText, text) != 0;
	}

	void StaticText::SetTextColor(BYTE r, BYTE g, BYTE b)
	{
		mTextColor = Color(r,g,b);
	}

	bool StaticText::ShowBkgColor(bool bShow)
	{
		if (mbDrawBkgColor!=bShow)
		{
			mbDrawBkgColor = bShow;
			return true;
		}
		return false;
	}

	bool StaticText::IsShowBkgColor()
	{
		return mbDrawBkgColor;
	}

	Color&StaticText::GetTextColor()
	{
		return mTextColor;
	}

	void StaticText::SetText(const char* text)
	{
		memset(mText, 0, 128);
		if (strlen(text)>127)
		{
			strncpy(mText, text, 124);
			strcat(mText, "...");
		}
		else
		{
			strcpy(mText, text);
		}
		wmemset(mWText, 0, 128);
		Alice::StringUtils::UTF8ToUnicode(mText, mWText);
	}

	void StaticText::operator=(const char*text)
	{
		memset(mText, 0, 128);
		strcpy(mText, text);
		wmemset(mWText, 0, 128);
		Alice::StringUtils::ASCIIToUnicode(mText, mWText);
	}

	void StaticText::operator=(LPCTSTR text)
	{
		wmemset(mWText, 0, 128);
		lstrcpy(mWText, text);
		memset(mText, 0, 128);
		Alice::StringUtils::UnicodeToASCII( mWText, mText,128);
	}

	void StaticText::SetAligning(AligningMode aligning)
	{
		mAligning=aligning;
	}

	const char* StaticText::GetText()
	{
		return mText;
	}

	LPCTSTR StaticText::GetWText()
	{
		return mWText;
	}

	void StaticText::Draw(Graphics&painter)
	{
		painter.SetSmoothingMode(SmoothingModeAntiAlias);
		painter.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

		SolidBrush brush(mBkgColor);
		if (mbDrawBkgColor)
		{
			painter.FillRectangle(&brush, mBkgColorRect);
		}
		//最后在自己的Folder@16的Icon旁边协商自己的文件夹名字
		brush.SetColor(mTextColor);
		//如果被选中，则绘制一个蓝色背景条
		if (mAligning==AligningModeLeft)
		{
			painter.DrawString(mWText, -1, SharedFont, Gdiplus::PointF(mRect.X, mRect.Y + 2), SharedStringFormat, &brush);
		}
		else if(mAligning==AligningModeMiddle)
		{
			RectF rect;
			painter.MeasureString(mWText,lstrlen(mWText),SharedFont,PointF(0,0),SharedStringFormat,&rect);
			int offsetX=mRect.X+mRect.Width / 2-rect.Width/2;
			painter.DrawString(mWText, -1, SharedFont, Gdiplus::PointF(offsetX, mRect.Y + 2), SharedStringFormat, &brush);
		}
		Control::Draw(painter);
	}
}