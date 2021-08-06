uniform sampler2D U_PositionTexture;
uniform sampler2D U_NormalTexture;
uniform sampler2D U_ColorTexture;

uniform vec4 U_LightPos;
uniform vec4 U_LightColor;
uniform vec4 U_LightSetting;
uniform vec4 U_LightSetting1;

varying vec4 V_Texcoord;
void main(){
	vec4 data=texture2D(U_PositionTexture,V_Texcoord.xy);
	vec3 world_pos=data.xyz;

	vec4 normal_data=texture2D(U_NormalTexture,V_Texcoord.xy);
	vec3 normal=normal_data.xyz;

	vec4 color_data=texture2D(U_ColorTexture,V_Texcoord.xy);
	vec3 color=color_data.rgb;

	float intensity=0.0;
	vec3 final_color=vec3(0.0,0.0,0.0);
	float point_radius=U_LightSetting1.x;
	if(U_LightSetting.w!=0.0){
		if(U_LightPos.w==0.0){
			vec3 object_to_light_source=normalize(U_LightPos.xyz);
			intensity=dot(object_to_light_source,normal);
		}else{
			vec3 object_to_light_source=U_LightPos.xyz-world_pos;
			vec3 light_direction=normalize(object_to_light_source);
			float distance_from_object_surface_to_light=length(object_to_light_source);
			float delta=point_radius-distance_from_object_surface_to_light;
			float factor_linear=2.0;
			float attenuation=1.0/(1.0+distance_from_object_surface_to_light*factor_linear);
			if(delta>=0.0){
				intensity=dot(light_direction,normal)*attenuation*delta/point_radius;
			}else{
				intensity=0.0;
			}
		}
		final_color=U_LightColor.rgb*intensity;
	}else{
		final_color=color;
	}
	if(length(normal)==0.0){
		gl_FragColor=vec4(0.0,0.0,0.0,1.0);
	}else{
		gl_FragColor=vec4(final_color,1.0);
	}
}