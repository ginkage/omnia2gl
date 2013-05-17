#ifdef GL_ES
precision highp float;
#endif



varying vec4 FrontColor;
varying float FogFragCoord;


//changes end

//=================================
// fog

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
uniform bool FogEnabled;

//=================================
// texturing
const int MaxTextureCoords = 2;
const int MaxTextureImageUnits = 2;
 
uniform sampler2D TexImages0;
uniform sampler2D TexImages1;
uniform bool Texture2DEnabled[MaxTextureImageUnits];

// this is temorary fix for multi texture after compiler bug fix for array we can revert it back (Sandeep Nanda)
//varying vec4 TexCoord[MaxTextureCoords];
varying vec4 TexCoord0;
varying vec4 TexCoord1;


//=================================
// texture environment

#define TEX_ENV_MODE_MODULATE   1    
#define TEX_ENV_MODE_REPLACE    2
#define TEX_ENV_MODE_DECAL        3
#define TEX_ENV_MODE_BLEND        4
#define TEX_ENV_MODE_ADD        5
#define TEX_ENV_MODE_COMBINE    6

// combine RGB
//
#define   TEX_COMBINE_RGB_MODULATE        1
#define   TEX_COMBINE_RGB_REPLACE         2
#define   TEX_COMBINE_RGB_ADD_SIGNED    3
#define   TEX_COMBINE_RGB_INTERPOLATE    4
#define   TEX_COMBINE_RGB_ADD                 5
#define   TEX_COMBINE_RGB_SUBTRACT        6
#define   TEX_COMBINE_RGB_DOT3_RGB        7
#define   TEX_COMBINE_RGB_DOT3_RGBA    8

// combine alpha
//
#define   TEX_COMBINE_ALPHA_MODULATE    1
#define   TEX_COMBINE_ALPHA_REPLACE         2
#define   TEX_COMBINE_ALPHA_ADD_SIGNED    3
#define   TEX_COMBINE_ALPHA_INTERPOLATE    4
#define   TEX_COMBINE_ALPHA_ADD             5
#define   TEX_COMBINE_ALPHA_SUBTRACT    6

// Combiner RGB and ALPHA source arguments
#define TEX_ENV_COMBINER_SRC_TEXTURE        1
#define TEX_ENV_COMBINER_SRC_CONSTANT        2
#define TEX_ENV_COMBINER_SRC_PRIMARY_COLOR    3
#define TEX_ENV_COMBINER_SRC_PREVIOUS        4

//
// Combiner RGB operands
//
#define TEX_ENV_COMBINER_RGB_OP_SRC_COLOR            1
#define TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_COLOR    2
#define TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA            3
#define TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_ALPHA    4

//
// Combiner ALPHA operands
//
#define TEX_ENV_COMBINER_ALPHA_OP_SRC_ALPHA                1
#define TEX_ENV_COMBINER_ALPHA_OP_ONE_MINUS_SRC_ALPHA    2

struct _TexEnv
{
    vec4 color;
    int mode;
    
    int combineRgb;
    int combineAlpha;
    
    int srcRgb[3];
    int srcAlpha[3];
    
    int operandRgb[3];
    int operandAlpha[3];
    
    float rgbScale;
    float alphaScale;
};

uniform _TexEnv TexEnv[MaxTextureImageUnits];

vec4 tex_env_combiner(vec4 fragColor, vec4 prevColor, vec4 srcColor, int nth);

