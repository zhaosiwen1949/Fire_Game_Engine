#pragma once
#include "Runtime/Base/Component.h"
#include <unordered_map>
#include "Mesh.h"
#include "Runtime/Geometry/AABB.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Serializer/Mesh.serializer.h"
#include "Runtime/Geometry/Geometry.h"

namespace Alice {
	class SubMeshRenderer {
	public:
		SmartPtr<Material> mMaterial;
	};
	class MeshRenderer :public Component {
	public:
		MeshRenderer();
		~MeshRenderer();
	public:
		void SetMaterialCount(int count);
		void SetMaterial(int index, Material*mat);
		void Update(float deltaTime);
		void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
		std::vector<SubMeshRenderer*> mSubMeshRenderers;
	public:
		DECLEAR_ALICE_CLASS(MeshRenderer)
	};
}