#pragma once
#include "Runtime/RuntimePrefix.h"

namespace Editor
{
	class BaseWindow;
	enum SplitLinePos
	{
		SplitLinePosLeft,
		SplitLinePosRight,
		SplitLinePosUpper,
		SplitLinePosBottom
	};

	class SplitLine
	{
	protected:
		Rect mRect;
		std::vector<BaseWindow*> mLeftWindows;
		std::vector<BaseWindow*> mRightWindows;
		std::vector<BaseWindow*> mUpperWindows;
		std::vector<BaseWindow*> mBottomWindows;
	protected:
		static void AddWindow(std::vector<BaseWindow*>&windows,BaseWindow*newWindow);
		static void RemoveWindow(std::vector<BaseWindow*>&windows, BaseWindow*window);
		static bool Contains(std::vector<BaseWindow*>&windows, BaseWindow*window);
	public:
		BaseWindow*mRefWindow;
		SplitLine();
		std::string mCursor;
		bool mbHorizontal;
		void SetRect(int x,int y,int width,int height);
		void RemoveWindow(SplitLinePos pos,BaseWindow*window);
		void AddWindow(SplitLinePos pos,BaseWindow*window);
		bool IsHoveringOnSplitLine(int containerSpaceCursorX,int containerSpaceCursorY);
		bool ContainsIn(int windowPos,BaseWindow*baseWindow);
		std::vector<BaseWindow*>&LeftWindows();
		std::vector<BaseWindow*>&RightWindows();
		std::vector<BaseWindow*>&UpperWindows();
		std::vector<BaseWindow*>&BottomWindows();
		Rect&GetRect();
	};
}