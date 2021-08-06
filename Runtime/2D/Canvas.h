#pragma once
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/2D/Component2D.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Math/Rect.h"

namespace Alice{
	//9�����ͼƬ��ʾ��ʽ��֮���Բ��þŹ���̳�ImageSprite������Ϊ��������д�Ź����ͼ���Ч�ʸ�һ��
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