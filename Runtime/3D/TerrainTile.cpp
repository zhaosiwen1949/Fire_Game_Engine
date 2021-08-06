#include "Runtime/Scene/GameObject.h"
#include "Runtime/Math/Vector3.h"
#include "Runtime/Math/Vector4.h"
#include "Runtime/Geometry/Intersection.h"
#include "Runtime/Render/Material.h"
#include "Runtime/Render/DrawCall.h"
#include "Runtime/Render/Camera.h"
#include "TerrainTile.h"
#include "Terrain.h"
namespace Alice{
	float TerrainHeightFromBarryCentric(const Vector3f&p1, const Vector3f&p2, const Vector3f&p3, const Vector3f&pos) {
		float det = (p2.z - p3.z)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.z - p3.z);
		float _1 = ((p2.z - p3.z)*(pos.x - p3.x) + (p3.x - p2.x)*(pos.z - p3.z)) / det;
		float _2 = ((p3.z - p1.z)*(pos.x - p3.x) + (p1.x - p3.x)*(pos.z - p3.z)) / det;
		float _3 = 1.0f - _1 - _2;
		return _1 * p1.y + _2 * p2.y + _3 * p3.y;
	}
	TerrainTile::TerrainTile(){
		mTerrain = nullptr;
		mMaterial = nullptr;
	}
	TerrainTile::~TerrainTile(){
	}
	void TerrainTile::InitWidthHeightData(
		AliceUInt8*heights, int image_width, int image_height, int start_left_top_X, int start_left_top_Y, int size/* =64 */) {
		int tile_width = size;
		int tile_height = size;
		if (start_left_top_X>0){//need to extend left
			tile_width++;
			start_left_top_X--;
			
		}
		if (start_left_top_Y > 0) {//need to extend upper
			tile_height++;
			start_left_top_Y--;
		}
		mPointCountForXAxis = tile_width;
		mPointCountForZAxis = tile_height;
		mExtendLenOnXAxis = mPointCountForXAxis - 1;
		mExtendLenOnZAxis = mPointCountForZAxis - 1;
		mVertexData.SetBufferSize(tile_width*tile_height);
		mVBO.Init(tile_width*tile_height);
		int max_region_x_index = tile_width - 1;
		int max_region_z_index = tile_height - 1;
		mIndexBuffer.Init(max_region_x_index*max_region_z_index * 6);
		mEBO.Init(max_region_x_index*max_region_z_index * 6);
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		int index_buffer_index = 0;
		MinMaxAABB min_max_aabb;
		for (int z=0;z< tile_height;++z){
			for (int x=0;x< tile_width;++x){
				int vertice_index = tile_width * z + x;
				int image_data_offset_x = start_left_top_X + x;
				int image_data_offset_y = image_height - start_left_top_Y - 1 - z ;
				int image_data_index = image_data_offset_y * image_width + image_data_offset_x;
				GetVertexNormal(heights, image_width, image_height, image_data_offset_x, image_data_offset_y, vertexes[vertice_index].mNormal);
				vertexes[vertice_index].mVertex.Set(x, 100.0f*(float(heights[image_data_index])/255.0f), z, 1.0f);
				min_max_aabb.Encapsulate(vertexes[vertice_index].mVertex);
				vertexes[vertice_index].mTexCoord0.Set(
					float(image_data_offset_x) / float(image_width-1), float(image_data_offset_y) / float(image_height-1),
					float(x+mLocalPosition.x) / float(2.0f), float(z+mLocalPosition.z) / float(2.0f)
				);
				if (z < max_region_z_index && x < max_region_x_index){
					int left_top_vertex_index = z * tile_width + x;
					int left_bottom_vertex_index = (z+1) * tile_width + x;
					mIndexBuffer.mIndexes[index_buffer_index++] = left_bottom_vertex_index;
					mIndexBuffer.mIndexes[index_buffer_index++] = left_bottom_vertex_index+1;
					mIndexBuffer.mIndexes[index_buffer_index++] = left_top_vertex_index + 1;

					mIndexBuffer.mIndexes[index_buffer_index++] = left_bottom_vertex_index;
					mIndexBuffer.mIndexes[index_buffer_index++] = left_top_vertex_index + 1;
					mIndexBuffer.mIndexes[index_buffer_index++] = left_top_vertex_index;
				}
			}
		}
		mAABB=min_max_aabb;
		SubmitData();
	}
	void TerrainTile::GetVertexNormal(AliceUInt8*heights, int image_width, int image_height, int x, int y, Vector4f&n) {
		float heightL = GetHeight(heights,image_width,image_height,x - 1, y);
		float heightR = GetHeight(heights, image_width, image_height, x + 1, y);
		float heightU = GetHeight(heights, image_width, image_height, x, y + 1);
		float heightD = GetHeight(heights, image_width, image_height, x, y - 1);
		Vector3f v(heightL - heightR, 2.0f, heightU - heightD);
		v.Normalize();
		n.Set(v.x, v.y, v.z);
	}
	AliceUInt8 TerrainTile::GetHeight(AliceUInt8*heights, int image_width, int image_height, int x, int y) {
		x = x < 0 ? 0 : x;
		y = y < 0 ? 0 : y;
		x = x >= image_width ? image_width - 1 : x;
		y = y >= image_height ? image_height - 1 : y;
		return heights[y * image_width + x];
	}
	float TerrainTile::GetHeight(float tile_pos_x, float tile_pos_z) {
		int x = FloorfToInt(tile_pos_x);
		int z = FloorfToInt(tile_pos_z);
		float texcoordx = tile_pos_x - x;
		float texcoordz = tile_pos_z - z;
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		if (texcoordx <= (1 - texcoordz)) {
			return TerrainHeightFromBarryCentric(Vector3f(0.0f,vertexes[mPointCountForXAxis * z + x].mVertex.y,0.0f),
				Vector3f(1.0f, vertexes[mPointCountForXAxis*z + x+1].mVertex.y, 0.0f),
				Vector3f(0.0f, vertexes[mPointCountForXAxis*(z+1)+x].mVertex.y, 1.0f),Vector3f(texcoordx,0.0,texcoordz));
		}
		else {
			return TerrainHeightFromBarryCentric(
				Vector3f(1.0f, vertexes[mPointCountForXAxis*z + x + 1].mVertex.y, 0.0f),
				Vector3f(1.0f, vertexes[mPointCountForXAxis*(z + 1) + x + 1].mVertex.y, 1.0f),
				Vector3f(0.0f, vertexes[mPointCountForXAxis*(z + 1) + x].mVertex.y, 1.0f), Vector3f(texcoordx, 0.0, texcoordz));
		}
	}
	void TerrainTile::GetInfo(float tile_pos_x, float tile_pos_z, Vector3f&n, float&height) {
		int x = FloorfToInt(tile_pos_x);
		int z = FloorfToInt(tile_pos_z);
		float texcoordx = tile_pos_x - x;
		float texcoordz = tile_pos_z - z;
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		if (texcoordx <= (1 - texcoordz)) {
			height=TerrainHeightFromBarryCentric(Vector3f(0.0f, vertexes[mPointCountForXAxis * z + x].mVertex.y, 0.0f),
				Vector3f(1.0f, vertexes[mPointCountForXAxis*z + x + 1].mVertex.y, 0.0f),
				Vector3f(0.0f, vertexes[mPointCountForXAxis*(z + 1) + x].mVertex.y, 1.0f), Vector3f(texcoordx, 0.0, texcoordz));
			Vector3f v1 = Vector3f(vertexes[mPointCountForXAxis*(z + 1) + x].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis * z + x].mVertex.v);
			Vector3f v2 = Vector3f(vertexes[mPointCountForXAxis*z + x + 1].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis * z + x].mVertex.v);
			n = Cross(v1, v2);
			n.Normalize();
		}
		else {
			height = TerrainHeightFromBarryCentric(
				Vector3f(1.0f, vertexes[mPointCountForXAxis*z + x + 1].mVertex.y, 0.0f),
				Vector3f(1.0f, vertexes[mPointCountForXAxis*(z + 1) + x + 1].mVertex.y, 1.0f),
				Vector3f(0.0f, vertexes[mPointCountForXAxis*(z + 1) + x].mVertex.y, 1.0f), Vector3f(texcoordx, 0.0, texcoordz));
			Vector3f v1 = Vector3f(vertexes[mPointCountForXAxis*(z + 1) + x].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis*z + x + 1].mVertex.v);
			Vector3f v2 = Vector3f(vertexes[mPointCountForXAxis*(z + 1) + x + 1].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis*z + x + 1].mVertex.v);
			n = Cross(v1, v2);
			n.Normalize();
		}
	}
	void TerrainTile::GetNormal(float tile_pos_x, float tile_pos_z, Vector3f&n) {
		int x = FloorfToInt(tile_pos_x);
		int z = FloorfToInt(tile_pos_z);
		float texcoordx = tile_pos_x - x;
		float texcoordz = tile_pos_z - z;
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		if (texcoordx <= (1 - texcoordz)) {
			Vector3f v1 = Vector3f(vertexes[mPointCountForXAxis*(z + 1) + x].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis * z + x].mVertex.v);
			Vector3f v2 = Vector3f(vertexes[mPointCountForXAxis*z + x + 1].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis * z + x].mVertex.v);
			n = Cross(v1, v2);
			n.Normalize();
		}
		else {
			Vector3f v1 = Vector3f(vertexes[mPointCountForXAxis*(z + 1) + x].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis*z + x + 1].mVertex.v);
			Vector3f v2 = Vector3f(vertexes[mPointCountForXAxis*(z + 1) + x + 1].mVertex.v) - Vector3f(vertexes[mPointCountForXAxis*z + x + 1].mVertex.v);
			n = Cross(v1, v2);
			n.Normalize();
		}
	}
	void TerrainTile::SetMaterial(Material*material){
		mMaterial = material;
	}
	void TerrainTile::UpdateMatrix(const Matrix4x4&model_matrix, const Matrix4x4&normal_matrix) {
		if (mMaterial==nullptr){
			return;
		}
		mMaterial->SetMatrix4Property(UniformDefines::Matrix_M_NameInShader, model_matrix.mData);
		mMaterial->SetMatrix4Property(UniformDefines::Matrix_IT_M_NameInShader, normal_matrix.mData);
	}
	void TerrainTile::SubmitData() {
		VertexDataFull*vertexes = mVertexData.GetBuffer<VertexDataFull>();
		mVBO.SubData(vertexes, mVertexData.mBufferSize);
		mEBO.SubData(mIndexBuffer.mIndexes, mIndexBuffer.mBufferSize);
	}
	void TerrainTile::Render(RenderQueue*rq, GameObject*go){
		if (mMaterial==nullptr){
			return;
		}
		if (mVBO.mSize == 0) {
			return;
		}
		float distance_from_camera = GetDistanceFromCamera(rq);
		if (distance_from_camera < 0.0f) {
			return;
		}
		DrawCall*newDC = new (kMemRendererId)DrawCall;
		newDC->mMaterial = mMaterial.mPtr;
		newDC->mVBO = &mVBO;
		newDC->mEBO = &mEBO;
		newDC->mGameObject = go;
		newDC->mDistanceToCamera = distance_from_camera;
		if (mMaterial->mRenderingQueue < kRenderingQueueTransparent) {
			rq->AppendOpaqueDrawCall(newDC);
		}
		else {
			rq->AppendTransparentDrawCall(newDC);
		}
	}
	void TerrainTile::RenderShadowMap(RenderQueue*rq, GameObject*go) {
		if (mMaterial == nullptr) {
			return;
		}
		if (mVBO.mSize == 0) {
			return;
		}
		if (mMaterial->mbCastShadow==false){
			return;
		}
		float distance_from_camera = GetDistanceFromCamera(rq);
		if (distance_from_camera<0.0f){
			return;
		}
		DrawCall*newDC = new (kMemRendererId)DrawCall;
		newDC->mMaterial = mMaterial.mPtr;
		newDC->mVBO = &mVBO;
		newDC->mEBO = &mEBO;
		newDC->mGameObject = go;
		newDC->mDistanceToCamera = distance_from_camera;
		rq->AppendDepthDrawCall(newDC);
	}
	float TerrainTile::GetDistanceFromCamera(RenderQueue*rq) {
		AABB aabb = mAABB;
		const Matrix4x4&world_matrix = mTerrain->mOwner->GetWorldMatrix();
		aabb.mCenter.x += world_matrix.mData[12] + mLocalPosition.x;
		aabb.mCenter.y += world_matrix.mData[13] + mLocalPosition.y;
		aabb.mCenter.z += world_matrix.mData[14] + mLocalPosition.z;
		if (!IntersectAABBFrustumFull(aabb, rq->mCamera->mCullingParameters.cullingPlanes)) {
			return -1.0f;
		}
		const Matrix4x4 & camera_world_matrix = rq->mCamera->mOwner->GetWorldMatrix();
		Vector3f camera_to_object(aabb.mCenter.x - camera_world_matrix.mData[12],
			aabb.mCenter.y - camera_world_matrix.mData[13],
			aabb.mCenter.z - camera_world_matrix.mData[14]);
		return camera_to_object.LengthSquared();
	}
}
