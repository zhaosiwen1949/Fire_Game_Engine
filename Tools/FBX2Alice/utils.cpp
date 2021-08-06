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
FbxAMatrix GetGeometricMatrix(FbxNode* node) {
	const FbxVector4 T = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = node->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(T, R, S);
}
void MatrixAdd(FbxAMatrix& dst, FbxAMatrix& src) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			dst[i][j] += src[i][j];
		}
	}
}
void MatrixScale(FbxAMatrix& matrix, double scalar) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			matrix[i][j] *= scalar;
		}
	}
}
void FBXMatrixToGeneralMatrix(FbxAMatrix&matrix, float *out) {
	double *temp_matrix = (double*)matrix;
	out[0] = temp_matrix[0];
	out[1] = temp_matrix[1];
	out[2] = temp_matrix[2];
	out[3] = temp_matrix[3];
	out[4] = temp_matrix[4];
	out[5] = temp_matrix[5];
	out[6] = temp_matrix[6];
	out[7] = temp_matrix[7];
	out[8] = temp_matrix[8];
	out[9] = temp_matrix[9];
	out[10] = temp_matrix[10];
	out[11] = temp_matrix[11];
	out[12] = temp_matrix[12];
	out[13] = temp_matrix[13];
	out[14] = temp_matrix[14];
	out[15] = temp_matrix[15];
}
void FBXGetNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, float * outNormal) {
	if (inMesh->GetElementNormalCount() < 1) {
		throw std::exception("Invalid Normal Number");
	}
	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode()) {
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
										  break;
		case FbxGeometryElement::eIndexToDirect: {
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
										  break;
		case FbxGeometryElement::eIndexToDirect: {
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
void FBXGetUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, float* outUV) {
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer) {
		throw std::exception("Invalid UV Layer Number");
	}
	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);
	switch (vertexUV->GetMappingMode()) {
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			outUV[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV[1] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
										  break;
		case FbxGeometryElement::eIndexToDirect: {
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV[1] = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode()) {
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect: {
			outUV[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			outUV[1] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
void FBXGetBinormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, float * outBinormal) {
	if (inMesh->GetElementBinormalCount() < 1) {
		throw std::exception("Invalid Binormal Number");
	}
	FbxGeometryElementBinormal* vertexBinormal = inMesh->GetElementBinormal(0);
	switch (vertexBinormal->GetMappingMode()) {
	case FbxGeometryElement::eByControlPoint:
		switch (vertexBinormal->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			outBinormal[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outBinormal[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outBinormal[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
										  break;
		case FbxGeometryElement::eIndexToDirect: {
			int index = vertexBinormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outBinormal[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			outBinormal[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			outBinormal[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexBinormal->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			outBinormal[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outBinormal[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outBinormal[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
										  break;
		case FbxGeometryElement::eIndexToDirect: {
			int index = vertexBinormal->GetIndexArray().GetAt(inVertexCounter);
			outBinormal[0] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			outBinormal[1] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			outBinormal[2] = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
void FBXGetTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, float* outTangent) {
	if (inMesh->GetElementTangentCount() < 1) {
		throw std::exception("Invalid Tangent Number");
	}
	FbxGeometryElementTangent* vertexTangent = inMesh->GetElementTangent(0);
	switch (vertexTangent->GetMappingMode()) {
	case FbxGeometryElement::eByControlPoint:
		switch (vertexTangent->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			outTangent[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outTangent[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outTangent[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
										  break;
		case FbxGeometryElement::eIndexToDirect: {
			int index = vertexTangent->GetIndexArray().GetAt(inCtrlPointIndex);
			outTangent[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			outTangent[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			outTangent[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexTangent->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			outTangent[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outTangent[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outTangent[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
										  break;
		case FbxGeometryElement::eIndexToDirect: {
			int index = vertexTangent->GetIndexArray().GetAt(inVertexCounter);
			outTangent[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			outTangent[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			outTangent[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
												 break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
bool HaveAnimation(FbxMesh*mesh) {
	if (mesh == nullptr) {
		return false;
	}
	int lSkinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int lClusterCount = 0;
	for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex) {//这里可以判断出该模型有没有动画数据
		lClusterCount += ((FbxSkin *)(mesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
	}
	return lClusterCount > 0;
}
void GetStaticVertices(FbxMesh*mesh, FbxVector4**out_vertices) {
	const int vertice_count = mesh->GetControlPointsCount();
	FbxVector4*controlPoints = mesh->GetControlPoints();
	*out_vertices = new FbxVector4[vertice_count];
	memcpy(*out_vertices, controlPoints, sizeof(FbxVector4)*vertice_count);
}
void ComputeClusterDeformation(FbxMesh* pMesh, FbxCluster* pCluster, FbxTime&time, FbxAMatrix& globalPosition, FbxAMatrix& vertex_transform_matrix) {
	FbxAMatrix lBoneTransform;
	FbxAMatrix lGlobalInitPosition;
	FbxAMatrix lBoneCurrentGlobalPosition;
	FbxAMatrix lGeometricMatrix;
	FbxAMatrix lRelativeTransform;
	FbxAMatrix lAnimationMatrix;

	lBoneCurrentGlobalPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);
	lAnimationMatrix = globalPosition.Inverse() * lBoneCurrentGlobalPosition;

	pCluster->GetTransformMatrix(lBoneTransform);
	lGeometricMatrix = GetGeometricMatrix(pMesh->GetNode());
	lBoneTransform = lBoneTransform * lGeometricMatrix;
	pCluster->GetTransformLinkMatrix(lGlobalInitPosition);
	lRelativeTransform = lGlobalInitPosition.Inverse() * lBoneTransform;

	vertex_transform_matrix = lAnimationMatrix * lRelativeTransform;
}
void GetAnimatedVertices(FbxMesh*mesh, FbxTime&time, FbxAMatrix& globalPosition, FbxVector4**out_vertices) {
	GetStaticVertices(mesh, out_vertices);
	int skin_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int vertice_count = mesh->GetControlPointsCount();

	FbxAMatrix* deformation_matrix = new FbxAMatrix[vertice_count];
	memset(deformation_matrix, 0, vertice_count * sizeof(FbxAMatrix));

	double* weights = new double[vertice_count];
	memset(weights, 0, vertice_count * sizeof(double));

	for (int skin_index = 0; skin_index < skin_count; ++skin_index) {
		FbxSkin * skin = (FbxSkin *)mesh->GetDeformer(skin_index, FbxDeformer::eSkin);
		int cluster_count = skin->GetClusterCount();
		for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index) {
			FbxCluster* cluster = skin->GetCluster(cluster_index);
			FbxNode*bone = cluster->GetLink();
			if (nullptr == bone) continue;
			FbxAMatrix vertex_transform;
			ComputeClusterDeformation(mesh, cluster, time, globalPosition, vertex_transform);
			int vertex_index_count = cluster->GetControlPointIndicesCount();
			for (int k = 0; k < vertex_index_count; ++k) {
				int index = cluster->GetControlPointIndices()[k];
				double weight = cluster->GetControlPointWeights()[k];
				FbxAMatrix influence = vertex_transform;
				MatrixScale(influence, weight);
				MatrixAdd(deformation_matrix[index], influence);
				weights[index] += weight;
			}
		}
	}
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	for (int i = 0; i < vertice_count; i++) {
		double lWeight = weights[i];
		if (lWeight != 0.0) {
			if (lClusterMode == FbxCluster::eNormalize) {
				(*out_vertices)[i] = deformation_matrix[i].MultT((*out_vertices)[i]);
				(*out_vertices)[i] /= lWeight;
			}
			else if (lClusterMode == FbxCluster::eTotalOne) {
				(*out_vertices)[i] = (*out_vertices)[i] * (1.0 - lWeight) + deformation_matrix[i].MultT((*out_vertices)[i]);
			}
		}
	}
	delete[] deformation_matrix;
	delete[] weights;
}
FbxAnimCurve*GetFirstAnimationCurve(FbxNode*node, FbxAnimLayer*layer) {
	FbxAnimCurve* curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve != nullptr) {
		return curve;
	}
	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve != nullptr) {
		return curve;
	}
	return nullptr;
}
FbxNode*GetNodeByName(FbxNode*node, const char *name) {
	if (strcmp(node->GetName(), name) == 0) {
		return node;
	}
	int nChildCount = node->GetChildCount();
	for (int i = 0; i < nChildCount; i++) {
		FbxNode*ret = GetNodeByName(node->GetChild(i), name);
		if (ret != nullptr) {
			return ret;
		}
	}
	return nullptr;
}