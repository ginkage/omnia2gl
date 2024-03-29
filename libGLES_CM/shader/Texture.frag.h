#ifndef _TextureFrag_h_
#define _TextureFrag_h_


// Header generated from binary by WriteAsBinHeader()..
static const int TextureFragLength = 61;
static const unsigned int TextureFrag[TextureFragLength]={
	0x20205350,	0xFFFF0008,	0x00000048,	0x01020000,	0x00000002,	0x00000003,	0x00000000,	0x00000000,	0x00000001,	0x00000000,
	0x00000001,	0x00000001,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x0000001D,	0x00000000,	0x00000000,	0x0000E407,
	0x107A10E4,	0x00000000,	0x00000000,	0x00000000,	0x1E000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,
	0x3F800000,	0x3F800000,	0x3F800000,	0x3F800000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000013,	0x00000009,
	0x00000009,	0x00000000,	0x00000000,	0x00000000,	0x00000007,	0x00000003,	0x00020001,	0x00000008,	0x00000008,	0x0000000A,
	0x0000000F,	0x00030005,	0x00000000,	0x44786554,	0x00306D69,	0x49786554,	0x6567616D,	0x54003073,	0x6F437865,	0x3064726F,
	0x00000000,};

//checksum generated by simpleCheckSum()
static const unsigned int TextureFragCheckSum = 113;

static const char* TextureFragText = 
	"\n"
	"#-------------------------------------------------\n"
	"# ORION - OpenGL ES 2.0 Shading Language Compiler\n"
	"# SAMSUNG INDIA SOFTWARE OPERATIONS PVT. LTD.\n"
	"# Compiler Version    : v04.00.03\n"
	"# Release Date        : 30.06.2008\n"
	"# FIMG VERSION      : FIMGv1.5\n"
	"# Optimizer Options :  -O\n"
	"#-------------------------------------------------\n"
	"\n"
	"ps_3_0\n"
	"\n"
	"fimg_version    0x01020000\n"
	"\n"
	"dcl_f2_TexDim0    c2.x\n"
	"dcl_s2_TexImages0    s0\n"
	"dcl_f4_TexCoord0    v0.x\n"
	"\n"
	"def c1, 1.0, 1.0, 1.0, 1.0\n"
	"def c0, 0.0, 0.0, 0.0, 0.0\n"
	"\n"
	"label start\n"
	"label main_\n"
	"texld_sat oColor.xyzw, v0.xyzw, s0    # TexImages0=s0\n"
	"label main_end\n"
	"ret\n"
	"# 9 instructions, 4 C regs, 2 R regs \n"
	"";

#ifdef GL_HELPERS_INCLUDED
//glHelpers.h must be included BEFORE any of the shader header files. Also make sure you have the latest version of glHelpers.h
static ghShader TextureFragShader(TextureFragText, TextureFrag, TextureFragLength, TextureFragCheckSum);


#endif


#endif //_TextureFrag_h_
