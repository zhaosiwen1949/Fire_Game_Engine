#pragma once
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/VertexData.h"
#include "Runtime/Render/Material.h"
#include "Runtime/Render/GL20/VBO.h"
namespace Alice{
	class FullScreenQuad{
	public:
		FullScreenQuad();
		~FullScreenQuad();
		void Init();
		void Render(Material*material);
		VBO mVBO;
	};
}
