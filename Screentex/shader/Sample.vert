#ifdef GL_ES
precision highp float;
#endif

attribute vec4 vertexPos;
attribute vec4 texCoord;

uniform mat4 mvp;

varying vec4 texC;

void main()
{
	gl_Position = mvp * vertexPos;

	texC = texCoord;

}

