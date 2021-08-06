#include "HardCodedShader.h"
namespace Alice {
#define HARD_CODED_CODE(A)  #A
static const char* sTexture2DSpriteVertexShader = HARD_CODED_CODE(
attribute vec4 pos;
attribute vec4 texcoord;
attribute vec4 tangent;
attribute vec4 texcoord1;
uniform mat4 Alice_Matrix_M;
uniform mat4 Alice_Matrix_V;
uniform mat4 Alice_Matrix_P;
varying vec4 V_Light;
varying vec4 V_Dark;
varying vec4 V_Texcoord;
void main() {
	V_Light = tangent;
	V_Dark = texcoord1;
	V_Texcoord = texcoord;
	gl_Position = Alice_Matrix_P * Alice_Matrix_V*Alice_Matrix_M*pos;
}
);
static const char* sTexture2DSpriteFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
uniform sampler2D U_MainTexture;
varying vec4 V_Light;
varying vec4 V_Dark;
varying vec4 V_Texcoord;
void main() {
	vec4 texture_color = texture2D(U_MainTexture, V_Texcoord.xy);
	float alpha = texture_color.a * V_Light.a;
	gl_FragColor.a = alpha;
	gl_FragColor.rgb = (vec3((texture_color.a - 1.0) * V_Dark.a + 1.0) - texture_color.rgb) * V_Dark.rgb + texture_color.rgb * V_Light.rgb;
}
);
static const char* sColorVertexShader = HARD_CODED_CODE(
attribute vec4 pos;
attribute vec4 tangent;
uniform mat4 Alice_Matrix_M;
uniform mat4 Alice_Matrix_V;
uniform mat4 Alice_Matrix_P;
varying vec4 V_Color;
void main() {
	V_Color = tangent;
	gl_Position = Alice_Matrix_P * Alice_Matrix_V*Alice_Matrix_M*pos;
}
);
static const char* sColorFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
varying vec4 V_Color;
void main() {
	gl_FragColor = V_Color;
}
);
static const char* sSimpleColorVertexShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
attribute vec4 pos;
uniform mat4 Alice_Matrix_M;
uniform mat4 Alice_Matrix_V;
uniform mat4 Alice_Matrix_P;
void main() {
	gl_Position = Alice_Matrix_P * Alice_Matrix_V*Alice_Matrix_M*pos;
}
);
static const char* sSimpleColorFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
uniform vec4 U_Color;
void main() {
	gl_FragColor = U_Color;
}
);
static const char* sDynamicFontVertexShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
attribute vec4 pos;
attribute vec4 texcoord;
attribute vec4 normal;
attribute vec4 tangent;
attribute vec4 texcoord1;
uniform mat4 Alice_Matrix_M;
uniform mat4 Alice_Matrix_V;
uniform mat4 Alice_Matrix_P;
varying vec4 V_Color;
varying vec4 V_TexCoord; 
varying vec4 V_OutlineColor; 
varying vec4 V_ShadowColor;
void main()
{
	V_Color = tangent;
	V_TexCoord = texcoord;
	V_OutlineColor = normal;
	V_ShadowColor = texcoord1;
	gl_Position = Alice_Matrix_P * Alice_Matrix_V*Alice_Matrix_M * pos;
}
);
static const char* sDynamicFontFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
varying vec4 V_Color;
varying vec4 V_TexCoord;
varying vec4 V_OutlineColor;
varying vec4 V_ShadowColor;
uniform sampler2D U_MainTexture;
void main() {
	vec4 textureColor = texture2D(U_MainTexture, V_TexCoord.xy);
	float fontAlpha = textureColor.a;
	float outlineAlpha = textureColor.r;
	float shadowAlpha = textureColor.g;
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	if (shadowAlpha > 0.0) {
		if (outlineAlpha > 0.0) {
			if (fontAlpha > 0.0) {
				color.rgb = mix(V_OutlineColor.rgb, V_Color.rgb, fontAlpha);
				color.a = mix(outlineAlpha, fontAlpha, fontAlpha);
			}
			else if (outlineAlpha > 0.0) {
				color.rgb = mix(V_ShadowColor.rgb, V_OutlineColor.rgb, outlineAlpha);
				color.a = mix(shadowAlpha, outlineAlpha, outlineAlpha);
			}
			else {
				color = V_ShadowColor;
			}
		}
		else {
			if (fontAlpha>0.0){
				color.rgb = V_Color.rgb;
			}
			else {
				color.rgb = V_ShadowColor.rgb;
			}
			color.a = mix(shadowAlpha, fontAlpha, fontAlpha);
		}
	}
	else {
		if (outlineAlpha>0.0){
			color.rgb = mix(V_OutlineColor.rgb, V_Color.rgb, fontAlpha);
			color.a = mix(outlineAlpha, fontAlpha, fontAlpha);
		}
		else {
			color.rgb = V_Color.rgb;
			color.a = fontAlpha;
		}
	}
	color.a *= V_Color.a;
	gl_FragColor = color;
}
);
static const char* sDefaultFrameBufferRenderVertexShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
attribute vec4 pos;
attribute vec4 texcoord;
varying vec4 V_TexCoord;
void main()
{
	V_TexCoord = texcoord;
	gl_Position = pos;
}
);
static const char* sDefaultFrameBufferRenderFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
	precision lowp float;
