#include "Runtime/2D/Component2D.h"
#include "Runtime/Math/Vector3.h"
#include "Runtime/Geometry/Ray.h"
#include "Runtime/Geometry/Intersection.h"
#include "Runtime/Scene/SceneManager.h"
#include "Runtime/Scene/GameObject.h"
namespace Alice{
	Component2D::Component2D(){
		//默认的Anchor在图片的中心
		mbEnableEventRect = false;
		mbEnableClickedEvent = true;
		memset(&mBox, 0, sizeof(mBox));
	}
	Component2D::~Component2D(){
		if (mbEnableClickedEvent){
			GetEventDispatcher()->RemoveEventListener(mOwner);
		}
	}
	void Component2D::SetColliderBox(Vector2f *points){
	}
	void Component2D::EnableClickEvent(bool bEnable){
		mbEnableClickedEvent = bEnable;
		if (mbEnableClickedEvent){
			GetEventDispatcher()->AddEventListener(mOwner);
		}else{
			GetEventDispatcher()->RemoveEventListener(mOwner);
		}
	}
	void Component2D::GetRect(Rect<float>&rect) {
		if (mBox.mPoints[0].x > mBox.mPoints[1].x) {
			rect.Set(mBox.mPoints[1].x, mBox.mPoints[1].y, mBox.mPoints[0].x - mBox.mPoints[1].x, mBox.mPoints[2].y - mBox.mPoints[0].y);

		}
		else {
			rect.Set(mBox.mPoints[0].x, mBox.mPoints[0].y, mBox.mPoints[1].x - mBox.mPoints[0].x, mBox.mPoints[2].y - mBox.mPoints[0].y);
		}
	}
	bool Component2D::HitTest(int x, int y){
		if (!mbEnableClickedEvent){
			return false;
		}
		if (mbEnableEventRect){//如果启动了响应范围的检测，那么就先判断鼠标坐标范围是不是在响应范围内
			if (false==mValidEventRect.Contains(x, y)) {
				return false;
			}
		}
		Rect<float> rect;
		GetRect(rect);
		if (rect.Contains(x, y)) {
			return true;
		}
		return false;
	}

	int Component2D::API_EnableClickEvent(lua_State*L)
	{
		Component2D*ptr = TO_USERDATA(L, Component2D, 1);
		ptr->EnableClickEvent(lua_toboolean(L, 2)==0?false:true);
		return 0;
	}
	int Component2D::API_EnableEventRect(lua_State*L)
	{
		Component2D*ptr = TO_USERDATA(L, Component2D, 1);
		ptr->mbEnableEventRect = lua_toboolean(L, 2);
		return 0;
	}
	int Component2D::API_SetEventRect(lua_State*L)
	{
		Component2D*ptr = TO_USERDATA(L, Component2D, 1);
		float left_bottom_x = lua_tonumber(L, 2);
		float left_bottom_y = lua_tonumber(L, 3);
		float rect_width = lua_tonumber(L, 4);
		float rect_height = lua_tonumber(L, 5);
		ptr->mValidEventRect.Set(left_bottom_x, left_bottom_y, rect_width, rect_height);
		return 0;
	}

	int Component2D::Export(lua_State*L)
	{
		luaL_Reg api_m[] = {
			EXPORT_API(EnableClickEvent)
			EXPORT_API(EnableEventRect)
			EXPORT_API(SetEventRect)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, Component2D, api_m);
		SET_PARENT(L,Component);
		return 0;
	}
}