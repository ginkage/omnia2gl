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

char *enumName(GLenum attr)
{
	static char unk[16];
	switch (attr) {
	case 0x0000: return "POINTS";
	case 0x0001: return "LINES";
	case 0x0002: return "LINE_LOOP";
	case 0x0003: return "LINE_STRIP";
	case 0x0004: return "TRIANGLES";
	case 0x0005: return "TRIANGLE_STRIP";
	case 0x0006: return "TRIANGLE_FAN";
	case 0x0104: return "ADD";
	case 0x0200: return "NEVER";
	case 0x0201: return "LESS";
	case 0x0202: return "EQUAL";
	case 0x0203: return "LEQUAL";
	case 0x0204: return "GREATER";
	case 0x0205: return "NOTEQUAL";
	case 0x0206: return "GEQUAL";
	case 0x0207: return "ALWAYS";
	case 0x0300: return "SRC_COLOR";
	case 0x0301: return "ONE_MINUS_SRC_COLOR";
	case 0x0302: return "SRC_ALPHA";
	case 0x0303: return "ONE_MINUS_SRC_ALPHA";
	case 0x0304: return "DST_ALPHA";
	case 0x0305: return "ONE_MINUS_DST_ALPHA";
	case 0x0306: return "DST_COLOR";
	case 0x0307: return "ONE_MINUS_DST_COLOR";
	case 0x0308: return "SRC_ALPHA_SATURATE";
	case 0x0404: return "FRONT";
	case 0x0405: return "BACK";
	case 0x0408: return "FRONT_AND_BACK";
	case 0x0500: return "INVALID_ENUM";
	case 0x0501: return "INVALID_VALUE";
	case 0x0502: return "INVALID_OPERATION";
	case 0x0503: return "STACK_OVERFLOW";
	case 0x0504: return "STACK_UNDERFLOW";
	case 0x0505: return "OUT_OF_MEMORY";
	case 0x0506: return "INVALID_FRAMEBUFFER_OPERATION";
	case 0x0601: return "NO_IMAGECHUNK_SLSI";
	case 0x0602: return "NO_TEXOBJCHUNK_SLSI";
	case 0x0603: return "NOTHING_SLSI";
	case 0x0800: return "EXP";
	case 0x0801: return "EXP2";
	case 0x0900: return "CW";
	case 0x0901: return "CCW";
	case 0x0B00: return "CURRENT_COLOR";
	case 0x0B02: return "CURRENT_NORMAL";
	case 0x0B03: return "CURRENT_TEXTURE_COORDS";
	case 0x0B10: return "POINT_SMOOTH";
	case 0x0B11: return "POINT_SIZE";
	case 0x0B12: return "SMOOTH_POINT_SIZE_RANGE";
	case 0x0B20: return "LINE_SMOOTH";
	case 0x0B21: return "LINE_WIDTH";
	case 0x0B22: return "SMOOTH_LINE_WIDTH_RANGE";
	case 0x0B44: return "CULL_FACE";
	case 0x0B45: return "CULL_FACE_MODE";
	case 0x0B46: return "FRONT_FACE";
	case 0x0B50: return "LIGHTING";
	case 0x0B52: return "LIGHT_MODEL_TWO_SIDE";
	case 0x0B53: return "LIGHT_MODEL_AMBIENT";
	case 0x0B54: return "SHADE_MODEL";
	case 0x0B57: return "COLOR_MATERIAL";
	case 0x0B60: return "FOG";
	case 0x0B62: return "FOG_DENSITY";
	case 0x0B63: return "FOG_START";
	case 0x0B64: return "FOG_END";
	case 0x0B65: return "FOG_MODE";
	case 0x0B66: return "FOG_COLOR";
	case 0x0B70: return "DEPTH_RANGE";
	case 0x0B71: return "DEPTH_TEST";
	case 0x0B72: return "DEPTH_WRITEMASK";
	case 0x0B73: return "DEPTH_CLEAR_VALUE";
	case 0x0B74: return "DEPTH_FUNC";
	case 0x0B90: return "STENCIL_TEST";
	case 0x0B91: return "STENCIL_CLEAR_VALUE";
	case 0x0B92: return "STENCIL_FUNC";
	case 0x0B93: return "STENCIL_VALUE_MASK";
	case 0x0B94: return "STENCIL_FAIL";
	case 0x0B95: return "STENCIL_PASS_DEPTH_FAIL";
	case 0x0B96: return "STENCIL_PASS_DEPTH_PASS";
	case 0x0B97: return "STENCIL_REF";
	case 0x0B98: return "STENCIL_WRITEMASK";
	case 0x0BA0: return "MATRIX_MODE";
	case 0x0BA1: return "NORMALIZE";
	case 0x0BA2: return "VIEWPORT";
	case 0x0BA3: return "MODELVIEW_STACK_DEPTH";
	case 0x0BA4: return "PROJECTION_STACK_DEPTH";
	case 0x0BA5: return "TEXTURE_STACK_DEPTH";
	case 0x0BA6: return "MODELVIEW_MATRIX";
	case 0x0BA7: return "PROJECTION_MATRIX";
	case 0x0BA8: return "TEXTURE_MATRIX";
	case 0x0BC0: return "ALPHA_TEST";
	case 0x0BC1: return "ALPHA_TEST_FUNC";
	case 0x0BC2: return "ALPHA_TEST_REF";
	case 0x0BD0: return "DITHER";
	case 0x0BE0: return "BLEND_DST";
	case 0x0BE1: return "BLEND_SRC";
	case 0x0BE2: return "BLEND";
	case 0x0BF0: return "LOGIC_OP_MODE";
	case 0x0BF2: return "COLOR_LOGIC_OP";
	case 0x0C10: return "SCISSOR_BOX";
	case 0x0C11: return "SCISSOR_TEST";
	case 0x0C22: return "COLOR_CLEAR_VALUE";
	case 0x0C23: return "COLOR_WRITEMASK";
	case 0x0C50: return "PERSPECTIVE_CORRECTION_HINT";
	case 0x0C51: return "POINT_SMOOTH_HINT";
	case 0x0C52: return "LINE_SMOOTH_HINT";
	case 0x0C53: return "POLYGON_SMOOTH_HINT";
	case 0x0C54: return "FOG_HINT";
	case 0x0CF5: return "UNPACK_ALIGNMENT";
	case 0x0D05: return "PACK_ALIGNMENT";
	case 0x0D1C: return "ALPHA_SCALE";
	case 0x0D31: return "MAX_LIGHTS";
	case 0x0D32: return "MAX_CLIP_PLANES";
	case 0x0D33: return "MAX_TEXTURE_SIZE";
	case 0x0D36: return "MAX_MODELVIEW_STACK_DEPTH";
	case 0x0D38: return "MAX_PROJECTION_STACK_DEPTH";
	case 0x0D39: return "MAX_TEXTURE_STACK_DEPTH";
	case 0x0D3A: return "MAX_VIEWPORT_DIMS";
	case 0x0D50: return "SUBPIXEL_BITS";
	case 0x0D52: return "RED_BITS";
	case 0x0D53: return "GREEN_BITS";
	case 0x0D54: return "BLUE_BITS";
	case 0x0D55: return "ALPHA_BITS";
	case 0x0D56: return "DEPTH_BITS";
	case 0x0D57: return "STENCIL_BITS";
	case 0x0DE1: return "TEXTURE_2D";
	case 0x1100: return "DONT_CARE";
	case 0x1101: return "FASTEST";
	case 0x1102: return "NICEST";
	case 0x1200: return "AMBIENT";
	case 0x1201: return "DIFFUSE";
	case 0x1202: return "SPECULAR";
	case 0x1203: return "POSITION";
	case 0x1204: return "SPOT_DIRECTION";
	case 0x1205: return "SPOT_EXPONENT";
	case 0x1206: return "SPOT_CUTOFF";
	case 0x1207: return "CONSTANT_ATTENUATION";
	case 0x1208: return "LINEAR_ATTENUATION";
	case 0x1209: return "QUADRATIC_ATTENUATION";
	case 0x1400: return "BYTE";
	case 0x1401: return "UNSIGNED_BYTE";
	case 0x1402: return "SHORT";
	case 0x1403: return "UNSIGNED_SHORT";
	case 0x1404: return "INT";
	case 0x1405: return "UNSIGNED_INT";
	case 0x1406: return "FLOAT";
	case 0x140C: return "FIXED";
	case 0x1500: return "CLEAR";
	case 0x1501: return "AND";
	case 0x1502: return "AND_REVERSE";
	case 0x1503: return "COPY";
	case 0x1504: return "AND_INVERTED";
	case 0x1505: return "NOOP";
	case 0x1506: return "XOR";
	case 0x1507: return "OR";
	case 0x1508: return "NOR";
	case 0x1509: return "EQUIV";
	case 0x150A: return "INVERT";
	case 0x150B: return "OR_REVERSE";
	case 0x150C: return "COPY_INVERTED";
	case 0x150D: return "OR_INVERTED";
	case 0x150E: return "NAND";
	case 0x150F: return "SET";
	case 0x1600: return "EMISSION";
	case 0x1601: return "SHININESS";
	case 0x1602: return "AMBIENT_AND_DIFFUSE";
	case 0x1700: return "MODELVIEW";
	case 0x1701: return "PROJECTION";
	case 0x1702: return "TEXTURE";
	case 0x1800: return "COLOR_EXT";
	case 0x1801: return "DEPTH_EXT";
	case 0x1802: return "STENCIL_EXT";
	case 0x1906: return "ALPHA";
	case 0x1907: return "RGB";
	case 0x1908: return "RGBA";
	case 0x1909: return "LUMINANCE";
	case 0x190A: return "LUMINANCE_ALPHA";
	case 0x190B: return "BGRA";
	case 0x1911: return "PVRTC2_OES";
	case 0x1912: return "PVRTC4_OES";
	case 0x1D00: return "FLAT";
	case 0x1D01: return "SMOOTH";
	case 0x1E00: return "KEEP";
	case 0x1E01: return "REPLACE";
	case 0x1E02: return "INCR";
	case 0x1E03: return "DECR";
	case 0x1F00: return "VENDOR";
	case 0x1F01: return "RENDERER";
	case 0x1F02: return "VERSION";
	case 0x1F03: return "EXTENSIONS";
	case 0x2100: return "MODULATE";
	case 0x2101: return "DECAL";
	case 0x2200: return "TEXTURE_ENV_MODE";
	case 0x2201: return "TEXTURE_ENV_COLOR";
	case 0x2300: return "TEXTURE_ENV";
	case 0x2600: return "NEAREST";
	case 0x2601: return "LINEAR";
	case 0x2700: return "NEAREST_MIPMAP_NEAREST";
	case 0x2701: return "LINEAR_MIPMAP_NEAREST";
	case 0x2702: return "NEAREST_MIPMAP_LINEAR";
	case 0x2703: return "LINEAR_MIPMAP_LINEAR";
	case 0x2800: return "TEXTURE_MAG_FILTER";
	case 0x2801: return "TEXTURE_MIN_FILTER";
	case 0x2802: return "TEXTURE_WRAP_S";
	case 0x2803: return "TEXTURE_WRAP_T";
	case 0x2901: return "REPEAT";
	case 0x2A00: return "POLYGON_OFFSET_UNITS";
	case 0x3000: return "SUCCESS";
	case 0x3001: return "NOT_INITIALIZED";
	case 0x3002: return "BAD_ACCESS";
	case 0x3003: return "BAD_ALLOC";
	case 0x3004: return "BAD_ATTRIBUTE";
	case 0x3005: return "BAD_CONFIG";
	case 0x3006: return "BAD_CONTEXT";
	case 0x3007: return "BAD_CURRENT_SURFACE";
	case 0x3008: return "BAD_DISPLAY";
	case 0x3009: return "BAD_MATCH";
	case 0x300A: return "BAD_NATIVE_PIXMAP";
	case 0x300B: return "BAD_NATIVE_WINDOW";
	case 0x300C: return "BAD_PARAMETER";
	case 0x300D: return "BAD_SURFACE";
	case 0x300E: return "CONTEXT_LOST";
	case 0x3020: return "BUFFER_SIZE";
	case 0x3021: return "ALPHA_SIZE";
	case 0x3022: return "BLUE_SIZE";
	case 0x3023: return "GREEN_SIZE";
	case 0x3024: return "RED_SIZE";
	case 0x3025: return "DEPTH_SIZE";
	case 0x3026: return "STENCIL_SIZE";
	case 0x3027: return "CONFIG_CAVEAT";
	case 0x3028: return "CONFIG_ID";
	case 0x3029: return "LEVEL";
	case 0x302A: return "MAX_PBUFFER_HEIGHT";
	case 0x302B: return "MAX_PBUFFER_PIXELS";
	case 0x302C: return "MAX_PBUFFER_WIDTH";
	case 0x302D: return "NATIVE_RENDERABLE";
	case 0x302E: return "NATIVE_VISUAL_ID";
	case 0x302F: return "NATIVE_VISUAL_TYPE";
	case 0x3030: return "PRESERVED_RESOURCES";
	case 0x3031: return "SAMPLES";
	case 0x3032: return "SAMPLE_BUFFERS";
	case 0x3033: return "SURFACE_TYPE";
	case 0x3034: return "TRANSPARENT_TYPE";
	case 0x3035: return "TRANSPARENT_BLUE_VALUE";
	case 0x3036: return "TRANSPARENT_GREEN_VALUE";
	case 0x3037: return "TRANSPARENT_RED_VALUE";
	case 0x3038: return "NONE";
	case 0x3039: return "BIND_TO_TEXTURE_RGB";
	case 0x303A: return "BIND_TO_TEXTURE_RGBA";
	case 0x303B: return "MIN_SWAP_INTERVAL";
	case 0x303C: return "MAX_SWAP_INTERVAL";
	case 0x303D: return "LUMINANCE_SIZE";
	case 0x303E: return "ALPHA_MASK_SIZE";
	case 0x303F: return "COLOR_BUFFER_TYPE";
	case 0x3040: return "RENDERABLE_TYPE";
	case 0x3041: return "MATCH_NATIVE_PIXMAP";
	case 0x3042: return "CONFORMANT";
	case 0x3050: return "SLOW_CONFIG";
	case 0x3051: return "NON_CONFORMANT_CONFIG";
	case 0x3052: return "TRANSPARENT_RGB";
	case 0x3053: return "VENDOR";
	case 0x3054: return "VERSION";
	case 0x3055: return "EXTENSIONS";
	case 0x3056: return "HEIGHT";
	case 0x3057: return "WIDTH";
	case 0x3058: return "LARGEST_PBUFFER";
	case 0x3059: return "DRAW";
	case 0x305A: return "READ";
	case 0x305B: return "CORE_NATIVE_ENGINE";
	case 0x305C: return "NO_TEXTURE";
	case 0x305D: return "TEXTURE_RGB";
	case 0x305E: return "TEXTURE_RGBA";
	case 0x305F: return "TEXTURE_2D";
	case 0x3080: return "TEXTURE_FORMAT";
	case 0x3081: return "TEXTURE_TARGET";
	case 0x3082: return "MIPMAP_TEXTURE";
	case 0x3083: return "MIPMAP_LEVEL";
	case 0x3084: return "BACK_BUFFER";
	case 0x3085: return "SINGLE_BUFFER";
	case 0x3086: return "RENDER_BUFFER";
	case 0x3087: return "VG_COLORSPACE";
	case 0x3088: return "VG_ALPHA_FORMAT";
	case 0x3089: return "VG_COLORSPACE_sRGB";
	case 0x308A: return "VG_COLORSPACE_LINEAR";
	case 0x308B: return "VG_ALPHA_FORMAT_NONPRE";
	case 0x308C: return "VG_ALPHA_FORMAT_PRE";
	case 0x308D: return "CLIENT_APIS";
	case 0x308E: return "RGB_BUFFER";
	case 0x308F: return "LUMINANCE_BUFFER";
	case 0x3090: return "HORIZONTAL_RESOLUTION";
	case 0x3091: return "VERTICAL_RESOLUTION";
	case 0x3092: return "PIXEL_ASPECT_RATIO";
	case 0x3093: return "SWAP_BEHAVIOR";
	case 0x3094: return "BUFFER_PRESERVED";
	case 0x3095: return "BUFFER_DESTROYED";
	case 0x3096: return "OPENVG_IMAGE";
	case 0x3097: return "CONTEXT_CLIENT_TYPE";
	case 0x3098: return "CONTEXT_CLIENT_VERSION";
	case 0x30A0: return "OPENGL_ES_API";
	case 0x30A1: return "OPENVG_API";
	case 0x4000: return "LIGHT0";
	case 0x4001: return "LIGHT1";
	case 0x4002: return "LIGHT2";
	case 0x4003: return "LIGHT3";
	case 0x4004: return "LIGHT4";
	case 0x4005: return "LIGHT5";
	case 0x4006: return "LIGHT6";
	case 0x4007: return "LIGHT7";
	case 0x8001: return "CONSTANT_COLOR";
	case 0x8002: return "ONE_MINUS_CONSTANT_COLOR";
	case 0x8003: return "CONSTANT_ALPHA";
	case 0x8004: return "ONE_MINUS_CONSTANT_ALPHA";
	case 0x8005: return "BLEND_COLOR";
	case 0x8006: return "FUNC_ADD";
	case 0x8007: return "MIN_EXT";
	case 0x8008: return "MAX_EXT";
	case 0x8009: return "BLEND_EQUATION";
	case 0x800A: return "FUNC_SUBTRACT";
	case 0x800B: return "FUNC_REVERSE_SUBTRACT";
	case 0x8033: return "UNSIGNED_SHORT_4_4_4_4";
	case 0x8034: return "UNSIGNED_SHORT_5_5_5_1";
	case 0x8037: return "POLYGON_OFFSET_FILL";
	case 0x8038: return "POLYGON_OFFSET_FACTOR";
	case 0x803A: return "RESCALE_NORMAL";
	case 0x8051: return "RGB8";
	case 0x8056: return "RGBA4";
	case 0x8057: return "RGB5_A1";
	case 0x8058: return "RGBA8";
	case 0x8069: return "TEXTURE_BINDING_2D";
	case 0x806A: return "TEXTURE_BINDING_3D";
	case 0x806F: return "TEXTURE_3D";
	case 0x8072: return "TEXTURE_WRAP_R";
	case 0x8073: return "MAX_3D_TEXTURE_SIZE";
	case 0x8074: return "VERTEX_ARRAY";
	case 0x8075: return "NORMAL_ARRAY";
	case 0x8076: return "COLOR_ARRAY";
	case 0x8078: return "TEXTURE_COORD_ARRAY";
	case 0x807A: return "VERTEX_ARRAY_SIZE";
	case 0x807B: return "VERTEX_ARRAY_TYPE";
	case 0x807C: return "VERTEX_ARRAY_STRIDE";
	case 0x807E: return "NORMAL_ARRAY_TYPE";
	case 0x807F: return "NORMAL_ARRAY_STRIDE";
	case 0x8081: return "COLOR_ARRAY_SIZE";
	case 0x8082: return "COLOR_ARRAY_TYPE";
	case 0x8083: return "COLOR_ARRAY_STRIDE";
	case 0x8088: return "TEXTURE_COORD_ARRAY_SIZE";
	case 0x8089: return "TEXTURE_COORD_ARRAY_TYPE";
	case 0x808A: return "TEXTURE_COORD_ARRAY_STRIDE";
	case 0x808E: return "VERTEX_ARRAY_POINTER";
	case 0x808F: return "NORMAL_ARRAY_POINTER";
	case 0x8090: return "COLOR_ARRAY_POINTER";
	case 0x8092: return "TEXTURE_COORD_ARRAY_POINTER";
	case 0x809D: return "MULTISAMPLE";
	case 0x809E: return "SAMPLE_ALPHA_TO_COVERAGE";
	case 0x809F: return "SAMPLE_ALPHA_TO_ONE";
	case 0x80A0: return "SAMPLE_COVERAGE";
	case 0x80A8: return "SAMPLE_BUFFERS";
	case 0x80A9: return "SAMPLES";
	case 0x80AA: return "SAMPLE_COVERAGE_VALUE";
	case 0x80AB: return "SAMPLE_COVERAGE_INVERT";
	case 0x80C8: return "BLEND_DST_RGB";
	case 0x80C9: return "BLEND_SRC_RGB";
	case 0x80CA: return "BLEND_DST_ALPHA";
	case 0x80CB: return "BLEND_SRC_ALPHA";
	case 0x80E1: return "BGRA";
	case 0x80E8: return "MAX_ELEMENTS_VERTICES";
	case 0x80E9: return "MAX_ELEMENTS_INDICES";
	case 0x8126: return "POINT_SIZE_MIN";
	case 0x8127: return "POINT_SIZE_MAX";
	case 0x8128: return "POINT_FADE_THRESHOLD_SIZE";
	case 0x8129: return "POINT_DISTANCE_ATTENUATION";
	case 0x812F: return "CLAMP_TO_EDGE";
	case 0x8191: return "GENERATE_MIPMAP";
	case 0x8192: return "GENERATE_MIPMAP_HINT";
	case 0x81A5: return "DEPTH_COMPONENT16";
	case 0x81A6: return "DEPTH_COMPONENT24";
	case 0x81A7: return "DEPTH_COMPONENT32";
	case 0x8363: return "UNSIGNED_SHORT_5_6_5";
	case 0x8365: return "UNSIGNED_SHORT_4_4_4_4_REV";
	case 0x8366: return "UNSIGNED_SHORT_1_5_5_5_REV";
	case 0x8368: return "UNSIGNED_INT_2_10_10_10_REV_EXT";
	case 0x8370: return "MIRRORED_REPEAT";
	case 0x846D: return "ALIASED_POINT_SIZE_RANGE";
	case 0x846E: return "ALIASED_LINE_WIDTH_RANGE";
	case 0x84C0: return "TEXTURE0";
	case 0x84C1: return "TEXTURE1";
	case 0x84C2: return "TEXTURE2";
	case 0x84C3: return "TEXTURE3";
	case 0x84C4: return "TEXTURE4";
	case 0x84C5: return "TEXTURE5";
	case 0x84C6: return "TEXTURE6";
	case 0x84C7: return "TEXTURE7";
	case 0x84C8: return "TEXTURE8";
	case 0x84C9: return "TEXTURE9";
	case 0x84CA: return "TEXTURE10";
	case 0x84CB: return "TEXTURE11";
	case 0x84CC: return "TEXTURE12";
	case 0x84CD: return "TEXTURE13";
	case 0x84CE: return "TEXTURE14";
	case 0x84CF: return "TEXTURE15";
	case 0x84D0: return "TEXTURE16";
	case 0x84D1: return "TEXTURE17";
	case 0x84D2: return "TEXTURE18";
	case 0x84D3: return "TEXTURE19";
	case 0x84D4: return "TEXTURE20";
	case 0x84D5: return "TEXTURE21";
	case 0x84D6: return "TEXTURE22";
	case 0x84D7: return "TEXTURE23";
	case 0x84D8: return "TEXTURE24";
	case 0x84D9: return "TEXTURE25";
	case 0x84DA: return "TEXTURE26";
	case 0x84DB: return "TEXTURE27";
	case 0x84DC: return "TEXTURE28";
	case 0x84DD: return "TEXTURE29";
	case 0x84DE: return "TEXTURE30";
	case 0x84DF: return "TEXTURE31";
	case 0x84E0: return "ACTIVE_TEXTURE";
	case 0x84E1: return "CLIENT_ACTIVE_TEXTURE";
	case 0x84E2: return "MAX_TEXTURE_UNITS";
	case 0x84E7: return "SUBTRACT";
	case 0x84E8: return "MAX_RENDERBUFFER_SIZE";
	case 0x84F2: return "ALL_COMPLETED_NV";
	case 0x84F3: return "FENCE_STATUS_NV";
	case 0x84F4: return "FENCE_CONDITION_NV";
	case 0x84F9: return "DEPTH_STENCIL_OES";
	case 0x84FA: return "UNSIGNED_INT_24_8_OES";
	case 0x84FE: return "TEXTURE_MAX_ANISOTROPY_EXT";
	case 0x84FF: return "MAX_TEXTURE_MAX_ANISOTROPY_EXT";
	case 0x8507: return "INCR_WRAP";
	case 0x8508: return "DECR_WRAP";
	case 0x8513: return "TEXTURE_CUBE_MAP";
	case 0x8514: return "TEXTURE_BINDING_CUBE_MAP";
	case 0x8515: return "TEXTURE_CUBE_MAP_POSITIVE_X";
	case 0x8516: return "TEXTURE_CUBE_MAP_NEGATIVE_X";
	case 0x8517: return "TEXTURE_CUBE_MAP_POSITIVE_Y";
	case 0x8518: return "TEXTURE_CUBE_MAP_NEGATIVE_Y";
	case 0x8519: return "TEXTURE_CUBE_MAP_POSITIVE_Z";
	case 0x851A: return "TEXTURE_CUBE_MAP_NEGATIVE_Z";
	case 0x851C: return "MAX_CUBE_MAP_TEXTURE_SIZE";
	case 0x8570: return "COMBINE";
	case 0x8571: return "COMBINE_RGB";
	case 0x8572: return "COMBINE_ALPHA";
	case 0x8573: return "RGB_SCALE";
	case 0x8574: return "ADD_SIGNED";
	case 0x8575: return "INTERPOLATE";
	case 0x8576: return "CONSTANT";
	case 0x8577: return "PRIMARY_COLOR";
	case 0x8578: return "PREVIOUS";
	case 0x8580: return "SRC0_RGB";
	case 0x8581: return "SRC1_RGB";
	case 0x8582: return "SRC2_RGB";
	case 0x8588: return "SRC0_ALPHA";
	case 0x8589: return "SRC1_ALPHA";
	case 0x858A: return "SRC2_ALPHA";
	case 0x8590: return "OPERAND0_RGB";
	case 0x8591: return "OPERAND1_RGB";
	case 0x8592: return "OPERAND2_RGB";
	case 0x8598: return "OPERAND0_ALPHA";
	case 0x8599: return "OPERAND1_ALPHA";
	case 0x859A: return "OPERAND2_ALPHA";
	case 0x8622: return "VERTEX_ATTRIB_ARRAY_ENABLED";
	case 0x8623: return "VERTEX_ATTRIB_ARRAY_SIZE";
	case 0x8624: return "VERTEX_ATTRIB_ARRAY_STRIDE";
	case 0x8625: return "VERTEX_ATTRIB_ARRAY_TYPE";
	case 0x8626: return "CURRENT_VERTEX_ATTRIB";
	case 0x8642: return "VERTEX_PROGRAM_POINT_SIZE";
	case 0x8645: return "VERTEX_ATTRIB_ARRAY_POINTER";
	case 0x86A2: return "NUM_COMPRESSED_TEXTURE_FORMATS";
	case 0x86A3: return "COMPRESSED_TEXTURE_FORMATS";
	case 0x86A4: return "MAX_VERTEX_UNITS_OES";
	case 0x86A9: return "WEIGHT_ARRAY_TYPE_OES";
	case 0x86AA: return "WEIGHT_ARRAY_STRIDE_OES";
	case 0x86AB: return "WEIGHT_ARRAY_SIZE_OES";
	case 0x86AC: return "WEIGHT_ARRAY_POINTER_OES";
	case 0x86AD: return "WEIGHT_ARRAY_OES";
	case 0x86AE: return "DOT3_RGB";
	case 0x86AF: return "DOT3_RGBA";
	case 0x8740: return "Z400_BINARY_AMD";
	case 0x8741: return "PROGRAM_BINARY_LENGTH_OES";
	case 0x8764: return "BUFFER_SIZE";
	case 0x8765: return "BUFFER_USAGE";
	case 0x87EE: return "ATC_RGBA_INTERPOLATED_ALPHA_AMD";
	case 0x87F9: return "3DC_X_AMD";
	case 0x87FA: return "3DC_XY_AMD";
	case 0x87FE: return "NUM_PROGRAM_BINARY_FORMATS_OES";
	case 0x87FF: return "PROGRAM_BINARY_FORMATS_OES";
	case 0x8800: return "STENCIL_BACK_FUNC";
	case 0x8801: return "STENCIL_BACK_FAIL";
	case 0x8802: return "STENCIL_BACK_PASS_DEPTH_FAIL";
	case 0x8803: return "STENCIL_BACK_PASS_DEPTH_PASS";
	case 0x8823: return "WRITEONLY_RENDERING_AMD";
	case 0x883D: return "BLEND_EQUATION_ALPHA";
	case 0x8840: return "MATRIX_PALETTE_OES";
	case 0x8842: return "MAX_PALETTE_MATRICES_OES";
	case 0x8844: return "MATRIX_INDEX_ARRAY_OES";
	case 0x8846: return "MATRIX_INDEX_ARRAY_SIZE_OES";
	case 0x8847: return "MATRIX_INDEX_ARRAY_TYPE_OES";
	case 0x8848: return "MATRIX_INDEX_ARRAY_STRIDE_OES";
	case 0x8849: return "MATRIX_INDEX_ARRAY_POINTER_OES";
	case 0x8861: return "POINT_SPRITE_OES";
	case 0x8862: return "COORD_REPLACE_OES";
	case 0x8869: return "MAX_VERTEX_ATTRIBS";
	case 0x886A: return "VERTEX_ATTRIB_ARRAY_NORMALIZED";
	case 0x8872: return "MAX_TEXTURE_IMAGE_UNITS";
	case 0x8892: return "ARRAY_BUFFER";
	case 0x8893: return "ELEMENT_ARRAY_BUFFER";
	case 0x8894: return "ARRAY_BUFFER_BINDING";
	case 0x8895: return "ELEMENT_ARRAY_BUFFER_BINDING";
	case 0x8896: return "VERTEX_ARRAY_BUFFER_BINDING";
	case 0x8897: return "NORMAL_ARRAY_BUFFER_BINDING";
	case 0x8898: return "COLOR_ARRAY_BUFFER_BINDING";
	case 0x889A: return "TEXTURE_COORD_ARRAY_BUFFER_BINDING";
	case 0x889E: return "WEIGHT_ARRAY_BUFFER_BINDING_OES";
	case 0x889F: return "VERTEX_ATTRIB_ARRAY_BUFFER_BINDING";
	case 0x88B8: return "READ_ONLY";
	case 0x88B9: return "WRITE_ONLY";
	case 0x88BA: return "READ_WRITE";
	case 0x88BB: return "BUFFER_ACCESS";
	case 0x88BC: return "BUFFER_MAPPED";
	case 0x88BD: return "BUFFER_MAP_POINTER";
	case 0x88E0: return "STREAM_DRAW";
	case 0x88E4: return "STATIC_DRAW";
	case 0x88E8: return "DYNAMIC_DRAW";
	case 0x88F0: return "DEPTH24_STENCIL8_OES";
	case 0x898A: return "POINT_SIZE_ARRAY_TYPE_OES";
	case 0x898B: return "POINT_SIZE_ARRAY_STRIDE_OES";
	case 0x898C: return "POINT_SIZE_ARRAY_POINTER_OES";
	case 0x898D: return "MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES";
	case 0x898E: return "PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES";
	case 0x898F: return "TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES";
	case 0x8B30: return "FRAGMENT_SHADER";
	case 0x8B31: return "VERTEX_SHADER";
	case 0x8B4C: return "MAX_VERTEX_TEXTURE_IMAGE_UNITS";
	case 0x8B4D: return "MAX_COMBINED_TEXTURE_IMAGE_UNITS";
	case 0x8B4F: return "SHADER_TYPE";
	case 0x8B50: return "FLOAT_VEC2";
	case 0x8B51: return "FLOAT_VEC3";
	case 0x8B52: return "FLOAT_VEC4";
	case 0x8B53: return "INT_VEC2";
	case 0x8B54: return "INT_VEC3";
	case 0x8B55: return "INT_VEC4";
	case 0x8B56: return "BOOL";
	case 0x8B57: return "BOOL_VEC2";
	case 0x8B58: return "BOOL_VEC3";
	case 0x8B59: return "BOOL_VEC4";
	case 0x8B5A: return "FLOAT_MAT2";
	case 0x8B5B: return "FLOAT_MAT3";
	case 0x8B5C: return "FLOAT_MAT4";
	case 0x8B5E: return "SAMPLER_2D";
	case 0x8B5F: return "SAMPLER_3D";
	case 0x8B60: return "SAMPLER_CUBE";
	case 0x8B80: return "DELETE_STATUS";
	case 0x8B81: return "COMPILE_STATUS";
	case 0x8B82: return "LINK_STATUS";
	case 0x8B83: return "VALIDATE_STATUS";
	case 0x8B84: return "INFO_LOG_LENGTH";
	case 0x8B85: return "ATTACHED_SHADERS";
	case 0x8B86: return "ACTIVE_UNIFORMS";
	case 0x8B87: return "ACTIVE_UNIFORM_MAX_LENGTH";
	case 0x8B88: return "SHADER_SOURCE_LENGTH";
	case 0x8B89: return "ACTIVE_ATTRIBUTES";
	case 0x8B8A: return "ACTIVE_ATTRIBUTE_MAX_LENGTH";
	case 0x8B8B: return "FRAGMENT_SHADER_DERIVATIVE_HINT";
	case 0x8B8C: return "SHADING_LANGUAGE_VERSION";
	case 0x8B8D: return "CURRENT_PROGRAM";
	case 0x8B90: return "PALETTE4_RGB8_OES";
	case 0x8B91: return "PALETTE4_RGBA8_OES";
	case 0x8B92: return "PALETTE4_R5_G6_B5_OES";
	case 0x8B93: return "PALETTE4_RGBA4_OES";
	case 0x8B94: return "PALETTE4_RGB5_A1_OES";
	case 0x8B95: return "PALETTE8_RGB8_OES";
	case 0x8B96: return "PALETTE8_RGBA8_OES";
	case 0x8B97: return "PALETTE8_R5_G6_B5_OES";
	case 0x8B98: return "PALETTE8_RGBA4_OES";
	case 0x8B99: return "PALETTE8_RGB5_A1_OES";
	case 0x8B9A: return "IMPLEMENTATION_COLOR_READ_TYPE";
	case 0x8B9B: return "IMPLEMENTATION_COLOR_READ_FORMAT";
	case 0x8B9C: return "POINT_SIZE_ARRAY_OES";
	case 0x8B9D: return "TEXTURE_CROP_RECT_OES";
	case 0x8B9E: return "MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES";
	case 0x8B9F: return "POINT_SIZE_ARRAY_BUFFER_BINDING_OES";
	case 0x8BC0: return "COUNTER_TYPE_AMD";
	case 0x8BC1: return "COUNTER_RANGE_AMD";
	case 0x8BC2: return "UNSIGNED_INT64_AMD";
	case 0x8BC3: return "PERCENTAGE_AMD";
	case 0x8BC4: return "PERFMON_RESULT_AVAILABLE_AMD";
	case 0x8BC5: return "PERFMON_RESULT_SIZE_AMD";
	case 0x8BC6: return "PERFMON_RESULT_AMD";
	case 0x8BD2: return "TEXTURE_WIDTH_QCOM";
	case 0x8BD3: return "TEXTURE_HEIGHT_QCOM";
	case 0x8BD4: return "TEXTURE_DEPTH_QCOM";
	case 0x8BD5: return "TEXTURE_INTERNAL_FORMAT_QCOM";
	case 0x8BD6: return "TEXTURE_FORMAT_QCOM";
	case 0x8BD7: return "TEXTURE_TYPE_QCOM";
	case 0x8BD8: return "TEXTURE_IMAGE_VALID_QCOM";
	case 0x8BD9: return "TEXTURE_NUM_LEVELS_QCOM";
	case 0x8BDA: return "TEXTURE_TARGET_QCOM";
	case 0x8BDB: return "TEXTURE_OBJECT_VALID_QCOM";
	case 0x8BDC: return "STATE_RESTORE";
	case 0x8C00: return "COMPRESSED_RGB_PVRTC_4BPPV1_IMG";
	case 0x8C01: return "COMPRESSED_RGB_PVRTC_2BPPV1_IMG";
	case 0x8C02: return "COMPRESSED_RGBA_PVRTC_4BPPV1_IMG";
	case 0x8C03: return "COMPRESSED_RGBA_PVRTC_2BPPV1_IMG";
	case 0x8C92: return "ATC_RGB_AMD";
	case 0x8C93: return "ATC_RGBA_EXPLICIT_ALPHA_AMD";
	case 0x8CA3: return "STENCIL_BACK_REF";
	case 0x8CA4: return "STENCIL_BACK_VALUE_MASK";
	case 0x8CA5: return "STENCIL_BACK_WRITEMASK";
	case 0x8CA6: return "FRAMEBUFFER_BINDING";
	case 0x8CA7: return "RENDERBUFFER_BINDING";
	case 0x8CD0: return "FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE";
	case 0x8CD1: return "FRAMEBUFFER_ATTACHMENT_OBJECT_NAME";
	case 0x8CD2: return "FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL";
	case 0x8CD3: return "FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE";
	case 0x8CD4: return "FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET";
	case 0x8CD5: return "FRAMEBUFFER_COMPLETE";
	case 0x8CD6: return "FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
	case 0x8CD7: return "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case 0x8CD8: return "FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT";
	case 0x8CD9: return "FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
	case 0x8CDA: return "FRAMEBUFFER_INCOMPLETE_FORMATS";
	case 0x8CDD: return "FRAMEBUFFER_UNSUPPORTED";
	case 0x8CDE: return "FRAMEBUFFER_STATUS_ERROR";
	case 0x8CDF: return "MAX_COLOR_ATTACHMENTS";
	case 0x8CE0: return "COLOR_ATTACHMENT0";
	case 0x8CE1: return "COLOR_ATTACHMENT1";
	case 0x8CE2: return "COLOR_ATTACHMENT2";
	case 0x8CE3: return "COLOR_ATTACHMENT3";
	case 0x8CE4: return "COLOR_ATTACHMENT4";
	case 0x8CE5: return "COLOR_ATTACHMENT5";
	case 0x8CE6: return "COLOR_ATTACHMENT6";
	case 0x8CE7: return "COLOR_ATTACHMENT7";
	case 0x8CE8: return "COLOR_ATTACHMENT8";
	case 0x8CE9: return "COLOR_ATTACHMENT9";
	case 0x8CEA: return "COLOR_ATTACHMENT10";
	case 0x8CEB: return "COLOR_ATTACHMENT11";
	case 0x8CEC: return "COLOR_ATTACHMENT12";
	case 0x8CED: return "COLOR_ATTACHMENT13";
	case 0x8CEE: return "COLOR_ATTACHMENT14";
	case 0x8CEF: return "COLOR_ATTACHMENT15";
	case 0x8D00: return "DEPTH_ATTACHMENT";
	case 0x8D20: return "STENCIL_ATTACHMENT";
	case 0x8D40: return "FRAMEBUFFER";
	case 0x8D41: return "RENDERBUFFER";
	case 0x8D42: return "RENDERBUFFER_WIDTH";
	case 0x8D43: return "RENDERBUFFER_HEIGHT";
	case 0x8D44: return "RENDERBUFFER_INTERNAL_FORMAT";
	case 0x8D45: return "STENCIL_INDEX";
	case 0x8D46: return "STENCIL_INDEX1_OES";
	case 0x8D47: return "STENCIL_INDEX4_OES";
	case 0x8D48: return "STENCIL_INDEX8_OES";
	case 0x8D50: return "RENDERBUFFER_RED_SIZE";
	case 0x8D51: return "RENDERBUFFER_GREEN_SIZE";
	case 0x8D52: return "RENDERBUFFER_BLUE_SIZE";
	case 0x8D53: return "RENDERBUFFER_ALPHA_SIZE";
	case 0x8D54: return "RENDERBUFFER_DEPTH_SIZE";
	case 0x8D55: return "RENDERBUFFER_STENCIL_SIZE";
	case 0x8D61: return "HALF_FLOAT_OES";
	case 0x8D62: return "RGB565";
	case 0x8D63: return "PLATFORM_BINARY_OES";
	case 0x8D64: return "ETC1_RGB8_OES";
	case 0x8D65: return "RGB_S3TC_OES";
	case 0x8D66: return "RGBA_S3TC_OES";
	case 0x8DF0: return "LOW_FLOAT";
	case 0x8DF1: return "MEDIUM_FLOAT";
	case 0x8DF2: return "HIGH_FLOAT";
	case 0x8DF3: return "LOW_INT";
	case 0x8DF4: return "MEDIUM_INT";
	case 0x8DF5: return "HIGH_INT";
	case 0x8DF6: return "UNSIGNED_INT_10_10_10_2_OES";
	case 0x8DF7: return "INT_10_10_10_2_OES";
	case 0x8DF8: return "SHADER_BINARY_FORMATS";
	case 0x8DF9: return "NUM_SHADER_BINARY_FORMATS";
	case 0x8DFA: return "SHADER_COMPILER";
	case 0x8DFB: return "MAX_VERTEX_UNIFORM_VECTORS";
	case 0x8DFC: return "MAX_VARYING_VECTORS";
	case 0x8DFD: return "MAX_FRAGMENT_UNIFORM_VECTORS";
	case 0x8FA0: return "PERFMON_GLOBAL_MODE_QCOM";
	default: sprintf(unk, "0x%04x", attr); return unk;
	}
	return 0;
}

