#pragma once
#include "ggl.h"
#include "fbxsdk.h"
unsigned char * LoadFileContent(const char*path, int&filesize);
GLuint CompileShader(GLenum shaderType, const char*shaderCode);
GLuint CreateProgram(GLuint vsShader, GLuint fsShader);
float GetFrameTime();
FbxAMatrix GetGeometricMatrix(FbxNode* node);
void MatrixAdd(FbxAMatrix& dst, FbxAMatrix& src);
void MatrixScale(FbxAMatrix& matrix, double scalar);
void FBXGetNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, float * outNormal);
bool HaveAnimation(FbxMesh*mesh);
void GetAnimatedVertices(FbxMesh*mesh, FbxTime&time, FbxAMatrix& globalPosition, FbxVector4**out_vertices);
void GetStaticVertices(FbxMesh*mesh, FbxVector4**out_vertices);
void ComputeClusterDeformation(FbxMesh* pMesh, FbxCluster* pCluster, FbxTime&time, FbxAMatrix& globalPosition, FbxAMatrix& vertex_transform_matrix);
void FBXMatrixToGeneralMatrix(FbxAMatrix&matrix, float *out);
FbxAnimCurve* GetFirstAnimationCurve(FbxNode*node, FbxAnimLayer*layer);
FbxNode*GetNodeByName(FbxNode*node, const char *name);