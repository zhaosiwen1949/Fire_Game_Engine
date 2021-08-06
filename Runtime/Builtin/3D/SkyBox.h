#pragma once
#include "Runtime/Render/GL20/TextureCube.h"
#include "Runtime/Render/GL20/VBO.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/VertexData.h"

namespace Alice{
	class Material;
	class SkyBox{
	public:
		SkyBox();
		void Render();
		AliceAny mCamera;
		VBO mVBO;
		VertexData *mVertexData;
		SmartPtr<Material> mMaterial;
		SmartPtr<TextureCube> mCubeMap;
	};
}