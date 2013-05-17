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

#include "stdafx.h"
#include "libgles_cm.h"

#include "./shader/Flat.vert.h"
#include "./shader/Flat.frag.h"
#include "./shader/Color.vert.h"
#include "./shader/Color.frag.h"
#include "./shader/Texture.vert.h"
#include "./shader/Texture.frag.h"
#include "./shader/TextureC.frag.h"
#include "./shader/TexCol.vert.h"
#include "./shader/TexCol.frag.h"
#include "./shader/TexColC.frag.h"
#include "./shader/TexMat.vert.h"
#include "./shader/TexMatCol.vert.h"
#include "./shader/TexMat2.vert.h"
#include "./shader/TexAlpha.vert.h"
#include "./shader/TexAlpha.frag.h"
#include "./shader/TexAdd.frag.h"
#include "./shader/Universal.vert.h"
#include "./shader/Universal.frag.h"
#include "./shader/MatPal.vert.h"
#include "./shader/Fog.vert.h"
//#include "./shader/Fog.frag.h"
#include "./shader/FogLin.frag.h"
#include "./shader/FogExp.frag.h"
#include "./shader/FogExp2.frag.h"

struct uniMaterialParameters
{
	int emission;
	int ambient;
	int diffuse;
	int specular;
	int shininess;
};

struct uniLightSourceParameters
{
	int ambient;
	int diffuse;
	int specular;
	int position;
	int halfVector;
	int spotDirection;
	int spotExponent;
	int spotCutoff;
	int spotCosCutoff;
	int constantAttenuation;
	int linearAttenuation;
	int quadraticAttenuation;
	int isDirectional;
};

struct uniLightModelProducts
{
	int sceneColor;
};

struct uniFogParameters
{
	int density;
	int start;
	int end;
	int color;
	int scale;
	int mode;
};

struct uni_TexEnv
{
	int color;
	int mode;
	int combineRgb;
	int combineAlpha;
	int srcRgb;
	int srcAlpha;
	int operandRgb;
	int operandAlpha;
	int rgbScale;
	int alphaScale;
};

struct UniformLocation
{
	int ModelViewProjectionMatrix;
	int ModelViewMatrix;
	int NormalMatrix;
	int TextureMatrix;
	int PointEnabled;
	int FogEnabled;
	uniMaterialParameters FrontNBackMaterial;
	int colorMaterialEnabled;
	uniLightSourceParameters LightSource[MaxLights];
	int LightingEnabled;
	uniLightModelProducts FrontNBackLightModelProduct;
	int numLightUsed;
	int Texture2DEnabled;
	int TexImages0;
	int TexImages1;
	int TexShift0;
	int TexShift1;
	uniFogParameters Fog;
	uni_TexEnv TexEnv[MaxTextureImageUnits];
};

static UniformLocation ul_universal, ul_unitexcol, ul_unicol;

mat4 ModelViewProjectionMatrix;
mat4 ModelViewMatrix;
mat4 ProjectionMatrix;
mat3 NormalMatrix;
mat4 TextureMatrix[2];
mat4 MatrixPalette[32];
GLint MatrixPaletteEnabled = false;
GLuint PaletteSize;
GLint PointEnabled = false;
GLint FogEnabled = false;
MaterialParameters FrontNBackMaterial;
GLint colorMaterialEnabled = false;
LightSourceParameters LightSource[MaxLights];
GLint LightEnabled[MaxLights] = { false };
GLint LightingEnabled = false;
LightModelProducts FrontNBackLightModelProduct;
GLint Texture2DEnabled[MaxTextureCoords] = { false };
vec4 CurrentColor;
mat4 texUnity;

float proj_sx, proj_sy, proj_ox, proj_oy;

FogParameters Fog;
_TexEnv TexEnv[MaxTextureImageUnits];
vec4 TexShift0;
vec4 TexShift1;

