#version 420
layout(location=0)in vec4 position;
layout(location=1)in vec4 texcoord;//skeleton joint info
layout(location=2)in vec4 normal;//vertex weight info
layout(location=0)out vec4 V_Color;
layout(binding=0)uniform AliceBuiltinVertexVectors{
	vec4 Value[8];
}U_DefaultVertexVectors;
layout(binding=1)uniform AliceBuiltinVertexMatrix{
	mat4 Model;
	mat4 View;
	mat4 Projection;
	mat4 IT_Model;
	mat4 InvertJoints[256];
	mat4 AnimatedJoints[256];
}U_DefaultVertexMatrices;
void main(){
	vec4 pos=vec4(0.0);
	for(int i=0;i<4;i++){
		float w=normal[i];
		if(w!=0.0){
			int index=int(texcoord[i]);
			pos+=w*U_DefaultVertexMatrices.AnimatedJoints[index]*U_DefaultVertexMatrices.InvertJoints[index]*position;
		}
	}
	gl_Position=U_DefaultVertexMatrices.Projection*U_DefaultVertexMatrices.View*U_DefaultVertexMatrices.Model*pos;
}