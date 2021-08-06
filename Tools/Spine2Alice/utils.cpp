#include "utils.h"
void _spAtlasPage_createTexture(spAtlasPage* self, const char* path) {
	self->rendererObject = 0;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self) {
}

char* _spUtil_readFile(const char* path, int* length) {
	return _spReadFile(path, length);
}
void SaveFile(const char * path, const void * bytes, int len) {
	FILE*file = fopen(path, "wb");
	if (file != nullptr) {
		fwrite(bytes, 1, len, file);
		fclose(file);
	}
}
char* LoadFileContent(const char * path, int & len) {
	FILE*file = fopen(path, "rb");
	if (file != nullptr) {
		fseek(file, 0, SEEK_END);
		len = ftell(file);
		rewind(file);
		char * buffer = new char[len + 1];
		fread(buffer, 1, len, file);
		buffer[len] = 0;
		fclose(file);
		return buffer;
	}
	return nullptr;
}
float HexToColor(const char* value, int index) {
	char digits[3];
	char *error;
	int color;

	if ((size_t)index >= strlen(value) / 2)
		return -1;
	value += index * 2;

	digits[0] = *value;
	digits[1] = *(value + 1);
	digits[2] = '\0';
	color = (int)strtoul(digits, &error, 16);
	if (*error != 0) return -1;
	return color / (float)255;
}
void ReadSpineVertices(Json* attachmentMap, spVertexAttachment* attachment, int verticesLength) {
	Json* entry;
	float* vertices;
	int i, n, nn, entrySize;
	spFloatArray* weights;
	spIntArray* bones;

	attachment->worldVerticesLength = verticesLength;

	entry = Json_getItem(attachmentMap, "vertices");
	entrySize = entry->size;
	vertices = MALLOC(float, entrySize);
	for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
		vertices[i] = entry->valueFloat;

	if (verticesLength == entrySize) {
		attachment->verticesCount = verticesLength;
		attachment->vertices = vertices;

		attachment->bonesCount = 0;
		attachment->bones = 0;
		return;
	}

	weights = spFloatArray_create(verticesLength * 3 * 3);
	bones = spIntArray_create(verticesLength * 3);

	for (i = 0, n = entrySize; i < n;) {
		int boneCount = (int)vertices[i++];
		spIntArray_add(bones, boneCount);
		for (nn = i + boneCount * 4; i < nn; i += 4) {
			spIntArray_add(bones, (int)vertices[i]);
			spFloatArray_add(weights, vertices[i + 1]);
			spFloatArray_add(weights, vertices[i + 2]);
			spFloatArray_add(weights, vertices[i + 3]);
		}
	}

	attachment->verticesCount = weights->size;
	attachment->vertices = weights->items;
	FREE(weights);
	attachment->bonesCount = bones->size;
	attachment->bones = bones->items;
	FREE(bones);

	FREE(vertices);
}