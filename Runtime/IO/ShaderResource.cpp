#include "ShaderResource.h"
#include "Runtime/Serializer/GLSL.serializer.h"

namespace Alice{
	ShaderResource::ShaderResource(){
		mType = ResourceTypeShader;
	}
	ShaderResource* ShaderResource::LoadShader(Serializer::RuntimeAsset*asset){
		Serializer::GLSL*shaderData = new (kMemShaderId)Serializer::GLSL;
		if (shaderData->ParseFromArray(asset->rawdata().c_str(), asset->rawdata().length())){
			ShaderResource*shaderResource = new (kMemShaderId)ShaderResource;
			shaderResource->mResourceData = shaderData;
			return shaderResource;
		}else{
			delete shaderData;
		}
		return nullptr;
	}
}