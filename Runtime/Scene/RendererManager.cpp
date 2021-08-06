#include "RendererManager.h"
#include "Runtime/Allocator/DefaultAllocator.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/Builtin/HardCodedShader.h"
#include "Runtime/Render/Material.h"
#include "Runtime/Exported/Render/ExportedMaterial.h"
#include "Runtime/Exported/Render/ExportedRenderPass.h"
namespace Alice{
	static RendererManager * spRenderManager=nullptr;
	RendererManager::RendererManager(){
		mRenderArchitecture = kRenderArchitectureForward;
	}
	void RendererManager::InitDynamicFontMaterial() {
		Material*material = CreateMaterialWithStandardShader("builtin/Material/Font", GetBuiltinDynamicFontVertexShader(), GetBuiltinDynamicFontFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->EnableBlend(true);
			material->EnableZTest(false);
			material->EnableZWrite(false);
			material->BlendFunc(ALICE_SRC_ALPHA, ALICE_ONE_MINUS_SRC_ALPHA);
			material->GetPass(0)->mCatagory = kRenderPassCatagoryNoLit;
			mDynamicFontMaterial = material;
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>("builtin/Material/Font", material));
		}
		else {
			Error("InitDynamicFontMaterial fail");
		}
	}
	void RendererManager::InitDefault2DMaterial(){
		Material*material = CreateMaterialWithStandardShader("builtin/Material/Texture2D", GetBuiltinTexture2DSpriteVertexShader(), GetBuiltinTexture2DSpriteFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->EnableBlend(true);
			material->EnableZTest(false);
			material->EnableZWrite(false);
			material->BlendFunc(ALICE_SRC_ALPHA, ALICE_ONE_MINUS_SRC_ALPHA);
			material->GetPass(0)->mCatagory = kRenderPassCatagoryNoLit;
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>("builtin/Material/Texture2D", material));
		}
		else {
			Error("InitDefault2DMaterial fail");
		}
	}
	void RendererManager::InitUIMaterial() {//instance
		static const char * material_name = "builtin/UI";
		Material*material = CreateMaterialWithStandardShader(material_name, GetBuiltinUIVertexShader(), GetBuiltinUIFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->EnableBlend(true);
			material->EnableZTest(false);
			material->EnableZWrite(false);
			material->BlendFunc(ALICE_SRC_ALPHA, ALICE_ONE_MINUS_SRC_ALPHA);
			material->GetPass(0)->mCatagory = kRenderPassCatagoryUI;
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>(material_name, material));
		}
		else {
			Error("InitDefault2DMaterial fail");
		}
	}
	void RendererManager::InitStencilUIMaterial() {
		static const char * material_name = "builtin/StencilUI";
		Material*material = CreateMaterialWithStandardShader(material_name, GetBuiltinUIVertexShader(), GetBuiltinUIFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->EnableBlend(true);
			material->EnableZTest(false);
			material->EnableZWrite(false);
			material->EnableWriteStencilMask(true);
			material->BlendFunc(ALICE_SRC_ALPHA, ALICE_ONE_MINUS_SRC_ALPHA);
			material->GetPass(0)->mCatagory = kRenderPassCatagoryUI;
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>(material_name, material));
		}
		else {
			Error("InitStencilUIMaterial fail");
		}
	}
	void RendererManager::InitDefaultColorMaterial() {
		Material*material = CreateMaterialWithStandardShader("builtin/Material/Color", GetBuiltinColorVertexShader(), GetBuiltinColorFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->EnableBlend(true);
			material->EnableZTest(false);
			material->EnableZWrite(false);
			material->BlendFunc(ALICE_SRC_ALPHA, ALICE_ONE_MINUS_SRC_ALPHA);
			material->GetPass(0)->mCatagory = kRenderPassCatagoryNoLit;
			Material::mDefault2DMaterial = material;
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>("builtin/Material/Color", material));
		}
		else {
			Error("InitDefaultColorMaterial fail");
		}
	}
	void RendererManager::InitDefaultSimpleColorMaterial() {
		Material*material = CreateMaterialWithStandardShader("builtin/Material/SimpleColor", GetBuiltinSimpleColorVertexShader(), GetBuiltinSimpleColorFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->EnableBlend(true);
			material->EnableZTest(false);
			material->EnableZWrite(false);
			material->BlendFunc(ALICE_SRC_ALPHA, ALICE_ONE_MINUS_SRC_ALPHA);
			material->GetPass(0)->mCatagory = kRenderPassCatagoryNoLit;
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>("builtin/Material/SimpleColor", material));
		}
		else {
			Error("InitDefaultSimpleColorMaterial fail");
		}
	}
	void RendererManager::InitDefaultFrameBufferRenderMaterial() {
		Material*material = CreateMaterialWithStandardShader("builtin/Material/DefaultFrameBufferRender", GetBuiltinDefaultFrameBufferRenderVertexShader(), GetBuiltinDefaultFrameBufferRenderFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->GetPass(0)->mCatagory = kRenderPassCatagoryNoLit;
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>("builtin/Material/DefaultFrameBufferRender", material));
		}
		else {
			Error("InitDefaultFrameBufferRenderMaterial fail");
		}
	}
	void RendererManager::InitStencilRenderMaterial() {
		Material*material = CreateMaterialWithStandardShader("builtin/Material/Stencil", GetBuiltinStencilRendererVertexShader(), GetBuiltinStencilRendererFragmentShader());
		if (material != nullptr) {
			material->mObjectMask = 0;
			material->retain();
			material->GetPass(0)->mCatagory = kRenderPassCatagoryNoLit;
			material->EnableWriteStencilMask(true);
			Material::mCachedMaterials.insert(std::pair<std::string, Material*>("builtin/Material/Stencil", material));
		}
		else {
			Error("InitStencilRenderMaterial fail");
		}
	}
	RendererManager&GetRendererManager()
	{
		if (spRenderManager==nullptr)
		{
			spRenderManager = new RendererManager();
		}
		return *spRenderManager;
	}
	Material*RendererManager::CreateMaterialWithStandardShader(const char * shader_user_path, const char *vscode, const char *fscode) {
		Shader*shader = Shader::CreateStandard(shader_user_path, vscode, fscode);
		ExportedMaterial*material = Alice::CreateExportedAliceType<Alice::ExportedMaterial>();
		RenderPass*pass = Alice::CreateExportedAliceType<Alice::ExportedRenderPass>();
		pass->SetShader(shader);
		material->AddPass(pass);
		material->mbSharedMaterial = true;
		material->mName = shader_user_path;
		return material;
	}
}
