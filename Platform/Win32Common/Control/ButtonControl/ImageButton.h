#pragma once
#include "Platform/Win32Common/Control/ImageControl/ImageControl.h"
#include <functional>

namespace Editor
{
	class ImageButton :public ImageControl
	{
	protected:
		std::function<void()> mOnClickHandler;
		Point mOriginalPos;
		bool mbIsDown;
	public:
		ImageButton();
		Control*OnClickDown(int x,int y);
		Control*OnClickUp(int x,int y);
		void ResetState();
		bool IsDown();
		void SetOnClick(std::function<void()>onClicked);
	};
}