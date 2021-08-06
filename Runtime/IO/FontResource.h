#pragma once
#include "AliceResource.h"

namespace Alice{
	class FontResource:public Resource{
	public:
		static FontResource * LoadFontResource(Serializer::RuntimeAsset*name);
		static FontResource * CreateFontResourceWithRawData(const char*path, Data&data);
	};
}