void shInitUniversalState()
{
	int i;
	FrontNBackMaterial.ambient[0] = FrontNBackMaterial.ambient[1] = FrontNBackMaterial.ambient[2] = 0.2; FrontNBackMaterial.ambient[3] = 1;
	FrontNBackMaterial.diffuse[0] = FrontNBackMaterial.diffuse[1] = FrontNBackMaterial.diffuse[2] = 0.8; FrontNBackMaterial.diffuse[3] = 1;
	FrontNBackMaterial.specular[0] = FrontNBackMaterial.specular[1] = FrontNBackMaterial.specular[2] = 0; FrontNBackMaterial.specular[3] = 1;
	FrontNBackMaterial.emission[0] = FrontNBackMaterial.emission[1] = FrontNBackMaterial.emission[2] = 0; FrontNBackMaterial.emission[3] = 1;
	FrontNBackMaterial.shininess = 0;

	for (i = 0; i < MaxLights; i++)
	{
		if (i == 0)	LightSource[i].ambient[0] = LightSource[i].ambient[1] = LightSource[i].ambient[2] = 1;
		else		LightSource[i].ambient[0] = LightSource[i].ambient[1] = LightSource[i].ambient[2] = 0;
		LightSource[i].ambient[3] = 1;
		if (i == 0)	LightSource[i].diffuse[0] = LightSource[i].diffuse[1] = LightSource[i].diffuse[2] = 1;
		else		LightSource[i].diffuse[0] = LightSource[i].diffuse[1] = LightSource[i].diffuse[2] = 0;
		LightSource[i].diffuse[3] = 1;
		if (i == 0)	LightSource[i].specular[0] = LightSource[i].specular[1] = LightSource[i].specular[2] = 1;
		else		LightSource[i].specular[0] = LightSource[i].specular[1] = LightSource[i].specular[2] = 0;
		LightSource[i].specular[3] = 1;
		LightSource[i].position[0] = LightSource[i].position[1] = LightSource[i].position[3] = 0; LightSource[i].position[2] = 1;
		LightSource[i].spotDirection[0] = LightSource[i].spotDirection[1] = 0; LightSource[i].spotDirection[2] = -1;
		LightSource[i].spotExponent = 0;
		LightSource[i].spotCutoff = 180;
		LightSource[i].constantAttenuation = 1;
		LightSource[i].linearAttenuation = 0;
		LightSource[i].quadraticAttenuation = 0;
	}

	Fog.mode = FOGMODE_EXP;
	Fog.density = 1;
	Fog.start = 0;
	Fog.end = 1;
	Fog.color[0] = Fog.color[1] = Fog.color[2] = Fog.color[3] = 0;

	for (i = 0; i < MaxTextureImageUnits; i++)
	{
		TexEnv[i].mode = TEX_ENV_MODE_MODULATE;
		TexEnv[i].color[0] = TexEnv[i].color[1] = TexEnv[i].color[2] = TexEnv[i].color[3] = 0;
		TexEnv[i].combineRgb = TEX_COMBINE_RGB_MODULATE;
		TexEnv[i].combineAlpha = TEX_COMBINE_ALPHA_MODULATE;
		TexEnv[i].srcRgb[0] = TexEnv[i].srcAlpha[0] = TEX_ENV_COMBINER_SRC_TEXTURE;
		TexEnv[i].srcRgb[1] = TexEnv[i].srcAlpha[1] = TEX_ENV_COMBINER_SRC_PREVIOUS;
		TexEnv[i].srcRgb[2] = TexEnv[i].srcAlpha[2] = TEX_ENV_COMBINER_SRC_CONSTANT;
		TexEnv[i].operandRgb[0] = TEX_ENV_COMBINER_RGB_OP_SRC_COLOR;
		TexEnv[i].operandRgb[1] = TEX_ENV_COMBINER_RGB_OP_SRC_COLOR;
		TexEnv[i].operandRgb[2] = TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA;
		TexEnv[i].operandAlpha[0] = TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA;
		TexEnv[i].operandAlpha[1] = TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA;
		TexEnv[i].operandAlpha[2] = TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA;
		TexEnv[i].rgbScale = 1;
		TexEnv[i].alphaScale = 1;
	}

	proj_sx = proj_sy = 1;
	proj_ox = proj_oy = 0;
}

