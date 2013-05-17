#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Vertex;
attribute vec4 Color;

uniform mat4 ModelViewProjectionMatrix;

varying vec4 FrontColor;

void main()
{
	gl_Position = ModelViewProjectionMatrix * Vertex;
	FrontColor = Color;
}

