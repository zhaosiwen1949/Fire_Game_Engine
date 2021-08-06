#pragma once
#include "Runtime/Base/Component.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/Render/IndexBuffer.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/GL20/VBO.h"
#include "Runtime/Render/GL20/EBO.h"
#include "Runtime/Render/Material.h"
#include "Runtime/Serializer/Mesh.serializer.h"
#include "Runtime/Geometry/Geometry.h"
namespace Alice{
	struct SubMesh {
		IndexBuffer mIndexBuffer;
		EBO mEBO;
		bool mbDataChanged;
	};
	class Mesh :public Component{
	public:
		VertexData *mVertexData;
		VBO mVBO;
		bool mbDataChanged;
		bool mbIndexChanged;
		std::vector<SubMesh*> mSubMeshes;
	public:
		Mesh();
		~Mesh();
		void UpdateVertexPosition(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTexcoord(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexNormal(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTangent(int nIndex, float x, float y, float z, float w = 1.0f);
		void UpdateVertexTexcoord1(int nIndex, float x, float y, float z, float w = 1.0f);
		void SetVertexCount(int vertexCount);
		void SetSubMeshCount(int count);
		void SetSubMeshIndexCount(int submesh_index,int count);
		void UpdateSubMeshIndex(int submesh_index,AliceUInt16 index_location, AliceUInt16 indice);
		void Update(float deltaTime);
	public:
		DECLEAR_ALICE_CLASS(Mesh)
	};
}