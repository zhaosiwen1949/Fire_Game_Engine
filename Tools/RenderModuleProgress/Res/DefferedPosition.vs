attribute vec4 position;

uniform mat4 ModelMatrix;
uniform mat4 Alice_ViewMatrix;
uniform mat4 Alice_ProjectionMatrix;

varying vec4 V_WorldPos;
void main(){
	V_WorldPos=ModelMatrix*position;
	gl_Position=Alice_ProjectionMatrix*Alice_ViewMatrix*ModelMatrix*position;
}