// Functions below are the modified Samsung's helper functions from M8 SDK samples.

void matIdentity(float *m)
{
	for (int i=0; i<16; i++)
		m[i] = (i%5==0) ? 1.0f : 0.0f;  //The first and every fifth element after that is 1.0 other are 0.0
}

void matMult(float *m3, const float *m1, const float *m2) 
{
	m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
}

//will update the current matrix' m' with the tranalation vector x,y,z
void matTranslate(float *m, const float x, const float y, const float z)
{
	m[12] += m[0]*x + m[4]*y + m[8]*z;
	m[13] += m[1]*x + m[5]*y + m[9]*z;
	m[14] += m[2]*x + m[6]*y + m[10]*z;
	m[15] += m[3]*x + m[7]*y + m[11]*z;
}  

//rotate about arbit axis
BOOL matRotate(float *m, float DEGAngle, float x, float y, float z)
{
	float Sq = (float)sqrtf(x*x + y*y + z*z);
	if (Sq > -FLT_EPSILON && Sq < FLT_EPSILON) //chk for divide by zero......
		return false;

	float inv = 1.0f/Sq;	
	x = x * inv;   
	y = y * inv;
	z = z * inv;

	float radian = (float)(PI/180) * DEGAngle;
	float f32c = (float)cos(radian);
	float f32s = (float)sin(radian);
	float f32OneMinC = 1 - f32c;

	float RotMat[16];
	RotMat[0] = f32c + f32OneMinC * x * x;
	RotMat[1] = (f32OneMinC * x * y) + (z * f32s);
	RotMat[2] = (f32OneMinC * x * z) - (y * f32s);
	RotMat[3] = 0.0;
	RotMat[4] = (f32OneMinC * x * y) - (z * f32s);
	RotMat[5] = f32c + f32OneMinC * y * y;
	RotMat[6] = (f32OneMinC * y * z) + (x * f32s);
	RotMat[7] = 0.0;
	RotMat[8] = (f32OneMinC * x * z) + (y * f32s);
	RotMat[9] = (f32OneMinC * y * z) - (x * f32s);
	RotMat[10] = f32c + f32OneMinC * z * z;
	RotMat[11] = RotMat[12] =RotMat[13] = RotMat[14] = 0.0;RotMat[15] =1.0f;

	float t[16];
	matMult(&t[0], m, &RotMat[0]);
	memcpy(m, t, sizeof(t));

	return true;
}

