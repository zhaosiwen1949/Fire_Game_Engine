#pragma once
#include "ImageControl.h"
#include <functional>

namespace Editor
{
	enum ShowSelectedMode
	{
		kShowSelectedModeAddColor,
		kShowSelectedModeShowBkgColor
	};
	class SelectableImageControl :public ImageControl
	{
	protected:
		bool mbSelected;
		std::function<void()> mOnClickHandler;
		ShowSelectedMode mShowSelectedMode;
		Color mBKGColor;
		static ImageAttributes*mImageAttributeWhenSelected;
	public:
		SelectableImageControl();
		void Select();
		void Unselect();
		bool IsSelected();
		void SetShowSelectedMode(ShowSelectedMode selectedMode);
		void SetBKGColor(Color&color);

		Control*OnClickDown(int x, int y);
		Control*OnClickUp(int x, int y);
		void ResetState();
		void SetOnClick(std::function<void()>onClicked);
	public:
		virtual void Draw(Graphics&painter);
	};
}