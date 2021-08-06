#include "Runtime/Base/AliceGUID.h"
#if ALICE_PLATFORM_WIN
namespace Alice {
	static const char sHexToLiteral[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	void GUID::Init() {
		static_assert (sizeof(Alice::GUID) == 16, "GUID shoud be 16 bytes");
		::GUID guid;
		if (S_OK == ::CoCreateGuid(&guid)){
			memcpy(mData, &guid, sizeof(guid));
		}else{
			printf("create guid fail\n");
		}
	}
	bool CompareGUIDStringLess(const Alice::GUID& l, const Alice::GUID& r) {
		char lData[32];
		char rData[32];
		GUIDToString(l, lData);
		GUIDToString(r, rData);
		for (int i = 0; i < 32; i++) {
			if (lData[i] != rData[i])
				return lData[i] < rData[i];
		}

		return false;
	}
	void GUID::GetStringGUID(char*out) {
		GUIDToString(*this, out);
	}

	std::string GUIDToString(const Alice::GUID& guid) {
		char name[GUIDStringLength + 1];
		GUIDToString(guid, name);
		name[GUIDStringLength] = '\0';
		return name;
	}

	void GUIDToString(const Alice::GUID& guid, char* name) {
		for (int i = 0; i < 4; i++) {
			for (int j = 8; j--;) {
				AliceUInt32 cur = guid.mData[i];
				cur >>= (j * 4);
				cur &= 0xF;
				name[i * 8 + j] = sHexToLiteral[cur];
			}
		}
	}

	Alice::GUID StringToGUID(const std::string& guidString) {
		return StringToGUID(guidString.c_str(), guidString.size());
	}

	Alice::GUID StringToGUID(const char* guidString, size_t length){
		if (length != GUIDStringLength)
			return Alice::GUID();
		static char sLiteralToHex[255];
		static bool sIsInitialized = false;
		if (!sIsInitialized){
			for (int i = 0; i < 255; i++)
				sLiteralToHex[i] = -1;
			sLiteralToHex['0'] = 0;
			sLiteralToHex['1'] = 1;
			sLiteralToHex['2'] = 2;
			sLiteralToHex['3'] = 3;
			sLiteralToHex['4'] = 4;
			sLiteralToHex['5'] = 5;
			sLiteralToHex['6'] = 6;
			sLiteralToHex['7'] = 7;
			sLiteralToHex['8'] = 8;
			sLiteralToHex['9'] = 9;
			sLiteralToHex['a'] = 10;
			sLiteralToHex['b'] = 11;
			sLiteralToHex['c'] = 12;
			sLiteralToHex['d'] = 13;
			sLiteralToHex['e'] = 14;
			sLiteralToHex['f'] = 15;
			sIsInitialized = true;
		}
		int hex[GUIDStringLength];
		for (int i = 0; i < GUIDStringLength; i++)
			hex[i] = sLiteralToHex[guidString[i]];
		Alice::GUID guid;
		for (int i = 0; i < 4; i++){
			AliceUInt32 cur = 0;
			for (int j = 8; j--;){
				int curHex = hex[i * 8 + j];
				if (curHex == -1)
					return Alice::GUID();
				cur |= (curHex << (j * 4));
			}
			guid.mData[i] = cur;
		}
		return guid;
	}
}
#endif