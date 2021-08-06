#include "utils.h"
#include "soil.h"
GLuint CreateTexture2DFromPNG(const char *imgFilePath, bool invertY) {
	int nFileSize = 0;
	unsigned char *filecontent = LoadFileContent(imgFilePath, nFileSize);
	if (filecontent == nullptr) {
		return 0;
	}
	unsigned int flags = SOIL_FLAG_POWER_OF_TWO;
	if (invertY) {
		flags |= SOIL_FLAG_INVERT_Y;
	}
	GLuint texture = SOIL_load_OGL_texture_from_memory(filecontent, nFileSize, 0, 0, flags);
	delete [] filecontent;
	return texture;
}