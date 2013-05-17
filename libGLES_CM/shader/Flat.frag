#ifdef GL_ES
precision highp float;
#endif

uniform vec4 FrontColor;

void main()
{
	gl_FragColor = FrontColor;
}