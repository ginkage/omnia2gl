#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D TexImages0;

varying vec4 texC;

void main()
{
	gl_FragColor = texture2D(TexImages0, texC.xy*1.0);
}