void shInitUniversalShader(GLuint prog, UniformLocation &uniLoc)
{
	int i, j;

	uniLoc.ModelViewProjectionMatrix = glGetUniformLocation(prog, "ModelViewProjectionMatrix");
	uniLoc.ModelViewMatrix = glGetUniformLocation(prog, "ModelViewMatrix");
	uniLoc.NormalMatrix = glGetUniformLocation(prog, "NormalMatrix");
	uniLoc.TextureMatrix = glGetUniformLocation(prog, "TextureMatrix");
	uniLoc.PointEnabled = glGetUniformLocation(prog, "PointEnabled");
	uniLoc.FogEnabled = glGetUniformLocation(prog, "FogEnabled");
	uniLoc.FrontNBackMaterial.emission = glGetUniformLocation(prog, "FrontNBackMaterial.emission");
	uniLoc.FrontNBackMaterial.ambient = glGetUniformLocation(prog, "FrontNBackMaterial.ambient");
	uniLoc.FrontNBackMaterial.diffuse = glGetUniformLocation(prog, "FrontNBackMaterial.diffuse");
	uniLoc.FrontNBackMaterial.specular = glGetUniformLocation(prog, "FrontNBackMaterial.specular");
	uniLoc.FrontNBackMaterial.shininess = glGetUniformLocation(prog, "FrontNBackMaterial.shininess");
	uniLoc.colorMaterialEnabled = glGetUniformLocation(prog, "colorMaterialEnabled");

	char uniformName[][256] = {
		"LightSource[0].ambient",
		"LightSource[0].diffuse",
		"LightSource[0].specular",
		"LightSource[0].position",
		"LightSource[0].halfVector",
		"LightSource[0].spotDirection",
		"LightSource[0].spotExponent",
		"LightSource[0].spotCutoff",
		"LightSource[0].spotCosCutoff",
		"LightSource[0].constantAttenuation",
		"LightSource[0].linearAttenuation",
		"LightSource[0].quadraticAttenuation",
		"LightSource[0].isDirectional"
	};

	int numLightUsed = 0, numNames = sizeof(uniformName) / sizeof(uniformName[0]);
	for (i = 0; i < MaxLights; i++)
	{
		for (j = 0; j < numNames; j++)
			uniformName[j][12] = '0' + i;

		uniLoc.LightSource[i].ambient = glGetUniformLocation(prog, uniformName[0]);
		uniLoc.LightSource[i].diffuse = glGetUniformLocation(prog, uniformName[1]);
		uniLoc.LightSource[i].specular = glGetUniformLocation(prog, uniformName[2]);
		uniLoc.LightSource[i].position = glGetUniformLocation(prog, uniformName[3]);
		uniLoc.LightSource[i].halfVector = glGetUniformLocation(prog, uniformName[4]);
		uniLoc.LightSource[i].spotDirection = glGetUniformLocation(prog, uniformName[5]);
		uniLoc.LightSource[i].spotExponent = glGetUniformLocation(prog, uniformName[6]);
		uniLoc.LightSource[i].spotCutoff = glGetUniformLocation(prog, uniformName[7]);
		uniLoc.LightSource[i].spotCosCutoff = glGetUniformLocation(prog, uniformName[8]);
		uniLoc.LightSource[i].constantAttenuation = glGetUniformLocation(prog, uniformName[9]);
		uniLoc.LightSource[i].linearAttenuation = glGetUniformLocation(prog, uniformName[10]);
		uniLoc.LightSource[i].quadraticAttenuation = glGetUniformLocation(prog, uniformName[11]);
		uniLoc.LightSource[i].isDirectional = glGetUniformLocation(prog, uniformName[12]);
	}

	uniLoc.numLightUsed = glGetUniformLocation(prog, "numLightUsed");
	uniLoc.LightingEnabled = glGetUniformLocation(prog, "LightingEnabled");
	uniLoc.FrontNBackLightModelProduct.sceneColor = glGetUniformLocation(prog, "FrontNBackLightModelProduct.sceneColor");
	uniLoc.Texture2DEnabled = glGetUniformLocation(prog, "Texture2DEnabled");
	uniLoc.TexImages0 = glGetUniformLocation(prog, "TexImages0");
	uniLoc.TexImages1 = glGetUniformLocation(prog, "TexImages1");
	uniLoc.TexShift0 = glGetUniformLocation(prog, "TexShift0");
	uniLoc.TexShift1 = glGetUniformLocation(prog, "TexShift1");
	uniLoc.Fog.density = glGetUniformLocation(prog, "Fog.density");
	uniLoc.Fog.start = glGetUniformLocation(prog, "Fog.start");
	uniLoc.Fog.end = glGetUniformLocation(prog, "Fog.end");
	uniLoc.Fog.color = glGetUniformLocation(prog, "Fog.color");
	uniLoc.Fog.scale = glGetUniformLocation(prog, "Fog.scale");
	uniLoc.Fog.mode = glGetUniformLocation(prog, "Fog.mode");

	char texenvName[][256] = {
		"TexEnv[0].color",
		"TexEnv[0].mode",
		"TexEnv[0].combineRgb",
		"TexEnv[0].combineAlpha",
		"TexEnv[0].srcRgb",
		"TexEnv[0].srcAlpha",
		"TexEnv[0].operandRgb",
		"TexEnv[0].operandAlpha",
		"TexEnv[0].rgbScale",
		"TexEnv[0].alphaScale"
	};

	int numTex = sizeof(texenvName) / sizeof(texenvName[0]);
	for (i = 0; i < MaxTextureImageUnits; i++)
	{
		for (j = 0; j < numTex; j++)
			texenvName[j][7] = '0' + i;

		uniLoc.TexEnv[i].color = glGetUniformLocation(prog, texenvName[0]);
		uniLoc.TexEnv[i].mode = glGetUniformLocation(prog, texenvName[1]);
		uniLoc.TexEnv[i].combineRgb = glGetUniformLocation(prog, texenvName[2]);
		uniLoc.TexEnv[i].combineAlpha = glGetUniformLocation(prog, texenvName[3]);
		uniLoc.TexEnv[i].srcRgb = glGetUniformLocation(prog, texenvName[4]);
		uniLoc.TexEnv[i].srcAlpha = glGetUniformLocation(prog, texenvName[5]);
		uniLoc.TexEnv[i].operandRgb = glGetUniformLocation(prog, texenvName[6]);
		uniLoc.TexEnv[i].operandAlpha = glGetUniformLocation(prog, texenvName[7]);
		uniLoc.TexEnv[i].rgbScale = glGetUniformLocation(prog, texenvName[8]);
		uniLoc.TexEnv[i].alphaScale = glGetUniformLocation(prog, texenvName[9]);
	}

	glVertexAttrib3f(1, 0, 0, 0);
	glVertexAttrib4f(2, 1, 1, 1, 1);
	glVertexAttrib1f(5, 1);
}

class ShaderProgram
{
public:
	ShaderProgram(const GLuint *vs_data, int vs_len, const GLuint *fs_data, int fs_len);
	~ShaderProgram();
	GLuint UseProgram();
	GLuint GetProgram();

protected:
	GLuint vs, fs, prog;
};

ShaderProgram *sh_flat, *sh_texture, *sh_texture_c, *sh_color, *sh_tex_col, *sh_tex_col_c, *sh_tex_mat, *sh_tex_mat_col;
ShaderProgram *sh_tex_alpha, *sh_tex_add, *sh_tex_alpha2, *sh_tex_add2;
ShaderProgram *sh_universal, *sh_unitexcol, *sh_unicol;
ShaderProgram *sh_mat_pal, *sh_fog_lin, *sh_fog_exp, *sh_fog_exp2;//, *sh_fog;
bool vertex, texture, color, normal, changed, texture2;
uniFogParameters uniFogLin, uniFogExp, uniFogExp2;//, uniFog;