vec4 tex_arg_calculation(vec4 fragColor, vec4 prevColor, vec4 srcColor, vec4 EnvColor, int srcrgb, int opndrgb, int srcAlpha, int operandAlpha);
vec4 tex_arg_calculation(vec4 fragColor, vec4 prevColor, vec4 srcColor, vec4 EnvColor, int srcrgb, int opndrgb, int srcAlpha, int operandAlpha)
{
    vec4 temp;
        if((srcrgb) == TEX_ENV_COMBINER_SRC_TEXTURE)
        {
            if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_COLOR)
                temp.rgb = srcColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_COLOR)
                temp.rgb = 1.0 - srcColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA)
                temp.rgb = vec3(srcColor.a);
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_ALPHA)
                temp.rgb = vec3(1.0 - srcColor.a);
        }
        else if((srcrgb) == TEX_ENV_COMBINER_SRC_CONSTANT)
        {
            if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_COLOR)
                temp.rgb = EnvColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_COLOR)
                temp.rgb = 1.0 - EnvColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA)
                temp.rgb = vec3(EnvColor.a);
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_ALPHA)
                temp.rgb = vec3(1.0 - EnvColor.a);
        }
        else if((srcrgb) == TEX_ENV_COMBINER_SRC_PRIMARY_COLOR)
        {
            if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_COLOR)
                temp.rgb = fragColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_COLOR)
                temp.rgb = 1.0 - fragColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA)
                temp.rgb = vec3(fragColor.a);
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_ALPHA)
                temp.rgb = vec3(1.0 - fragColor.a);
        }
        else if((srcrgb) == TEX_ENV_COMBINER_SRC_PREVIOUS)
        {
            if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_COLOR)
                temp.rgb = prevColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_COLOR)
                temp.rgb = 1.0 - prevColor.rgb;
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA)
                temp.rgb = vec3(prevColor.a);
            else if((opndrgb) == TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_ALPHA)
                temp.rgb = vec3(1.0 - prevColor.a);
        }
        
        //
        // COMBINE_ALPHA
        //
        if((srcAlpha) == TEX_ENV_COMBINER_SRC_TEXTURE)
        {
            if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_SRC_ALPHA)
                temp.a = srcColor.a;
            else if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_ONE_MINUS_SRC_ALPHA)
                temp.a = 1.0 - srcColor.a;
        }
        else if((srcAlpha) == TEX_ENV_COMBINER_SRC_CONSTANT)
        {
            if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_SRC_ALPHA)
                temp.a = EnvColor.a;
            else if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_ONE_MINUS_SRC_ALPHA)
                temp.a = 1.0 - EnvColor.a;
        }
        else if((srcAlpha) == TEX_ENV_COMBINER_SRC_PRIMARY_COLOR)
        {
            if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_SRC_ALPHA)
                temp.a = fragColor.a;
            else if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_ONE_MINUS_SRC_ALPHA)
                temp.a = 1.0 - fragColor.a;
        }
        else if((srcAlpha) == TEX_ENV_COMBINER_SRC_PREVIOUS)
        {
            if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_SRC_ALPHA)
                temp.a = prevColor.a;
            else if((operandAlpha) == TEX_ENV_COMBINER_ALPHA_OP_ONE_MINUS_SRC_ALPHA)
                temp.a = 1.0 - prevColor.a;
        }
    return temp;
}
vec4 tex_env_combiner(vec4 fragColor, vec4 prevColor, vec4 srcColor, int nth)
{
    int i;
    vec4 arg[3], c;
    //vec4 temp;
     int srcrgb, opndrgb,srcAlpha, operandAlpha;
    vec4 EnvColor;
    float rgbScale, alphaScale;
    int combrgb, combalpha;
    
    if (nth == 0)
    {
            srcrgb = TexEnv[0].srcRgb[0];
            opndrgb = TexEnv[0].operandRgb[0];
            srcAlpha = TexEnv[0].srcAlpha[0];
            operandAlpha = TexEnv[0].operandAlpha[0];
            EnvColor = TexEnv[0].color;
            rgbScale = TexEnv[0].rgbScale;
            alphaScale = TexEnv[0].alphaScale;
            combrgb = TexEnv[0].combineRgb;
            combalpha = TexEnv[0].combineAlpha;
    }
    else if (nth == 1)
    {
            srcrgb = TexEnv[1].srcRgb[0];
            opndrgb = TexEnv[1].operandRgb[0];
            srcAlpha = TexEnv[1].srcAlpha[0];
            operandAlpha = TexEnv[1].operandAlpha[0];
             EnvColor = TexEnv[1].color;
            rgbScale = TexEnv[1].rgbScale;
            alphaScale = TexEnv[1].alphaScale;
            combrgb = TexEnv[1].combineRgb;
            combalpha = TexEnv[1].combineAlpha;
    }
    arg[0] = tex_arg_calculation (fragColor,prevColor,srcColor,EnvColor, srcrgb, opndrgb, srcAlpha, operandAlpha);
    
    if (nth == 0)
    {
            srcrgb = TexEnv[0].srcRgb[1];
            opndrgb = TexEnv[0].operandRgb[1];
            srcAlpha = TexEnv[0].srcAlpha[1];
            operandAlpha = TexEnv[0].operandAlpha[1];
    }
    else if (nth == 1)
    {
            srcrgb = TexEnv[1].srcRgb[1];
            opndrgb = TexEnv[1].operandRgb[1];
            srcAlpha = TexEnv[1].srcAlpha[1];
            operandAlpha = TexEnv[1].operandAlpha[1];
    }
    
    arg[1] = tex_arg_calculation (fragColor,prevColor,srcColor,EnvColor, srcrgb, opndrgb, srcAlpha, operandAlpha);
    
    if (nth == 0)
    {
            srcrgb = TexEnv[0].srcRgb[2];
            opndrgb = TexEnv[0].operandRgb[2];
            srcAlpha = TexEnv[0].srcAlpha[2];
            operandAlpha = TexEnv[0].operandAlpha[2];
    }
    else if (nth == 1)
    {
            srcrgb = TexEnv[1].srcRgb[2];
            opndrgb = TexEnv[1].operandRgb[2];
            srcAlpha = TexEnv[1].srcAlpha[2];
            operandAlpha = TexEnv[1].operandAlpha[2];
    }
    
    arg[2] = tex_arg_calculation (fragColor,prevColor,srcColor,EnvColor, srcrgb, opndrgb, srcAlpha, operandAlpha);

    //
    // COMBINE
    //
    
    if(combrgb == TEX_COMBINE_RGB_REPLACE)
        c.rgb = arg[0].rgb;
    else if(combrgb == TEX_COMBINE_RGB_MODULATE)
        c.rgb = arg[0].rgb * arg[1].rgb;
    else if(combrgb == TEX_COMBINE_RGB_ADD)
        c.rgb = arg[0].rgb + arg[1].rgb;
    else if(combrgb == TEX_COMBINE_RGB_ADD_SIGNED)
        c.rgb = arg[0].rgb + arg[1].rgb - 0.5;
    else if(combrgb == TEX_COMBINE_RGB_INTERPOLATE)
        c.rgb = mix(arg[1].rgb, arg[0].rgb, arg[2].rgb);
    else if(combrgb == TEX_COMBINE_RGB_SUBTRACT)
        c.rgb = arg[0].rgb - arg[1].rgb;
    else if(combrgb == TEX_COMBINE_RGB_DOT3_RGB ||
        combrgb == TEX_COMBINE_RGB_DOT3_RGBA)
    {
        float d;
        
        d = 4.0 *((arg[0].r - 0.5)*(arg[1].r - 0.5) + (arg[0].g - 0.5)*(arg[1].g - 0.5) + (arg[0].b - 0.5)*(arg[1].b - 0.5));
        c = vec4(d);
    }
    
   c.rgb = rgbScale * c.rgb;
    
    if (combrgb != TEX_COMBINE_RGB_DOT3_RGBA)
    {
            if(combalpha == TEX_COMBINE_ALPHA_REPLACE)
                c.a = arg[0].a;
            else if(combalpha == TEX_COMBINE_ALPHA_MODULATE)
                c.a = arg[0].a * arg[1].a;
            else if(combalpha == TEX_COMBINE_ALPHA_ADD)
                c.a = arg[0].a + arg[1].a;
            else if(combalpha == TEX_COMBINE_ALPHA_ADD_SIGNED)
                c.a = arg[0].a + arg[1].a - 0.5;
            else if(combalpha == TEX_COMBINE_ALPHA_INTERPOLATE)
                c.a = mix(arg[1].a, arg[0].a, arg[2].a);
            else if(combalpha == TEX_COMBINE_ALPHA_SUBTRACT)
                c.a = arg[0].a - arg[1].a;
    }
    
    c.a = alphaScale * c.a;
    
    return c;
}


