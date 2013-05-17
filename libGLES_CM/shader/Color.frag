#ifdef GL_ES
precision highp float;
#endif

varying vec4 FrontColor;

void main()
{
	gl_FragColor = FrontColor;
}