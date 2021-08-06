#include "ExportedMesh.h"
//2016 08/24 2.8.7 pass 
namespace Alice
{
	int ExportedMesh::Export(lua_State*L)
	{
		luaL_Reg apis[] = {
			EXPORT_API(SetVertexCount)
			EXPORT_API(UpdatePosition)
			EXPORT_API(UpdateTexcoord)
			EXPORT_API(UpdateNormal)
			EXPORT_API(UpdateTangent)
			EXPORT_API(UpdateTexcoord1)
			EXPORT_API(SetSubMeshCount)
			EXPORT_API(SetSubMeshIndexCount)
			EXPORT_API(UpdateSubMeshIndex)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Mesh, apis);
		SET_PARENT(L, Component);
		luaL_Reg api_c[] = 
		{ 
			EXPORT_API(ClassID)
			{ NULL,NULL } 
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Mesh, api_c);
		/*lua_pushnumber(L,GL_TRIANGLES);
		lua_setfield(L, -2, "TRIANGLES");
		lua_pushnumber(L, GL_TRIANGLE_FAN);
		lua_setfield(L, -2, "TRIANGLE_FAN");
		lua_pushnumber(L, GL_TRIANGLE_STRIP);
		lua_setfield(L, -2, "TRIANGLE_STRIP");
#ifdef ALICE_IPHONE
#elif ALICE_ANDROID
#else
        lua_pushnumber(L, GL_QUADS);
        lua_setfield(L, -2, "QUADS");
        lua_pushnumber(L, GL_QUAD_STRIP);
        lua_setfield(L, -2, "QUAD_STRIP");
        lua_pushnumber(L, GL_POLYGON);
        lua_setfield(L, -2, "POLYGON");
#endif
		lua_pushnumber(L, GL_POINTS);
		lua_setfield(L, -2, "POINTS");
		lua_pushnumber(L, GL_LINES);
		lua_setfield(L, -2, "LINES");
		lua_pushnumber(L, GL_LINE_STRIP);
		lua_setfield(L, -2, "LINE_STRIP");
		lua_pushnumber(L, GL_LINE_LOOP);
		lua_setfield(L, -2, "LINE_LOOP");*/
		return 0;
	}
	int ExportedMesh::API_UpdatePosition(lua_State*L)
	{
		ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -5);
		ptr->UpdateVertexPosition(lua_tointeger(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1));
		return 0;
	}

	int ExportedMesh::API_UpdateTexcoord(lua_State*L)
	{
		int argCount = lua_gettop(L);
		if (argCount == 4) {
			ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -4);
			ptr->UpdateVertexTexcoord(lua_tointeger(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1), 0.0f);
		}
		else if (argCount == 5) {
			ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -5);
			ptr->UpdateVertexTexcoord(lua_tointeger(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1));
		}
		else if (argCount == 6) {
			ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -6);
			ptr->UpdateVertexTexcoord(lua_tointeger(L, -5), lua_tonumber(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1));
		}
		else {
			errorC("Mesh:UpdateTexcoord  args count error,current is %d", lua_gettop(L));
		}
		return 0;
	}

	int ExportedMesh::API_UpdateNormal(lua_State*L)
	{
		ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -5);
		ptr->UpdateVertexNormal(lua_tointeger(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1));
		return 0;
	}

	int ExportedMesh::API_UpdateTangent(lua_State*L)
	{
		ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -6);
		ptr->UpdateVertexTangent(lua_tointeger(L, -5), lua_tonumber(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1));
		return 0;
	}

	int ExportedMesh::API_UpdateTexcoord1(lua_State*L)
	{
		int argCount = lua_gettop(L);
		if (argCount == 4) {
			ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -4);
			ptr->UpdateVertexTexcoord1(lua_tointeger(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1), 0.0f);
		}
		else if (argCount == 5) {
			ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -5);
			ptr->UpdateVertexTexcoord1(lua_tointeger(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1));
		}
		else if (argCount == 6) {
			ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -6);
			ptr->UpdateVertexTexcoord1(lua_tointeger(L, -5), lua_tonumber(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1));
		}
		else {
			errorC("Mesh:UpdateTexcoord1  args count error,current is %d", lua_gettop(L));
		}
		return 0;
	}

	int ExportedMesh::API_SetVertexCount(lua_State*L)
	{
		ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -2);
		ptr->SetVertexCount(lua_tointeger(L, -1));
		return 0;
	}

	int ExportedMesh::API_SetSubMeshCount(lua_State*L)
	{
		ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -2);
		ptr->SetSubMeshCount(lua_tointeger(L, -1));
		return 0;
	}


	int ExportedMesh::API_SetSubMeshIndexCount(lua_State*L)
	{
		ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -3);
		ptr->SetSubMeshIndexCount(lua_tointeger(L, -2),lua_tointeger(L,-1));
		return 0;
	}
	int ExportedMesh::API_UpdateSubMeshIndex(lua_State*L)
	{
		ExportedMesh*ptr = TO_USERDATA(L, ExportedMesh, -4);
		ptr->UpdateSubMeshIndex(lua_tointeger(L, -3), (AliceUInt16)lua_tointeger(L, -2), (AliceUInt16)lua_tointeger(L, -1));
		return 0;
	}
}
