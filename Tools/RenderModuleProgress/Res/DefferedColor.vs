attribute vec4 position;
attribute vec4 texcoord;

uniform mat4 ModelMatrix;
uniform mat4 Alice_ViewMatrix;
uniform mat4 Alice_ProjectionMatrix;

varying vec4 V_Texcoord;

void main(){
	V_Texcoord=texcoord;
	gl_Position=Alice_ProjectionMatrix*Alice_ViewMatrix*ModelMatrix*position;
}