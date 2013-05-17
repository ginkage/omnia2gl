
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
dcl_f1_Fog.density	c4.x
dcl_f1_Fog.start	c5.x
dcl_f1_Fog.end	c6.x
dcl_f4_Fog.color	c7.x
dcl_f1_Fog.scale	c8.x
dcl_i1_Fog.mode	c9.x

def c13, 1.442695, 0, 0, 0
def c11, 1.000000, 0, 0, 0
def c3, 1, 0, 0, 0
def c1, 1.0, 1.0, 1.0, 1.0
def c0, 0.0, 0.0, 0.0, 0.0
def c10, 0.000000, 0, 0, 0
def c12, 2, 0, 0, 0
def c14, 3, 0, 0, 0

label start
label main_
texld r2.xyzw, v0.xyzw, s0	# TexImages0=s0
mul r1.xyzw, r2.xyzw, v1.xyzw	# fragColor=r1.xyzw, FrontColor=v1.xyzw
setp_eq p1.x, c9.x, c3.x
bf L1, p1.x
# mad_sat r0.x, c8.x, !v2.x, c6.x
add r0.y, !v2.x, c6.x
mul_sat r0.x, r0.y, c8.x	# fog=r0.x, FogFragCoord=!v2.x
b L2
label L1
setp_eq p1.x, c9.x, c12.x
bf L3, p1.x
mul r0.y, !c4.x, v2.x	# FogFragCoord=v2.x
mul r0.y, r0.y, c13.x
exp_sat r0.x, r0.y
b L2
label L3
setp_eq p1.x, c9.x, c14.x
bf L2, p1.x
mul r0.y, !c4.x, c4.x
mul r0.y, r0.y, v2.x	# FogFragCoord=v2.x
mul r0.y, r0.y, v2.x	# FogFragCoord=v2.x
mul r0.y, r0.y, c13.x
exp_sat r0.x, r0.y
label L2
add r2.xyzw, !r0.xxxx, c1.xyzw
mul r2.xyzw, r2.xyzw, c7.xyzw	# fog=!r0.xxxx
mad_sat oColor.xyzw, r1.xyzw, r0.xxxx, r2.xyzw	# fragColor=r1.xyzw, fog=r0.xxxx
label main_end
ret
# 40 instructions, 16 C regs, 3 R regs 
