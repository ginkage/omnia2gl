#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D TexImages0;

varying vec4 TexCoord0;
varying vec4 FrontColor;
varying float FogFragCoord;

#define FOGMODE_LINEAR 1
#define FOGMODE_EXP 2
#define FOGMODE_EXP2 3

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
	float fog;
	vec4 fragColor = texture2D(TexImages0, TexCoord0.xy) * FrontColor;

	if((Fog.mode) == FOGMODE_LINEAR)
	{
		// f= (end - z) * (end - start)
		fog = clamp((Fog.end - FogFragCoord) * Fog.scale, 0.0, 1.0);
	}
	else if((Fog.mode) == FOGMODE_EXP)
	{
		// f = e^(-density * z)
		fog = clamp(exp2(-Fog.density * FogFragCoord * LOG2E), 0.0, 1.0);
	}
	else if((Fog.mode) == FOGMODE_EXP2)
	{
		// f = e^(-(density * z)^2)
		fog = clamp(exp2(-Fog.density * Fog.density * FogFragCoord * FogFragCoord * LOG2E), 0.0, 1.0);
	}

	gl_FragColor = mix(Fog.color, fragColor, fog);
}