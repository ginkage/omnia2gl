//Joydeep
uniform mat4 ModelViewProjectionMatrix ;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 TextureMatrix[2];

attribute vec4 Vertex;
attribute vec4 Color;
attribute vec3    Normal;
attribute vec4 MultiTexCoord0;
attribute vec4 MultiTexCoord1;
attribute float PointSize;
vec4 Position;


varying vec4 FrontColor;
varying float FogFragCoord;

//=============================
// point

struct PointParameters
{
    float size;
    float sizeMin;
    float sizeMax;
    float fadeThresholdSize;
    float distanceConstantAttenuation;
    float distanceLinearAttenuation;
    float distanceQuadraticAttenuation;
};

uniform PointParameters Point;
uniform bool PointEnabled;

//varying float DerivedPointSize; 
float DerivedPointSize;              //  not used in fragment shader varying originally

//=============================
// fog

uniform bool FogEnabled;

//
//=============================
// lighting

const int MaxLights = 8;

struct MaterialParameters
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform MaterialParameters FrontNBackMaterial;
uniform bool colorMaterialEnabled;
// currentMaterial global variable is used for copying FrontNBackMaterial because incase color material is enabled then 
// we have to modify the material ambient and diffuse properties with current color which is not possible for uniforms
MaterialParameters currentMaterial; 

struct LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
    vec4 halfVector;
    vec3 spotDirection;
    float spotExponent;
    float spotCutoff; // (range: [0.0, 90.0], 180.0)
    float spotCosCutoff; // cos(spotCutoff) (range: [1.0, 0.0], -1.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    bool isDirectional;
};

uniform LightSourceParameters LightSource[MaxLights];

struct LightModelParameters
{
    vec4 ambient;
    bool isTwoSided;
};

uniform LightModelParameters LightModel;

uniform bool LightEnabled[MaxLights];
uniform bool LightingEnabled;

struct LightModelProducts
{
    vec4 sceneColor;
};

uniform LightModelProducts FrontNBackLightModelProduct;

struct LightProducts
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform LightProducts FrontNBackLightProduct[MaxLights];
    uniform int numLightUsed;

//
//=============================
// texturing

const int MaxTextureCoords = 2;

uniform bool Texture2DEnabled[MaxTextureCoords];

// this is temorary fix for multi texture after compiler bug fix for array we can revert it back (Sandeep Nanda)
//varying vec4 TexCoord[MaxTextureCoords];
varying vec4 TexCoord0;
varying vec4 TexCoord1;

//=============================
// miscellaneous

uniform bool MultiSampleEnabled;

//
// functions
//

vec4 lit(float NdotL, float NdotH, float m);


vec4 light_point_view_local(
    vec3 epos, // the eye space vertex position
    vec3 normal, // the vertex normal
    int idx );

vec4 light_directional_view_local(
    vec3 epos,         //the eye space vertex position
    vec3 normal,         //the vertex normal
    int idx )  ;


void main(void)
{
    // position
    gl_Position = ModelViewProjectionMatrix * Vertex;   //Joydeep
         
    vec4 epos = ModelViewMatrix * Vertex; // The eye-space vertex position  
    
    vec3 normal = normalize(NormalMatrix * Normal);   
    
    FrontColor = Color ;  // Joydeep

    if(Texture2DEnabled[0]) TexCoord0 = TextureMatrix[0]*MultiTexCoord0; // Temporary Fix for Multitexture (Sandeep Nanda)
    if(Texture2DEnabled[1]) TexCoord1 = TextureMatrix[1]*MultiTexCoord1;// Temporary Fix for Multitexture (Sandeep Nanda)

    //
    //==================================
    // lighting
    currentMaterial.ambient = FrontNBackMaterial.ambient;
    currentMaterial.diffuse = FrontNBackMaterial.diffuse;
    currentMaterial.specular = FrontNBackMaterial.specular;
    currentMaterial.shininess = FrontNBackMaterial.shininess;
    
    if (colorMaterialEnabled)
    {
        // If color material is enabled then update the material ambient and diffuse color to
        // current color(generic vertex color or vertex color pointer)
        currentMaterial.ambient = Color;
        currentMaterial.diffuse = Color;
    }
    
    if(LightingEnabled)
    {
        int i;
        
        vec4 vcolor =FrontNBackLightModelProduct.sceneColor; // emission_material + ambient_light * ambient_material             
         
        for(i=0; i <  numLightUsed ; i++)
    {
                 //directional light                          
        if(LightSource[i].isDirectional)
                       vcolor += light_directional_view_local(epos.xyz/epos.w, normal, i);                                
        else // point light
            vcolor += light_point_view_local(epos.xyz, normal, i);
            
    }
        
        vcolor =  clamp(vcolor, 0.0, 1.0);
        FrontColor = vcolor ;
    }

    //
    //==================================
    // points
    // If PointEnabled = 1 means point mode is enabled
    if(PointEnabled)
    {
        gl_PointSize =  PointSize; 
    }
   /* else
    {
        float d;
    
        d = length(epos);
        
        // derivedSize = clamp(size * sqrt(1/(a + b*d + c * d^2)))
        DerivedPointSize = Point.distanceConstantAttenuation + Point.distanceLinearAttenuation * d + 
            Point.distanceQuadraticAttenuation * d * d;
        DerivedPointSize = clamp(Point.size * sqrt(1.0 / DerivedPointSize), Point.sizeMin, Point.sizeMax);
        
        if(MultiSampleEnabled)
        {
            if(DerivedPointSize < Point.fadeThresholdSize)
            {
                // fade
                //FrontColor.a *= (DerivedPointSize * DerivedPointSize / Point.fadeThresholdSize * Point.fadeThresholdSize);
                FrontColor[3] *= (DerivedPointSize * DerivedPointSize / Point.fadeThresholdSize * Point.fadeThresholdSize);
                DerivedPointSize = Point.fadeThresholdSize;
            }
        }
    }*/

    //gl_PointSize = PointSize;
    
    //==================================
    // fog
    
    if(FogEnabled)
    {
        FogFragCoord = abs(epos.z);  
    }
}

