
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

dcl_f2_TexDim0	c49.x
dcl_f2_TexDim1	c53.x
dcl_f4_FrontColor	v0.x
dcl_f1_FogFragCoord	v3.x
dcl_f1_Fog.density	c56.x
dcl_f1_Fog.start	c57.x
dcl_f1_Fog.end	c58.x
dcl_f4_Fog.color	c59.x
dcl_f1_Fog.scale	c60.x
dcl_i1_Fog.mode	c61.x
dcl_b1_FogEnabled	c55.x
dcl_s2_TexImages0	s0
dcl_s2_TexImages1	s1
dcl_2b1_Texture2DEnabled	c50.x, 4
dcl_f4_TexCoord0	v1.x
dcl_f4_TexCoord1	v2.x
dcl_f4_TexEnv[0].color	c8.x
dcl_i1_TexEnv[0].mode	c9.x
dcl_i1_TexEnv[0].combineRgb	c10.x
dcl_i1_TexEnv[0].combineAlpha	c11.x
dcl_3i1_TexEnv[0].srcRgb	c12.x, 4
dcl_3i1_TexEnv[0].srcAlpha	c15.x, 4
dcl_3i1_TexEnv[0].operandRgb	c18.x, 4
dcl_3i1_TexEnv[0].operandAlpha	c21.x, 4
dcl_f1_TexEnv[0].rgbScale	c24.x
dcl_f1_TexEnv[0].alphaScale	c25.x
dcl_f4_TexEnv[1].color	c26.x
dcl_i1_TexEnv[1].mode	c27.x
dcl_i1_TexEnv[1].combineRgb	c28.x
dcl_i1_TexEnv[1].combineAlpha	c29.x
dcl_3i1_TexEnv[1].srcRgb	c30.x, 4
dcl_3i1_TexEnv[1].srcAlpha	c33.x, 4
dcl_3i1_TexEnv[1].operandRgb	c36.x, 4
dcl_3i1_TexEnv[1].operandAlpha	c39.x, 4
dcl_f1_TexEnv[1].rgbScale	c42.x
dcl_f1_TexEnv[1].alphaScale	c43.x

def c63, 1.0, 0.0, 1.0, 1.0
def c62, 1.0, 0.0, 1.0, 1.0
def c54, 1.442695, 0, 0, 0
def c48, 4.000000, 0, 0, 0
def c46, 7, 0, 0, 0
def c44, 0.500000, 0, 0, 0
def c6, 4, 0, 0, 0
def c4, 1.000000, 0, 0, 0
def c2, 1, 0, 0, 0
def c1, 1.0, 1.0, 1.0, 1.0
def c0, 0.0, 0.0, 0.0, 0.0
def c3, 2, 0, 0, 0
def c5, 3, 0, 0, 0
def c7, 5, 0, 0, 0
def c45, 6, 0, 0, 0
def c47, 8, 0, 0, 0
def c52, 0.000000, 0, 0, 0

