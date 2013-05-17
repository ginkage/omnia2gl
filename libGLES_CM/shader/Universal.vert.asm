
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
dcl_f1_gl_PointSize	o9.x
dcl_m4_ModelViewProjectionMatrix	c2.x
dcl_m4_ModelViewMatrix	c6.x
dcl_m3_NormalMatrix	c10.x
dcl_2m4_TextureMatrix	c16.x, 16
dcl_f4_Vertex	v0.x
dcl_f4_Color	v2.x
dcl_f3_Normal	v1.x
dcl_f4_MultiTexCoord0	v3.x
dcl_f4_MultiTexCoord1	v4.x
dcl_f1_PointSize	v5.x
dcl_f4_FrontColor	o1.x
dcl_f1_FogFragCoord	o4.x
dcl_b1_PointEnabled	c140.x
dcl_b1_FogEnabled	c141.x
dcl_f4_FrontNBackMaterial.emission	c24.x
dcl_f4_FrontNBackMaterial.ambient	c25.x
dcl_f4_FrontNBackMaterial.diffuse	c26.x
dcl_f4_FrontNBackMaterial.specular	c27.x
dcl_f1_FrontNBackMaterial.shininess	c28.x
dcl_b1_colorMaterialEnabled	c29.x
dcl_f4_LightSource[0].ambient	c34.x
dcl_f4_LightSource[0].diffuse	c35.x
dcl_f4_LightSource[0].specular	c36.x
dcl_f4_LightSource[0].position	c37.x
dcl_f4_LightSource[0].halfVector	c38.x
dcl_f3_LightSource[0].spotDirection	c39.x
dcl_f1_LightSource[0].spotExponent	c40.x
dcl_f1_LightSource[0].spotCutoff	c41.x
dcl_f1_LightSource[0].spotCosCutoff	c42.x
dcl_f1_LightSource[0].constantAttenuation	c43.x
dcl_f1_LightSource[0].linearAttenuation	c44.x
dcl_f1_LightSource[0].quadraticAttenuation	c45.x
dcl_b1_LightSource[0].isDirectional	c46.x
dcl_f4_LightSource[1].ambient	c47.x
dcl_f4_LightSource[1].diffuse	c48.x
dcl_f4_LightSource[1].specular	c49.x
dcl_f4_LightSource[1].position	c50.x
dcl_f4_LightSource[1].halfVector	c51.x
dcl_f3_LightSource[1].spotDirection	c52.x
dcl_f1_LightSource[1].spotExponent	c53.x
dcl_f1_LightSource[1].spotCutoff	c54.x
dcl_f1_LightSource[1].spotCosCutoff	c55.x
dcl_f1_LightSource[1].constantAttenuation	c56.x
dcl_f1_LightSource[1].linearAttenuation	c57.x
dcl_f1_LightSource[1].quadraticAttenuation	c58.x
dcl_b1_LightSource[1].isDirectional	c59.x
dcl_f4_LightSource[2].ambient	c60.x
dcl_f4_LightSource[2].diffuse	c61.x
dcl_f4_LightSource[2].specular	c62.x
dcl_f4_LightSource[2].position	c63.x
dcl_f4_LightSource[2].halfVector	c64.x
dcl_f3_LightSource[2].spotDirection	c65.x
dcl_f1_LightSource[2].spotExponent	c66.x
dcl_f1_LightSource[2].spotCutoff	c67.x
dcl_f1_LightSource[2].spotCosCutoff	c68.x
dcl_f1_LightSource[2].constantAttenuation	c69.x
dcl_f1_LightSource[2].linearAttenuation	c70.x
dcl_f1_LightSource[2].quadraticAttenuation	c71.x
dcl_b1_LightSource[2].isDirectional	c72.x
dcl_f4_LightSource[3].ambient	c73.x
dcl_f4_LightSource[3].diffuse	c74.x
dcl_f4_LightSource[3].specular	c75.x
dcl_f4_LightSource[3].position	c76.x
dcl_f4_LightSource[3].halfVector	c77.x
dcl_f3_LightSource[3].spotDirection	c78.x
dcl_f1_LightSource[3].spotExponent	c79.x
dcl_f1_LightSource[3].spotCutoff	c80.x
dcl_f1_LightSource[3].spotCosCutoff	c81.x
dcl_f1_LightSource[3].constantAttenuation	c82.x
dcl_f1_LightSource[3].linearAttenuation	c83.x
dcl_f1_LightSource[3].quadraticAttenuation	c84.x
dcl_b1_LightSource[3].isDirectional	c85.x
dcl_f4_LightSource[4].ambient	c86.x
dcl_f4_LightSource[4].diffuse	c87.x
dcl_f4_LightSource[4].specular	c88.x
dcl_f4_LightSource[4].position	c89.x
dcl_f4_LightSource[4].halfVector	c90.x
dcl_f3_LightSource[4].spotDirection	c91.x
dcl_f1_LightSource[4].spotExponent	c92.x
dcl_f1_LightSource[4].spotCutoff	c93.x
dcl_f1_LightSource[4].spotCosCutoff	c94.x
dcl_f1_LightSource[4].constantAttenuation	c95.x
dcl_f1_LightSource[4].linearAttenuation	c96.x
dcl_f1_LightSource[4].quadraticAttenuation	c97.x
dcl_b1_LightSource[4].isDirectional	c98.x
dcl_f4_LightSource[5].ambient	c99.x
dcl_f4_LightSource[5].diffuse	c100.x
dcl_f4_LightSource[5].specular	c101.x
dcl_f4_LightSource[5].position	c102.x
dcl_f4_LightSource[5].halfVector	c103.x
dcl_f3_LightSource[5].spotDirection	c104.x
dcl_f1_LightSource[5].spotExponent	c105.x
dcl_f1_LightSource[5].spotCutoff	c106.x
dcl_f1_LightSource[5].spotCosCutoff	c107.x
dcl_f1_LightSource[5].constantAttenuation	c108.x
dcl_f1_LightSource[5].linearAttenuation	c109.x
dcl_f1_LightSource[5].quadraticAttenuation	c110.x
dcl_b1_LightSource[5].isDirectional	c111.x
dcl_f4_LightSource[6].ambient	c112.x
dcl_f4_LightSource[6].diffuse	c113.x
dcl_f4_LightSource[6].specular	c114.x
dcl_f4_LightSource[6].position	c115.x
dcl_f4_LightSource[6].halfVector	c116.x
dcl_f3_LightSource[6].spotDirection	c117.x
dcl_f1_LightSource[6].spotExponent	c118.x
dcl_f1_LightSource[6].spotCutoff	c119.x
dcl_f1_LightSource[6].spotCosCutoff	c120.x
dcl_f1_LightSource[6].constantAttenuation	c121.x
dcl_f1_LightSource[6].linearAttenuation	c122.x
dcl_f1_LightSource[6].quadraticAttenuation	c123.x
dcl_b1_LightSource[6].isDirectional	c124.x
dcl_f4_LightSource[7].ambient	c125.x
dcl_f4_LightSource[7].diffuse	c126.x
dcl_f4_LightSource[7].specular	c127.x
dcl_f4_LightSource[7].position	c128.x
dcl_f4_LightSource[7].halfVector	c129.x
dcl_f3_LightSource[7].spotDirection	c130.x
dcl_f1_LightSource[7].spotExponent	c131.x
dcl_f1_LightSource[7].spotCutoff	c132.x
dcl_f1_LightSource[7].spotCosCutoff	c133.x
dcl_f1_LightSource[7].constantAttenuation	c134.x
dcl_f1_LightSource[7].linearAttenuation	c135.x
dcl_f1_LightSource[7].quadraticAttenuation	c136.x
dcl_b1_LightSource[7].isDirectional	c137.x
dcl_b1_LightingEnabled	c30.x
dcl_f4_FrontNBackLightModelProduct.sceneColor	c31.x
dcl_i1_numLightUsed	c33.x
dcl_2b1_Texture2DEnabled	c13.x, 4
dcl_f4_TexCoord0	o2.x
dcl_f4_TexCoord1	o3.x