ShaderProgram::ShaderProgram(const GLuint *vs_data, int vs_len, const GLuint *fs_data, int fs_len)
{
	GLint status;

	prog = glCreateProgram();

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderBinary(1, &vs, (GLenum)0, vs_data, sizeof(int)*vs_len);
	glAttachShader(prog, vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderBinary(1, &fs, (GLenum)0, fs_data, sizeof(int)*fs_len);
	glAttachShader(prog, fs);

	glBindAttribLocation(prog, 0, "Vertex");
	glBindAttribLocation(prog, 1, "Normal");
	glBindAttribLocation(prog, 2, "Color");
	glBindAttribLocation(prog, 3, "MultiTexCoord0");
	glBindAttribLocation(prog, 4, "MultiTexCoord1");
	glBindAttribLocation(prog, 5, "PointSize");
	glBindAttribLocation(prog, 6, "PaletteIndex");
	glBindAttribLocation(prog, 7, "PaletteWeight");

	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (!status)
	{
		int loglen;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &loglen);
		if (loglen > 1)
		{
			char *infolog = new char[loglen];
			int written;
			glGetProgramInfoLog(prog, loglen, &written, infolog);
			log_printf(ll_err, "error linking shader program: %s", infolog);
			delete infolog;
		}
		glDeleteProgram(prog);
		return;
	}
}

ShaderProgram::~ShaderProgram()
{
	if (prog)
	{
		glDetachShader(prog, vs);
		glDetachShader(prog, fs);
	}

	if (vs)
	{
		glDeleteShader(vs);
		vs = 0;
	}

	if (fs)
	{
		glDeleteShader(fs);
		fs = 0;
	}

	if (prog)
	{
		glDeleteProgram(prog);
		prog = 0;
	}
}

GLuint ShaderProgram::UseProgram()
{
	glUseProgram(prog);
	return prog;
}

GLuint ShaderProgram::GetProgram()
{
	return prog;
}

void shSetupFog(GLuint prog, uniFogParameters &runiFog)
{
	runiFog.density = glGetUniformLocation(prog, "Fog.density");
	runiFog.start = glGetUniformLocation(prog, "Fog.start");
	runiFog.end = glGetUniformLocation(prog, "Fog.end");
	runiFog.color = glGetUniformLocation(prog, "Fog.color");
	runiFog.scale = glGetUniformLocation(prog, "Fog.scale");
	runiFog.mode = glGetUniformLocation(prog, "Fog.mode");
}

void InitShaders()
{
	shInitUniversalState();
	sh_flat = new ShaderProgram(FlatVert, FlatVertLength, FlatFrag, FlatFragLength);
	sh_color = new ShaderProgram(ColorVert, ColorVertLength, ColorFrag, ColorFragLength);
	sh_texture = new ShaderProgram(TextureVert, TextureVertLength, TextureFrag, TextureFragLength);
	sh_texture_c = new ShaderProgram(TextureVert, TextureVertLength, TextureCFrag, TextureCFragLength);
	sh_tex_mat = new ShaderProgram(TexMatVert, TexMatVertLength, TextureFrag, TextureFragLength);
	sh_tex_col = new ShaderProgram(TexColVert, TexColVertLength, TexColFrag, TexColFragLength);
	sh_tex_col_c = new ShaderProgram(TexColVert, TexColVertLength, TexColCFrag, TexColCFragLength);
	sh_tex_mat_col = new ShaderProgram(TexMatColVert, TexMatColVertLength, TexColFrag, TexColFragLength);
	sh_tex_alpha = new ShaderProgram(TexAlphaVert, TexAlphaVertLength, TexAlphaFrag, TexAlphaFragLength);
	sh_tex_add = new ShaderProgram(TexAlphaVert, TexAlphaVertLength, TexAddFrag, TexAddFragLength);
	sh_tex_alpha2 = new ShaderProgram(TexMat2Vert, TexMat2VertLength, TexAlphaFrag, TexAlphaFragLength);
	sh_tex_add2 = new ShaderProgram(TexMat2Vert, TexMat2VertLength, TexAddFrag, TexAddFragLength);
	sh_universal = new ShaderProgram(UniversalVert, UniversalVertLength, UniversalFrag, UniversalFragLength);
	shInitUniversalShader(sh_universal->UseProgram(), ul_universal);
	sh_unitexcol = new ShaderProgram(UniversalVert, UniversalVertLength, TexColFrag, TexColFragLength);
	shInitUniversalShader(sh_unitexcol->UseProgram(), ul_unitexcol);
	sh_unicol = new ShaderProgram(UniversalVert, UniversalVertLength, ColorFrag, ColorFragLength);
	shInitUniversalShader(sh_unicol->UseProgram(), ul_unicol);
	sh_mat_pal = new ShaderProgram(MatPalVert, MatPalVertLength, TexColFrag, TexColFragLength);
	sh_fog_lin = new ShaderProgram(FogVert, FogVertLength, FogLinFrag, FogLinFragLength);
	shSetupFog(sh_fog_lin->GetProgram(), uniFogLin);
	sh_fog_exp = new ShaderProgram(FogVert, FogVertLength, FogExpFrag, FogExpFragLength);
	shSetupFog(sh_fog_exp->GetProgram(), uniFogExp);
	sh_fog_exp2 = new ShaderProgram(FogVert, FogVertLength, FogExp2Frag, FogExp2FragLength);
	shSetupFog(sh_fog_exp2->GetProgram(), uniFogExp2);
//	sh_fog = new ShaderProgram(FogVert, FogVertLength, FogFrag, FogFragLength);
//	shSetupFog(sh_fog->GetProgram(), uniFog);

	vertex = false;
	texture = false;
	texture2 = false;
	color = false;
	normal = false;
	changed = true;
	CurrentColor[0] = CurrentColor[1] = CurrentColor[2] = CurrentColor[3] = 1.0;
	matIdentity(texUnity);

	memset(TexShift0, 0, sizeof(TexShift0));
	memset(TexShift1, 0, sizeof(TexShift1));
}

