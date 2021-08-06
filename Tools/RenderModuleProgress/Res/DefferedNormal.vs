attribute vec4 position;
attribute vec4 normal;

uniform mat4 ModelMatrix;
uniform mat4 Alice_ViewMatrix;
uniform mat4 Alice_ProjectionMatrix;

varying vec4 V_Normal;
void main(){
	V_Normal=ModelMatrix*normal;
	gl_Position=Alice_ProjectionMatrix*Alice_ViewMatrix*ModelMatrix*position;
}