label tex_arg_calculation_vf4_vf4_vf4_vf4_i1_i1_i1_i1_
setp_eq p1.x, r7.y, c2.x	# srcrgb=r7.y
bf L1, p1.x
setp_eq p1.x, r7.z, c2.x	# opndrgb=r7.z
bf L3, p1.x
mov r3.xyz, r8.xyz	# temp=r3.xyz, srcColor=r8.xyz
b L4
label L3
setp_eq p1.x, r7.z, c3.x	# opndrgb=r7.z
bf L5, p1.x
add r3.xyz, c4.xxx, !r8.xyz	# temp=r3.xyz, srcColor=!r8.xyz
b L6
label L5
setp_eq p1.x, r7.z, c5.x	# opndrgb=r7.z
bf L7, p1.x
mov r3.xyz, r8.www	# temp=r3.xyz, srcColor=r8.www
b L8
label L7
setp_eq p1.x, r7.z, c6.x	# opndrgb=r7.z
bf L9, p1.x
add r7.y, c4.x, !r8.w	# srcColor=!r8.w
mov r3.xyz, r7.yyy	# temp=r3.xyz
label L9
label L8
label L6
label L4
b L2
label L1
setp_eq p1.x, r7.y, c3.x	# srcrgb=r7.y
bf L11, p1.x
setp_eq p1.x, r7.z, c2.x	# opndrgb=r7.z
bf L13, p1.x
mov r3.xyz, r9.xyz	# temp=r3.xyz, EnvColor=r9.xyz
b L14
label L13
setp_eq p1.x, r7.z, c3.x	# opndrgb=r7.z
bf L15, p1.x
add r3.xyz, c4.xxx, !r9.xyz	# temp=r3.xyz, EnvColor=!r9.xyz
b L16
label L15
setp_eq p1.x, r7.z, c5.x	# opndrgb=r7.z
bf L17, p1.x
mov r3.xyz, r9.www	# temp=r3.xyz, EnvColor=r9.www
b L18
label L17
setp_eq p1.x, r7.z, c6.x	# opndrgb=r7.z
bf L19, p1.x
add r7.y, c4.x, !r9.w	# EnvColor=!r9.w
mov r3.xyz, r7.yyy	# temp=r3.xyz
label L19
label L18
label L16
label L14
b L12
label L11
setp_eq p1.x, r7.y, c5.x	# srcrgb=r7.y
bf L21, p1.x
setp_eq p1.x, r7.z, c2.x	# opndrgb=r7.z
bf L23, p1.x
mov r3.xyz, r0.xyz	# temp=r3.xyz, fragColor=r0.xyz
b L24
label L23
setp_eq p1.x, r7.z, c3.x	# opndrgb=r7.z
bf L25, p1.x
add r3.xyz, c4.xxx, !r0.xyz	# temp=r3.xyz, fragColor=!r0.xyz
b L26
label L25
setp_eq p1.x, r7.z, c5.x	# opndrgb=r7.z
bf L27, p1.x
mov r3.xyz, r0.www	# temp=r3.xyz, fragColor=r0.www
b L28
label L27
setp_eq p1.x, r7.z, c6.x	# opndrgb=r7.z
bf L29, p1.x
add r7.y, c4.x, !r0.w	# fragColor=!r0.w
mov r3.xyz, r7.yyy	# temp=r3.xyz
label L29
label L28
label L26
label L24
b L22
label L21
setp_eq p1.x, r7.y, c6.x	# srcrgb=r7.y
bf L31, p1.x
setp_eq p1.x, r7.z, c2.x	# opndrgb=r7.z
bf L33, p1.x
mov r3.xyz, r1.xyz	# temp=r3.xyz, prevColor=r1.xyz
b L34
label L33
setp_eq p1.x, r7.z, c3.x	# opndrgb=r7.z
bf L35, p1.x
add r3.xyz, c4.xxx, !r1.xyz	# temp=r3.xyz, prevColor=!r1.xyz
b L36
label L35
setp_eq p1.x, r7.z, c5.x	# opndrgb=r7.z
bf L37, p1.x
mov r3.xyz, r1.www	# temp=r3.xyz, prevColor=r1.www
b L38
label L37
setp_eq p1.x, r7.z, c6.x	# opndrgb=r7.z
bf L39, p1.x
add r7.y, c4.x, !r1.w	# prevColor=!r1.w
mov r3.xyz, r7.yyy	# temp=r3.xyz
label L39
label L38
label L36
label L34
label L31
label L22
label L12
label L2
setp_eq p1.x, r7.w, c2.x	# srcAlpha=r7.w
bf L41, p1.x
setp_eq p1.x, r10.x, c2.x	# operandAlpha=r10.x
bf L43, p1.x
mov r3.w, r8.w	# temp=r3.w, srcColor=r8.w
b L44
label L43
setp_eq p1.x, r10.x, c3.x	# operandAlpha=r10.x
bf L45, p1.x
add r3.w, c4.x, !r8.w	# temp=r3.w, srcColor=!r8.w
label L45
label L44
b L42
label L41
setp_eq p1.x, r7.w, c3.x	# srcAlpha=r7.w
bf L47, p1.x
setp_eq p1.x, r10.x, c2.x	# operandAlpha=r10.x
bf L49, p1.x
mov r3.w, r9.w	# temp=r3.w, EnvColor=r9.w
b L50
label L49
setp_eq p1.x, r10.x, c3.x	# operandAlpha=r10.x
bf L51, p1.x
add r3.w, c4.x, !r9.w	# temp=r3.w, EnvColor=!r9.w
label L51
label L50
b L48
label L47
setp_eq p1.x, r7.w, c5.x	# srcAlpha=r7.w
bf L53, p1.x
setp_eq p1.x, r10.x, c2.x	# operandAlpha=r10.x
bf L55, p1.x
mov r3.w, r0.w	# temp=r3.w, fragColor=r0.w
b L56
label L55
setp_eq p1.x, r10.x, c3.x	# operandAlpha=r10.x
bf L57, p1.x
add r3.w, c4.x, !r0.w	# temp=r3.w, fragColor=!r0.w
label L57
label L56
b L54
label L53
setp_eq p1.x, r7.w, c6.x	# srcAlpha=r7.w
bf L59, p1.x
setp_eq p1.x, r10.x, c2.x	# operandAlpha=r10.x
bf L61, p1.x
mov r3.w, r1.w	# temp=r3.w, prevColor=r1.w
b L62
label L61
setp_eq p1.x, r10.x, c3.x	# operandAlpha=r10.x
bf L63, p1.x
add r3.w, c4.x, !r1.w	# temp=r3.w, prevColor=!r1.w
label L63
label L62
label L59
label L54
label L48
label L42
mov r0.xyzw, r3.xyzw	# PARAM=r0.xyzw, temp=r3.xyzw
label tex_arg_calculation_vf4_vf4_vf4_vf4_i1_i1_i1_i1_end
ret
label tex_env_combiner_vf4_vf4_vf4_i1_
setp_eq p1.x, r10.y, c0.x	# nth=r10.y
bf L65, p1.x
mov r2.y, c12.x	# srcrgb=r2.y
mov r2.z, c18.x	# opndrgb=r2.z
mov r2.w, c15.x	# srcAlpha=r2.w
mov r5.x, c21.x	# operandAlpha=r5.x
mov r6.xyzw, c8.xyzw	# EnvColor=r6.xyzw
mov r5.y, c24.x	# rgbScale=r5.y
mov r5.z, c25.x	# alphaScale=r5.z
mov r5.w, c10.x	# combrgb=r5.w
mov r7.x, c11.x	# combalpha=r7.x
b L66
label L65
setp_eq p1.x, r10.y, c2.x	# nth=r10.y
bf L67, p1.x
mov r2.y, c30.x	# srcrgb=r2.y
mov r2.z, c36.x	# opndrgb=r2.z
mov r2.w, c33.x	# srcAlpha=r2.w
mov r5.x, c39.x	# operandAlpha=r5.x
mov r6.xyzw, c26.xyzw	# EnvColor=r6.xyzw
mov r5.y, c42.x	# rgbScale=r5.y
mov r5.z, c43.x	# alphaScale=r5.z
mov r5.w, c28.x	# combrgb=r5.w
mov r7.x, c29.x	# combalpha=r7.x
label L67
label L66
mov r0.xyzw, r11.xyzw	# PARAM=r0.xyzw, fragColor=r11.xyzw
mov r1.xyzw, r12.xyzw	# PARAM=r1.xyzw, prevColor=r12.xyzw
mov r8.xyzw, r13.xyzw	# PARAM=r8.xyzw, srcColor=r13.xyzw
mov r9.xyzw, r6.xyzw	# PARAM=r9.xyzw, EnvColor=r6.xyzw
mov r7.y, r2.y	# PARAM=r7.y, srcrgb=r2.y
mov r7.z, r2.z	# PARAM=r7.z, opndrgb=r2.z
mov r7.w, r2.w	# PARAM=r7.w, srcAlpha=r2.w
mov r10.x, r5.x	# PARAM=r10.x, operandAlpha=r5.x
call tex_arg_calculation_vf4_vf4_vf4_vf4_i1_i1_i1_i1_
mov r14.xyzw, r0.xyzw
setp_eq p1.x, r10.y, c0.x	# nth=r10.y
bf L69, p1.x
mov r2.y, c13.x	# srcrgb=r2.y
mov r2.z, c19.x	# opndrgb=r2.z
mov r2.w, c16.x	# srcAlpha=r2.w
mov r5.x, c22.x	# operandAlpha=r5.x
b L70
label L69
setp_eq p1.x, r10.y, c2.x	# nth=r10.y
bf L71, p1.x
mov r2.y, c31.x	# srcrgb=r2.y
mov r2.z, c37.x	# opndrgb=r2.z
mov r2.w, c34.x	# srcAlpha=r2.w
mov r5.x, c40.x	# operandAlpha=r5.x
label L71
label L70
mov r0.xyzw, r11.xyzw	# PARAM=r0.xyzw, fragColor=r11.xyzw
mov r1.xyzw, r12.xyzw	# PARAM=r1.xyzw, prevColor=r12.xyzw
mov r8.xyzw, r13.xyzw	# PARAM=r8.xyzw, srcColor=r13.xyzw
mov r9.xyzw, r6.xyzw	# PARAM=r9.xyzw, EnvColor=r6.xyzw
mov r7.y, r2.y	# PARAM=r7.y, srcrgb=r2.y
mov r7.z, r2.z	# PARAM=r7.z, opndrgb=r2.z
mov r7.w, r2.w	# PARAM=r7.w, srcAlpha=r2.w
mov r10.x, r5.x	# PARAM=r10.x, operandAlpha=r5.x
call tex_arg_calculation_vf4_vf4_vf4_vf4_i1_i1_i1_i1_
mov r15.xyzw, r0.xyzw
setp_eq p1.x, r10.y, c0.x	# nth=r10.y
bf L73, p1.x
mov r2.y, c14.x	# srcrgb=r2.y
mov r2.z, c20.x	# opndrgb=r2.z
mov r2.w, c17.x	# srcAlpha=r2.w
mov r5.x, c23.x	# operandAlpha=r5.x
b L74
label L73
setp_eq p1.x, r10.y, c2.x	# nth=r10.y
bf L75, p1.x
mov r2.y, c32.x	# srcrgb=r2.y
mov r2.z, c38.x	# opndrgb=r2.z
mov r2.w, c35.x	# srcAlpha=r2.w
mov r5.x, c41.x	# operandAlpha=r5.x
label L75
label L74
mov r0.xyzw, r11.xyzw	# PARAM=r0.xyzw, fragColor=r11.xyzw
mov r1.xyzw, r12.xyzw	# PARAM=r1.xyzw, prevColor=r12.xyzw
mov r8.xyzw, r13.xyzw	# PARAM=r8.xyzw, srcColor=r13.xyzw
mov r9.xyzw, r6.xyzw	# PARAM=r9.xyzw, EnvColor=r6.xyzw
mov r7.y, r2.y	# PARAM=r7.y, srcrgb=r2.y
mov r7.z, r2.z	# PARAM=r7.z, opndrgb=r2.z
mov r7.w, r2.w	# PARAM=r7.w, srcAlpha=r2.w
mov r10.x, r5.x	# PARAM=r10.x, operandAlpha=r5.x
call tex_arg_calculation_vf4_vf4_vf4_vf4_i1_i1_i1_i1_
setp_eq p1.x, r5.w, c3.x	# combrgb=r5.w
bf L77, p1.x
mov r4.xyz, r14.xyz	# c=r4.xyz
b L78
label L77
setp_eq p1.x, r5.w, c2.x	# combrgb=r5.w
bf L79, p1.x
mul r4.xyz, r14.xyz, r15.xyz	# c=r4.xyz
b L80
label L79
setp_eq p1.x, r5.w, c7.x	# combrgb=r5.w
bf L81, p1.x
add r4.xyz, r14.xyz, r15.xyz	# c=r4.xyz
b L82
label L81
setp_eq p1.x, r5.w, c5.x	# combrgb=r5.w
bf L83, p1.x
add r7.yzw, r14.xxyz, r15.xxyz
add r4.xyz, r7.yzw, !c44.xxx	# c=r4.xyz
b L84
label L83
setp_eq p1.x, r5.w, c6.x	# combrgb=r5.w
bf L85, p1.x
add r7.yzw, c1.xxyz, !r0.xxyz
mul r7.yzw, r7.xyzw, r15.xxyz
mad r7.yzw, r14.xxyz, r0.xxyz, r7.xyzw
mov r4.xyz, r7.yzw	# c=r4.xyz
b L86
label L85
setp_eq p1.x, r5.w, c45.x	# combrgb=r5.w
bf L87, p1.x
add r4.xyz, r14.xyz, !r15.xyz	# c=r4.xyz
b L88
label L87
setp_eq p0.x, r5.w, c46.x	# combrgb=r5.w
p0.x mov r7.y, c62.x
!p0.x mov r7.y, c62.y
setp_eq p0.x, r5.w, c47.x	# combrgb=r5.w
p0.x mov r7.z, c63.x
!p0.x mov r7.z, c63.y
add_sat r7.z, r7.y, r7.z
setp_eq p1.x, c1.x, r7.z
bf L89, p1.x
add r7.y, r14.x, !c44.x
add r7.z, r15.x, !c44.x
mul r7.z, r7.y, r7.z
add r7.y, r14.y, !c44.x
add r7.w, r15.y, !c44.x
mov r8.x, r7.z
mad r8.x, r7.y, r7.w, r8.x
add r8.y, r14.z, !c44.x
add r8.z, r15.z, !c44.x
mov r9.x, r8.x
mad r9.x, r8.y, r8.z, r9.x
mul r9.x, c48.x, r9.x	# d=r9.x
mov r4.xyzw, r9.xxxx	# c=r4.xyzw, d=r9.xxxx
label L89
label L88
label L86
label L84
label L82
label L80
label L78
mul r4.xyz, r5.yyy, r4.xyz	# c=r4.xyz, rgbScale=r5.yyy, c=r4.xyz
setp_ne p1.x, r5.w, c47.x	# combrgb=r5.w
bf L91, p1.x
setp_eq p1.x, r7.x, c3.x	# combalpha=r7.x
bf L93, p1.x
mov r4.w, r14.w	# c=r4.w
b L94
label L93
setp_eq p1.x, r7.x, c2.x	# combalpha=r7.x
bf L95, p1.x
mul r4.w, r14.w, r15.w	# c=r4.w
b L96
label L95
setp_eq p1.x, r7.x, c7.x	# combalpha=r7.x
bf L97, p1.x
add r4.w, r14.w, r15.w	# c=r4.w
b L98
label L97
setp_eq p1.x, r7.x, c5.x	# combalpha=r7.x
bf L99, p1.x
add r7.y, r14.w, r15.w
add r4.w, r7.y, !c44.x	# c=r4.w
b L100
label L99
setp_eq p1.x, r7.x, c6.x	# combalpha=r7.x
bf L101, p1.x
add r7.y, c1.x, !r0.w
mul r7.y, r7.y, r15.w
mad r7.y, r14.w, r0.w, r7.y
mov r4.w, r7.y	# c=r4.w
b L102
label L101
setp_eq p1.x, r7.x, c45.x	# combalpha=r7.x
bf L103, p1.x
add r4.w, r14.w, !r15.w	# c=r4.w
label L103
label L102
label L100
label L98
label L96
label L94
label L91
mul r4.w, r5.z, r4.w	# c=r4.w, alphaScale=r5.z, c=r4.w
mov r0.xyzw, r4.xyzw	# PARAM=r0.xyzw, c=r4.xyzw
label tex_env_combiner_vf4_vf4_vf4_i1_end
ret
label start
label main_
mov r1.xyzw, v0.xyzw	# prevColor=r1.xyzw, fragColor=v0.xyzw
mov r7.y, c1.x
setp_eq p1.x, r7.y, c50.x
bf L105, p1.x
#mul r7.y, vPos.w, vPos.w
#mad r1.xyzw, vPos.yyxx, d1.wwww, d1.wwww
#mul r1.xyzw, r1.xyzw, c49.xyxy
#maxcomp r1.w, |r1.xyzw|
#mul r1.w, r1.w, r7.y
#log r1.w, r1.w
#mov r1.xyz, v1.xyz	# TexCoord0=v1.xyz
#texld r0.xyzw, r1.xyzw, s0	# TexImages0=s0
texld r0.xyzw, v1.xyzw, s0	# TexImages0=s0
mov r7.y, c9.x	# mode=c9.x
setp_eq p1.x, r7.y, c2.x
bf L107, p1.x
mul r1.xyzw, v0.xyzw, r0.xyzw	# prevColor=r1.xyzw, prevColor=v0.xyzw, srcColor=r0.xyzw
b L108
label L107
mov r7.y, c9.x	# mode=c9.x
setp_eq p1.x, r7.y, c3.x
bf L109, p1.x
mov r1.xyzw, r0.xyzw	# prevColor=r1.xyzw, srcColor=r0.xyzw
b L110
label L109
mov r7.y, c9.x	# mode=c9.x
setp_eq p1.x, r7.y, c5.x
bf L111, p1.x
add r7.yzw, c1.xxyz, !r0.xwww	# srcColor=!r0.xwww
mul r7.yzw, r7.xyzw, v0.xxyz	# prevColor=v0.xxyz
mad r7.yzw, r0.xxyz, r0.xwww, r7.xyzw	# srcColor=r0.xxyz, srcColor=r0.xwww
mov r1.xyz, r7.yzw	# prevColor=r1.xyz
mov r1.w, v0.w	# prevColor=r1.w, prevColor=v0.w
b L112
label L111
mov r7.y, c9.x	# mode=c9.x
setp_eq p1.x, r7.y, c6.x
bf L113, p1.x
add r7.yzw, c1.xxyz, !r0.xxyz	# srcColor=!r0.xxyz
mul r7.yzw, r7.xyzw, v0.xxyz	# prevColor=v0.xxyz
mad r7.yzw, c8.xxyz, r0.xxyz, r7.xyzw	# srcColor=r0.xxyz
mov r1.xyz, r7.yzw	# prevColor=r1.xyz
mul r7.y, v0.w, r0.w	# prevColor=v0.w, srcColor=r0.w
mov r1.w, r7.y	# prevColor=r1.w
b L114
label L113
mov r7.y, c9.x	# mode=c9.x
setp_eq p1.x, r7.y, c7.x
bf L115, p1.x
add r7.yzw, v0.xxyz, r0.xxyz	# prevColor=v0.xxyz, srcColor=r0.xxyz
mov r1.xyz, r7.yzw	# prevColor=r1.xyz
mul r7.y, v0.w, r0.w	# prevColor=v0.w, srcColor=r0.w
mov r1.w, r7.y	# prevColor=r1.w
b L116
label L115
mov r7.y, c9.x	# mode=c9.x
setp_eq p1.x, r7.y, c45.x
bf L117, p1.x
mov r11.xyzw, v0.xyzw	# PARAM=r11.xyzw, fragColor=v0.xyzw
mov r12.xyzw, v0.xyzw	# PARAM=r12.xyzw, prevColor=v0.xyzw
mov r13.xyzw, r0.xyzw	# PARAM=r13.xyzw, srcColor=r0.xyzw
mov r10.y, c0.x	# PARAM=r10.y
call tex_env_combiner_vf4_vf4_vf4_i1_
mov r1.xyzw, r0.xyzw	# prevColor=r1.xyzw
b L118
label L117
mul r1.xyzw, v0.xyzw, r0.xyzw	# prevColor=r1.xyzw, prevColor=v0.xyzw, srcColor=r0.xyzw
label L118
label L116
label L114
label L112
label L110
label L108
mov_sat r0.xyzw, r1.xyzw	# prevColor=r1.xyzw
mov r1.xyzw, r0.xyzw	# prevColor=r1.xyzw
label L105
mov r7.y, c1.x
setp_eq p1.x, r7.y, c51.x
bf L119, p1.x
#mul r7.y, vPos.w, vPos.w
#mad r8.xyzw, vPos.yyxx, d2.wwww, d2.wwww
#mul r8.xyzw, r8.xyzw, c53.xyxy
#maxcomp r8.w, |r8.xyzw|
#mul r8.w, r8.w, r7.y
#log r8.w, r8.w
#mov r8.xyz, v2.xyz	# TexCoord1=v2.xyz
#texld r0.xyzw, r8.xyzw, s1	# TexImages1=s1
texld r0.xyzw, v2.xyzw, s1	# TexImages1=s1
mov r7.y, c27.x	# mode=c27.x
setp_eq p1.x, r7.y, c2.x
bf L121, p1.x
mul r1.xyzw, r1.xyzw, r0.xyzw	# prevColor=r1.xyzw, prevColor=r1.xyzw, srcColor=r0.xyzw
b L122
label L121
mov r7.y, c27.x	# mode=c27.x
setp_eq p1.x, r7.y, c3.x
bf L123, p1.x
mov r1.xyzw, r0.xyzw	# prevColor=r1.xyzw, srcColor=r0.xyzw
b L124
label L123
mov r7.y, c27.x	# mode=c27.x
setp_eq p1.x, r7.y, c5.x
bf L125, p1.x
add r2.yzw, c1.xxyz, !r0.xwww	# srcColor=!r0.xwww
mul r2.yzw, r2.xyzw, r1.xxyz	# prevColor=r1.xxyz
mov r3.xyz, r2.yzw
mad r2.yzw, r0.xxyz, r0.xwww, r3.xxyz	# srcColor=r0.xxyz, srcColor=r0.xwww
mov r1.xyz, r2.yzw	# prevColor=r1.xyz
mov r1.w, r1.w	# prevColor=r1.w, prevColor=r1.w
b L126
label L125
mov r7.y, c27.x	# mode=c27.x
setp_eq p1.x, r7.y, c6.x
bf L127, p1.x
add r2.yzw, c1.xxyz, !r0.xxyz	# srcColor=!r0.xxyz
mul r2.yzw, r2.xyzw, r1.xxyz	# prevColor=r1.xxyz
mad r2.yzw, c8.xxyz, r0.xxyz, r2.xyzw	# srcColor=r0.xxyz
mov r1.xyz, r2.yzw	# prevColor=r1.xyz
mul r2.y, r1.w, r0.w	# prevColor=r1.w, srcColor=r0.w
mov r1.w, r2.y	# prevColor=r1.w
b L128
label L127
mov r7.y, c27.x	# mode=c27.x
setp_eq p1.x, r7.y, c7.x
bf L129, p1.x
add r2.yzw, r1.xxyz, r0.xxyz	# prevColor=r1.xxyz, srcColor=r0.xxyz
mov r1.xyz, r2.yzw	# prevColor=r1.xyz
mul r2.y, r1.w, r0.w	# prevColor=r1.w, srcColor=r0.w
mov r1.w, r2.y	# prevColor=r1.w
b L130
label L129
mov r7.y, c27.x	# mode=c27.x
setp_eq p1.x, r7.y, c45.x
bf L131, p1.x
mov r11.xyzw, v0.xyzw	# PARAM=r11.xyzw, fragColor=v0.xyzw
mov r12.xyzw, r1.xyzw	# PARAM=r12.xyzw, prevColor=r1.xyzw
mov r13.xyzw, r0.xyzw	# PARAM=r13.xyzw, srcColor=r0.xyzw
mov r10.y, c2.x	# PARAM=r10.y
call tex_env_combiner_vf4_vf4_vf4_i1_
mov r1.xyzw, r0.xyzw	# prevColor=r1.xyzw
b L132
label L131
mul r1.xyzw, r1.xyzw, r0.xyzw	# prevColor=r1.xyzw, prevColor=r1.xyzw, srcColor=r0.xyzw
label L132
label L130
label L128
label L126
label L124
label L122
mov_sat r0.xyzw, r1.xyzw	# prevColor=r1.xyzw
mov r1.xyzw, r0.xyzw	# prevColor=r1.xyzw
label L119
mov r0.xyzw, r1.xyzw	# fragColor=r0.xyzw, prevColor=r1.xyzw
mov r2.y, c1.x
setp_eq p1.x, r2.y, c55.x	# FogEnabled=c55.x
bf L133, p1.x
mov r2.y, c61.x
setp_eq p1.x, r2.y, c2.x
bf L135, p1.x
add r2.y, c58.x, !v3.x	# FogFragCoord=!v3.x
mul r2.y, r2.y, c60.x
mov_sat r2.y, r2.y
mov r2.x, r2.y	# fog=r2.x
b L136
label L135
mov r2.y, c61.x
setp_eq p1.x, r2.y, c3.x
bf L137, p1.x
mul r2.y, !c56.x, v3.x	# FogFragCoord=v3.x
mul r2.y, r2.y, c54.x
exp r2.y, r2.y
mov_sat r2.y, r2.y
mov r2.x, r2.y	# fog=r2.x
b L138
label L137
mov r2.y, c61.x
setp_eq p1.x, r2.y, c5.x
bf L139, p1.x
mul r2.y, !c56.x, c56.x
mul r2.y, r2.y, v3.x	# FogFragCoord=v3.x
mul r2.y, r2.y, v3.x	# FogFragCoord=v3.x
mul r2.y, r2.y, c54.x
exp r2.y, r2.y
mov_sat r2.y, r2.y
mov r2.x, r2.y	# fog=r2.x
label L139
label L138
label L136
add r3.xyzw, c1.xyzw, !r2.xxxx	# fog=!r2.xxxx
mul r3.xyzw, r3.xyzw, c59.xyzw
mad r3.xyzw, r1.xyzw, r2.xxxx, r3.xyzw	# fragColor=r1.xyzw, fog=r2.xxxx
mov r0.xyzw, r3.xyzw	# fragColor=r0.xyzw
label L133
label main_end
mov_sat oColor.xyzw, r0.xyzw
ret
# 438 instructions, 65 C regs, 16 R regs 