void DestroyShaders()
{
	if (sh_flat)
	{
		delete sh_flat;
		sh_flat = 0;
	}

	if (sh_color)
	{
		delete sh_color;
		sh_color = 0;
	}

	if (sh_texture)
	{
		delete sh_texture;
		sh_texture = 0;
	}

	if (sh_texture_c)
	{
		delete sh_texture_c;
		sh_texture_c = 0;
	}

	if (sh_tex_mat)
	{
		delete sh_tex_mat;
		sh_tex_mat = 0;
	}

	if (sh_tex_col)
	{
		delete sh_tex_col;
		sh_tex_col = 0;
	}

	if (sh_tex_col_c)
	{
		delete sh_tex_col_c;
		sh_tex_col_c = 0;
	}

	if (sh_tex_mat_col)
	{
		delete sh_tex_mat_col;
		sh_tex_mat_col = 0;
	}

	if (sh_tex_alpha)
	{
		delete sh_tex_alpha;
		sh_tex_alpha = 0;
	}

	if (sh_tex_add)
	{
		delete sh_tex_add;
		sh_tex_add = 0;
	}

	if (sh_tex_alpha2)
	{
		delete sh_tex_alpha2;
		sh_tex_alpha2 = 0;
	}

	if (sh_tex_add2)
	{
		delete sh_tex_add2;
		sh_tex_add2 = 0;
	}

	if (sh_universal)
	{
		delete sh_universal;
		sh_universal = 0;
	}

	if (sh_unitexcol)
	{
		delete sh_unitexcol;
		sh_unitexcol = 0;
	}

	if (sh_unicol)
	{
		delete sh_unicol;
		sh_unicol = 0;
	}

	if (sh_mat_pal)
	{
		delete sh_mat_pal;
		sh_mat_pal = 0;
	}

	if (sh_fog_lin)
	{
		delete sh_fog_lin;
		sh_fog_lin = 0;
	}

	if (sh_fog_exp)
	{
		delete sh_fog_exp;
		sh_fog_exp = 0;
	}

	if (sh_fog_exp2)
	{
		delete sh_fog_exp2;
		sh_fog_exp2 = 0;
	}
/*
	if (sh_fog)
	{
		delete sh_fog;
		sh_fog = 0;
	}*/
}

void shColor(float r, float g, float b, float a)
{
	if (CurrentColor[0] != r || CurrentColor[1] != g || CurrentColor[2] != b ||CurrentColor[3] != a)
		changed = true;
	CurrentColor[0] = r;
	CurrentColor[1] = g;
	CurrentColor[2] = b;
	CurrentColor[3] = a;

	glVertexAttrib4f(2, r, g, b, a);
}

void shNormal(float x, float y, float z)
{
	glVertexAttrib3f(1, x, y, z);
}

void shPointSize(float s)
{
	glVertexAttrib1f(5, s);
}

void shEnableState(GLenum state, bool tex0)
{
	switch (state)
	{
	case GL_VERTEX_ARRAY:			glEnableVertexAttribArray(0);	if (!vertex) changed = true;	vertex = true;	break;
	case GL_NORMAL_ARRAY:			glEnableVertexAttribArray(1);	if (!normal) changed = true;	normal = true;	break;
	case GL_COLOR_ARRAY:			glEnableVertexAttribArray(2);	if (!color) changed = true;		color = true;	break;
	case GL_TEXTURE_COORD_ARRAY:
		if (tex0)	{				glEnableVertexAttribArray(3);	if (!texture) changed = true;	texture = true;		}
		else		{				glEnableVertexAttribArray(4);	if (!texture2) changed = true;	texture2 = true;	}
		break;
	case GL_POINT_SIZE:				glEnableVertexAttribArray(5);	PointEnabled = true;							break;
	case GL_MATRIX_INDEX_ARRAY_OES:	glEnableVertexAttribArray(6);	break;
	case GL_WEIGHT_ARRAY_OES:		glEnableVertexAttribArray(7);	break;
	}
}

void shDisableState(GLenum state, bool tex0)
{
	switch (state)
	{
	case GL_VERTEX_ARRAY:			glDisableVertexAttribArray(0);	if (vertex) changed = true;		vertex = false;		break;
	case GL_NORMAL_ARRAY:			glDisableVertexAttribArray(1);	if (normal) changed = true;		normal = false;		break;
	case GL_COLOR_ARRAY:			glDisableVertexAttribArray(2);	if (color) changed = true;		color = false;		break;
	case GL_TEXTURE_COORD_ARRAY:
		if (tex0)	{				glDisableVertexAttribArray(3);	if (texture) changed = true;	texture = false;	}
		else		{				glDisableVertexAttribArray(4);	if (texture2) changed = true;	texture2 = false;	}
		break;
	case GL_POINT_SIZE:				glDisableVertexAttribArray(5);	PointEnabled = false;								break;
	case GL_MATRIX_INDEX_ARRAY_OES:	glDisableVertexAttribArray(6);	break;
	case GL_WEIGHT_ARRAY_OES:		glDisableVertexAttribArray(7);	break;
	}
}

