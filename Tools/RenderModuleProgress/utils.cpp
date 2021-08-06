#include "utils.h"
GLuint CompileShader(GLenum shaderType, const char*shaderCode) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader, 1024, &logLen, szLog);
		printf("Compile Shader fail error log : %s \nshader code :\n%s\n", szLog, shaderCode);
		glDeleteShader(shader);
		shader = 0;
	}
	return shader;
}
GLuint CreateProgram(GLuint vsShader, GLuint fsShader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	GLint nResult;
	glGetProgramiv(program, GL_LINK_STATUS, &nResult);
	if (nResult == GL_FALSE) {
		char log[1024] = { 0 };
		GLsizei writed = 0;
		glGetProgramInfoLog(program, 1024, &writed, log);
		printf("create gpu program fail,link error : %s\n", log);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}
unsigned char* DecodeBMP(unsigned char*bmpFileData, int&width, int&height) {
	if (0x4D42 == *((unsigned short*)bmpFileData)) {
		int pixelDataOffset = *((int*)(bmpFileData + 10));
		width = *((int*)(bmpFileData + 18));
		height = *((int*)(bmpFileData + 22));
		unsigned char*pixelData = bmpFileData + pixelDataOffset;
		for (int i = 0; i < width*height * 3; i += 3) {
			unsigned char temp = pixelData[i];
			pixelData[i] = pixelData[i + 2];
			pixelData[i + 2] = temp;
		}
		return pixelData;
	}
	return nullptr;
}
GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum type) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixelData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
GLuint CreateTexture2DFromBMP(const char*bmpPath) {
	int nFileSize = 0;
	unsigned char *bmpFileContent = LoadFileContent(bmpPath, nFileSize);
	if (bmpFileContent == nullptr) {
		return 0;
	}
	int bmpWidth = 0, bmpHeight = 0;
	unsigned char*pixelData = DecodeBMP(bmpFileContent, bmpWidth, bmpHeight);
	if (bmpWidth == 0) {
		delete bmpFileContent;
		return 0;
	}
	GLuint texture = CreateTexture2D(pixelData, bmpWidth, bmpHeight, GL_RGB);
	delete bmpFileContent;
	return texture;
}
GLuint CreateProcedureTexture(int size) {
	unsigned char *imageData = new unsigned char[size*size * 4];
	float halfSize = (float)size / 2.0f;
	float maxDistance = sqrtf(halfSize*halfSize + halfSize*halfSize);
	float centerX = halfSize;
	float centerY = halfSize;
	for (int y = 0; y < size; ++y) {
		for (int x = 0; x < size; ++x) {
			int currentPixelOffset = (x + y*size) * 4;
			imageData[currentPixelOffset] = 255;
			imageData[currentPixelOffset + 1] = 255;
			imageData[currentPixelOffset + 2] = 255;
			float deltaX = (float)x - centerX;
			float deltaY = (float)y - centerY;
			float distance = sqrtf(deltaX*deltaX + deltaY*deltaY);
			float alpha = powf(1.0f - (distance / maxDistance), 8.0f);
			alpha = alpha > 1.0f ? 1.0f : alpha;
			imageData[currentPixelOffset + 3] = (unsigned char)(alpha*255.0f);
		}
	}
	GLuint texture = CreateTexture2D(imageData, size, size, GL_RGBA);
	delete imageData;
	return texture;
}
GLuint CreateComputeProgram(const char * path) {
	int file_size = 0;
	unsigned char* file_content = LoadFileContent(path, file_size);
	if (file_content == nullptr) {
		return 0;
	}
	GLuint shader = CompileShader(GL_COMPUTE_SHADER, (char*)file_content);
	delete[] file_content;

	GLuint program = glCreateProgram();
	glAttachShader(program, shader);
	glLinkProgram(program);
	glDetachShader(program, shader);
	glDeleteShader(shader);
	GLint nResult;
	glGetProgramiv(program, GL_LINK_STATUS, &nResult);
	if (nResult == GL_FALSE) {
		char log[1024] = { 0 };
		GLsizei writed = 0;
		glGetProgramInfoLog(program, 1024, &writed, log);
		printf("CreateComputeProgram,link error : %s\n", log);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}
GLuint CreateFeedbackProgram(const char*shaderPath, const char **values,int count, GLenum param){
	GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);
	int fileSize = 0;
	const char* vsCode = (char*)LoadFileContent(shaderPath,fileSize);
	if (vsCode==nullptr){
		return 0;
	}
	glShaderSource(vsShader, 1, &vsCode, nullptr);
	glCompileShader(vsShader);
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glTransformFeedbackVaryings(program, count, values, param);
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDeleteShader(vsShader);
	return program;
}
void ExtractPlane(const glm::mat4& finalMatrix,int index,float distance,Plane*out_plane) {
	float M4[4];
	float MX[4];
	int x_index = index / 2;
	const float* final_matrix_data = glm::value_ptr(finalMatrix);
	M4[0] = final_matrix_data[3 + 0 * 4];
	M4[1] = final_matrix_data[3 + 1 * 4];
	M4[2] = final_matrix_data[3 + 2 * 4];
	M4[3] = final_matrix_data[3 + 3 * 4];
	MX[0] = final_matrix_data[x_index + 0 * 4];
	MX[1] = final_matrix_data[x_index + 1 * 4];
	MX[2] = final_matrix_data[x_index + 2 * 4];
	MX[3] = final_matrix_data[x_index + 3 * 4];
	float sign = 1.0f;
	if (index % 2 == 1) {
		sign = -1.0f;
	}
	out_plane->SetABCD(
		sign * MX[0] + M4[0] * distance,
		sign * MX[1] + M4[1] * distance,
		sign * MX[2] + M4[2] * distance,
		sign * MX[3] + M4[3] * distance);
	out_plane->Normalize();
}
void ExtractProjectionPlanes(const glm::mat4& finalMatrix, const Plane* frustum_space_ndc, Plane* outPlanes) {
	for (int i = 0; i < 6; ++i) {
		ExtractPlane(finalMatrix, i, frustum_space_ndc[i].mD, &outPlanes[i]);
	}
}
float GetDistanceToPoint(const Plane&plane, const glm::vec3& inPt) {
	return glm::dot(glm::vec3(plane.mA,plane.mB,plane.mC),inPt) + plane.mD;
}
bool IntersectSphereFrustumFull(const glm::vec3& center, float radius, const Plane *p)
{
	for (int i = 0; i < 6; ++i){
		float dist = GetDistanceToPoint(p[i],center);
		if (dist + radius < 0) return false;
	}
	return true;
}