// point light and local viewer
vec4 light_point_view_local(
    vec3 epos, // the eye space vertex position
    vec3 normal, // the vertex normal
    int idx )
{
        //LightSourceParameters light = LightSource[idx];

    // vector from vertex to the light
    vec3 vert2light = LightSource[idx].position.xyz - epos; //changed
    
    // light direction
    vec3 ldir = normalize(vert2light);           

    // distance between the two, need this for attenuation
    float d = length(vert2light);         

    //view direction, redundent for multiple lights
    //vec3 vdir = normalize(-epos);  //changed
    vec3 vdir = vec3(0.0,0.0,1.0);
    
    //vec3 eye = vec3(0.0,0.0,0.0);     

    //halfway vector
    vec3 halfv =  normalize(ldir + vdir); 

    // Compute the diffuse and specular coefficients
    //  the diffuse cosine is in coeffs.y, the specular is in coeffs.z
            
    vec4 coeffs = lit(dot(normal,ldir), dot(normal,halfv), FrontNBackMaterial.shininess);

///////////////////////////////////////////////////////////////////////////////////
    float spotEffect ;
    float NdotL = max(dot(normal,ldir),0.0);
    
    float spotDot = dot(-ldir, LightSource[idx].spotDirection);

    if(LightSource[idx].spotCutoff >= 180.00)
        spotEffect = 1.0;
    else if (spotDot < LightSource[idx].spotCosCutoff)
        spotEffect = 0.0; // light adds no contribution
    else
        spotEffect = pow(2.0,log2(spotDot)*LightSource[idx].spotExponent);

//////////////////////////////////////////////////////////////////////////////////

    // Compute the standard shading model
    float att = 1.0/(LightSource[idx].constantAttenuation 
    + d*LightSource[idx].linearAttenuation + d*d*LightSource[idx].quadraticAttenuation);
    vec4 outCol = att* spotEffect *  //attenuation term
        (LightSource[idx].ambient*FrontNBackMaterial.ambient +                       //ambient term
        LightSource[idx].diffuse*FrontNBackMaterial.diffuse*coeffs.y +            //diffuse term
        LightSource[idx].specular*FrontNBackMaterial.specular*coeffs.z);            //specular term

//    vec4 outCol = vec4(0.0,pow(2.0,log2(spotDot)*LightSource[idx].spotExponent),0.0,0.0);
//    vec4 outCol = vec4(0.0,pow(spotDot,LightSource[idx].spotExponent),0.0,0.0);
//    vec4 outCol = vec4(0.0,spotEffect,0.0,0.0);

    return outCol;
}

// directional light and local viewer
vec4 light_directional_view_local(
    vec3 epos,         //the eye space vertex position
    vec3 normal,         //the vertex normal
    int idx )    
{
    //LightSourceParameters light = LightSource[idx];
        //vec3 vdir = normalize(-epos); //changed
    
        vec3 lightDir = normalize(LightSource[idx].position.xyz);
        
    vec3 halfv = normalize(lightDir);
        
    vec4 coeffs = lit(dot(normal, lightDir), dot(normal,halfv),FrontNBackMaterial.shininess);
      
    // Compute the standard shading model
    vec4 outCol = LightSource[idx].ambient*FrontNBackMaterial.ambient +               //ambient term
        LightSource[idx].diffuse* FrontNBackMaterial.diffuse*coeffs.y  +      //diffuse term
        LightSource[idx].specular*FrontNBackMaterial.specular*coeffs.z ;            //specular term

    return outCol;
}

vec4 lit(float NdotL, float NdotH, float m)
{
    vec4 coeff;
    // ambient coefficient, which is always 1.0
    coeff.x = 1.0;
    
    // diffuse coefficient
    if(NdotL < 0.0) coeff.y = 0.0;
    else coeff.y = NdotL;
    
    //specular coefficient
    if(NdotL <= 0.0 || NdotH < 0.0) 
        coeff.z = 0.0;
    else 
        coeff.z = pow(NdotH, m);
    
    coeff.w = 1.0;
        
    return coeff;
}
