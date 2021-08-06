#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Render/RenderMisc.h"
#include "GL20/Texture2D.h"
#include "GL20/GLSL.h"
#include "Runtime/Render/MaterialProperty.h"
#include "Runtime/Serializer/Material.serializer.h"
#include "Runtime/Utils/SmartPtr.h"
#include "Runtime/Math/Vector4.h"
#include "Runtime/Render/UniformUpdater.h"
#include "Runtime/Render/Light/Light.h"
#include "RenderMisc.h"
#include "RenderState.h"
#include "RenderPass.h"
namespace Alice{
	class RenderPass;
	struct ShaderUniformDefine;
	class Material:public Object{
	public:
		Material();
		virtual ~Material();
		std::list<RenderPass*> mRenderPasses;
		bool mbCastShadow;
		bool mbReceiveShadow;
		bool mbSharedMaterial;
		RenderingQueue mRenderingQueue;
		void SetStencilBuffer();
		void RestoreStencilBuffer();
		void EnableBlend(bool enable);
		void BlendFunc(int src, int dst);
		void EnableZTest(bool enable);
		void EnableZWrite(bool enable);
		void EnableCull(bool enable);
		void EnablePointSprite(bool enable);
		void EnableWriteStencilMask(bool enable);
		void EnableUseStencilMask(bool enable);
		void ZTestFunction(int func);
		void ZOffset(float factor, float unit);
		void SetCullFace(int face);
		void SetPolygonMode(int mode);
		void SetPrimitive(int primitive);
		TextureUnitProperty* GetTextureProperty(const char*uniformName);
		UniformMatrix4Property* GetMatrix4Property(const char*uniformName);
		UniformVector4Property* GetVec4Property(const char*uniformName);
		void SetTextureProperty(const char*uniformName, TextureUnit*texture);
		void SetTextureCubeProperty(const char*uniformName, TextureCube*texture);
		void SetVec4Property(const char*uniformName, float x, float y, float z, float w);
		void SetVec4Property(const char*uniformName, float * v);
		void SetVec4Property(const char*uniformName, const Serializer::Vector4f&value);
		void SetVec4ArrayProperty(const char*uniformName,int size, float *v);
		void SetIntArrayProperty(const char*uniformName, int size, int *v);
		void SetMatrix4Property(const char*uniformName,const float*v);
		void SetMatrix3Property(const char*uniformName, float*v);
		void CleanUpProperties();
		void AddPass(RenderPass * pass);
		void RemovePass(int index);
		RenderPass * GetPass(int index);
	public:
		//clone
		void operator=(const Material&r);
		bool operator == (const Material &r);
		bool operator == (const std::unordered_map<std::string, MaterialProperty*>&mpm);
		static Material* GetCachedMaterial(const char* matPath);
		static Material* LoadMaterial(const char*name);
		static void UnloadMaterial(const char*name);
		static std::unordered_map<std::string, Material*> mCachedMaterials;
		static Material* mDefault2DMaterial;
	public:
		static UniformVector4Property mTime;
		static UniformVector4Property mSinTime;
		static UniformVector4Property mCosTime;
		DECLEAR_ALICE_CLASS(Material)
	};
}