void shClientAttrib(GLenum state, bool tex0, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	switch (state)
	{
	case GL_VERTEX_ARRAY:	glVertexAttribPointer(0, size, type, GL_FALSE, stride, pointer);	break;
	case GL_NORMAL_ARRAY:	glVertexAttribPointer(1, size, type, GL_FALSE, stride, pointer);	break;
	case GL_COLOR_ARRAY:	glVertexAttribPointer(2, size, type,
								(type == GL_BYTE || type == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE),
																		stride, pointer);		break;
	case GL_TEXTURE_COORD_ARRAY:
		if (tex0)	{		glVertexAttribPointer(3, size, type,
								(type == GL_BYTE || type == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE),
																			stride, pointer);	}
		else		{		glVertexAttribPointer(4, size, type,
								(type == GL_BYTE || type == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE),
																		stride, pointer);	}	break;
	case GL_POINT_SIZE:		glVertexAttribPointer(5, size, type, GL_FALSE, stride, pointer);	break;
	case GL_MATRIX_INDEX_ARRAY_OES:	glVertexAttribPointer(6, size, type, GL_FALSE, stride, pointer);	PaletteSize = size;	break;
	case GL_WEIGHT_ARRAY_OES:		glVertexAttribPointer(7, size, type, GL_FALSE, stride, pointer);	break;
	}
}

bool shChanged()
{
	return true; //changed;
}

void vecMult(const float *m, float *in, float *out, bool is3x3)
{
	if (is3x3)
	{
		out[0] = m[0] * in[0] + m[3] * in[1] + m[6] * in[2];
		out[1] = m[1] * in[0] + m[4] * in[1] + m[7] * in[2];
		out[2] = m[2] * in[0] + m[5] * in[1] + m[8] * in[2];
	}
	else
	{
		out[0] = m[0] * in[0] + m[4] * in[1] + m[8] * in[2] + m[12];
		out[1] = m[1] * in[0] + m[5] * in[1] + m[9] * in[2] + m[13];
		out[2] = m[2] * in[0] + m[6] * in[1] + m[10] * in[2] + m[14];
	}
}

void vecNormalize(float *v)
{
	float len = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
}

void shSetupUniversalShader(GLuint prog, UniformLocation &uniLoc, GLenum mode)
{
	int i;

	glUniformMatrix4fv(uniLoc.ModelViewProjectionMatrix, 1, GL_FALSE, ModelViewProjectionMatrix);
	glUniformMatrix4fv(uniLoc.ModelViewMatrix, 1, GL_FALSE, ModelViewMatrix);
	for (i = 0; i < 3; i++)
		memcpy((float *)NormalMatrix + i*3, (float *)ModelViewMatrix + i*4, 3*sizeof(float));
	glUniformMatrix3fv(uniLoc.NormalMatrix, 1, GL_FALSE, NormalMatrix);
	
	mat4 texMat[2];
	memcpy(texMat[0], TextureMatrix[0], sizeof(mat4));
	matTranslate(texMat[0], TexShift0[0], TexShift0[1], TexShift0[2]);
	memcpy(texMat[1], TextureMatrix[1], sizeof(mat4));
	matTranslate(texMat[1], TexShift1[0], TexShift1[1], TexShift1[2]);
	glUniformMatrix4fv(uniLoc.TextureMatrix, 2, GL_FALSE, texMat[0]);

	glUniform1i(uniLoc.PointEnabled, mode == GL_POINTS); //PointEnabled);
	glUniform1i(uniLoc.FogEnabled, FogEnabled);
	glUniform4fv(uniLoc.FrontNBackMaterial.emission, 1, FrontNBackMaterial.emission);
	glUniform4fv(uniLoc.FrontNBackMaterial.ambient, 1, FrontNBackMaterial.ambient);
	glUniform4fv(uniLoc.FrontNBackMaterial.diffuse, 1, FrontNBackMaterial.diffuse);
	glUniform4fv(uniLoc.FrontNBackMaterial.specular, 1, FrontNBackMaterial.specular);
	glUniform1f(uniLoc.FrontNBackMaterial.shininess, FrontNBackMaterial.shininess);
	glUniform1i(uniLoc.colorMaterialEnabled, colorMaterialEnabled);

	vec3 dir;
	int numLightUsed = 0;
	for (i = 0; i < MaxLights; i++)
		if (LightEnabled[i])
		{
			glUniform4fv(uniLoc.LightSource[numLightUsed].ambient, 1, LightSource[i].ambient);
			glUniform4fv(uniLoc.LightSource[numLightUsed].diffuse, 1, LightSource[i].diffuse);
			glUniform4fv(uniLoc.LightSource[numLightUsed].specular, 1, LightSource[i].specular);

			if (LightSource[i].position[3] == 0)
			{
				vecMult(NormalMatrix, LightSource[i].position, dir, true);
				vecNormalize(dir);
			}
			else
				vecMult(ModelViewMatrix, LightSource[i].position, dir, false);
			glUniform4fv(uniLoc.LightSource[numLightUsed].position, 1, dir); //LightSource[i].position);

			vecMult(NormalMatrix, LightSource[i].spotDirection, dir, true);
			vecNormalize(dir);
			glUniform3fv(uniLoc.LightSource[numLightUsed].spotDirection, 1, dir); //LightSource[i].spotDirection);

			glUniform1f(uniLoc.LightSource[numLightUsed].spotExponent, LightSource[i].spotExponent);
			glUniform1f(uniLoc.LightSource[numLightUsed].spotCutoff, LightSource[i].spotCutoff);
			glUniform1f(uniLoc.LightSource[numLightUsed].spotCosCutoff, (float)cos(LightSource[i].spotCutoff * PI / 180));
			glUniform1f(uniLoc.LightSource[numLightUsed].constantAttenuation, LightSource[i].constantAttenuation);
			glUniform1f(uniLoc.LightSource[numLightUsed].linearAttenuation, LightSource[i].linearAttenuation);
			glUniform1f(uniLoc.LightSource[numLightUsed].quadraticAttenuation, LightSource[i].quadraticAttenuation);
			glUniform1i(uniLoc.LightSource[numLightUsed].isDirectional, LightSource[i].position[3] == 0);

			numLightUsed++;
		}

	glUniform1i(uniLoc.numLightUsed, numLightUsed);
	glUniform1i(uniLoc.LightingEnabled, LightingEnabled);

	vec4 sceneColor;
	sceneColor[0] = FrontNBackLightModelProduct.sceneColor[0] * FrontNBackMaterial.ambient[0] + FrontNBackMaterial.emission[0];
	sceneColor[1] = FrontNBackLightModelProduct.sceneColor[1] * FrontNBackMaterial.ambient[1] + FrontNBackMaterial.emission[1];
	sceneColor[2] = FrontNBackLightModelProduct.sceneColor[2] * FrontNBackMaterial.ambient[2] + FrontNBackMaterial.emission[2];
	sceneColor[3] = FrontNBackLightModelProduct.sceneColor[3] * FrontNBackMaterial.ambient[3] + FrontNBackMaterial.emission[3];
	glUniform4fv(uniLoc.FrontNBackLightModelProduct.sceneColor, 1, sceneColor);

	glUniform1iv(uniLoc.Texture2DEnabled, MaxTextureCoords, Texture2DEnabled);
	glUniform1i(uniLoc.TexImages0, 0);
	glUniform1i(uniLoc.TexImages1, 1);

	glUniform1f(uniLoc.Fog.density, Fog.density);
	glUniform1f(uniLoc.Fog.start, Fog.start);
	glUniform1f(uniLoc.Fog.end, Fog.end);
	glUniform4fv(uniLoc.Fog.color, 1, Fog.color);
	glUniform1f(uniLoc.Fog.scale, 1 / (Fog.end - Fog.start));
	glUniform1i(uniLoc.Fog.mode, Fog.mode);

	for (i = 0; i < MaxTextureImageUnits; i++)
	{
		glUniform4fv(uniLoc.TexEnv[i].color, 1, TexEnv[i].color);
		glUniform1i(uniLoc.TexEnv[i].mode, TexEnv[i].mode);
		glUniform1i(uniLoc.TexEnv[i].combineRgb, TexEnv[i].combineRgb);
		glUniform1i(uniLoc.TexEnv[i].combineAlpha, TexEnv[i].combineAlpha);
		glUniform3iv(uniLoc.TexEnv[i].srcRgb, 1, TexEnv[i].srcRgb);
		glUniform3iv(uniLoc.TexEnv[i].srcAlpha, 1, TexEnv[i].srcAlpha);
		glUniform3iv(uniLoc.TexEnv[i].operandRgb, 1, TexEnv[i].operandRgb);
		glUniform3iv(uniLoc.TexEnv[i].operandAlpha, 1, TexEnv[i].operandAlpha);
		glUniform1f(uniLoc.TexEnv[i].rgbScale, TexEnv[i].rgbScale);
		glUniform1f(uniLoc.TexEnv[i].alphaScale, TexEnv[i].alphaScale);
	}
}

