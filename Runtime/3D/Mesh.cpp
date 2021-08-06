#include "Mesh.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Serializer/Mesh.serializer.h"
#include "Runtime/Geometry/Intersection.h"
#include "Runtime/Render/DrawCall.h"
namespace Alice{
	Mesh::Mesh() :mVertexData(nullptr){
		mbDataChanged = false;
		mbIndexChanged = false;
		mVertexData = new (kMemDefaultId)VertexData;
	}
	Mesh::~Mesh() {
		delete mVertexData;
	}
	void Mesh::SetVertexCount(int vertexCount) {
		mVertexData->SetBufferSize(vertexCount);
		mVBO.Init(vertexCount);
	}
	void Mesh::UpdateVertexPosition(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		mbDataChanged = true;
		mVertexData->GetBuffer<VertexDataFull>()[nIndex].mVertex.Set(x, y, z, w);
	}
	void Mesh::UpdateVertexTexcoord(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		mbDataChanged = true;
		mVertexData->GetBuffer<VertexDataFull>()[nIndex].mTexCoord0.Set(x, y, z, w);
	}
	void Mesh::UpdateVertexNormal(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		mbDataChanged = true;
		mVertexData->GetBuffer<VertexDataFull>()[nIndex].mNormal.Set(x, y, z, w);
	}
	void Mesh::UpdateVertexTangent(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		mbDataChanged = true;
		mVertexData->GetBuffer<VertexDataFull>()[nIndex].mTangent.Set(x, y, z, w);
	}
	void Mesh::UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w /* = 1.0f */){
		mbDataChanged = true;
		mVertexData->GetBuffer<VertexDataFull>()[nIndex].mTexCoord1.Set(x,y,z,w);
	}
	void Mesh::SetSubMeshCount(int count){
		for (int i=0;i<count;++i){
			SubMesh*mesh = new SubMesh;
			mSubMeshes.push_back(mesh);
		}
	}
	void Mesh::SetSubMeshIndexCount(int submesh_index,int count) {
		mSubMeshes[submesh_index]->mIndexBuffer.Init(count);
		mSubMeshes[submesh_index]->mEBO.Init(count);
	}
	void Mesh::UpdateSubMeshIndex(int submesh_index, AliceUInt16 index_location, AliceUInt16 indice) {
		mSubMeshes[submesh_index]->mIndexBuffer.mIndexes[index_location] = indice;
		mSubMeshes[submesh_index]->mbDataChanged = true;
		mbIndexChanged = true;
	}
	void Mesh::Update(float deltaTime){
		if (mbDataChanged){
			VertexDataFull*vertexes = mVertexData->GetBuffer<VertexDataFull>();
			mVBO.SubData(vertexes, mVertexData->mBufferSize);
			mbDataChanged = false;
		}
		if (mbIndexChanged) {
			mbIndexChanged = false;
			for (auto iter = mSubMeshes.begin(); iter != mSubMeshes.end(); ++iter) {
				SubMesh*mesh = *iter;
				if (mesh->mbDataChanged) {
					mesh->mbDataChanged = false;
					mesh->mEBO.SubData(mesh->mIndexBuffer.mIndexes, mesh->mIndexBuffer.mBufferSize);
				}
			}
		}
	}
}
