#include "SplitLine.h"

namespace Editor
{
	SplitLine::SplitLine()
	{
		mbHorizontal = true;
	}

	void SplitLine::SetRect(int x, int y, int width, int height)
	{
		mRect.X = x;
		mRect.Y = y;
		mRect.Width = width;
		mRect.Height = height;
	}
	
	Rect& SplitLine::GetRect()
	{
		return mRect;
	}

	void SplitLine::AddWindow(std::vector<BaseWindow*>&windows, BaseWindow*newWindow)
	{
		for (std::vector<BaseWindow*>::iterator iter = windows.begin(); iter != windows.end();iter++)
		{
			if (*iter==newWindow)
			{
				return;
			}
		}
		windows.push_back(newWindow);
	}

	void SplitLine::RemoveWindow(std::vector<BaseWindow*>&windows, BaseWindow*window)
	{
		for (std::vector<BaseWindow*>::iterator iter = windows.begin(); iter != windows.end(); iter++)
		{
			if (*iter == window)
			{
				windows.erase(iter);
				break;
			}
		}
	}

	bool SplitLine::Contains(std::vector<BaseWindow*>&windows, BaseWindow*window)
	{
		for (std::vector<BaseWindow*>::iterator iter = windows.begin(); iter != windows.end(); iter++)
		{
			if (*iter == window)
			{
				return true;
			}
		}
		return false;
	}


	void SplitLine::AddWindow(SplitLinePos pos, BaseWindow*window)
	{
		switch (pos)
		{
		case Editor::SplitLinePosLeft:
			AddWindow(mRightWindows, window);
			break;
		case Editor::SplitLinePosRight:
			AddWindow(mLeftWindows, window);
			break;
		case Editor::SplitLinePosUpper:
			AddWindow(mBottomWindows, window);
			break;
		case Editor::SplitLinePosBottom:
			AddWindow(mUpperWindows, window);
			break;
		default:
			break;
		}
	}

	bool SplitLine::IsHoveringOnSplitLine(int containerSpaceCursorX, int containerSpaceCursorY)
	{
		return mRect.Contains(containerSpaceCursorX,containerSpaceCursorY)?true:false;
	}

	bool SplitLine::ContainsIn(int pos, BaseWindow*window)
	{
		switch (pos)
		{
		case 1:
			return Contains(mLeftWindows, window);
		case 2:
			return Contains(mRightWindows, window);
		case 3:
			return Contains(mUpperWindows, window);
		case 4:
			return Contains(mBottomWindows, window);
		}
		return false;
	}

	void SplitLine::RemoveWindow(SplitLinePos pos, BaseWindow*window)
	{
		switch (pos)
		{
		case Editor::SplitLinePosLeft:
			RemoveWindow(mRightWindows, window);
			break;
		case Editor::SplitLinePosRight:
			RemoveWindow(mLeftWindows, window);
			break;
		case Editor::SplitLinePosUpper:
			RemoveWindow(mBottomWindows, window);
			break;
		case Editor::SplitLinePosBottom:
			RemoveWindow(mUpperWindows, window);
			break;
		default:
			break;
		}
	}

	std::vector<BaseWindow*>&SplitLine::LeftWindows()
	{
		return mLeftWindows;
	}

	std::vector<BaseWindow*>&SplitLine::RightWindows()
	{
		return mRightWindows;
	}

	std::vector<BaseWindow*>&SplitLine::UpperWindows()
	{
		return mUpperWindows;
	}

	std::vector<BaseWindow*>&SplitLine::BottomWindows()
	{
		return mBottomWindows;
	}
}