void shSetupShader(GLenum mode)
{
	if (!vertex) return;
	GLuint prog = 0;
	color = true;

	if (MatrixPaletteEnabled)
	{
		prog = sh_mat_pal->UseProgram();
		glUniform1i(glGetUniformLocation(prog, "TexImages0"), 0);
		glUniform1i(glGetUniformLocation(prog, "PaletteSize"), PaletteSize);
		glUniformMatrix4fv(glGetUniformLocation(prog, "MatrixPalette"), 32, GL_FALSE, MatrixPalette[0]);
		glUniformMatrix4fv(glGetUniformLocation(prog, "ProjectionMatrix"), 1, GL_FALSE, ProjectionMatrix);
		return;
	}

	mat4 matViewport, temp;
	matIdentity(matViewport);
	matViewport[0] = proj_sx;
	matViewport[5] = proj_sy;
	matViewport[12] = proj_ox;
	matViewport[13] = proj_oy;

	matMult(temp, ProjectionMatrix, ModelViewMatrix);
	matMult(ModelViewProjectionMatrix, matViewport, temp);

	if (LightingEnabled)
	{
		if (!Texture2DEnabled[0] && !FogEnabled)
		{
			prog = sh_unicol->UseProgram();
			shSetupUniversalShader(prog, ul_unitexcol, mode);
		}
		else if (!Texture2DEnabled[1] && !FogEnabled && TexEnv[0].mode == TEX_ENV_MODE_MODULATE)
		{
			prog = sh_unitexcol->UseProgram();
			shSetupUniversalShader(prog, ul_unicol, mode);
		}
		else
		{
			prog = sh_universal->UseProgram();
			shSetupUniversalShader(prog, ul_universal, mode);
		}

		return;
	}

	if (texture2)
	{
		if (memcmp(TextureMatrix[0], texUnity, sizeof(texUnity)) || memcmp(TextureMatrix[1], texUnity, sizeof(texUnity)))
		{
			mat4 texMat[2];
			memcpy(texMat[0], TextureMatrix[0], sizeof(mat4));
			matTranslate(texMat[0], TexShift0[0], TexShift0[1], TexShift0[2]);
			memcpy(texMat[1], TextureMatrix[1], sizeof(mat4));
			matTranslate(texMat[1], TexShift1[0], TexShift1[1], TexShift1[2]);

			if (TexEnv[1].mode == TEX_ENV_MODE_ADD)
				prog = sh_tex_add2->UseProgram();
			else
				prog = sh_tex_alpha2->UseProgram();
			glUniformMatrix4fv(glGetUniformLocation(prog, "TextureMatrix"), 2, GL_FALSE, texMat[0]);
		}
		else
		{
			if (TexEnv[1].mode == TEX_ENV_MODE_ADD)
				prog = sh_tex_add->UseProgram();
			else
				prog = sh_tex_alpha->UseProgram();
		}

		glUniform1i(glGetUniformLocation(prog, "TexImages0"), 0);
		glUniform1i(glGetUniformLocation(prog, "TexImages1"), 1);
		glUniform4fv(glGetUniformLocation(prog, "TexShift0"), 1, TexShift0);
		glUniform4fv(glGetUniformLocation(prog, "TexShift1"), 1, TexShift1);
	}
	else if (texture)
	{
		bool tex = memcmp(TextureMatrix[0], texUnity, sizeof(texUnity)) != 0;
		if (color)
		{
			if (TexEnv[0].srcRgb[1] == TEX_ENV_COMBINER_SRC_CONSTANT)
			{
				prog = sh_tex_col_c->UseProgram();
				glUniform4fv(glGetUniformLocation(prog, "TexColor"), 1, TexEnv[0].color);
				tex = false;
			}
			else if (FogEnabled)
			{
				uniFogParameters *puniFog;
				switch (Fog.mode)
				{
				case FOGMODE_LINEAR:	prog = sh_fog_lin->UseProgram();	puniFog = &uniFogLin;	break;
				case FOGMODE_EXP:		prog = sh_fog_exp->UseProgram();	puniFog = &uniFogExp;	break;
				case FOGMODE_EXP2:		prog = sh_fog_exp2->UseProgram();	puniFog = &uniFogExp2;	break;
				}

//				puniFog = &uniFog;
//				prog = sh_fog->UseProgram();

				glUniform1f(puniFog->density, (Fog.mode == FOGMODE_EXP ? Fog.density * (float)LOG2E : Fog.density));
				glUniform1f(puniFog->start, Fog.start);
				glUniform1f(puniFog->end, Fog.end); // / (Fog.end - Fog.start));
				glUniform4fv(puniFog->color, 1, Fog.color);
				glUniform1f(puniFog->scale, 1 / (Fog.end - Fog.start));
				glUniform1i(puniFog->mode, Fog.mode);

				glUniform4f(glGetUniformLocation(prog, "ModelViewZ"), ModelViewMatrix[2], ModelViewMatrix[6], ModelViewMatrix[10], ModelViewMatrix[14]);
				tex = false;
			}
			else
				prog = (tex ? sh_tex_mat_col->UseProgram() : sh_tex_col->UseProgram());
		}
		else if (tex)
			prog = sh_tex_mat->UseProgram();
		else
		{
			if (CurrentColor[0] != 1 || CurrentColor[1] != 1 || CurrentColor[2] != 1 ||CurrentColor[3] != 1)
			{
				prog = sh_texture_c->UseProgram();
				glUniform4f(glGetUniformLocation(prog, "FrontColor"), CurrentColor[0], CurrentColor[1], CurrentColor[2], CurrentColor[3]);
			}
			else
				prog = sh_texture->UseProgram();
		}

		if (tex)
		{
			mat4 texMat;
			memcpy(texMat, TextureMatrix[0], sizeof(mat4));
			matTranslate(texMat, TexShift0[0], TexShift0[1], TexShift0[2]);
			glUniformMatrix4fv(glGetUniformLocation(prog, "TextureMatrix"), 1, GL_FALSE, texMat);
		}

		glUniform1i(glGetUniformLocation(prog, "TexImages0"), 0);
		glUniform4fv(glGetUniformLocation(prog, "TexShift0"), 1, TexShift0);
	}
	else if (color)
		prog = sh_color->UseProgram();
	else
	{
		prog = sh_flat->UseProgram();
		glUniform4f(glGetUniformLocation(prog, "FrontColor"), CurrentColor[0], CurrentColor[1], CurrentColor[2], CurrentColor[3]);
	}

	glUniformMatrix4fv(glGetUniformLocation(prog, "ModelViewProjectionMatrix"), 1, GL_FALSE, ModelViewProjectionMatrix);
	changed = false;
}