def c144, 2.000000, 0, 0, 0
def c142, 0.000000, 0.000000, 1.000000, 0
def c138, 0.000000, 0, 0, 0
def c15, 1, 0, 0, 0
def c1, 1.0, 1.0, 1.0, 1.0
def c0, 0.0, 0.0, 0.0, 0.0
def c32, 13, 0, 0, 0
def c139, 1.000000, 0, 0, 0
def c143, 180.000000, 0, 0, 0

label light_point_view_local_vf3_vf3_i1_
mul r4.w, r0.x, c32.x	# idx=r0.x
mova a0.x, r4.w
add r5.xyz, c37[a0.x].xyz, !r2.xyz	# vert2light=r5.xyz, epos=!r2.xyz
dp3 r3.x, r5.xyz, r5.xyz	# vert2light=r5.xyz, vert2light=r5.xyz
rsq r3.x, r3.x
mul r3.xyz, r3.xxx, r5.xyz	# vert2light=r5.xyz
dp3 r5.w, r5.xyz, r5.xyz	# vert2light=r5.xyz, vert2light=r5.xyz
rsq r3.w, r5.w
rcp r3.w, r3.w
add r5.xyz, r3.xyz, c142.xyz	# ldir=r3.xyz, vdir=c142.xyz
dp3 r6.x, r5.xyz, r5.xyz
rsq r6.x, r6.x
mul r6.xyz, r6.xxx, r5.xyz
dp3 r6.w, r0.yzw, r3.xyz	# normal=r0.yzw, ldir=r3.xyz
mov r4.z, r6.w	# PARAM=r4.z
dp3 r6.w, r0.yzw, r6.xyz	# normal=r0.yzw, halfv=r6.xyz
mov r4.x, r6.w	# PARAM=r4.x
mov r4.y, c28.x	# PARAM=r4.y
call lit_f1_f1_f1_
mul r5.x, r0.x, c32.x	# idx=r0.x
mova a0.y, r5.x
dp3 r4.x, !r3.xyz, c39[a0.y].xyz	# ldir=!r3.xyz
mul r5.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r5.x
mov r5.x, c41[a0.x].x
setp_ge p1.x, r5.x, c143.x
bf L17, p1.x
mov r5.x, c139.x	# spotEffect=r5.x
b L18
label L17
mul r5.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r5.x
setp_gt p1.x, c42[a0.x].x, r4.x	# spotDot=r4.x
bf L19, p1.x
mov r5.x, c138.x	# spotEffect=r5.x
b L20
label L19
log r4.x, r4.x	# spotDot=r4.x
mul r5.w, r0.x, c32.x	# idx=r0.x
mova a0.y, r5.w
mul r4.x, r4.x, c40[a0.y].x
log r5.w, c144.x
mul r5.w, r4.x, r5.w
exp r5.w, r5.w
mov r5.x, r5.w	# spotEffect=r5.x
label L20
label L18
mul r5.w, r0.x, c32.x	# idx=r0.x
mul r6.x, r0.x, c32.x	# idx=r0.x
mova a0.y, r6.x
mul r6.x, r3.w, c44[a0.y].x	# d=r3.w
mova a0.x, r5.w
add r6.x, c43[a0.x].x, r6.x
mul r5.w, r3.w, r3.w	# d=r3.w, d=r3.w
mul r6.y, r0.x, c32.x	# idx=r0.x
mova a0.y, r6.y
mad r6.x, r5.w, c45[a0.y].x, r6.x
rcp r6.x, r6.x
mul r5.x, r6.x, r5.x	# att=r6.x, spotEffect=r5.x
mul r6.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r6.x
mov r7.xyzw, c34[a0.x].xyzw
mul r7.xyzw, r7.xyzw, c25.xyzw
mul r6.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r6.x
mov r8.xyzw, c35[a0.x].xyzw
mul r8.xyzw, r8.xyzw, c26.xyzw
mad r7.xyzw, r8.xyzw, r5.yyyy, r7.xyzw	# coeffs=r5.yyyy
mul r6.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r6.x
mov r8.xyzw, c36[a0.x].xyzw
mul r8.xyzw, r8.xyzw, c27.xyzw
mad r7.xyzw, r8.xyzw, r5.zzzz, r7.xyzw	# coeffs=r5.zzzz
mul r4.xyzw, r5.xxxx, r7.xyzw	# outCol=r4.xyzw
label light_point_view_local_vf3_vf3_i1_end
ret
label light_directional_view_local_vf3_vf3_i1_
mul r4.w, r0.x, c32.x	# idx=r0.x
mova a0.x, r4.w
mova a0.y, r4.w
mov r5.xyz, c37[a0.x].xyz
dp3 r5.x, r5.xyz, c37[a0.y].xyz
rsq r5.x, r5.x
mova a0.y, r4.w
mul r5.xyz, r5.xxx, c37[a0.y].xyz
dp3 r6.x, r5.xyz, r5.xyz	# lightDir=r5.xyz, lightDir=r5.xyz
rsq r6.x, r6.x
mul r6.xyz, r6.xxx, r5.xyz	# lightDir=r5.xyz
dp3 r6.w, r0.yzw, r5.xyz	# normal=r0.yzw, lightDir=r5.xyz
mov r4.z, r6.w	# PARAM=r4.z
dp3 r6.w, r0.yzw, r6.xyz	# normal=r0.yzw, halfv=r6.xyz
mov r4.x, r6.w	# PARAM=r4.x
mov r4.y, c28.x	# PARAM=r4.y
call lit_f1_f1_f1_
mul r5.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r5.x
mov r6.xyzw, c34[a0.x].xyzw
mul r6.xyzw, r6.xyzw, c25.xyzw
mul r5.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r5.x
mov r7.xyzw, c35[a0.x].xyzw
mul r7.xyzw, r7.xyzw, c26.xyzw
mad r6.xyzw, r7.xyzw, r5.yyyy, r6.xyzw	# coeffs=r5.yyyy
mul r5.x, r0.x, c32.x	# idx=r0.x
mova a0.x, r5.x
mov r7.xyzw, c36[a0.x].xyzw
mul r7.xyzw, r7.xyzw, c27.xyzw
mad r4.xyzw, r7.xyzw, r5.zzzz, r6.xyzw	# outCol=r4.xyzw, coeffs=r5.zzzz
label light_directional_view_local_vf3_vf3_i1_end
ret
label lit_f1_f1_f1_
setp_gt p1.x, c138.x, r4.z	# NdotL=r4.z
bf L21, p1.x
mov r5.y, c138.x	# coeff=r5.y
b L22
label L21
mov r5.y, r4.z	# coeff=r5.y, NdotL=r4.z
label L22
sge r4.z, c138.x, r4.z	# NdotL=r4.z
slt r4.w, r4.x, c138.x	# NdotH=r4.x
add_sat r4.w, r4.z, r4.w
setp_eq p1.x, c1.x, r4.w
bf L23, p1.x
mov r5.z, c138.x	# coeff=r5.z
b L24
label L23
log r4.x, r4.x	# NdotH=r4.x
mul r4.x, r4.y, r4.x	# m=r4.y
exp r4.x, r4.x
mov r5.z, r4.x	# coeff=r5.z
label L24
label lit_f1_f1_f1_end
ret
label start
label main_
mul r1.xyzw, c2.xyzw, v0.xxxx	# Vertex=v0.xxxx
mad r1.xyzw, c3.xyzw, v0.yyyy, r1.xyzw	# Vertex=v0.yyyy
mad r1.xyzw, c4.xyzw, v0.zzzz, r1.xyzw	# Vertex=v0.zzzz
mad o0.xyzw, c5.xyzw, v0.wwww, r1.xyzw	# Vertex=v0.wwww
mul r2.xyzw, c6.xyzw, v0.xxxx	# Vertex=v0.xxxx
mad r2.xyzw, c7.xyzw, v0.yyyy, r2.xyzw	# Vertex=v0.yyyy
mad r2.xyzw, c8.xyzw, v0.zzzz, r2.xyzw	# Vertex=v0.zzzz
mad r2.xyzw, c9.xyzw, v0.wwww, r2.xyzw	# Vertex=v0.wwww
mul r1.xyz, c10.xyz, v1.xxx	# Normal=v1.xxx
mad r1.xyz, c11.xyz, v1.yyy, r1.xyz	# Normal=v1.yyy
mad r1.xyz, c12.xyz, v1.zzz, r1.xyz	# Normal=v1.zzz
dp3 r0.y, r1.xyz, r1.xyz
rsq r0.y, r0.y
mul r0.yzw, r0.xyyy, r1.xxyz
mov o1.xyzw, v2.xyzw	# FrontColor=o1.xyzw, Color=v2.xyzw
mov r1.w, c1.x
setp_eq p1.x, r1.w, c13.x
bf L1, p1.x
mul r1.xyzw, c16.xyzw, v3.xxxx	# MultiTexCoord0=v3.xxxx
mad r1.xyzw, c17.xyzw, v3.yyyy, r1.xyzw	# MultiTexCoord0=v3.yyyy
mad r1.xyzw, c18.xyzw, v3.zzzz, r1.xyzw	# MultiTexCoord0=v3.zzzz
mad o2.xyzw, c19.xyzw, v3.wwww, r1.xyzw	# MultiTexCoord0=v3.wwww
label L1
mov r0.x, c1.x
setp_eq p1.x, r0.x, c14.x
bf L3, p1.x
mul r1.xyzw, c20.xyzw, v4.xxxx	# MultiTexCoord1=v4.xxxx
mad r1.xyzw, c21.xyzw, v4.yyyy, r1.xyzw	# MultiTexCoord1=v4.yyyy
mad r1.xyzw, c22.xyzw, v4.zzzz, r1.xyzw	# MultiTexCoord1=v4.zzzz
mad o3.xyzw, c23.xyzw, v4.wwww, r1.xyzw	# MultiTexCoord1=v4.wwww
label L3
mov r0.x, c1.x
setp_eq p1.x, r0.x, c29.x	# colorMaterialEnabled=c29.x
label L5
mov r0.x, c1.x
setp_eq p1.x, r0.x, c30.x	# LightingEnabled=c30.x
bf L7, p1.x
mov r1.xyzw, c31.xyzw	# vcolor=r1.xyzw
mov r0.x, c0.x	# i=r0.x
label L9
slt r4.x, r0.x, c33.x	# i=r0.x, numLightUsed=c33.x
setp_eq p1.x, r4.x, c1.x
bf L10, p1.x
mul r4.x, r0.x, c32.x	# i=r0.x
mova a0.y, r4.x
mov r4.x, c1.x
setp_eq p1.x, r4.x, c46[a0.y].x
bf L11, p1.x
call light_directional_view_local_vf3_vf3_i1_
add r1.xyzw, r1.xyzw, r4.xyzw	# vcolor=r1.xyzw, vcolor=r1.xyzw
b L12
label L11
call light_point_view_local_vf3_vf3_i1_
add r1.xyzw, r1.xyzw, r4.xyzw	# vcolor=r1.xyzw, vcolor=r1.xyzw
label L12
add r0.x, r0.x, c1.x	# i=r0.x, i=r0.x
b L9
label L10
mov_sat r1.xyzw, r1.xyzw	# vcolor=r1.xyzw
mov o1.xyzw, r1.xyzw	# FrontColor=o1.xyzw, vcolor=r1.xyzw
label L7
mov r0.x, c1.x
setp_eq p1.x, r0.x, c140.x	# PointEnabled=c140.x
label L13
mov r0.x, c1.x
setp_eq p1.x, r0.x, c141.x	# FogEnabled=c141.x
label L15
label main_end
mov o9.x, v5.x
mov o4.x, |r2.z|
ret
# 180 instructions, 146 C regs, 9 R regs 
