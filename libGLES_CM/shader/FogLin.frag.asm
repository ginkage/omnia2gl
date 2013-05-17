
#-------------------------------------------------
# ORION - OpenGL ES 2.0 Shading Language Compiler
# SAMSUNG INDIA SOFTWARE OPERATIONS PVT. LTD.
# Compiler Version	: v04.00.03
# Release Date		: 30.06.2008
# FIMG VERSION      : FIMGv1.5
# Optimizer Options :  -O
#-------------------------------------------------

ps_3_0

fimg_version	0x01020000

dcl_f2_TexDim0	c2.x
dcl_s2_TexImages0	s0
dcl_f4_TexCoord0	v0.x
dcl_f4_FrontColor	v1.x
dcl_f1_FogFragCoord	v2.x
dcl_f1_Fog.density	c3.x
dcl_f1_Fog.start	c4.x
dcl_f1_Fog.end	c5.x
dcl_f4_Fog.color	c6.x
dcl_f1_Fog.scale	c7.x
dcl_i1_Fog.mode	c8.x

def c9, 0.000000, 0, 0, 0
def c1, 1.0, 1.0, 1.0, 1.0
def c0, 0.0, 0.0, 0.0, 0.0
def c10, 1.000000, 0, 0, 0

label start
label main_
texld r1.xyzw, v0.xyzw, s0	# TexImages0=s0
mul r1.xyzw, r1.xyzw, v1.xyzw	# fragColor=r1.xyzw, FrontColor=v1.xyzw
add r0.x, c5.x, !v2.x
mul_sat r0.x, r0.x, c7.x
# mad_sat r0.x, v2.x, c7.x, c5.x	# FogFragCoord=!v2.x
mad r2.xyzw, -r0.xxxx, c6.xyzw, c6.xyzw	# fog=!r0.xxxx
mad_sat oColor.xyzw, r1.xyzw, r0.xxxx, r2.xyzw	# fragColor=r1.xyzw, fog=r0.xxxx
label main_end
ret
# 16 instructions, 12 C regs, 3 R regs 