void shDrawTex(glRect tex, glRect crop, glRect sizes, float z, float zn, float zf)
{
	GLuint prog = sh_texture->UseProgram();
	mat4 mat;
	GLfloat verts[] = {
		tex.x, tex.y, z,
		tex.x + tex.width, tex.y, z,
		tex.x + tex.width, tex.y + tex.height, z,
		tex.x, tex.y + tex.height, z,
	};
	GLfloat s0 = crop.x / sizes.x, s1 = (crop.x + crop.width) / sizes.x;
	GLfloat t0 = crop.y / sizes.y, t1 = (crop.y + crop.height) / sizes.y;
	GLfloat coords[] = {
		s0, t0,
		s1, t0,
		s1, t1,
		s0, t1,
	};
	matOrtho(mat, 0, (float)sizes.width, 0, (float)sizes.height, zn, zf);

	mat4 matViewport, temp;
	matIdentity(matViewport);
	matViewport[0] = proj_sx;
	matViewport[5] = proj_sy;
	matViewport[12] = proj_ox;
	matViewport[13] = proj_oy;
	matMult(temp, matViewport, mat);

	glEnable(GL_CULL_FACE);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, verts);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, coords);
	glUniform1i(glGetUniformLocation(prog, "TexImages0"), 0);
	glUniform4fv(glGetUniformLocation(prog, "TexShift0"), 1, TexShift0);
	glUniformMatrix4fv(glGetUniformLocation(prog, "ModelViewProjectionMatrix"), 1, GL_FALSE, temp);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	changed = true;
}
