#include "Object.h"
namespace Alice {
	Object::Object(int autorelease, int name_len) :mReferenceCount(0), mObjectMask(autorelease) {
		if (name_len > 0) {
			mName.Resize(name_len);
			mName = "Object";
		}
	}

	Object::~Object() {
	}
	void Object::retain() {
		mReferenceCount++;
	}
	void Object::release() {
		mReferenceCount--;
		if (mReferenceCount == 0) {
			if (mObjectMask == 1) {
				delete this;
			}
		}
	}
	void Object::GetUserData(lua_State*L) {
		mLuaData = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_getref(L, mLuaData);
	}
	bool Object::IsValid(lua_State*L, int index) {
		bool ret = false;
		lua_getmetatable(L, index);
		if (lua_istable(L, -1)) {
			lua_pop(L, 1);
			ret = true;
		}
		return ret;
	}
	int Object::DefaultGC(lua_State*L) {
		Object*ptr = TO_USERDATA(L, Object, -1);
		ptr->release();
		return 0;
	}
}