\n#endif\n
varying vec4 V_TexCoord;
uniform sampler2D Alice_FrameBuffer;
void main() {
	vec4 color = texture2D(Alice_FrameBuffer, V_TexCoord.xy);
	gl_FragColor = color;
}
);
static const char* sStencilVertexShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
attribute vec4 pos;
uniform mat4 Alice_Matrix_M;
uniform mat4 Alice_Matrix_V;
uniform mat4 Alice_Matrix_P;
void main(){
	gl_Position = Alice_Matrix_P * Alice_Matrix_V*Alice_Matrix_M*pos;
}
);
static const char* sStencilFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
void main() {
	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
);
static const char* sUIVertexShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
attribute vec4 pos;
attribute vec4 texcoord;
attribute vec4 tangent;
attribute vec4 texcoord1;
uniform mat4 Alice_Matrix_M;
uniform vec4 Alice_Canvas;
varying vec4 V_Light;
varying vec4 V_Dark;
varying vec4 V_Texcoord;
void main() {
	V_Light = tangent;
	V_Dark = texcoord1;
	V_Texcoord = texcoord;
	vec4 screen_pos = Alice_Matrix_M * pos;
	gl_Position = vec4(screen_pos.x/ Alice_Canvas.x,screen_pos.y/ Alice_Canvas.y,screen_pos.z/ Alice_Canvas.z,1.0);
}
);
static const char* sUIFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
uniform sampler2D U_Texture;
varying vec4 V_Texcoord;
varying vec4 V_Light;
varying vec4 V_Dark;
void main() {
	vec4 texture_color = texture2D(U_Texture, V_Texcoord.xy);
	float alpha = texture_color.a * V_Light.a;
	gl_FragColor.a = alpha;
	gl_FragColor.rgb = (vec3((texture_color.a - 1.0) * V_Dark.a + 1.0) - texture_color.rgb) * V_Dark.rgb + texture_color.rgb * V_Light.rgb;
}
);
static const char* sStencilUIVertexShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
attribute vec4 pos;
uniform mat4 Alice_Matrix_M;
uniform vec4 Alice_Canvas;
void main() {
	vec4 screen_pos = Alice_Matrix_M * pos;
	gl_Position = vec4(screen_pos.x / Alice_Canvas.x, screen_pos.y / Alice_Canvas.y, screen_pos.z / Alice_Canvas.z, 1.0);
}
);
static const char* sStencilUIFragmentShader = HARD_CODED_CODE(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n
void main() {
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
);
	const char * GetBuiltinTexture2DSpriteVertexShader() {
		return sTexture2DSpriteVertexShader;
	}
	const char * GetBuiltinTexture2DSpriteFragmentShader() {
		return sTexture2DSpriteFragmentShader;
	}
	const char * GetBuiltinColorVertexShader() {
		return sColorVertexShader;
	}
	const char * GetBuiltinColorFragmentShader() {
		return sColorFragmentShader;
	}
	const char * GetBuiltinSimpleColorVertexShader() {
		return sSimpleColorVertexShader;
	}
	const char * GetBuiltinSimpleColorFragmentShader() {
		return sSimpleColorFragmentShader;
	}
	const char * GetBuiltinDynamicFontVertexShader() {
		return sDynamicFontVertexShader;
	}
	const char * GetBuiltinDynamicFontFragmentShader() {
		return sDynamicFontFragmentShader;
	}
	const char * GetBuiltinDefaultFrameBufferRenderVertexShader() {
		return sDefaultFrameBufferRenderVertexShader;
	}
	const char * GetBuiltinDefaultFrameBufferRenderFragmentShader() {
		return sDefaultFrameBufferRenderFragmentShader;
	}
	const char * GetBuiltinStencilRendererVertexShader() {
		return sStencilVertexShader;
	}
	const char * GetBuiltinStencilRendererFragmentShader() {
		return sStencilFragmentShader;
	}
	const char * GetBuiltinUIVertexShader() {
		return sUIVertexShader;
	}
	const char * GetBuiltinUIFragmentShader() {
		return sUIFragmentShader;
	}
	const char * GetBuiltinStencilUIVertexShader() {
		return sStencilUIVertexShader;
	}
	const char * GetBuiltinStencilUIFragmentShader() {
		return sStencilUIFragmentShader;
	}
}
