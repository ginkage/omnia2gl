#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Vertex;
attribute vec4 MultiTexCoord0;
attribute vec4 MultiTexCoord1;

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 TextureMatrix[2];

varying vec4 TexCoord0;
varying vec4 TexCoord1;

void main()
{
	gl_Position = ModelViewProjectionMatrix * Vertex;
	TexCoord0 = MultiTexCoord0 * TextureMatrix[0];
	TexCoord1 = MultiTexCoord1 * TextureMatrix[1];
}

