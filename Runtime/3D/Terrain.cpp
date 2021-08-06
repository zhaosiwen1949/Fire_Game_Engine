
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
#include "Runtime/Exported/IO/ExportedImageResource.h"
#include "Terrain.h"
namespace Alice{
	Terrain::Terrain() :mRes(nullptr){
		mHeightValueCountForTilePerRow = 64;
		mFirstTileSize = mHeightValueCountForTilePerRow-1;
		mTileCountForXAxis = 0;
		mTileCountForZAxis = 0;
		mXValueScale = 1.0f;
		mYValueScale = 1.0f;
		mZValueScale = 1.0f;
	}
	Terrain::~Terrain(){
	}
	void Terrain::InitWidthHeightData(AliceUInt8*heights, int image_width, int image_height) {
		mTileCountForXAxis = image_width / mHeightValueCountForTilePerRow;
		mTileCountForZAxis = image_height / mHeightValueCountForTilePerRow;
		float offset_z = 0.0f;
		for (int z = 0; z < mTileCountForZAxis; ++z) {
			float offset_x = 0.0f;
			float delta_z = 0.0f;
			for (int x=0;x< mTileCountForXAxis;++x){
				TerrainTile*terrain_tile = new TerrainTile;
				terrain_tile->mTerrain = this;
				terrain_tile->mLocalPosition.Set(offset_x, 0.0f, offset_z);
				terrain_tile->InitWidthHeightData(heights, image_width, image_height, x * mHeightValueCountForTilePerRow, z * mHeightValueCountForTilePerRow,mHeightValueCountForTilePerRow);
				mTerrainTiles.push_back(terrain_tile);
				offset_x += terrain_tile->mExtendLenOnXAxis;
				delta_z = terrain_tile->mExtendLenOnZAxis;
			}
			offset_z += delta_z;
		}
	}
	void Terrain::SetMaterial(Material*material) {
		for (auto iter = mTerrainTiles.begin(); iter != mTerrainTiles.end(); ++iter) {
			Material*ret = CreateExportedAliceType<ExportedMaterial>();
			*ret = *material;
			ret->mbSharedMaterial = false;
			(*iter)->SetMaterial(ret);
		}
	}
	void Terrain::SetXYZScale(float xscale, float yscale, float zscale) {
		mXValueScale = xscale;
		mYValueScale = yscale;
		mZValueScale = zscale;
	}
	void Terrain::GetInfo(float world_pos_x, float world_pos_z, Vector3f&n, float&height) {
		const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
		world_pos_x -= world_matrix.mData[12];
		world_pos_z -= world_matrix.mData[14];
		n.Set(0.0f, 1.0f, 0.0f);
		height = 0.0f;
		if (world_pos_x < 0.0f || world_pos_z < 0.0f) {
			return;
		}
		TerrainTile*terrain_tile = GetTerrainTile(world_pos_x, world_pos_z);
		if (terrain_tile == nullptr) {
			return;
		}
		terrain_tile->GetInfo(world_pos_x - terrain_tile->mLocalPosition.x, world_pos_z - terrain_tile->mLocalPosition.z, n, height);
	}
	void Terrain::GetNormal(float world_pos_x, float world_pos_z, Vector3f&n) {
		const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
		world_pos_x -= world_matrix.mData[12];
		world_pos_z -= world_matrix.mData[14];
		n.Set(0.0f, 1.0f, 0.0f);
		if (world_pos_x < 0.0f || world_pos_z < 0.0f) {
			return ;
		}
		TerrainTile*terrain_tile = GetTerrainTile(world_pos_x, world_pos_z);
		if (terrain_tile == nullptr) {
			return ;
		}
		terrain_tile->GetNormal(world_pos_x - terrain_tile->mLocalPosition.x, world_pos_z - terrain_tile->mLocalPosition.z, n);
	}
	float Terrain::GetHeight(float world_pos_x, float world_pos_z) {
		const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
		world_pos_x -= world_matrix.mData[12];
		world_pos_z -= world_matrix.mData[14];
		if (world_pos_x<0.0f||world_pos_z<0.0f){
			return 0.0f;
		}
		TerrainTile*terrain_tile = GetTerrainTile(world_pos_x,world_pos_z);
		if (terrain_tile==nullptr){
			return 0.0f;
		}
		float height= terrain_tile->GetHeight(world_pos_x - terrain_tile->mLocalPosition.x, world_pos_z - terrain_tile->mLocalPosition.z);
		return height;
	}
	TerrainTile*Terrain::GetTerrainTile(float terrain_pos_x, float terrain_pos_z) {
		if (terrain_pos_x < mFirstTileSize) {
			if (terrain_pos_z < mFirstTileSize) {
				return mTerrainTiles[0];
			}
			terrain_pos_z -= mFirstTileSize;
			int z_tile_index = FloorfToInt(terrain_pos_z / (mFirstTileSize + 1.0f)) + 1;
			int target_tile_index = z_tile_index * mTileCountForXAxis;
			return mTerrainTiles[target_tile_index];
		}
		else {
			if (terrain_pos_z < mFirstTileSize) {
				terrain_pos_x -= mFirstTileSize;
				int x_tile_index = FloorfToInt(terrain_pos_x / (mFirstTileSize + 1.0f)) + 1;
				return mTerrainTiles[x_tile_index];
			}
			terrain_pos_x -= mFirstTileSize;
			terrain_pos_z -= mFirstTileSize;
			int x_tile_index = FloorfToInt(terrain_pos_x / (mFirstTileSize + 1.0f)) + 1;
			int z_tile_index = FloorfToInt(terrain_pos_z / (mFirstTileSize + 1.0f)) + 1;
			int target_tile_index = z_tile_index * mTileCountForXAxis + x_tile_index;
			return mTerrainTiles[target_tile_index];
		}
		return nullptr;
	}
	void Terrain::Update(float deltaTime){
		if (mbPositionChanged) {
			const Matrix4x4&world_matrix = mOwner->GetWorldMatrix();
			Matrix3x3 rotate_matrix(world_matrix);
			rotate_matrix.InvertTranspose();
			Matrix4x4 normal_matrix(rotate_matrix);
			for (auto iter = mTerrainTiles.begin(); iter != mTerrainTiles.end(); ++iter) {
				Matrix4x4 model_matrix = world_matrix;
				TerrainTile*terrain_tile = *iter;
				model_matrix.LocalTranslate(terrain_tile->mLocalPosition);
				terrain_tile->UpdateMatrix(model_matrix, normal_matrix);
			}
			mbPositionChanged = false;
		}
	}
	void Terrain::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	){
		for (auto iter=mTerrainTiles.begin();iter!=mTerrainTiles.end();++iter){
			(*iter)->Render(rq,mOwner);
		}
	}
	void Terrain::RenderShadowMap(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	) {
		for (auto iter = mTerrainTiles.begin(); iter != mTerrainTiles.end(); ++iter) {
			(*iter)->RenderShadowMap(rq, mOwner);
		}
	}
}
