varying vec4 V_Normal;
void main(){
	gl_FragColor=vec4(normalize(V_Normal.xyz),0.0);
}