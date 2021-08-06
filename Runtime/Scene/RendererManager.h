#pragma once
#include "Runtime/Render/VertexData.h"
#include "Runtime/Utils/LinkedList.h"
#include <map>
#include "Runtime/Render/RenderMisc.h"

namespace Alice{
	class Material;
	class RendererManager{
	public:
		RendererManager();
		RenderArchitecture mRenderArchitecture;
	public:
		Material *mDefaultMaterial;
		Material *mDynamicFontMaterial;
		Material *mDefaultColorOnlyMaterial;
		Material *mPointParticleMaterial;
		void InitDefaultColorMaterial();
		void InitDefaultSimpleColorMaterial();
		void InitDefault2DMaterial();
		void InitUIMaterial();
		void InitStencilUIMaterial();
		void InitDynamicFontMaterial();
		void InitDefaultFrameBufferRenderMaterial();
		void InitStencilRenderMaterial();
		Material * CreateMaterialWithStandardShader(const char * shader_name, const char *vscode, const char *fscode);
	};
	RendererManager&GetRendererManager();
}