void matScale(float *m, float x, float y, float z)
{
	m[0] *= x; m[4] *= y; m[8] *= z;
	m[1] *= x; m[5] *= y; m[9] *= z;
	m[2] *= x; m[6] *= y; m[10] *= z;
	m[3] *= x; m[7] *= y; m[11] *= z;
}

//like glfrustum
bool matFrustum(float *m, float f32Left, float f32Right, float f32Bottom, float f32Top, float f32ZNear, float f32ZFar)
{
	float diff = f32Right - f32Left;
	float eps = FLT_EPSILON, meps = -eps;
	if (diff > meps && diff < eps) //chk for divide by zero......
		return false;

	diff = f32Top - f32Bottom;
	if (diff > meps && diff < eps) //chk for divide by zero......
		return false;

	diff = f32ZFar - f32ZNear;
	if (diff > meps && diff < eps) //chk for divide by zero......
		return false;

	m[0] = float(2.0 * f32ZNear / (f32Right - f32Left));
	m[1] = m[2] = m[3] = 0;

	m[4] = 0;
	m[5] = float(2.0 * f32ZNear / (f32Top - f32Bottom));
	m[6] = m[7] = 0;

	m[8] = (f32Right + f32Left) / (f32Right - f32Left);
	m[9] = (f32Top + f32Bottom) / (f32Top - f32Bottom);
	m[10] = ((f32ZNear + f32ZFar) / (f32ZNear - f32ZFar));
	m[11] = -1;

	m[12] = m[13] = 0;
	m[14] = ((-2.0f * f32ZNear * f32ZFar) / (f32ZFar - f32ZNear));
	m[15] = 0;

	return true;
}

