attribute vec4 position;
attribute vec4 texcoord;
attribute vec4 normal;

uniform mat4 ModelMatrix;
uniform mat4 Alice_ViewMatrix;
uniform mat4 Alice_ProjectionMatrix;

uniform vec4 U_LightPosition;
uniform vec4 U_LightColor;
uniform vec4 U_VertexLitPosition[4];
uniform vec4 U_VertexLitColor[4];

varying vec4 V_Color;
vec4 GetLightColor1(){
	vec4 pos=vec4(1.0,1.0,0.0,0.0);
	return dot(pos.xyz,normal.xyz)*vec4(1.0,0.0,0.0,1.0);
}
vec4 GetLightColor2(){
	vec4 pos=vec4(-1.0,1.0,0.0,0.0);
	return dot(pos.xyz,normal.xyz)*vec4(0.0,1.0,0.0,1.0);
}
void main(){
	V_Color=dot(U_LightPosition.xyz,normal.xyz)*U_LightColor;//GetLightColor1()+GetLightColor2();
	gl_Position=Alice_ProjectionMatrix*Alice_ViewMatrix*ModelMatrix*position;
}