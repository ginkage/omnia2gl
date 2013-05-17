#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Vertex;

uniform mat4 ModelViewProjectionMatrix;

void main()
{
	gl_Position = ModelViewProjectionMatrix * Vertex;
}