void main( void )
{
    int i;
    vec4 fragColor = FrontColor;  

    // texturing

    vec4 prevColor, srcColor;
    
    prevColor = fragColor;
//    for(i=0; i<MaxTextureImageUnits; i++)
    {
        if(Texture2DEnabled[0])
        {
            srcColor = texture2D(TexImages0, TexCoord0.xy);
            int mode = (TexEnv[0].mode);
            if(mode == TEX_ENV_MODE_MODULATE)
                prevColor *= srcColor;
            else if(mode == TEX_ENV_MODE_REPLACE)
                prevColor = srcColor;
            else if(mode == TEX_ENV_MODE_DECAL)
                 prevColor = vec4(mix(prevColor.rgb, srcColor.rgb, srcColor.a), prevColor.a);
            else if(mode == TEX_ENV_MODE_BLEND)
                prevColor = vec4(mix(prevColor.rgb, TexEnv[0].color.rgb, srcColor.rgb), prevColor.a * srcColor.a);
            else if(mode == TEX_ENV_MODE_ADD)
                prevColor = vec4(prevColor.rgb + srcColor.rgb, prevColor.a * srcColor.a);
            else if(mode == TEX_ENV_MODE_COMBINE)
                prevColor = tex_env_combiner(fragColor, prevColor, srcColor, 0);
            else prevColor *= srcColor;
            
            prevColor = clamp(prevColor, 0.0, 1.0);
        }
       // this is the change for multi texture implementation (we unroll the loop because array subscript is not supported by compiler )  (Sandeep Nanda)
        if(Texture2DEnabled[1])
        {
            srcColor = texture2D(TexImages1, TexCoord1.xy);
            int mode = (TexEnv[1].mode);
            if(mode == TEX_ENV_MODE_MODULATE)
                prevColor *= srcColor;
            else if(mode == TEX_ENV_MODE_REPLACE)
                prevColor = srcColor;
            else if(mode == TEX_ENV_MODE_DECAL)
                 prevColor = vec4(mix(prevColor.rgb, srcColor.rgb, srcColor.a), prevColor.a);
            else if(mode == TEX_ENV_MODE_BLEND)
                prevColor = vec4(mix(prevColor.rgb, TexEnv[0].color.rgb, srcColor.rgb), prevColor.a * srcColor.a);
            else if(mode == TEX_ENV_MODE_ADD)
                prevColor = vec4(prevColor.rgb + srcColor.rgb, prevColor.a * srcColor.a);
            else if(mode == TEX_ENV_MODE_COMBINE)
                prevColor = tex_env_combiner(fragColor, prevColor, srcColor, 1);
            else prevColor *= srcColor;
            
            prevColor = clamp(prevColor, 0.0, 1.0);
        }
    }  
    fragColor = prevColor;

    //=========================================
    // fog

    if(FogEnabled)
    {
        float fog;
        
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
        fragColor = mix(Fog.color, fragColor, fog);
    }
    
    //=========================================
    gl_FragColor = fragColor;
}
