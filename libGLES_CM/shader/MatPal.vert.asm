
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
dcl_f4_Color	v2.x
dcl_f4_MultiTexCoord0	v3.x
dcl_f3_PaletteWeight	v1.x
dcl_f3_PaletteIndex	v4.x
dcl_m4_ProjectionMatrix	c1.x
dcl_i1_PaletteSize	c5.x
dcl_32m4_MatrixPalette	c6.x, 16
dcl_f4_TexShift0	c134.x
dcl_f4_FrontColor	o1.x
dcl_f4_TexCoord0	o2.x

def c0, 0, 1, 2, 4

label start
label main_
mul r4.w, v4.x, c0.w	# idx=v4.x
mova a0.x, r4.w
mul r1.xyzw, c6[a0.x].xyzw, v0.xxxx	# Vertex=v0.xxxx
mad r1.xyzw, c7[a0.x].xyzw, v0.yyyy, r1.xyzw	# Vertex=v0.yyyy
mad r1.xyzw, c8[a0.x].xyzw, v0.zzzz, r1.xyzw	# Vertex=v0.zzzz
mad r1.xyzw, c9[a0.x].xyzw, v0.wwww, r1.xyzw	# Vertex=v0.wwww
mul r0.xyzw, r1.xyzw, v1.xxxx	# vPos=r0.xyzw, PaletteWeight=v1.xxxx
mov r2.x, c5.x
setp_gt p1.x, r2.x, c0.y
bf L1, p1.x
mul r4.w, v4.y, c0.w	# idx=v4.x
mova a0.x, r4.w
mul r1.xyzw, c6[a0.x].xyzw, v0.xxxx	# Vertex=v0.xxxx
mad r1.xyzw, c7[a0.x].xyzw, v0.yyyy, r1.xyzw	# Vertex=v0.yyyy
mad r1.xyzw, c8[a0.x].xyzw, v0.zzzz, r1.xyzw	# Vertex=v0.zzzz
mad r1.xyzw, c9[a0.x].xyzw, v0.wwww, r1.xyzw	# Vertex=v0.wwww
mad r0.xyzw, r1.xyzw, v1.yyyy, r0.xyzw	# vPos=r0.xyzw, PaletteWeight=v1.yyyy, vPos=r0.xyzw
setp_gt p1.x, r2.x, c0.z
bf L1, p1.x
mul r4.w, v4.z, c0.w	# idx=v4.x
mova a0.x, r4.w
mul r1.xyzw, c6[a0.x].xyzw, v0.xxxx	# Vertex=v0.xxxx
mad r1.xyzw, c7[a0.x].xyzw, v0.yyyy, r1.xyzw	# Vertex=v0.yyyy
mad r1.xyzw, c8[a0.x].xyzw, v0.zzzz, r1.xyzw	# Vertex=v0.zzzz
mad r1.xyzw, c9[a0.x].xyzw, v0.wwww, r1.xyzw	# Vertex=v0.wwww
mad r0.xyzw, r1.xyzw, v1.zzzz, r0.xyzw	# vPos=r0.xyzw, PaletteWeight=v1.zzzz, vPos=r0.xyzw
label L1
mul r1.xyzw, c1.xyzw, r0.xxxx	# vPos=r0.xxxx
mad r1.xyzw, c2.xyzw, r0.yyyy, r1.xyzw	# vPos=r0.yyyy
mad r1.xyzw, c3.xyzw, r0.zzzz, r1.xyzw	# vPos=r0.zzzz
mad o0.xyzw, c4.xyzw, r0.wwww, r1.xyzw	# vPos=r0.wwww
label main_end
mov o1.xyzw, v2.xyzw
add o2.xyzw, v3.xyzw, c134.xyzw
ret
# 28 instructions, 46 C regs, 2 R regs 
