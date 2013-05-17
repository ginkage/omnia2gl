#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D TexImages0;

varying vec4 TexCoord0;
varying vec4 FrontColor;

void main()
{
	gl_FragColor = texture2D(TexImages0, TexCoord0.xy) * FrontColor;
}