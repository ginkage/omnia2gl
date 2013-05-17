#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D TexImages0;

varying vec4 TexCoord0;
varying vec4 FrontColor;
varying float FogFragCoord;

struct FogParameters
{
    float density;
    float start;
    float end;
    vec4 color;
    float scale; // Derived: 1.0 / (end - start)
    int mode;
};

uniform FogParameters Fog;

void main()
{
	// f= (end - z) * (end - start)
	vec4 fragColor = texture2D(TexImages0, TexCoord0.xy) * FrontColor;
	float fog = clamp((Fog.end - FogFragCoord) * Fog.scale, 0.0, 1.0);
	gl_FragColor = mix(Fog.color, fragColor, fog);
}