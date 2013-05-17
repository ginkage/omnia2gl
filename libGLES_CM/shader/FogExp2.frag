#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D TexImages0;

varying vec4 TexCoord0;
varying vec4 FrontColor;
varying float FogFragCoord;

#define LOG2E 1.442695 // 1 / log(2)

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
	// f = e^(-(density * z)^2)
	vec4 fragColor = texture2D(TexImages0, TexCoord0.xy) * FrontColor;
	float fog = clamp(exp2(-Fog.density * Fog.density * FogFragCoord * FogFragCoord * LOG2E), 0.0, 1.0);
	gl_FragColor = mix(Fog.color, fragColor, fog);
}