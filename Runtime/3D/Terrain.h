#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Geometry/Geometry.h"
#include "TerrainTile.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/IO/ImageResource.h"
namespace Alice{
	class ImageResource;
	class Terrain :public Component{
	public:
		std::vector<TerrainTile*>mTerrainTiles;
		SmartPtr<ImageResource> mRes;
		int mHeightValueCountForTilePerRow;
		float mFirstTileSize;
		int mTileCountForXAxis;
		int mTileCountForZAxis;
		float mXValueScale,mYValueScale,mZValueScale;
	public:
		Terrain();
		~Terrain();
		void InitWidthHeightData(AliceUInt8*heights, int image_width, int image_height);
		void SetMaterial(Material*mat);
		void SetXYZScale(float xscale, float yscale, float zscale);
		float GetHeight(float world_pos_x, float world_pos_z);
		void GetNormal(float world_pos_x, float world_pos_z, Vector3f&n);
		void GetInfo(float world_pos_x, float world_pos_z, Vector3f&n,float&height);
		TerrainTile*GetTerrainTile(float terrain_pos_x, float terrain_pos_z);
		void Update(float deltaTime);
		void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		void RenderShadowMap(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
	public:
		DECLEAR_ALICE_CLASS(Terrain)
	};
}