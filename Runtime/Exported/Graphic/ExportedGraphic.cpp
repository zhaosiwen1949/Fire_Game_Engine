#include "ExportedGraphic.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Render/Camera.h"
extern "C" {
#include "Runtime/Plugins/libqrencode/quirc_internal.h"
#include "Runtime/Plugins/libqrencode/quirc.h"
}
#include <zxing/LuminanceSource.h>
#include <zxing/common/Counted.h>
#include <zxing/Reader.h>
#include <zxing/aztec/AztecReader.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/DecodeHints.h>
#include <zxing/datamatrix/DataMatrixReader.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/pdf417/PDF417Reader.h>
#include <zxing/qrcode/QRCodeReader.h>
#include "Runtime/Graphics/LuminaceImage.h"
namespace Alice {
	int ExportedGraphic::API_DecodeQRCodeImageData(lua_State*L) {
		const char * image_data = lua_tostring(L, 1);
		int image_data_len = lua_tonumber(L, 2);
		zxing::Ref<zxing::LuminanceSource> source = LuminaceImage::create((unsigned char*)image_data,image_data_len);
		if (source.empty()) {
			lua_pushnil(L);
			return 1;
		}
		zxing::Ref<zxing::Reader> reader;
		reader.reset(new zxing::qrcode::QRCodeReader);
		zxing::Ref<zxing::Binarizer> binarizer(new zxing::GlobalHistogramBinarizer(source));
		zxing::Ref<zxing::BinaryBitmap> bitmap(new zxing::BinaryBitmap(binarizer));
		zxing::Ref<zxing::Result> result(reader->decode(bitmap, zxing::DecodeHints(zxing::DecodeHints::QR_CODE_HINT)));
		lua_pushlstring(L, result->getText()->getText().c_str(), result->getText()->getText().length());
		return 1;
	}
	int ExportedGraphic::API_Render(lua_State*L){
		// 1 : 使用哪一个摄像机
		// 2 : 使用什么材质去渲染这个Buffer
		// 3 : 渲染到什么纹理上去
		Camera*camera = TO_USERDATA(L, Camera, 1);
		Material*material = TO_USERDATA(L, Material, 2);
		Texture2D*dstTexture = TO_USERDATA(L, Texture2D, 3);
		return 0;
	}
	int ExportedGraphic::API_Baking(lua_State*L) {
		// 1 : 使用哪一个摄像机
		// 2 : 存储路径
		Camera*camera = TO_USERDATA(L, Camera, 1);
		const char * path = lua_tostring(L, 2);
		return 0;
	}
	int ExportedGraphic::Export(lua_State*L){
		luaL_Reg api_c[] = {
			EXPORT_API(DecodeQRCodeImageData)
			EXPORT_API(Render)
			EXPORT_API(Baking)
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Graphic, api_c);
		return 0;
	}
}
