#pragma once
#include "AliceResource.h"

namespace Alice
{
	class ShaderResource :public Resource
	{
	public:
		ShaderResource();
		static ShaderResource*LoadShader(Serializer::RuntimeAsset*asset);
	};
}