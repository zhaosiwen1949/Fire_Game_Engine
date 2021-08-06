#pragma once
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/2D/Component2D.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Math/Rect.h"

namespace Alice{
	//9宫格的图片显示方式，之所以不让九宫格继承ImageSprite，是因为这样独立写九宫格的图像会效率高一点
	class Canvas :public Component{
	public:
		Vector4f mCanvasInfo;//x : width , y : height , z : max_depth
		Rect<float> mRect;
		int mOrder;
	public:
		Canvas();
		~Canvas();
		void SetOrder(int order);
		void Set(float width, float height, float depth);
		void SetDepthRange(float depth);
	public:
		static void OnViewportChanged(float width,float height);
		static std::vector<Canvas*> mCanvases;
		static bool SortCanvas(const Canvas *l, const Canvas *r) {
			if (l->mOrder < r->mOrder) {
				return true;
			}
			else {
				return false;
			}
		}
	public:
		DECLEAR_ALICE_CLASS(Canvas)
	};
}