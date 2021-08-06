#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Shader.h"
#include "RenderMisc.h"
#include "RenderState.h"
#include "MaterialProperty.h"
#include "UniformUpdater.h"
#include "Runtime/Serializer/Vector.serializer.h"
namespace Alice{
	class Material;
	class RenderPass:public Object{
	public:
		RenderPass();
		virtual ~RenderPass();
		SmartPtr<Shader> mShader;
		RenderState mRenderState;
		bool mbWriteStencilMask;
		bool mbUseStencilMask;
		RenderPassCatagory mCatagory;
		std::unordered_map<std::string, MaterialProperty*> mProperties;
		std::unordered_map<std::string, UniformUpdater*> mPropertyUpdaters;
		std::set<UniformUpdater*> mActiveUniformList;
	public:
		void operator=(const RenderPass&r);
		RenderPass * Clone() const;
		void SetShader(Shader*shader);
		void ActiveUniform(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformFloat(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformVec2(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformVec3(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformVec4(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformMat3(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformMat4(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformSampler2D(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformSamplerCube(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformIntArray(const char *name, const ShaderUniformDefine * sud);
		void ActiveUniformVec4Array(const char *name, const ShaderUniformDefine * sud);
		TextureUnitProperty* GetTextureProperty(const char*uniformName);
		UniformMatrix4Property* GetMatrix4Property(const char*uniformName);
		UniformVector4Property* GetVec4Property(const char*uniformName);
		void SetTextureProperty(const char*uniformName, TextureUnit*texture);
		void SetTextureCubeProperty(const char*uniformName, TextureCube*texture);
		void SetFloatProperty(const char*uniformName, float v);
		void SetVec2Property(const char*uniformName, float x, float y);
		void SetVec3Property(const char*uniformName, float x, float y, float z);
		void SetVec4Property(const char*uniformName, float x, float y, float z, float w);
		void SetVec4Property(const char*uniformName, float * v);
		void SetVec4Property(const char*uniformName, const Serializer::Vector4f&value);
		void SetVec4ArrayProperty(const char*uniformName, int size, float *v);
		void SetIntArrayProperty(const char*uniformName, int size, int *v);
		void SetMatrix4Property(const char*uniformName, const float*v);
		void SetMatrix3Property(const char*uniformName, float*v);

		void UpdateUniforms(void * camera);
		void ClearUniformUpdaters();
		void AddUniformUpdater(UniformUpdater*updater);
		void ActiveRenderState();
		void SetupStencilBuffer();
		void RestoreStencilBuffer();
	public:
		DECLEAR_ALICE_CLASS(RenderPass)
	};
}