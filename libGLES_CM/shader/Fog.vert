#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Vertex;
attribute vec4 Color;
attribute vec4 MultiTexCoord0;

uniform mat4 ModelViewProjectionMatrix;
// uniform mat4 TextureMatrix;
uniform vec4 ModelViewZ;
uniform vec4 TexShift0;

varying vec4 FrontColor;
varying vec4 TexCoord0;
varying float FogFragCoord;

void main()
{
	gl_Position = ModelViewProjectionMatrix * Vertex;
	FrontColor = Color;
	TexCoord0 = MultiTexCoord0 + TexShift0; // * TextureMatrix;
	FogFragCoord = dot(Vertex, ModelViewZ);	
}

