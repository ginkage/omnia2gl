
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
dcl_f4_Color	v1.x
dcl_f4_MultiTexCoord0	v2.x
dcl_m4_ModelViewProjectionMatrix	c2.x
dcl_m4_TextureMatrix	c6.x
dcl_f4_FrontColor	o1.x
dcl_f4_TexCoord0	o2.x

def c1, 1.0, 1.0, 1.0, 1.0
def c0, 0.0, 0.0, 0.0, 0.0

label start
label main_
mul r0.xyzw, c2.xyzw, v0.xxxx	# Vertex=v0.xxxx
mad r0.xyzw, c3.xyzw, v0.yyyy, r0.xyzw	# Vertex=v0.yyyy
mad r0.xyzw, c4.xyzw, v0.zzzz, r0.xyzw	# Vertex=v0.zzzz
mad o0.xyzw, c5.xyzw, v0.wwww, r0.xyzw	# Vertex=v0.wwww
dp4 o2.x, v2.xyzw, c6.xyzw	# MultiTexCoord0=v2.xyzw
dp4 o2.y, v2.xyzw, c7.xyzw	# MultiTexCoord0=v2.xyzw
dp4 o2.z, v2.xyzw, c8.xyzw	# MultiTexCoord0=v2.xyzw
dp4 o2.w, v2.xyzw, c9.xyzw	# MultiTexCoord0=v2.xyzw
label main_end
mov o1.xyzw, v1.xyzw
ret
# 10 instructions, 11 C regs, 1 R regs 