BOOL matPerspective(float *m, float fieldOfViewDegree, float aspectRatio, float zNear, float zFar)
{
	if (fieldOfViewDegree <= 0.0f || fieldOfViewDegree >= 180.0f)
		//fieldOfViewDegree = 45.0f;   //assign FOV to 45 deg if value passed is not in proper range 
		return false;

	float FOVrad = float(fieldOfViewDegree * 0.5f * PI / 180); //angle divided by 2 !!!
	float f32Top = float(zNear * tan(FOVrad)), f32Bottom = -f32Top;
	float f32Right = aspectRatio * f32Top, f32Left = -f32Right;
	return matFrustum(m, f32Left, f32Right, f32Bottom, f32Top, zNear, zFar);
}  

BOOL matOrtho(float* m, float f32Left, float f32Right, float f32Bottom, float f32Top, float f32ZNear, float f32ZFar)
{
	float diff = f32Right - f32Left;
	float eps = FLT_EPSILON, meps = -eps;
	if (diff > meps && diff < eps) //chk for divide by zero......
		return false;

	diff = f32Top - f32Bottom;
	if (diff > meps && diff < eps) //chk for divide by zero......
		return false;

	diff = f32ZFar - f32ZNear;
	if (diff > meps && diff < eps) //chk for divide by zero......
		return false;

	m[0] = float(2.0 / (f32Right - f32Left));
	m[1] = m[2] = m[3] = 0;

	m[4] = 0;
	m[5] = float(2.0 / (f32Top - f32Bottom));
	m[6] = m[7] = 0;

	m[8] = m[9] = 0;
	m[10] = float(-2.0f / (f32ZFar - f32ZNear));
	m[11] = 0;

	m[12] = ((f32Right + f32Left) / (f32Left - f32Right));
	m[13] = ((f32Top + f32Bottom) / (f32Bottom - f32Top));
	m[14] = ((f32ZNear + f32ZFar) / (f32ZNear - f32ZFar));
	m[15] = 1;

	return true;
}

