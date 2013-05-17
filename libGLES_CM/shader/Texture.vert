#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Vertex;
attribute vec4 MultiTexCoord0;

uniform mat4 ModelViewProjectionMatrix;
uniform vec4 TexShift0;

varying vec4 TexCoord0;
invariant gl_Position;

void main()
{
	gl_Position = ModelViewProjectionMatrix * Vertex;
	TexCoord0 = MultiTexCoord0 + TexShift0;
}

