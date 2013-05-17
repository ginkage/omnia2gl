#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D TexImages0;
uniform sampler2D TexImages1;

varying vec4 TexCoord0;
varying vec4 TexCoord1;

void main()
{
	gl_FragColor = texture2D(TexImages0, TexCoord0.xy) + texture2D(TexImages1, TexCoord1.xy);
}