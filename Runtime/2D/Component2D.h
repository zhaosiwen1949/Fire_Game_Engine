#pragma once
#include "Runtime/Base/Transform.h"
#include "Runtime/Math/Rect.h"
namespace Alice{
	struct ColliderBox2D{
		Vector3f mPoints[4];
	};
	class Component2D:public Component
	{
	public:
		Component2D();
		virtual ~Component2D();
		Rect<float> mValidEventRect;//2D��������Ӧ�¼������귶Χ�����������һ��Χ����ô�Ͳ����¼�������Ӧ
		bool mbEnableEventRect;
		void GetRect(Rect<float>&rect);
		bool HitTest(int x, int y);
		void EnableClickEvent(bool bEnable);
		void SetColliderBox(Vector2f *points);
		DEFINE_LUA_API(EnableEventRect);
		DEFINE_LUA_API(SetEventRect);
		DEFINE_LUA_API(EnableClickEvent);
		static int Export(lua_State*L);
		bool mbEnableClickedEvent;
		ColliderBox2D mBox;
	public:
		DECLEAR_ALICE_CLASS(Component2D)
	};
}
