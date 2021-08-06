#pragma once
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/Base/Component.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/Render/IndexBuffer.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/GL20/EBO.h"
#include "Runtime/Render/GL20/VBO.h"
#include "Runtime/Geometry/Geometry.h"
namespace Alice{
	class Terrain;
	class Material;
	class TerrainTile :public Object{
	public:
		VertexData mVertexData;
		IndexBuffer mIndexBuffer;
		VBO mVBO;
		EBO	mEBO;
		SmartPtr<Material> mMaterial;
		Vector3f mLocalPosition;
		int mPointCountForXAxis;
		int mPointCountForZAxis;
		float mExtendLenOnXAxis;
		float mExtendLenOnZAxis;
		AABB mAABB;
		Terrain*mTerrain;
	public:
		TerrainTile();
		~TerrainTile();
		void InitWidthHeightData(AliceUInt8*heights,int image_width,int image_height,int start_left_top_X,int start_left_top_Y,int size=64);
		void GetVertexNormal(AliceUInt8*heights, int image_width, int image_height, int x, int y,Vector4f&n);
		AliceUInt8 GetHeight(AliceUInt8*heights, int image_width, int image_height, int x, int y);
		void SetMaterial(Material*material);
		void SubmitData();
		void UpdateMatrix(const Matrix4x4&model_matrix, const Matrix4x4&normal_matrix);
		float GetHeight(float tile_pos_x,float tile_pos_z);
		void GetNormal(float tile_pos_x, float tile_pos_z, Vector3f&n);
		void GetInfo(float tile_pos_x, float tile_pos_z, Vector3f&n, float&height);
		void Render(RenderQueue*rq, GameObject*go);
		void RenderShadowMap(RenderQueue*rq, GameObject*go);
		float GetDistanceFromCamera(RenderQueue*rq);
	public:
		DECLEAR_ALICE_CLASS(TerrainTile)
	};
}