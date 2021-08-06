#pragma once
#include "Runtime/Utils/LinkedList.h"
#include "Runtime/Render/AliceGL.h"

namespace Alice
{
	typedef void(*SetUniformFoo)(GLint loc, void*camera, void*userData);
	class UniformUpdater :public LinkedList
	{
	public:
		GLint mLocation;
		SetUniformFoo mFoo;
		void*mUserData;
		UniformUpdater(SetUniformFoo foo, GLint loc,void*userData);
		UniformUpdater(UniformUpdater*uniform);
		~UniformUpdater();
		void Update(void* param);
		void operator=(UniformUpdater&r);
		static void UpdateTextureUnit(GLint loc, void*camera, void*userData);
		static void UpdateProjectionMatrix(GLint loc, void*camera, void*userData);
		static void UpdateViewMatrix(GLint loc, void*camera, void*userData);
		static void UpdateCanvas(GLint loc, void*canvas, void*userData);
		static void UpdateFloat(GLint loc, void*camera, void*userData);
		static void UpdateVec2(GLint loc, void*camera, void*userData);
		static void UpdateVec3(GLint loc, void*camera, void*userData);
		static void UpdateVec4(GLint loc, void*camera, void*userData);
		static void UpdateMat3(GLint loc, void*camera, void*userData);
		static void UpdateMat4(GLint loc, void*camera, void*userData);
		static void UpdateIntArray(GLint loc, void*camera, void*userData);
		static void UpdateVec4Array(GLint loc, void*camera, void*userData);
	};
}