
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

def c1, 1.0, 1.0, 1.0, 1.0
def c0, 0.0, 0.0, 0.0, 0.0

label start
label main_
texld_sat oColor.xyzw, v0.xyzw, s0	# TexImages0=s0
label main_end
ret
# 9 instructions, 4 C regs, 2 R regs 
