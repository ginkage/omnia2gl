#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Vertex;
attribute vec4 MultiTexCoord0;

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 TextureMatrix;

varying vec4 TexCoord0;

void main()
{
	gl_Position = ModelViewProjectionMatrix * Vertex;
	TexCoord0 = MultiTexCoord0 * TextureMatrix;
}

