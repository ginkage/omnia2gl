
#-------------------------------------------------
# ORION - OpenGL ES 2.0 Shading Language Compiler
# SAMSUNG INDIA SOFTWARE OPERATIONS PVT. LTD.
# Compiler Version	: v04.00.03
# Release Date		: 30.06.2008
# FIMG VERSION      : FIMGv1.5
# Optimizer Options :  -O
#-------------------------------------------------

vs_3_0

fimg_version	0x01020000

dcl_f4_gl_Position	o0.x
dcl_f4_Vertex	v0.x
dcl_f4_MultiTexCoord0	v1.x
dcl_f4_MultiTexCoord1	v2.x
dcl_m4_ModelViewProjectionMatrix	c2.x
dcl_f4_TexShift0	c6.x
dcl_f4_TexShift1	c7.x
dcl_f4_TexCoord0	o1.x
dcl_f4_TexCoord1	o2.x

def c1, 1.0, 1.0, 1.0, 1.0
def c0, 0.0, 0.0, 0.0, 0.0

label start
label main_
mul r0.xyzw, c2.xyzw, v0.xxxx	# Vertex=v0.xxxx
mad r0.xyzw, c3.xyzw, v0.yyyy, r0.xyzw	# Vertex=v0.yyyy
mad r0.xyzw, c4.xyzw, v0.zzzz, r0.xyzw	# Vertex=v0.zzzz
mad o0.xyzw, c5.xyzw, v0.wwww, r0.xyzw	# Vertex=v0.wwww
add o1.xyzw, v1.xyzw, c6.xyzw	# TexCoord0=o1.xyzw, MultiTexCoord0=v1.xyzw, TexShift0=c6.xyzw
add o2.xyzw, v2.xyzw, c7.xyzw	# TexCoord1=o2.xyzw, MultiTexCoord1=v2.xyzw, TexShift1=c7.xyzw
label main_end
ret
# 7 instructions, 9 C regs, 1 R regs 
