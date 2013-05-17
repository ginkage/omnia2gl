/*

OpenGL ES 1.0 compatibility layer for Samsung Omnia 2
Copyright (c) 2009 Ivan Podogov http://omnia2gl.sourceforge.net

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

typedef void GLvoid;

#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701
#define GL_TEXTURE                        0x1702

#define GL_FLAT                           0x1D00
#define GL_SMOOTH                         0x1D01

#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_TEXTURE_COORD_ARRAY            0x8078

#define GL_CURRENT_COLOR                  0x0B00
#define GL_CURRENT_NORMAL                 0x0B02
#define GL_CURRENT_TEXTURE_COORDS         0x0B03
#define GL_POINT_SIZE                     0x0B11
#define GL_POINT_SIZE_MIN                 0x8126
#define GL_POINT_SIZE_MAX                 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define GL_POINT_DISTANCE_ATTENUATION     0x8129
#define GL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define GL_LINE_WIDTH                     0x0B21
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_SHADE_MODEL                    0x0B54
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_MATRIX_MODE                    0x0BA0
#define GL_VIEWPORT                       0x0BA2
#define GL_MODELVIEW_STACK_DEPTH          0x0BA3
#define GL_PROJECTION_STACK_DEPTH         0x0BA4
#define GL_TEXTURE_STACK_DEPTH            0x0BA5
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_PROJECTION_MATRIX              0x0BA7
#define GL_TEXTURE_MATRIX                 0x0BA8
#define GL_ALPHA_TEST_FUNC                0x0BC1
#define GL_ALPHA_TEST_REF                 0x0BC2
#define GL_BLEND_DST                      0x0BE0
#define GL_BLEND_SRC                      0x0BE1
#define GL_LOGIC_OP_MODE                  0x0BF0
#define GL_SCISSOR_BOX                    0x0C10
#define GL_SCISSOR_TEST                   0x0C11
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAX_LIGHTS                     0x0D31
#define GL_MAX_CLIP_PLANES                0x0D32
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define GL_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_MAX_TEXTURE_UNITS              0x84E2
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_TEXTURE_BINDING_2D             0x8069
#define GL_VERTEX_ARRAY_SIZE              0x807A
#define GL_VERTEX_ARRAY_TYPE              0x807B
#define GL_VERTEX_ARRAY_STRIDE            0x807C
#define GL_NORMAL_ARRAY_TYPE              0x807E
#define GL_NORMAL_ARRAY_STRIDE            0x807F
#define GL_COLOR_ARRAY_SIZE               0x8081
#define GL_COLOR_ARRAY_TYPE               0x8082
#define GL_COLOR_ARRAY_STRIDE             0x8083
#define GL_TEXTURE_COORD_ARRAY_SIZE       0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE       0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE     0x808A
#define GL_VERTEX_ARRAY_POINTER           0x808E
#define GL_NORMAL_ARRAY_POINTER           0x808F
#define GL_COLOR_ARRAY_POINTER            0x8090
#define GL_TEXTURE_COORD_ARRAY_POINTER    0x8092
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB

#define GL_FOG                            0x0B60
#define GL_LIGHTING                       0x0B50
#define GL_TEXTURE_2D                     0x0DE1
#define GL_CULL_FACE                      0x0B44
#define GL_ALPHA_TEST                     0x0BC0
#define GL_BLEND                          0x0BE2
#define GL_COLOR_LOGIC_OP                 0x0BF2
#define GL_DITHER                         0x0BD0
#define GL_STENCIL_TEST                   0x0B90
#define GL_DEPTH_TEST                     0x0B71
#define GL_LIGHT0                         0x4000
#define GL_LIGHT1                         0x4001
#define GL_LIGHT2                         0x4002
#define GL_LIGHT3                         0x4003
#define GL_LIGHT4                         0x4004
#define GL_LIGHT5                         0x4005
#define GL_LIGHT6                         0x4006
#define GL_LIGHT7                         0x4007
#define GL_POINT_SMOOTH                   0x0B10
#define GL_LINE_SMOOTH                    0x0B20
#define GL_SCISSOR_TEST                   0x0C11
#define GL_COLOR_MATERIAL                 0x0B57
#define GL_NORMALIZE                      0x0BA1
#define GL_RESCALE_NORMAL                 0x803A
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_TEXTURE_COORD_ARRAY            0x8078
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0

#define GL_TEXTURE_CROP_RECT_OES          0x8B9D

#define GL_MODULATE                       0x2100
#define GL_DECAL                          0x2101
#define GL_ADD                            0x0104
#define GL_DOT3_RGB                       0x86AE
#define GL_DOT3_RGBA                      0x86AF

#define GL_TEXTURE_ENV_MODE               0x2200
#define GL_TEXTURE_ENV_COLOR              0x2201

#define GL_SUBTRACT                       0x84E7
#define GL_COMBINE                        0x8570
#define GL_COMBINE_RGB                    0x8571
#define GL_COMBINE_ALPHA                  0x8572
#define GL_RGB_SCALE                      0x8573
#define GL_ADD_SIGNED                     0x8574
#define GL_INTERPOLATE                    0x8575
#define GL_CONSTANT                       0x8576
#define GL_PRIMARY_COLOR                  0x8577
#define GL_PREVIOUS                       0x8578
#define GL_OPERAND0_RGB                   0x8590
#define GL_OPERAND1_RGB                   0x8591
#define GL_OPERAND2_RGB                   0x8592
#define GL_OPERAND0_ALPHA                 0x8598
#define GL_OPERAND1_ALPHA                 0x8599
#define GL_OPERAND2_ALPHA                 0x859A

#define GL_ALPHA_SCALE                    0x0D1C

#define GL_SRC0_RGB                       0x8580
#define GL_SRC1_RGB                       0x8581
#define GL_SRC2_RGB                       0x8582
#define GL_SRC0_ALPHA                     0x8588
#define GL_SRC1_ALPHA                     0x8589
#define GL_SRC2_ALPHA                     0x858A

#define GL_LIGHT_MODEL_AMBIENT            0x0B53

#define GL_AMBIENT                        0x1200
#define GL_DIFFUSE                        0x1201
#define GL_SPECULAR                       0x1202
#define GL_POSITION                       0x1203
#define GL_SPOT_DIRECTION                 0x1204
#define GL_SPOT_EXPONENT                  0x1205
#define GL_SPOT_CUTOFF                    0x1206
#define GL_CONSTANT_ATTENUATION           0x1207
#define GL_LINEAR_ATTENUATION             0x1208
#define GL_QUADRATIC_ATTENUATION          0x1209

#define GL_EMISSION                       0x1600
#define GL_SHININESS                      0x1601
#define GL_AMBIENT_AND_DIFFUSE            0x1602

#define GL_EXP                            0x0800
#define GL_EXP2                           0x0801

#define GL_FOG_DENSITY                    0x0B62
#define GL_FOG_START                      0x0B63
#define GL_FOG_END                        0x0B64
#define GL_FOG_MODE                       0x0B65
#define GL_FOG_COLOR                      0x0B66

#define GL_MATRIX_PALETTE_OES                    0x8840

//    Accepted by the <pname> parameters of GetIntegerv:
#define GL_MAX_PALETTE_MATRICES_OES              0x8842 // 9 by default
#define GL_MAX_VERTEX_UNITS_OES                  0x86A4 // 3 by default
#define GL_CURRENT_PALETTE_MATRIX_OES            0x8843

//    Accepted by the <cap> parameters of EnableClientState and DisableClientState and by the <pname> parameter of IsEnabled:
#define GL_MATRIX_INDEX_ARRAY_OES                0x8844
#define GL_WEIGHT_ARRAY_OES                      0x86AD

//    Accepted by the <pname> parameter of GetIntegerv:
#define GL_MATRIX_INDEX_ARRAY_SIZE_OES           0x8846
#define GL_MATRIX_INDEX_ARRAY_TYPE_OES           0x8847
#define GL_MATRIX_INDEX_ARRAY_STRIDE_OES         0x8848
#define GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES 0x8B9E

#define GL_WEIGHT_ARRAY_SIZE_OES                 0x86AB
#define GL_WEIGHT_ARRAY_TYPE_OES                 0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_OES               0x86AA
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_OES       0x889E

//    Accepted by the <pname> parameter of GetPointerv:
#define GL_MATRIX_INDEX_ARRAY_POINTER_OES        0x8849
#define GL_WEIGHT_ARRAY_POINTER_OES              0x86AC


#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

typedef float vec3[3];
typedef float vec4[4];
typedef float mat3[9];
typedef float mat4[16];

void matIdentity(float *m);
void matMult(float *m3, const float *m1, const float *m2);
void matTranslate(float *m, const float x, const float y, const float z);
BOOL matRotate(float *m, float DEGAngle, float x, float y, float z);
void matTranslatel(float *m, const float x, const float y, const float z);
void matScale(float *m, float x, float y, float z);
bool matFrustum(float *m, float f32Left, float f32Right, float f32Bottom, float f32Top, float f32ZNear, float f32ZFar);
BOOL matPerspective(float *m, float fieldOfViewDegree, float aspectRatio, float zNear, float zFar);
BOOL matOrtho(float* m, float f32Left, float f32Right, float f32Bottom, float f32Top, float f32ZNear, float f32ZFar);

char *enumName(GLenum attr);

typedef struct {
	float x, y, width, height;
} glRect;

void InitShaders();
void DestroyShaders();
void shEnableState(GLenum state, bool tex0);
void shDisableState(GLenum state, bool tex0);
void shClientAttrib(GLenum state, bool tex0, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void shSetupShader(GLenum mode);
void shColor(float r, float g, float b, float a);
void shNormal(float x, float y, float z);
void shPointSize(float s);
bool shChanged();
void shDrawTex(glRect tex, glRect crop, glRect sizes, float z, float zn, float zf);

#define GL_OES_draw_texture
#define GL_OES_matrix_palette

namespace squish {

	// -----------------------------------------------------------------------------

//! Typedef a quantity that is a single unsigned byte.
typedef unsigned char u8;

// -----------------------------------------------------------------------------

enum
{
	//! Use DXT1 compression.
	kDxt1 = ( 1 << 0 ), 
	
	//! Use DXT3 compression.
	kDxt3 = ( 1 << 1 ), 
	
	//! Use DXT5 compression.
	kDxt5 = ( 1 << 2 ), 
	
	//! Use a very slow but very high quality colour compressor.
	kColourIterativeClusterFit = ( 1 << 8 ),	
	
	//! Use a slow but high quality colour compressor (the default).
	kColourClusterFit = ( 1 << 3 ),	
	
	//! Use a fast but low quality colour compressor.
	kColourRangeFit	= ( 1 << 4 ),
	
	//! Use a perceptual metric for colour error (the default).
	kColourMetricPerceptual = ( 1 << 5 ),

	//! Use a uniform metric for colour error.
	kColourMetricUniform = ( 1 << 6 ),
	
	//! Weight the colour by alpha during cluster fit (disabled by default).
	kWeightColourByAlpha = ( 1 << 7 )
};

void Compress( u8 const* rgba, void* block, int flags );
void CompressMasked( u8 const* rgba, int mask, void* block, int flags );
void Decompress( u8* rgba, void const* block, int flags );
int GetStorageRequirements( int width, int height, int flags );
void CompressImage( u8 const* rgba, int width, int height, void* blocks, int flags );
void DecompressImage( u8* rgba, int width, int height, void const* blocks, int flags );

// -----------------------------------------------------------------------------

}

void PVRTCDecompress(void *pCompressedData, const int Do2bitMode, const int XDim, const int YDim, unsigned char* pResultImage);

// Definitions taken directly from Universal vertex shader.

const int MaxLights = 8;

struct MaterialParameters
{
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct LightSourceParameters
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec4 halfVector;		// UNUSED!
	vec3 spotDirection;
	float spotExponent;
	float spotCutoff; // (range: [0.0, 90.0], 180.0)
	float spotCosCutoff; // cos(spotCutoff) (range: [1.0, 0.0], -1.0)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	bool isDirectional;
};

struct LightModelProducts
{
	vec4 sceneColor;
};

const int MaxTextureCoords = 2;

// Definitions taken directly from Universal fragment shader.

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

const int MaxTextureImageUnits = 2;

// Defines from Universal fragment shader, exactly the same as in gl.h
#define TEX_ENV_MODE_MODULATE   1    
#define TEX_ENV_MODE_REPLACE    2
#define TEX_ENV_MODE_DECAL      3
#define TEX_ENV_MODE_BLEND      4
#define TEX_ENV_MODE_ADD        5
#define TEX_ENV_MODE_COMBINE    6
// combine RGB
#define TEX_COMBINE_RGB_MODULATE     1
#define TEX_COMBINE_RGB_REPLACE      2
#define TEX_COMBINE_RGB_ADD_SIGNED   3
#define TEX_COMBINE_RGB_INTERPOLATE  4
#define TEX_COMBINE_RGB_ADD          5
#define TEX_COMBINE_RGB_SUBTRACT     6
#define TEX_COMBINE_RGB_DOT3_RGB     7
#define TEX_COMBINE_RGB_DOT3_RGBA    8
// combine alpha
#define TEX_COMBINE_ALPHA_MODULATE    1
#define TEX_COMBINE_ALPHA_REPLACE     2
#define TEX_COMBINE_ALPHA_ADD_SIGNED  3
#define TEX_COMBINE_ALPHA_INTERPOLATE 4
#define TEX_COMBINE_ALPHA_ADD         5
#define TEX_COMBINE_ALPHA_SUBTRACT    6
// Combiner RGB and ALPHA source arguments
#define TEX_ENV_COMBINER_SRC_TEXTURE        1
#define TEX_ENV_COMBINER_SRC_CONSTANT       2
#define TEX_ENV_COMBINER_SRC_PRIMARY_COLOR  3
#define TEX_ENV_COMBINER_SRC_PREVIOUS       4
// Combiner RGB operands
#define TEX_ENV_COMBINER_RGB_OP_SRC_COLOR            1
#define TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_COLOR  2
#define TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA            3
#define TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_ALPHA  4
// Combiner ALPHA operands
#define TEX_ENV_COMBINER_ALPHA_OP_SRC_ALPHA              1
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

// Constants as defined in Universal vertex shader.

extern float proj_sx, proj_sy, proj_ox, proj_oy;
extern mat4 ModelViewProjectionMatrix;
extern mat4 ModelViewMatrix;
extern mat4 ProjectionMatrix;
extern mat3 NormalMatrix;
extern mat4 TextureMatrix[2];
extern mat4 MatrixPalette[32];
extern GLint MatrixPaletteEnabled;
extern GLuint PaletteSize;
extern GLint PointEnabled;
extern GLint FogEnabled;
extern MaterialParameters FrontNBackMaterial;
extern GLint colorMaterialEnabled;
extern LightSourceParameters LightSource[MaxLights];
extern GLint LightEnabled[MaxLights];
extern GLint LightingEnabled;
extern LightModelProducts FrontNBackLightModelProduct;
//extern int numLightUsed;
extern GLint Texture2DEnabled[MaxTextureCoords];
extern vec4 CurrentColor;

// Constants as defined in Universal fragment shader.

extern FogParameters Fog;
extern _TexEnv TexEnv[MaxTextureImageUnits];
extern vec4 TexShift0;
extern vec4 TexShift1;

#define PRECISION 16
#define ONE	(1 << PRECISION)
#define ZERO 0
#define PI ((float) 3.141592654f)
inline int FixedFromInt(int value) {return value << PRECISION;};
inline int FixedFromFloat(float value) { return static_cast<int> (value * static_cast<float>(ONE) + 0.5f);};
inline float FixedToFloat(int value) { return (value / static_cast<float>(ONE));};
inline int MultiplyFixed(int op1, int op2) { return (op1 * op2) >> PRECISION;};

enum { ll_none, ll_minimal, ll_err, ll_app, ll_frame, ll_all, ll_extra };

//#define MYGL_LOG
//#define HEAVY_LOG
//#define DUMP_TEXTURES

#ifdef MYGL_LOG
void log_printf(int ll, char *msg, ...);
#else
#define log_printf(...) //Sleep(0)
#endif
