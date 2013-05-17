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

/*

----------------
 Release notes:
----------------

This project was initiated by Chainfire and NuShrike (thank you, guys!), then it was mainly done by GinKage (ginkage@yandex.ru).
Some code was borrowed from Squish compression library (see squish_s3tc.cpp) and Oolong Engine (see pvrtc.cpp), copyright notices kept safe.

Currently this lib (version 0.258) is able to run:
* Xtrakt
* Experiment 13
* glBenchmark 1.0/1.1 (with occasional hangs...)
* Opera Mobile 9.7
* TouchFLO 3D 1.3
* Tower Defense
* Flight Commander
* 10 Games for Betting
* some OpenGL CommonLite samples, TestOpenGL, Galatea Engine samples, etc.

*/

#include "stdafx.h"
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include "libgles_cm.h"

mat4 matUnity;
float *matCurrent = ModelViewMatrix;
mat4 matMVstack[256];
mat4 matPRJstack[256];
mat4 matTEX0stack[256];
mat4 matTEX1stack[256];
mat4 *matStack;
int stackMVtop = -1;
int stackPRJtop = -1;
int stackTEX0top = -1;
int stackTEX1top = -1;
int *stackTop = &stackMVtop;
bool shadersReady = false;
int texActive = 0;
GLuint curPalMat = 0;
GLenum matMode;
GLuint texCurrent[2] = {0};
bool tower_defense = false;
bool viewport_init = false;
EGLDisplay current_dpy;
EGLSurface current_draw;
EGLSurface current_read;
EGLContext current_ctx;

HDC hDCsrc, hDCdst;

glRect viewport;
std::map<GLuint, glRect> texCrop;
std::map<GLuint, glRect> texSize;
GLfloat viewZN = 0, viewZF = 1;
GLint view_x, view_y, view_w, view_h;

GLenum clientTex = GL_TEXTURE0;
GLenum activeTex = GL_TEXTURE0;

class GLbuffer
{
public:
	GLsizeiptr size;
	void *data;

	GLbuffer() : size(0), data(0) {}
	~GLbuffer() { if (data) delete[] data; }
};

class GLsurface
{
public:
	EGLDisplay dpy;
	EGLConfig config;
	NativeWindowType window;
	std::vector<EGLint> attrib_list;
	int width, height;
	EGLSurface new_surface;
};

std::map<GLuint, GLbuffer> created_buffers;
std::set<GLuint> deleted_buffers;
std::map<EGLSurface, GLsurface> window_surface;
GLuint array_buffer = 0, element_buffer = 0;
int count_buffers = 0;

#define MAX_GL_BUFFERS 100

void InitMyGLES()
{
	InitShaders();

	matIdentity(ModelViewMatrix);
	matIdentity(ProjectionMatrix);
	matIdentity(TextureMatrix[0]);
	matIdentity(TextureMatrix[1]);
	matIdentity(matUnity);
	matIdentity(ModelViewProjectionMatrix);
	for (int i = 0; i < 32; i++)
		matIdentity(MatrixPalette[i]);

	matCurrent = ModelViewMatrix;
	stackMVtop = -1;
	stackPRJtop = -1;
	stackTEX0top = -1;
	stackTEX1top = -1;
	stackTop = &stackMVtop;
	texActive = 0;
	curPalMat = 0;
	matMode = GL_MODELVIEW_MATRIX;
	viewZN = 0;
	viewZF = 1;
	clientTex = GL_TEXTURE0;
	activeTex = GL_TEXTURE0;
	array_buffer = 0;
	element_buffer = 0;
	count_buffers = 0;
	texCurrent[0] = texCurrent[1] = 0;
	current_dpy = 0;
	current_draw = 0;
	current_read = 0;
	current_ctx = 0;

	viewport_init = false;
	shadersReady = true;
}

void CleanupMyGLES()
{
	DestroyShaders();
	shadersReady = false;
	created_buffers.clear();
	deleted_buffers.clear();
	window_surface.clear();
	texCrop.clear();
	texSize.clear();
	tower_defense = false;
}

#ifdef MYGL_LOG
#ifndef HEAVY_LOG
std::list<std::string> loglist;
#endif

char logbuf[512];

void log_printf(int ll, char *msg, ...)
{
	if (ll > ll_extra) return;
	va_list args;
	va_start(args, msg);
	vsprintf(logbuf, msg, args);
	va_end(args);
#ifdef HEAVY_LOG
	FILE *logfile = fopen("\\My Storage\\gles_log.txt", "a");
	fputs(logbuf, logfile);
	fclose(logfile);
#else
	loglist.push_back(logbuf);
#endif
}
#endif

extern "C" {
void GL_APIENTRY myglActiveTexture(GLenum texture)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(texture));
	activeTex = texture;
	texActive = (texture == GL_TEXTURE0 ? 0 : 1);
	glActiveTexture(texture);
}

void GL_APIENTRY myglAlphaFunc(GLenum func, GLclampf ref)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %.6f)\n", enumName(func), ref);
	BYTE *pgv = (BYTE *)TlsGetValue(4);
	if (!pgv) return;
	GLenum *pval = (GLenum *)(pgv + 0x391C);
	*pval = func;
	*((GLclampf *)(pval + 1)) = ref;
}

void GL_APIENTRY myglAlphaFuncx(GLenum func, GLclampx ref)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglAlphaFunc(func, FixedToFloat(ref));
}

void GL_APIENTRY myglBindBuffer(GLenum target, GLuint buffer)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d)\n", enumName(target), buffer);

	if (buffer <= MAX_GL_BUFFERS)
	{
		glBindBuffer(target, buffer);
		buffer = 0;
	}
	else
		glBindBuffer(target, 0);

	if (target == GL_ARRAY_BUFFER)
		array_buffer = buffer;
	else if (target == GL_ELEMENT_ARRAY_BUFFER)
		element_buffer = buffer;
}

GLint texMagFilter = GL_NEAREST;
GLint texMinFilter = GL_NEAREST;
GLint texWrapS = GL_REPEAT;
GLint texWrapT = GL_REPEAT;
GLint texWrapR = GL_REPEAT;

void GL_APIENTRY myglBindTexture(GLenum target, GLuint texture)
{
	log_printf(ll_frame, __FUNCTION__ "(%s, %d)\n", enumName(target), texture);
	texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1] = texture;
	glBindTexture(target, texture);
	glGetError();
}

void GL_APIENTRY myglBlendFunc(GLenum sfactor, GLenum dfactor)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(sfactor), enumName(dfactor));
	glBlendFunc(sfactor, dfactor);
}

void GL_APIENTRY myglBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d, 0x%lx, %s)\n", enumName(target), size, data, enumName(usage));

	GLuint n = 0;
	if (target == GL_ARRAY_BUFFER)
		n = array_buffer;
	else if (target == GL_ELEMENT_ARRAY_BUFFER)
		n = element_buffer;

	if (n <= MAX_GL_BUFFERS)
		glBufferData(target, size, data, usage);
	else
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(n);
		if (it == created_buffers.end()) return;

		GLbuffer *buf = &(it->second);
		if (size > buf->size)
		{
			if (buf->data) delete[] buf->data;
			buf->data = new char[buf->size = size];
		}

		if (data) memcpy(buf->data, data, size);
	}
}

void GL_APIENTRY myglBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d, %d, 0x%lx)\n", enumName(target), offset, size, data);
	if (!data) return;

	GLuint n = 0;
	if (target == GL_ARRAY_BUFFER)
		n = array_buffer;
	else if (target == GL_ELEMENT_ARRAY_BUFFER)
		n = element_buffer;

	if (n <= MAX_GL_BUFFERS)
		glBufferSubData(target, offset, size, data);
	else
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(n);
		if (it == created_buffers.end()) return;

		GLbuffer *buf = &(it->second);
		if (buf->data != 0 && buf->size >= offset + size)
			memcpy((char *)buf->data + offset, data, size);
	}
}

void GL_APIENTRY myglClear(GLbitfield mask)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glClear(mask);
}

void GL_APIENTRY myglClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	log_printf(ll_all, __FUNCTION__ "(%.6lf, %.6lf, %.6lf, %.6lf)\n", red, green, blue, alpha);
	glClearColor(red, green, blue, alpha);
}

void GL_APIENTRY myglClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglClearColor(FixedToFloat(red), FixedToFloat(green), FixedToFloat(blue), FixedToFloat(alpha));
}

void GL_APIENTRY myglClearDepthf(GLclampf depth)
{
	log_printf(ll_all, __FUNCTION__ "(%.6f)\n", depth);
	glClearDepthf(depth);
}

void GL_APIENTRY myglClearDepthx(GLclampx depth)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglClearDepthf(FixedToFloat(depth));
}

void GL_APIENTRY myglClearStencil(GLint s)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glClearStencil(s);
}

void GL_APIENTRY myglClientActiveTexture(GLenum texture)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(texture));
	clientTex = texture;
}

void GL_APIENTRY myglClipPlanef(GLenum plane, const GLfloat *equation) { log_printf(ll_all, __FUNCTION__ "\n");  }

void GL_APIENTRY myglClipPlanex(GLenum plane, const GLfixed *equation)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!equation) return;
	GLfloat eq[4] = { FixedToFloat(equation[0]), FixedToFloat(equation[1]), FixedToFloat(equation[2]), FixedToFloat(equation[3]) };
	myglClipPlanef(plane, eq);
}

void GL_APIENTRY myglColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	log_printf(ll_all, __FUNCTION__ "(%.6f, %.6f, %.6f, %.6f)\n", red, green, blue, alpha);
	shColor(red, green, blue, alpha);
}

void GL_APIENTRY myglColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglColor4f(float(red) / 255.0f, float(green) / 255.0f, float(blue) / 255.0f, float(alpha) / 255.0f);
}

void GL_APIENTRY myglColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglColor4f(FixedToFloat(red), FixedToFloat(green), FixedToFloat(blue), FixedToFloat(alpha));
}

void GL_APIENTRY myglColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glColorMask(red, green, blue, alpha);
}

void GL_APIENTRY myglColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	log_printf(ll_frame, __FUNCTION__ "(%d, %s, %d, 0x%x)\n", size, enumName(type), stride, pointer);

	if (array_buffer)
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(array_buffer);
		if (it != created_buffers.end())
			pointer = (char *)it->second.data + (size_t)pointer;
	}

	shClientAttrib(GL_COLOR_ARRAY, true, size, type, stride, pointer);
}

void texStoreParam(GLenum pname, GLint param)
{
	switch (pname)
	{
	case GL_TEXTURE_MAG_FILTER:	texMagFilter = param;	break;
	case GL_TEXTURE_MIN_FILTER:	texMinFilter = param;	break;
	case GL_TEXTURE_WRAP_S:	texWrapS = param;	break;
	case GL_TEXTURE_WRAP_T:	texWrapT = param;	break;
	case GL_TEXTURE_WRAP_R:	texWrapR = param;	break;
	}
}

void texLoadParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texMagFilter);
}

void GL_APIENTRY myglCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void GL_APIENTRY myglCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void GL_APIENTRY myglCullFace(GLenum mode)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(mode));
	glCullFace(mode);
}

void GL_APIENTRY myglDeleteBuffers(GLsizei n, const GLuint *buffers)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!buffers) return;

	while (n--)
		if (buffers[n] <= MAX_GL_BUFFERS)
			glDeleteBuffers(1, &(buffers[n]));
		else
		{
			std::map<GLuint, GLbuffer>::iterator it;
			for (int i = 0; i < n; i++)
			{
				it = created_buffers.find(buffers[i]);
				if (it != created_buffers.end())
				{
					created_buffers.erase(it);
					deleted_buffers.insert(buffers[i]);
				}
			}
		}	
}

void GL_APIENTRY myglDeleteTextures(GLsizei n, const GLuint *textures)
{
	log_printf(ll_all, __FUNCTION__ "(%d, %d)\n", n, *textures);
	glDeleteTextures(n, textures);
}

void GL_APIENTRY myglDepthFunc(GLenum func)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(func));
	glDepthFunc(func);
}

void GL_APIENTRY myglDepthMask(GLboolean flag)
{
	log_printf(ll_all, __FUNCTION__ "(0x%x)\n", flag);
	glDepthMask(flag);
}

void GL_APIENTRY myglDepthRangef(GLclampf zNear, GLclampf zFar)
{
	log_printf(ll_all, __FUNCTION__ "(%.6f, %.6f)\n", zNear, zFar);
	glDepthRangef(zNear, zFar);
	viewZN = zNear;
	viewZF = zFar;
}

void GL_APIENTRY myglDepthRangex(GLclampx zNear, GLclampx zFar)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglDepthRangef(FixedToFloat(zNear), FixedToFloat(zFar));
}

void GL_APIENTRY myglDisable(GLenum cap)
{
	log_printf(ll_extra, __FUNCTION__ "(%s)\n", enumName(cap));
	if (cap == GL_MATRIX_PALETTE_OES)
		MatrixPaletteEnabled = 0;
	else
		glDisable(cap);

	if (cap == GL_TEXTURE_2D)
	{
		shDisableState(GL_TEXTURE_COORD_ARRAY, (activeTex == GL_TEXTURE0));
		Texture2DEnabled[texActive] = 0;
	}
	else if (cap == GL_FOG)
		FogEnabled = 0;
	else if (cap >= GL_LIGHT0 && cap <= GL_LIGHT7)
		LightEnabled[cap - GL_LIGHT0] = 0;
	else if (cap == GL_LIGHTING)
		LightingEnabled = 0;
	else if (cap == GL_COLOR_MATERIAL)
		colorMaterialEnabled = 0;
}

void GL_APIENTRY myglDisableClientState(GLenum array)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(array));
	shDisableState(array, (clientTex == GL_TEXTURE0));
}

void GL_APIENTRY myglDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	log_printf(ll_frame, __FUNCTION__ "(%s, %d, %d)\n", enumName(mode), first, count);

	std::map<GLuint, glRect>::iterator ps = texSize.find(texCurrent[0]);
	if (ps != texSize.end())
	{
		TexShift0[0] = -0.5f / ps->second.width;
		TexShift0[1] = -0.5f / ps->second.height;
	}

	ps = texSize.find(texCurrent[1]);
	if (ps != texSize.end())
	{
		TexShift1[0] = -0.5f / ps->second.width;
		TexShift1[1] = -0.5f / ps->second.height;
	}

	shSetupShader(mode);
	glDrawArrays(mode, first, count);
}

void GL_APIENTRY myglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d, %s, 0x%x)\n", enumName(mode), count, enumName(type), indices);

	if (element_buffer)
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(element_buffer);
		if (it != created_buffers.end())
			indices = (char *)it->second.data + (size_t)indices;
	}

	std::map<GLuint, glRect>::iterator ps = texSize.find(texCurrent[0]);
	if (ps != texSize.end())
	{
		TexShift0[0] = -0.5f / ps->second.width;
		TexShift0[1] = -0.5f / ps->second.height;
	}

	ps = texSize.find(texCurrent[1]);
	if (ps != texSize.end())
	{
		TexShift1[0] = -0.5f / ps->second.width;
		TexShift1[1] = -0.5f / ps->second.height;
	}

	shSetupShader(mode);
	glDrawElements(mode, count, type, indices);
}

void GL_APIENTRY myglEnable(GLenum cap)
{
	log_printf(ll_extra, __FUNCTION__ "(%s)\n", enumName(cap));
	if (cap == GL_DEPTH_TEST && tower_defense)
		return;

	if (cap == GL_MATRIX_PALETTE_OES)
		MatrixPaletteEnabled = 1;
	else
		glEnable(cap);

	if (cap == GL_TEXTURE_2D)
	{
		shEnableState(GL_TEXTURE_COORD_ARRAY, (activeTex == GL_TEXTURE0));
		Texture2DEnabled[texActive] = 1;
	}
	else if (cap == GL_FOG)
		FogEnabled = 1;
	else if (cap >= GL_LIGHT0 && cap <= GL_LIGHT7)
		LightEnabled[cap - GL_LIGHT0] = 1;
	else if (cap == GL_LIGHTING)
		LightingEnabled = 1;
	else if (cap == GL_COLOR_MATERIAL)
	{
		colorMaterialEnabled = 1;
//		memcpy(FrontNBackMaterial.ambient, CurrentColor, 4*sizeof(GLfloat));
//		memcpy(FrontNBackMaterial.diffuse, CurrentColor, 4*sizeof(GLfloat));
	}
}

void GL_APIENTRY myglEnableClientState(GLenum array)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(array));
	shEnableState(array, (clientTex == GL_TEXTURE0));
}

void GL_APIENTRY myglFinish(void)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glFinish();
}

void GL_APIENTRY myglFlush(void)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glFlush();
}

void GL_APIENTRY myglFogf(GLenum pname, GLfloat param)
{
	log_printf(ll_all, __FUNCTION__ "\n");

	switch (pname)
	{
	case GL_FOG_MODE:
		switch ((int)((float)param))
		{
//		log_printf(ll_app, __FUNCTION__ "(%s)\n", enumName((int)((float)param)));
		case GL_LINEAR:	Fog.mode = FOGMODE_LINEAR;	break;
		case GL_EXP:	Fog.mode = FOGMODE_EXP;		break;
		case GL_EXP2:	Fog.mode = FOGMODE_EXP2;		break;
		}
		break;
	case GL_FOG_DENSITY:	Fog.density = param;	break;
	case GL_FOG_START:		Fog.start = param;	break;
	case GL_FOG_END:		Fog.end = param;	break;
	}
}

void GL_APIENTRY myglFogfv(GLenum pname, const GLfloat *params)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!params) return;

	switch (pname)
	{
	case GL_FOG_COLOR:
		memcpy(Fog.color, params, 4*sizeof(float));
		break;

	default:
		myglFogf(pname, *params);
		break;
	}
}

void GL_APIENTRY myglFogx(GLenum pname, GLfixed param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(pname), enumName(param));

	switch (pname)
	{
	case GL_FOG_MODE:
		switch (param)
		{
//		log_printf(ll_app, __FUNCTION__ "(%s)\n", enumName(param));
		case GL_LINEAR:	Fog.mode = FOGMODE_LINEAR;	break;
		case GL_EXP:	Fog.mode = FOGMODE_EXP;		break;
		case GL_EXP2:	Fog.mode = FOGMODE_EXP2;	break;
		}
		break;

	default:
		myglFogf(pname, FixedToFloat(param));
		break;
	}
}

void GL_APIENTRY myglFogxv(GLenum pname, const GLfixed *params)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!params) return;

	switch (pname)
	{
	case GL_FOG_COLOR:
		Fog.color[0] = FixedToFloat(params[0]);
		Fog.color[1] = FixedToFloat(params[1]);
		Fog.color[2] = FixedToFloat(params[2]);
		Fog.color[3] = FixedToFloat(params[3]);
		break;

	default:
		myglFogx(pname, *params);
		break;
	}
}

void GL_APIENTRY myglFrontFace(GLenum mode)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(mode));
	glFrontFace(mode);
}

void GL_APIENTRY myglFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	log_printf(ll_all, __FUNCTION__ "(%.6lf, %.6lf, %.6lf, %.6lf, %.6lf, %.6lf)\n", left, right, bottom, top, zNear, zFar);
	mat4 cur, fr;
	memcpy(cur, matCurrent, sizeof(cur));
	matFrustum(fr, left, right, bottom, top, zNear, zFar);
	matMult(matCurrent, fr, cur);
}

void GL_APIENTRY myglFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglFrustumf(FixedToFloat(left), FixedToFloat(right), FixedToFloat(bottom), FixedToFloat(top), FixedToFloat(zNear), FixedToFloat(zFar));
}

void GL_APIENTRY myglGetBooleanv(GLenum pname, GLboolean *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(pname));
	glGetBooleanv(pname, params);
}

void GL_APIENTRY myglGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(target), enumName(pname));
	glGetBufferParameteriv(target, pname, params);
}

void GL_APIENTRY myglGetClipPlanef(GLenum pname, GLfloat eqn[4]) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglGetClipPlanex(GLenum pname, GLfixed eqn[4]) { log_printf(ll_all, __FUNCTION__ "\n");  }

void GL_APIENTRY myglGenBuffers(GLsizei n, GLuint *buffers)
{
	log_printf(ll_all, __FUNCTION__ "(%d)", n);
	if (!buffers) return;
	glGetError();
/*
	std::set<GLuint>::iterator it;
	for (int i = 0; i < n; i++)
	{
		if (!deleted_buffers.empty())
		{
			it = deleted_buffers.begin();
			buffers[i] = *it;
			deleted_buffers.erase(it);
		}
		else
			buffers[i] = ++count_buffers;
		log_printf(ll_all, " = %d\n", buffers[i]);
		created_buffers[buffers[i]];
	}
*/
	glGenBuffers(n, buffers);
	log_printf(ll_all, " = %d\n", *buffers);

	for (int i = 0; i < n; i++)
		if (buffers[i] > MAX_GL_BUFFERS)
			created_buffers[buffers[i]];
}

void GL_APIENTRY myglGenTextures(GLsizei n, GLuint *textures)
{
	log_printf(ll_all, __FUNCTION__ "(%d)", n);
	if (!textures) return;
	glGetError();
	glGenTextures(n, textures);
	log_printf(ll_all, " = %d\n", *textures);
}

GLenum GL_APIENTRY myglGetError(void)
{
	log_printf(ll_all, __FUNCTION__);
	GLenum res = glGetError();
	log_printf(ll_all, " = %s\n", (res > 0 ? enumName(res) : "0"));
	return res;
}

void GL_APIENTRY myglGetFixedv(GLenum pname, GLfixed *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(pname));
	if (!params) return;

	int i;
	if (pname == GL_MODELVIEW_MATRIX)
		for (i = 0; i < 16; i++)
			params[i] = FixedFromFloat(ModelViewMatrix[i]);
	else if (pname == GL_PROJECTION_MATRIX)
		for (i = 0; i < 16; i++)
			params[i] = FixedFromFloat(ProjectionMatrix[i]);
}

void GL_APIENTRY myglGetFloatv(GLenum pname, GLfloat *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(pname));
	if (!params) return;

	if (pname == GL_MODELVIEW_MATRIX)
		memcpy(params, ModelViewMatrix, sizeof(ModelViewMatrix));
	else if (pname == GL_PROJECTION_MATRIX)
		memcpy(params, ProjectionMatrix, sizeof(ModelViewMatrix));
	else
		glGetFloatv(pname, params);
}

void GL_APIENTRY myglGetIntegerv(GLenum pname, GLint *params)
{
	log_printf(ll_extra, __FUNCTION__ "(%s)", enumName(pname));
	if (!params) return;

	if (pname == GL_VIEWPORT) {
		params[0] = view_x;
		params[1] = view_y;
		params[2] = view_w;
		params[3] = view_h;
	}
	else if (pname == GL_MAX_TEXTURE_UNITS)
		*params = 2;
	else if (pname == GL_MAX_CLIP_PLANES)
		*params = 6;
	else if (pname == GL_MAX_LIGHTS)
		*params = 8;
	else if (pname == GL_MAX_MODELVIEW_STACK_DEPTH)
		*params = 256;
	else if (pname == GL_MAX_PROJECTION_STACK_DEPTH)
		*params = 256;
	else if (pname == GL_MAX_TEXTURE_STACK_DEPTH)
		*params = 256;
	else if (pname == GL_MAX_PALETTE_MATRICES_OES)
		*params = 32;
	else if (pname == GL_MAX_VERTEX_UNITS_OES)
		*params = 3;
//	else if (pname == GL_MAX_TEXTURE_SIZE)
//		*params = 512;
	else
		glGetIntegerv(pname, params);
	log_printf(ll_extra, " = %d\n", *params);
}

void GL_APIENTRY myglGetLightfv(GLenum light, GLenum pname, GLfloat *params) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglGetLightxv(GLenum light, GLenum pname, GLfixed *params) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglGetMaterialfv(GLenum face, GLenum pname, GLfloat *params) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglGetMaterialxv(GLenum face, GLenum pname, GLfixed *params) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglGetPointerv(GLenum pname, void **params) { log_printf(ll_all, __FUNCTION__ "\n");  }

const GLubyte *GL_APIENTRY myglGetString(GLenum name)
{
	log_printf(ll_app, __FUNCTION__ "(%s)", enumName(name));

	const GLubyte *res = 0;
	if (shadersReady)
	{
		if (name == GL_EXTENSIONS)
			res = (GLubyte *)"GL_IMG_texture_compression_pvrtc OES_matrix_palette GL_OES_framebuffer_object GL_OES_read_format GL_OES_compressed_paletted_texture GL_OES_framebuffer_object GL_OES_stencil8 GL_OES_texture_compression_S3TC GL_OES_mapbuffer GL_OES_texture_npot GL_EXT_texture_compression_s3tc";
		else
			res = glGetString(name);
	}

	if (!res)
	{
		if (name == GL_VERSION)
			res = (GLubyte *)"1.3";
		else
			res = (GLubyte *)"GinGLES";
	}
	log_printf(ll_app, " = %s\n", res);
	return res;
}

void GL_APIENTRY myglGetTexEnviv(GLenum env, GLenum pname, GLint *params) { log_printf(ll_all, __FUNCTION__ "\n"); }
void GL_APIENTRY myglGetTexEnvfv(GLenum env, GLenum pname, GLfloat *params) { log_printf(ll_all, __FUNCTION__ "\n"); }
void GL_APIENTRY myglGetTexEnvxv(GLenum env, GLenum pname, GLfixed *params) { log_printf(ll_all, __FUNCTION__ "\n"); }

void GL_APIENTRY myglGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(target), enumName(pname));
	glGetTexParameteriv(target, pname, params);
}

void GL_APIENTRY myglGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(target), enumName(pname));
	glGetTexParameterfv(target, pname, params);
}

void GL_APIENTRY myglGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(target), enumName(pname));
	if (params)
	{
		GLfloat par = FixedToFloat(*params);
		myglGetTexParameterfv(target, pname, &par);
	}
}

void GL_APIENTRY myglHint(GLenum target, GLenum mode)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(target), enumName(mode));
	glHint(target, mode);
}

GLboolean GL_APIENTRY myglIsBuffer(GLuint buffer)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return glIsBuffer(buffer);
}

GLboolean GL_APIENTRY myglIsEnabled(GLenum cap)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return glIsEnabled(cap);
}

GLboolean GL_APIENTRY myglIsTexture(GLuint texture)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return glIsTexture(texture);
}

void GL_APIENTRY myglLightModelf(GLenum pname, GLfloat param) { log_printf(ll_all, __FUNCTION__ "\n"); }

void GL_APIENTRY myglLightModelfv(GLenum pname, const GLfloat *params)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (pname == GL_LIGHT_MODEL_AMBIENT)
	{
		FrontNBackLightModelProduct.sceneColor[0] = params[0];
		FrontNBackLightModelProduct.sceneColor[1] = params[1];
		FrontNBackLightModelProduct.sceneColor[2] = params[2];
		FrontNBackLightModelProduct.sceneColor[3] = params[3];
	}
}

void GL_APIENTRY myglLightModelx(GLenum pname, GLfixed param)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglLightModelf(pname, FixedToFloat(param));
}

void GL_APIENTRY myglLightModelxv(GLenum pname, const GLfixed *params)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (pname == GL_LIGHT_MODEL_AMBIENT)
	{
		FrontNBackLightModelProduct.sceneColor[0] = FixedToFloat(params[0]);
		FrontNBackLightModelProduct.sceneColor[1] = FixedToFloat(params[1]);
		FrontNBackLightModelProduct.sceneColor[2] = FixedToFloat(params[2]);
		FrontNBackLightModelProduct.sceneColor[3] = FixedToFloat(params[3]);
	}
}

void GL_APIENTRY myglLightf(GLenum light, GLenum pname, GLfloat param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %.06f)\n", enumName(light), enumName(pname), param);

	switch (pname)
	{
	case GL_SPOT_CUTOFF:			LightSource[light - GL_LIGHT0].spotCutoff = param;				break;
	case GL_SPOT_EXPONENT:			LightSource[light - GL_LIGHT0].spotExponent = param;			break;
	case GL_CONSTANT_ATTENUATION:	LightSource[light - GL_LIGHT0].constantAttenuation = param;		break;
	case GL_LINEAR_ATTENUATION:		LightSource[light - GL_LIGHT0].linearAttenuation = param;		break;
	case GL_QUADRATIC_ATTENUATION:	LightSource[light - GL_LIGHT0].quadraticAttenuation = param;	break;
	}
}

void GL_APIENTRY myglLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(light), enumName(pname));
	if (!params) return;

	switch (pname)
	{
	case GL_AMBIENT:				memcpy(LightSource[light - GL_LIGHT0].ambient, params, 4*sizeof(float));		break;
	case GL_DIFFUSE:				memcpy(LightSource[light - GL_LIGHT0].diffuse, params, 4*sizeof(float));		break;
	case GL_SPECULAR:				memcpy(LightSource[light - GL_LIGHT0].specular, params, 4*sizeof(float));		break;
	case GL_POSITION:				memcpy(LightSource[light - GL_LIGHT0].position, params, 4*sizeof(float));		break;
	case GL_SPOT_DIRECTION:			memcpy(LightSource[light - GL_LIGHT0].spotDirection, params, 3*sizeof(float));	break;
	default:
		myglLightf(light, pname, *params);
		break;
	}
}

void GL_APIENTRY myglLightx(GLenum light, GLenum pname, GLfixed param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(light), enumName(pname));
	myglLightf(light, pname, FixedToFloat(param));
}

void GL_APIENTRY myglLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(light), enumName(pname));
	if (!params) return;

	GLfloat vec[4];
	switch (pname)
	{
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_POSITION:
		vec[3] = FixedToFloat(params[3]);
	case GL_SPOT_DIRECTION:
		vec[0] = FixedToFloat(params[0]);
		vec[1] = FixedToFloat(params[1]);
		vec[2] = FixedToFloat(params[2]);
		myglLightfv(light, pname, vec);
		break;

	default:
		myglLightf(light, pname, FixedToFloat(*params));
		break;
	}
}

void GL_APIENTRY myglLineWidth(GLfloat width)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glLineWidth(width);
}

void GL_APIENTRY myglLineWidthx(GLfixed width)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglLineWidth(FixedToFloat(width));
}

void GL_APIENTRY myglLoadIdentity(void)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	memcpy(matCurrent, matUnity, sizeof(matUnity));
}

void GL_APIENTRY myglLoadMatrixf(const GLfloat *m)
{
	log_printf(ll_frame, __FUNCTION__ "\n");
	if (m)
	{
		memcpy(matCurrent, m, 16*sizeof(float));
/*		for (int i = 0; i < 16; i++)
		{
			log_printf(ll_frame, "\t%.06f", m[i]);
			if ((i & 3) == 3)
				log_printf(ll_frame, "\n");
		}
*/	}
}

void GL_APIENTRY myglLoadMatrixx(const GLfixed *m)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (m)
	{
		GLfloat mm[16];
		for (int i = 0; i < 16; i++)
			mm[i] = FixedToFloat(m[i]);
		myglLoadMatrixf(mm);
	}
}

void GL_APIENTRY myglLogicOp(GLenum opcode)
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(opcode));
}

void GL_APIENTRY myglMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %.06f)\n", enumName(face), enumName(pname), param);
	if ((float)param == GL_SHININESS)
		FrontNBackMaterial.shininess = param;
}

void GL_APIENTRY myglMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(face), enumName(pname));
	if (!params) return;

	switch (pname)
	{
	case GL_AMBIENT:	memcpy(FrontNBackMaterial.ambient, params, 4*sizeof(float));	break;
	case GL_DIFFUSE:	memcpy(FrontNBackMaterial.diffuse, params, 4*sizeof(float));	break;
	case GL_SPECULAR:	memcpy(FrontNBackMaterial.specular, params, 4*sizeof(float));	break;
	case GL_EMISSION:	memcpy(FrontNBackMaterial.emission, params, 4*sizeof(float));	break;
	case GL_SHININESS:	FrontNBackMaterial.shininess = *params;							break;
	case GL_AMBIENT_AND_DIFFUSE:
		memcpy(FrontNBackMaterial.ambient, params, 4*sizeof(float));
		memcpy(FrontNBackMaterial.diffuse, params, 4*sizeof(float));
		break;
	}
}

void GL_APIENTRY myglMaterialx(GLenum face, GLenum pname, GLfixed param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(face), enumName(pname));
	myglMaterialf(face, pname, FixedToFloat(param));
}

void GL_APIENTRY myglMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s)\n", enumName(face), enumName(pname));
	if (!params) return;

	GLfloat vec[4];

	switch (pname)
	{
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_EMISSION:
	case GL_AMBIENT_AND_DIFFUSE:
		vec[0] = FixedToFloat(params[0]);
		vec[1] = FixedToFloat(params[1]);
		vec[2] = FixedToFloat(params[2]);
		vec[3] = FixedToFloat(params[3]);
		myglMaterialfv(face, pname, vec);
		break;

	case GL_SHININESS:
		myglMaterialf(face, pname, FixedToFloat(*params));
		break;
	}
}

void GL_APIENTRY myglMatrixMode(GLenum mode)
{
	log_printf(ll_frame, __FUNCTION__ "(%s)\n", enumName(mode));
	switch (mode)
	{
	case GL_MODELVIEW:	matCurrent = ModelViewMatrix;	matStack = matMVstack;	stackTop = &stackMVtop;	break;
	case GL_PROJECTION:	matCurrent = ProjectionMatrix;	matStack = matPRJstack;	stackTop = &stackPRJtop;	break;
	case GL_TEXTURE:
		if (activeTex == GL_TEXTURE0)
		{	matCurrent = TextureMatrix[0];	matStack = matTEX0stack;	stackTop = &stackTEX0top;	}
		else
		{	matCurrent = TextureMatrix[1];	matStack = matTEX1stack;	stackTop = &stackTEX1top;	}
		break;
	case GL_MATRIX_PALETTE_OES:	matCurrent = MatrixPalette[curPalMat]; break;
	}
	matMode = mode;
}

void GL_APIENTRY myglMultMatrixf(const GLfloat *m)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (m)
	{
		mat4 cur;
		memcpy(cur, matCurrent, sizeof(cur));
/*		for (int i = 0; i < 16; i++)
		{
			log_printf(ll_extra, "\t%.03f", cur[i]);
			if ((i & 3) == 3)
				log_printf(ll_extra, "\n");
		}
		log_printf(ll_extra, "X\n");
*/		matMult(matCurrent, cur, (float *)m);
/*		for (int i = 0; i < 16; i++)
		{
			log_printf(ll_extra, "\t%.03f", m[i]);
			if ((i & 3) == 3)
				log_printf(ll_extra, "\n");
		}
		log_printf(ll_extra, "=\n");
		for (int i = 0; i < 16; i++)
		{
			log_printf(ll_extra, "\t%.03f", matCurrent[i]);
			if ((i & 3) == 3)
				log_printf(ll_extra, "\n");
		}
*/	}
}

void GL_APIENTRY myglMultMatrixx(const GLfixed *m)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (m)
	{
		mat4 mm;
		for (int i = 0; i < 16; i++)
			mm[i] = FixedToFloat(m[i]);
		myglMultMatrixf(mm);
	}
}

void GL_APIENTRY myglMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q) { log_printf(ll_all, __FUNCTION__ "\n");  }

void GL_APIENTRY myglNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	shNormal(nx, ny, nz);
}

void GL_APIENTRY myglNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	shNormal(FixedToFloat(nx), FixedToFloat(ny), FixedToFloat(nz));
}

void GL_APIENTRY myglNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	log_printf(ll_frame, __FUNCTION__ "(%s, %d, 0x%x)\n", enumName(type), stride, pointer);

	if (array_buffer)
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(array_buffer);
		if (it != created_buffers.end())
			pointer = (char *)it->second.data + (size_t)pointer;
	}

	shClientAttrib(GL_NORMAL_ARRAY, true, 3, type, stride, pointer);
}

void GL_APIENTRY myglOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	log_printf(ll_all, __FUNCTION__ "(%.6lf, %.6lf, %.6lf, %.6lf, %.6lf, %.6lf)\n", left, right, bottom, top, zNear, zFar);
	mat4 cur, fr;
	memcpy(cur, matCurrent, sizeof(cur));
	matOrtho(fr, left, right, bottom, top, zNear, zFar);
	matMult(matCurrent, fr, cur);
}

void GL_APIENTRY myglOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglOrthof(FixedToFloat(left), FixedToFloat(right), FixedToFloat(bottom), FixedToFloat(top), FixedToFloat(zNear), FixedToFloat(zFar));
}

void GL_APIENTRY myglPixelStorei(GLenum pname, GLint param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d)\n", enumName(pname), param);
	glPixelStorei(pname, param);
}

void GL_APIENTRY myglPointParameterf(GLenum pname, GLfloat param) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglPointParameterfv(GLenum pname, const GLfloat *params) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglPointParameterx(GLenum pname, GLfixed param) { log_printf(ll_all, __FUNCTION__ "\n");  }
void GL_APIENTRY myglPointParameterxv(GLenum pname, const GLfixed *params) { log_printf(ll_all, __FUNCTION__ "\n");  }

void GL_APIENTRY myglPointSize(GLfloat size)
{
	log_printf(ll_all, __FUNCTION__ "(%.06f)\n", size);
	shPointSize(size);
}

void GL_APIENTRY myglPointSizex(GLfixed size)
{
	log_printf(ll_all, __FUNCTION__ "(%d)\n", size);
	myglPointSize(FixedToFloat(size));
}

void GL_APIENTRY myglPolygonOffset(GLfloat factor, GLfloat units)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glPolygonOffset(factor, units);
}

void GL_APIENTRY myglPolygonOffsetx(GLfixed factor, GLfixed units)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglPolygonOffset(FixedToFloat(factor), FixedToFloat(units));
}

void GL_APIENTRY myglPopMatrix(void)
{
	if (*stackTop >= 0)
		memcpy(matCurrent, matStack[(*stackTop)--], 16*sizeof(float));
	log_printf(ll_all, __FUNCTION__ "\n");
}

void GL_APIENTRY myglPushMatrix(void)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	memcpy(matStack[++(*stackTop)], matCurrent, 16*sizeof(float));
}

void GL_APIENTRY myglReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glReadPixels(x, y, width, height, format, type, pixels);
}

void GL_APIENTRY myglRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	log_printf(ll_all, __FUNCTION__ "(%.6f, %.6f, %.6f, %.6f)\n", angle, x, y, z);
	if ((float)angle != 0)
		matRotate(matCurrent, angle, x, y, z);
}

void GL_APIENTRY myglRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglRotatef(FixedToFloat(angle), FixedToFloat(x), FixedToFloat(y), FixedToFloat(z));
}

void GL_APIENTRY myglSampleCoverage(GLclampf value, GLboolean invert)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glSampleCoverage(value, invert);
}

void GL_APIENTRY myglSampleCoveragex(GLclampx value, GLboolean invert)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glSampleCoverage(FixedToFloat(value), invert);
}

void GL_APIENTRY myglScalef(GLfloat x, GLfloat y, GLfloat z)
{
	log_printf(ll_all, __FUNCTION__ "(%.6f, %.6f, %.6f)\n", x, y, z);
/*	for (int i = 0; i < 16; i++)
	{
		log_printf(ll_extra, "\t%.03f", matCurrent[i]);
		if ((i & 3) == 3)
			log_printf(ll_extra, "\n");
	}
*/
	matScale(matCurrent, x, y, z);
/*
	log_printf(ll_extra, "=>\n");
	for (int i = 0; i < 16; i++)
	{
		log_printf(ll_extra, "\t%.03f", matCurrent[i]);
		if ((i & 3) == 3)
			log_printf(ll_extra, "\n");
	}
*/
}

void GL_APIENTRY myglScalex(GLfixed x, GLfixed y, GLfixed z)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglScalef(FixedToFloat(x), FixedToFloat(y), FixedToFloat(z));
}

void GL_APIENTRY myglScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glScissor(x, y, width, height);
}

void GL_APIENTRY myglShadeModel(GLenum mode) { log_printf(ll_all, __FUNCTION__ "(%s)\n", enumName(mode)); }

void GL_APIENTRY myglStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glStencilFunc(func, ref, mask);
}

void GL_APIENTRY myglStencilMask(GLuint mask)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glStencilMask(mask);
}

void GL_APIENTRY myglStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glStencilOp(fail, zfail, zpass);
}

void GL_APIENTRY myglTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	log_printf(ll_frame, __FUNCTION__ "(%d, %s, %d, 0x%x)\n", size, enumName(type), stride, pointer);

	if (array_buffer)
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(array_buffer);
		if (it != created_buffers.end())
			pointer = (char *)it->second.data + (size_t)pointer;
	}

	shClientAttrib(GL_TEXTURE_COORD_ARRAY, (clientTex == GL_TEXTURE0), size, type, stride, pointer);
}

int myglTexEnv(GLenum pname, GLint param)
{
	switch (pname)
	{
	case GL_TEXTURE_ENV_MODE:
		switch (param)
		{
		case GL_MODULATE:	return TEX_ENV_MODE_MODULATE;    
		case GL_REPLACE:	return TEX_ENV_MODE_REPLACE;
		case GL_DECAL:		return TEX_ENV_MODE_DECAL;
		case GL_BLEND:		return TEX_ENV_MODE_BLEND;
		case GL_ADD:		return TEX_ENV_MODE_ADD;
		case GL_COMBINE:	return TEX_ENV_MODE_COMBINE;
		}
		break;

	case GL_COMBINE_RGB:
		switch (param)
		{
		case GL_MODULATE:		return TEX_COMBINE_RGB_MODULATE;
		case GL_REPLACE:		return TEX_COMBINE_RGB_REPLACE;
		case GL_ADD_SIGNED:		return TEX_COMBINE_RGB_ADD_SIGNED;
		case GL_INTERPOLATE:	return TEX_COMBINE_RGB_INTERPOLATE;
		case GL_ADD:			return TEX_COMBINE_RGB_ADD;
		case GL_SUBTRACT:		return TEX_COMBINE_RGB_SUBTRACT;
		case GL_DOT3_RGB:		return TEX_COMBINE_RGB_DOT3_RGB;
		case GL_DOT3_RGBA:		return TEX_COMBINE_RGB_DOT3_RGBA;
		}
		break;

	case GL_COMBINE_ALPHA:
		switch (param)
		{
		case GL_MODULATE:		return TEX_COMBINE_ALPHA_MODULATE;
		case GL_REPLACE:		return TEX_COMBINE_ALPHA_REPLACE;
		case GL_ADD_SIGNED:		return TEX_COMBINE_ALPHA_ADD_SIGNED;
		case GL_INTERPOLATE:	return TEX_COMBINE_ALPHA_INTERPOLATE;
		case GL_ADD:			return TEX_COMBINE_ALPHA_ADD;
		case GL_SUBTRACT:		return TEX_COMBINE_ALPHA_SUBTRACT;
		}
		break;

	case GL_SRC0_RGB:
	case GL_SRC1_RGB:
	case GL_SRC2_RGB:
	case GL_SRC0_ALPHA:
	case GL_SRC1_ALPHA:
	case GL_SRC2_ALPHA:
		switch (param)
		{
		case GL_TEXTURE:		return TEX_ENV_COMBINER_SRC_TEXTURE;
		case GL_CONSTANT:		return TEX_ENV_COMBINER_SRC_CONSTANT;
		case GL_PRIMARY_COLOR:	return TEX_ENV_COMBINER_SRC_PRIMARY_COLOR;
		case GL_PREVIOUS:		return TEX_ENV_COMBINER_SRC_PREVIOUS;
		}
		break;

	case GL_OPERAND0_RGB:
	case GL_OPERAND1_RGB:
	case GL_OPERAND2_RGB:
		switch (param)
		{
		case GL_SRC_COLOR:				return TEX_ENV_COMBINER_RGB_OP_SRC_COLOR;
		case GL_ONE_MINUS_SRC_COLOR:	return TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_COLOR;
		case GL_SRC_ALPHA:				return TEX_ENV_COMBINER_RGB_OP_SRC_ALPHA;
		case GL_ONE_MINUS_SRC_ALPHA:	return TEX_ENV_COMBINER_RGB_OP_ONE_MINUS_SRC_ALPHA;
		}
		break;

	case GL_OPERAND0_ALPHA:
	case GL_OPERAND1_ALPHA:
	case GL_OPERAND2_ALPHA:
		switch (param)
		{
		case GL_SRC_ALPHA:				return TEX_ENV_COMBINER_ALPHA_OP_SRC_ALPHA;
		case GL_ONE_MINUS_SRC_ALPHA:	return TEX_ENV_COMBINER_ALPHA_OP_ONE_MINUS_SRC_ALPHA;
		}
		break;
	}

	return 1;
}

void GL_APIENTRY myglTexEnvi(GLenum target, GLenum pname, GLint param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName(param));

	switch (pname)
	{
	case GL_TEXTURE_ENV_MODE:	TexEnv[texActive].mode = myglTexEnv(pname, param);				break;
	case GL_COMBINE_RGB:		TexEnv[texActive].combineRgb = myglTexEnv(pname, param);		break;
	case GL_COMBINE_ALPHA:		TexEnv[texActive].combineAlpha = myglTexEnv(pname, param);		break;
	case GL_SRC0_RGB:			TexEnv[texActive].srcRgb[0] = myglTexEnv(pname, param);			break;
	case GL_SRC1_RGB:			TexEnv[texActive].srcRgb[1] = myglTexEnv(pname, param);			break;
	case GL_SRC2_RGB:			TexEnv[texActive].srcRgb[2] = myglTexEnv(pname, param);			break;
	case GL_SRC0_ALPHA:			TexEnv[texActive].srcAlpha[0] = myglTexEnv(pname, param);		break;
	case GL_SRC1_ALPHA:			TexEnv[texActive].srcAlpha[1] = myglTexEnv(pname, param);		break;
	case GL_SRC2_ALPHA:			TexEnv[texActive].srcAlpha[2] = myglTexEnv(pname, param);		break;
	case GL_OPERAND0_RGB:		TexEnv[texActive].operandRgb[0] = myglTexEnv(pname, param);		break;
	case GL_OPERAND1_RGB:		TexEnv[texActive].operandRgb[1] = myglTexEnv(pname, param);		break;
	case GL_OPERAND2_RGB:		TexEnv[texActive].operandRgb[2] = myglTexEnv(pname, param);		break;
	case GL_OPERAND0_ALPHA:		TexEnv[texActive].operandAlpha[0] = myglTexEnv(pname, param);	break;
	case GL_OPERAND1_ALPHA:		TexEnv[texActive].operandAlpha[1] = myglTexEnv(pname, param);	break;
	case GL_OPERAND2_ALPHA:		TexEnv[texActive].operandAlpha[2] = myglTexEnv(pname, param);	break;
	case GL_RGB_SCALE:			TexEnv[texActive].rgbScale = (float)param;						break;
	case GL_ALPHA_SCALE:		TexEnv[texActive].alphaScale = (float)param;					break;
	}
}

void GL_APIENTRY myglTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName((GLenum)param));
	switch (pname)
	{
	case GL_RGB_SCALE:		TexEnv[texActive].rgbScale = param;			break;
	case GL_ALPHA_SCALE:	TexEnv[texActive].alphaScale = param;		break;
	default:				myglTexEnvi(target, pname, (GLint)((float)param));	break;
	}
}

void GL_APIENTRY myglTexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName((GLenum)*params));
	if (!params) return;

	switch (pname)
	{
	case GL_TEXTURE_ENV_COLOR:
		TexEnv[texActive].color[0] = params[0];
		TexEnv[texActive].color[1] = params[1];
		TexEnv[texActive].color[2] = params[2];
		TexEnv[texActive].color[3] = params[3];
		break;

	case GL_RGB_SCALE:
	case GL_ALPHA_SCALE:
		myglTexEnvf(target, pname, *params);
		break;

	default:
		myglTexEnvi(target, pname, (GLint)((float)*params));
		break;
	}
}

void GL_APIENTRY myglTexEnviv(GLenum target, GLenum pname, const GLint *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName((GLenum)*params));
	if (!params) return;

	switch (pname)
	{
	case GL_TEXTURE_ENV_COLOR:
		TexEnv[texActive].color[0] = (float)params[0];
		TexEnv[texActive].color[1] = (float)params[1];
		TexEnv[texActive].color[2] = (float)params[2];
		TexEnv[texActive].color[3] = (float)params[3];
		break;

	default:
		myglTexEnvi(target, pname, *params);
		break;
	}
}

void GL_APIENTRY myglTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName((GLenum)param));
	switch (pname)
	{
	case GL_RGB_SCALE:
	case GL_ALPHA_SCALE:
		myglTexEnvf(target, pname, FixedToFloat(param));
		break;

	default:
		myglTexEnvi(target, pname, param);
		break;
	}
}

void GL_APIENTRY myglTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName((GLenum)*params));
	if (!params) return;

	switch (pname)
	{
	case GL_TEXTURE_ENV_COLOR:
		TexEnv[texActive].color[0] = FixedToFloat(params[0]);
		TexEnv[texActive].color[1] = FixedToFloat(params[1]);
		TexEnv[texActive].color[2] = FixedToFloat(params[2]);
		TexEnv[texActive].color[3] = FixedToFloat(params[3]);
		break;

	case GL_RGB_SCALE:
	case GL_ALPHA_SCALE:
		myglTexEnvf(target, pname, FixedToFloat(*params));
		break;

	default:
		myglTexEnvi(target, pname, *params);
		break;
	}
}

void DumpTexture(GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
#ifndef DUMP_TEXTURES
	return;
#endif
	BITMAPINFOHEADER bmih;
	ZeroMemory(&bmih, sizeof(bmih));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = (format == GL_RGBA) ? 32 : 24;
	bmih.biCompression = BI_RGB;
	bmih.biClrUsed = 0;
	int pitch = (((bmih.biWidth * bmih.biBitCount + 31) & (~31)) >> 3);
	bmih.biSizeImage = pitch * bmih.biHeight;

	BITMAPFILEHEADER bfh;
	bfh.bfType = 'MB';
	bfh.bfSize = bmih.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

	GLushort *quad = (GLushort *)pixels;
	GLubyte *data = 0, *line;
	int x, y;

	if (type == GL_UNSIGNED_SHORT_4_4_4_4 || type == GL_UNSIGNED_SHORT_5_6_5)
	{
		data = (GLubyte *)malloc(bmih.biSizeImage);
		if (!data) return;

		pixels = data;
		if (type == GL_UNSIGNED_SHORT_5_6_5)
		{
			for (y = 0; y < height; y++)
				for (x = 0; x < width; x++)
				{
					*data++ = (((*quad & 0xf800) >> 11) * 255) / 31;
					*data++ = (((*quad & 0x07e0) >> 5) * 255) / 63;
					*data++ = ((*quad & 0x001f) * 255) / 31;
					quad++;
				}
		}
		else
		{
			for (y = 0; y < height; y++)
				for (x = 0; x < width; x++)
				{
					*data++ = ((*quad & 0xf000) >> 12) * 17;
					*data++ = ((*quad & 0x0f00) >> 8) * 17;
					*data++ = ((*quad & 0x00f0) >> 4) * 17;
					*data++ = (*quad & 0x000f) * 17;
					quad++;
				}
		}
		data = (GLubyte *)pixels;
	}

	char fname[256];
	sprintf(fname, "\\My Storage\\qtc\\%03d_%d_%dx%d%s.bmp", texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1], level, width, height, ((format == GL_RGBA) ? "a" : ""));

	FILE *fout = fopen(fname, "wb");
	fwrite(&bfh, sizeof(bfh), 1, fout);
	fwrite(&bmih, sizeof(bmih), 1, fout);
	int r, g, b, a, d = bmih.biBitCount >> 3;
	GLubyte *pl;
	x = width * d;
	for (line = (GLubyte *)pixels + x * (height - 1); line >= pixels; line -= x)
	{
		pl = line;
		for (y = 0; y < x; y += d)
		{
			r = *pl++;
			g = *pl++;
			b = *pl++;
			if (d > 3) a = *pl++;
			fputc(b, fout);
			fputc(g, fout);
			fputc(r, fout);
			if (d > 3) fputc(a, fout);
		}
		for (y = x; y < pitch; y++) fputc(0, fout);
	}
	fclose(fout);

	if (data) free(data);
}

GLubyte lookup_0_55[4096];
GLubyte lookup_0_56[8192];
GLubyte lookup_1_55[4096];
GLubyte lookup_1_56[8192];
GLubyte lookup_48[256];

void ReduceTexture1024(bool isAlpha, GLsizei width, GLsizei height, const GLvoid *in, GLubyte *out)
{
	int dx = (width > 512 ? width >> 9 : 1);
	int dy = (height > 512 ? height >> 9 : 1);
	int db = (isAlpha ? 4 : 3);
	int dd = 0, dw, dh;
	GLubyte *pin = (GLubyte *)in, *pcur, *prow;
//	GLubyte *pout = out;
	WORD *pout = (WORD*)out;
	int r, g, b, a;
	int dpitch = (width - dx) * db, ypitch = (width * (dy - 1)) * db, xpitch = dx * db;

	for (dw = 1; dw < dx; dw <<= 1) dd++;
	for (dw = 1; dw < dy; dw <<= 1) dd++;
	dh = (1 << (dd - 1));

	prow = pin;
	for (int y = 0; y < height; y += dy)
	{
		for (int x = 0; x < width; x += dx)
		{
			r = g = b = a = 0;
			pcur = prow;
			for (int j = 0; j < dy; j++)
			{
				for (int i = 0; i < dx; i++)
				{
					r += *pcur++;
					g += *pcur++;
					b += *pcur++;
					if (isAlpha)
						a += *pcur++;
				}
				pcur += dpitch;
			}

			r = (r + dh) >> dd;
			g = (g + dh) >> dd;
			b = (b + dh) >> dd;
			if (isAlpha)
				a = (a + dh) >> dd;

			if (isAlpha)
				*pout++ = (((WORD)(r >> 4)) << 12) +
					(((WORD)(g >> 4)) << 8) +
					(((WORD)(b >> 4)) << 4) +
					((WORD)(a >> 4));
			else
				*pout++ = (((WORD)(r >> 3)) << 11) +
					(((WORD)(g >> 2)) << 5) +
					((WORD)(b >> 3));

/*			*pout++ = r;
			*pout++ = g;
			*pout++ = b;
			if (isAlpha)
				*pout++ = a;
*/			prow += xpitch;
		}
		prow += ypitch;
	}
}

void GL_APIENTRY myglTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	log_printf(ll_extra, __FUNCTION__ "(%s, %d, %s, %d, %d, %d, %s, %s)\n", enumName(target), level, enumName(internalformat), width, height, border, enumName(format), enumName(type));
	GLubyte *tempTexBuf = 0;

	if ((width > 512 || height > 512) && type == GL_UNSIGNED_BYTE && pixels)
	{
		if ((tempTexBuf = (BYTE *)malloc((width > 512 ? 512 : width) * (height > 512 ? 512 : height) * 2)))
		{
			ReduceTexture1024((format == GL_RGBA), width, height, pixels, tempTexBuf);
			type = ((format == GL_RGBA) ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_SHORT_5_6_5);
			if (width > 512) width = 512;
			if (height > 512) height = 512;
			pixels = tempTexBuf;
		}
	}

	DumpTexture(level, width, height, format, type, pixels);
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);

	if (tempTexBuf) free(tempTexBuf);

	if (level == 0)
	{
		glRect size = { 0, 0, (float)width, (float)height };
		texSize[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = size;
		texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = size;

		if ((texMinFilter == GL_NEAREST_MIPMAP_NEAREST || texMinFilter == GL_LINEAR_MIPMAP_NEAREST ||
			texMinFilter == GL_NEAREST_MIPMAP_LINEAR || texMinFilter == GL_LINEAR_MIPMAP_LINEAR ||
			texMagFilter == GL_NEAREST_MIPMAP_NEAREST || texMagFilter == GL_LINEAR_MIPMAP_NEAREST ||
			texMagFilter == GL_NEAREST_MIPMAP_LINEAR || texMagFilter == GL_LINEAR_MIPMAP_LINEAR))
			glGenerateMipmap(GL_TEXTURE_2D);
	}
}

// <-- The code below this point, right until "The cut line" is an implementation of Chainfire's QTC decompression algorithm and CFC unpacking. -->

inline int clamp(float val)
{
	if (val < 0) return 0;
	if (val > 255) return 255;
	return int(val + 0.5f);
}

void InitQTClookup()
{
	int a0, a1;
	float b0, b1;
	int idx;

	for (a0 = 0; a0 < 32; a0++)
	{
		b0 = (a0 * 255) / (float)31.0f;

		for (a1 = 0; a1 < 32; a1++)
		{
			b1 = (a1 * 255) / (float)31.0f;
			idx = ((a0 << 5) + a1) << 2;

			lookup_0_55[idx + 0] = clamp(b0);
			lookup_0_55[idx + 1] = clamp(b0 + (b1 - b0) / 3.0f);
			lookup_0_55[idx + 2] = clamp(b0 + (2.0f * (b1 - b0)) / 3.0f);
			lookup_0_55[idx + 3] = clamp(b1);

			lookup_1_55[idx + 0] = 0;
			lookup_1_55[idx + 1] = clamp(b0 - b1 * 0.25f);
			lookup_1_55[idx + 2] = clamp(b0);
			lookup_1_55[idx + 3] = clamp(b1);
		}

		for (a1 = 0; a1 < 64; a1++)
		{
			b1 = (a1 * 255) / (float)63.0f;
			idx = ((a0 << 6) + a1) << 2;

			lookup_0_56[idx + 0] = clamp(b0);
			lookup_0_56[idx + 1] = clamp(b0 + (b1 - b0) / 3.0f);
			lookup_0_56[idx + 2] = clamp(b0 + (2.0f * (b1 - b0)) / 3.0f);
			lookup_0_56[idx + 3] = clamp(b1);

			lookup_1_56[idx + 0] = 0;
			lookup_1_56[idx + 1] = clamp(b0 - b1 * 0.25f);
			lookup_1_56[idx + 2] = clamp(b0);
			lookup_1_56[idx + 3] = clamp(b1);
		}
	}

	for (a0 = 0; a0 < 16; a0++)
		lookup_48[a0] = a0 * 17;
}

bool UnrollTextureATC(bool isAlphaSrc, bool isAlphaDst, bool isWord, GLsizei width, GLsizei height, const GLvoid *in, GLubyte *out)
{
	void *p = (void *)in;
	DWORD dw, *pdw, mixdown;
	WORD w, *pw;
	GLubyte *res = out, *pb;
	int i, x, y, ix, iy;
	GLuint alpha[16], pixels, method;
	GLubyte keyR0, keyG0, keyB0, keyR1, keyG1, keyB1;
	GLubyte a = 255, r, g, b;
	bool a1 = true;
	int ri, gi, bi;

	for (y = 0; y < height; y += 4)
		for (x = 0; x < width; x += 4)
		{
			if (isAlphaSrc)
			{
				pdw = (DWORD *)p;
				for (i = 0; i < 16; i++)
				{
					if ((i & 7) == 0)
						dw = *pdw++;
					alpha[i] = lookup_48[dw & 0xF];
					dw >>= 4;
				}
				p = pdw;
			}

			pw = (WORD *)p;
			w = *pw++;
			method = w >> 15;
			keyR0 = ((w & 0x7C00) >> 10);
			keyG0 = ((w & 0x03E0) >> 5);
			keyB0 = (w & 0x001F);
			w = *pw++;
			keyR1 = ((w & 0xF800) >> 11);
			keyG1 = ((w & 0x07E0) >> 5);
			keyB1 = (w & 0x001F);
			p = pw;

			pdw = (DWORD *)p;
			mixdown = *pdw++;
			p = pdw;

			ri = (((keyR0 << 5) + keyR1) << 2);
			gi = (((keyG0 << 6) + keyG1) << 2);
			bi = (((keyB0 << 5) + keyB1) << 2);

			for (i = 0; i < 16; i++)
			{
				pixels = (mixdown & 0x3);
				mixdown >>= 2;
				if (isAlphaSrc)
					a = alpha[i];

				if (method == 0)
				{
					r = lookup_0_55[ri + pixels];
					g = lookup_0_56[gi + pixels];
					b = lookup_0_55[bi + pixels];
				}
				else
				{
					r = lookup_1_55[ri + pixels];
					g = lookup_1_56[gi + pixels];
					b = lookup_1_55[bi + pixels];
				}

				ix = x + (i & 0x3);
				iy = y + (i >> 2);

//				if (isAlphaDst) a1 &= ((a == 255) || (a == 0));

				if (isWord)
				{
					pw = (WORD *)(res + ((iy * width + ix) * 2));
					if (isAlphaDst)
						*pw = (((WORD)(r >> 4)) << 12) +
							(((WORD)(g >> 4)) << 8) +
							(((WORD)(b >> 4)) << 4) +
							((WORD)(a >> 4));
					else
						*pw = (((WORD)(r >> 2)) << 11) +
							(((WORD)(g >> 3)) << 5) +
							((WORD)(b >> 2));
				}
				else
				{
					pb = res + ((iy * width + ix) * (isAlphaDst ? 4 : 3));
					*pb++ = r;
					*pb++ = g;
					*pb++ = b;
					if (isAlphaDst)
						*pb++ = a;
				}
			}
		}

	return a1;
}

#define CFC_MAGIC									0x31434643 // CFC1
#define CFC_FORMAT_COMPRESSED_QTC_RGBA_EXPLICIT_ALPHA	0x3001 // 0x14 from QTC
#define CFC_FORMAT_COMPRESSED_QTC_RGB					0x3002 // 0x15 from QTC
#define CFC_FORMAT_COMPRESSED_QTC_8_8_8_8				0x3003 // Alpha, glTexImage2D format,internalformat = GL_FORMAT_RGBA, type = GL_TYPE_8
#define CFC_FORMAT_COMPRESSED_QTC_X_8_8_8				0x3004 // no-Alpha, glTexImage2D format,internalformat = GL_FORMAT_RGB, type = GL_TYPE_8
#define CFC_FORMAT_COMPRESSED_QTC_5_6_5					0x3005 // no-Alpha, glTexImage2D format,internalformat = GL_FORMAT_RGB, type = GL_TYPE_5_6_5
#define CFC_FORMAT_COMPRESSED_QTC_4_4_4_4				0x3007 // Alpha, glTexImage2D format,internalformat = GL_FORMAT_RGBA, type = GL_TYPE_4_4_4_4
#define CFC_FORMAT_COMPRESSED_RGBA						0x3101 // use 0x14 in QTC
#define CFC_FORMAT_COMPRESSED_RGB						0x3102 // use 0x15 in QTC

HINSTANCE hZLib = NULL;
typedef int (*_uncompress)(PVOID dest, PDWORD destLen, PVOID source, DWORD sourceLen);
_uncompress uncompress = 0;

void GL_APIENTRY myglCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d, %s, %d, %d, %d, %d)\n", enumName(target), level, enumName(internalformat), width, height, border, imageSize);

	BYTE *tempTexData = 0;
	BYTE *tempTexBuf = 0;
	bool isWord = (height <= 512 && width <= 512);
	bool isCompressed = false;
	PVOID newData = 0;
 
	if (imageSize >= 24 && uncompress)
	{
		DWORD *buf = (DWORD*)data;
		if (*buf++ == CFC_MAGIC)
		{
			isCompressed = true;
			DWORD format = *buf++;
			int newWidth = *buf++;
			int newHeight = *buf++;
			DWORD compSize = *buf++;
			DWORD decompSize = *buf++;
			bool isCompressedTexture = false;
			GLenum textureType = GL_UNSIGNED_BYTE;

			if (format == CFC_FORMAT_COMPRESSED_QTC_RGBA_EXPLICIT_ALPHA)
			{
				internalformat = GL_ATC_RGBA_EXPLICIT_ALPHA_AMD;
				isCompressedTexture = true;
			}
			else if (format == CFC_FORMAT_COMPRESSED_QTC_RGB)
			{
				internalformat = GL_ATC_RGB_AMD;
				isCompressedTexture = true;
			}
			else if (format == CFC_FORMAT_COMPRESSED_RGBA)
				internalformat = GL_RGBA;
			else if (format == CFC_FORMAT_COMPRESSED_RGB)
				internalformat = GL_RGB;
			else if (format == CFC_FORMAT_COMPRESSED_QTC_8_8_8_8)
				internalformat = GL_RGBA;
			else if (format == CFC_FORMAT_COMPRESSED_QTC_X_8_8_8)
				internalformat = GL_RGB;
			else if (format == CFC_FORMAT_COMPRESSED_QTC_5_6_5)
			{
				internalformat = GL_RGB;
				textureType = GL_UNSIGNED_SHORT_5_6_5;
			}
			else if (format == CFC_FORMAT_COMPRESSED_QTC_4_4_4_4)
			{
				internalformat = GL_RGBA;
				textureType = GL_UNSIGNED_SHORT_4_4_4_4;
			}

			newData = malloc(decompSize);
			if (newData)
			{
				if (uncompress(newData, &decompSize, (PVOID)buf, compSize) == 0)
				{
					if (isCompressedTexture)
					{
						width = newWidth;
						height = newHeight;
						imageSize = decompSize;
						data = newData;
					}
					else
					{
						myglTexImage2D(target, level, internalformat, newWidth, newHeight, border, internalformat, textureType, newData);
						free(newData);
						return;
					}
				}
			}
		}
	}

// <--- The cut line --->

	if (internalformat == GL_ATC_RGB_AMD)
	{
		int texSize = width * height * 4;
		if ((tempTexData = (BYTE *)malloc(texSize < 64 ? 64 : texSize)))
		{
//			UnrollTextureATC(false, false, false, width, height, data, tempTexData);
//			myglTexImage2D(target, level, GL_RGB, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, tempTexData);

//			UnrollTextureATC(false, false, isWord, width, height, data, tempTexData);
//			myglTexImage2D(target, level, GL_RGB, width, height, border, GL_RGB, (isWord ? GL_UNSIGNED_SHORT_5_6_5 : GL_UNSIGNED_BYTE), tempTexData);

			UnrollTextureATC(false, true, false, width, height, data, tempTexData);
			DumpTexture(level, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tempTexData);

			int compSize = (((width*height + 15) & (~15)) >> 1);
			if ((tempTexBuf = (BYTE *)malloc(compSize)))
			{
				squish::CompressImage(tempTexData, width, height, tempTexBuf, squish::kDxt1 | squish::kColourRangeFit);
				glCompressedTexImage2D(target, level, GL_RGBA_S3TC_OES, width, height, border, compSize, tempTexBuf);
			}
		}
	}
	else if (internalformat == GL_ATC_RGBA_EXPLICIT_ALPHA_AMD)
	{
		int texSize = width * height * 4;
		if ((tempTexData = (BYTE *)malloc(texSize < 64 ? 64 : texSize)))
		{
			UnrollTextureATC(true, true, isWord, width, height, data, tempTexData);
			myglTexImage2D(target, level, GL_RGBA, width, height, border, GL_RGBA, (isWord ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_BYTE), tempTexData);
//			UnrollTextureATC(true, true, false, width, height, data, tempTexData);
//			myglTexImage2D(target, level, GL_RGBA, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, tempTexData);
		}
	}
	else if (internalformat == GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG || internalformat == GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG)
	{
		int texSize = width * height * 4;
		if ((tempTexData = (BYTE *)malloc(texSize < 64 ? 64 : texSize)))
		{
			PVRTCDecompress((void *)data, 0, width, height, tempTexData);
			myglTexImage2D(target, level, GL_RGBA, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, tempTexData);
		}
	}
	else if (internalformat == GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG || internalformat == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG)
	{
		int texSize = width * height * 4;
		if ((tempTexData = (BYTE *)malloc(texSize < 64 ? 64 : texSize)))
		{
			PVRTCDecompress((void *)data, 1, width, height, tempTexData);
			myglTexImage2D(target, level, GL_RGBA, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, tempTexData);
		}
	}
	else
	{
		glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);

		if ((texMinFilter == GL_NEAREST_MIPMAP_NEAREST || texMinFilter == GL_LINEAR_MIPMAP_NEAREST ||
			texMinFilter == GL_NEAREST_MIPMAP_LINEAR || texMinFilter == GL_LINEAR_MIPMAP_LINEAR ||
			texMagFilter == GL_NEAREST_MIPMAP_NEAREST || texMagFilter == GL_LINEAR_MIPMAP_NEAREST ||
			texMagFilter == GL_NEAREST_MIPMAP_LINEAR || texMagFilter == GL_LINEAR_MIPMAP_LINEAR) && level == 0)
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	if (tempTexData) free(tempTexData);
	if (tempTexBuf) free(tempTexBuf);
	if (newData) free(newData);

	if (level == 0)
	{
		glRect size = { 0, 0, (float)width, (float)height };
		texSize[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = size;
		texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = size;
	}
}

void GL_APIENTRY myglTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void GL_APIENTRY myglCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{
	log_printf(ll_all, __FUNCTION__ "\n");

	BYTE *tempTexData = 0;
	BYTE *tempTexBuf = 0;
	bool isWord = (height <= 512 && width <= 512);

	if (format == GL_ATC_RGB_AMD)
	{
		int texSize = width * height * 4;
		if ((tempTexData = (BYTE *)malloc(texSize < 64 ? 64 : texSize)))
		{
//			UnrollTextureATC(false, false, false, width, height, data, tempTexData);
//			myglTexSubImage2D(target, level, xoffset, yoffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, tempTexData);
			UnrollTextureATC(false, true, false, width, height, data, tempTexData);

			int compSize = (((width*height + 15) & (~15)) >> 1);
			if ((tempTexBuf = (BYTE *)malloc(compSize)))
			{
				squish::CompressImage(tempTexData, width, height, tempTexBuf, squish::kDxt1 | squish::kColourRangeFit);
				glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, GL_RGBA_S3TC_OES, compSize, tempTexBuf);
			}
		}
	}
	else if (format == GL_ATC_RGBA_EXPLICIT_ALPHA_AMD)
	{
		int texSize = width * height * 4;
		if ((tempTexData = (BYTE *)malloc(texSize < 64 ? 64 : texSize)))
		{
			UnrollTextureATC(true, true, isWord, width, height, data, tempTexData);
			myglTexSubImage2D(target, level, xoffset, yoffset, width, height, GL_RGBA, (isWord ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_BYTE), tempTexData);
//			UnrollTextureATC(true, true, false, width, height, data, tempTexData);
//			myglTexSubImage2D(target, level, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tempTexData);
		}
	}
	else
		glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);

	if (tempTexData) free(tempTexData);
	if (tempTexBuf) free(tempTexBuf);

	glRect size = { 0, 0, (float)width, (float)height };
	texSize[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = size;
	texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = size;
}

void GL_APIENTRY myglTexParameteri(GLenum target, GLenum pname, GLint param)
{
	log_printf(ll_extra, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName(param));
//	if (param == GL_NEAREST_MIPMAP_NEAREST || param == GL_NEAREST_MIPMAP_LINEAR) param = GL_NEAREST;
//	if (param == GL_LINEAR_MIPMAP_NEAREST || param == GL_LINEAR_MIPMAP_LINEAR) param = GL_LINEAR;
	texStoreParam(pname, param);
	glTexParameteri(target, pname, param);
}

void GL_APIENTRY myglTexParameteriv(GLenum target, GLenum pname, const GLint *param)
{
	log_printf(ll_extra, __FUNCTION__ "(%s, %s)\n", enumName(target), enumName(pname));
	if (param) texStoreParam(pname, *param);
	if (pname == GL_TEXTURE_CROP_RECT_OES)
	{
		glRect crop = { (float)param[0], (float)param[1], (float)param[2], (float)param[3] };
		texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = crop;
	}
	glTexParameteriv(target, pname, param);
}

void GL_APIENTRY myglTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	log_printf(ll_extra, __FUNCTION__ "(%s, %s, %s)\n", enumName(target), enumName(pname), enumName((GLenum)param));
	texStoreParam(pname, (GLint)((float)param));
	glTexParameterf(target, pname, param);
}

void GL_APIENTRY myglTexParameterfv(GLenum target, GLenum pname, const GLfloat *param)
{
	log_printf(ll_extra, __FUNCTION__ "\n");
	if (param) texStoreParam(pname, (GLint)((float)*param));
	if (pname == GL_TEXTURE_CROP_RECT_OES)
	{
		glRect crop = { param[0], param[1], param[2], param[3] };
		texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = crop;
	}
	glTexParameterfv(target, pname, param);
}

void GL_APIENTRY myglTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
	log_printf(ll_extra, __FUNCTION__ "\n");
	myglTexParameteri(target, pname, param);
}

void GL_APIENTRY myglTexParameterxv(GLenum target, GLenum pname, const GLfixed *param)
{
	log_printf(ll_extra, __FUNCTION__ "\n");
	if (pname == GL_TEXTURE_CROP_RECT_OES)
	{
		glRect crop = { FixedToFloat(param[0]), FixedToFloat(param[1]), FixedToFloat(param[2]), FixedToFloat(param[3]) };
		texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]] = crop;
	}
	else
		myglTexParameteriv(target, pname, param);
}

void GL_APIENTRY myglTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	log_printf(ll_all, __FUNCTION__ "(%.6f, %.6f, %.6f)\n", x, y, z);
	if ((float)x != 0 || (float)y != 0 || (float)z != 0)
		matTranslate(matCurrent, x, y, z);
}

void GL_APIENTRY myglTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglTranslatef(FixedToFloat(x), FixedToFloat(y), FixedToFloat(z));
}

void GL_APIENTRY myglVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	log_printf(ll_frame, __FUNCTION__ "(%d, %s, %d, 0x%x)\n", size, enumName(type), stride, pointer);

	if (array_buffer)
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(array_buffer);
		if (it != created_buffers.end())
			pointer = (char *)it->second.data + (size_t)pointer;
	}

	shClientAttrib(GL_VERTEX_ARRAY, true, size, type, stride, pointer);
}

void GL_APIENTRY myglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	log_printf(ll_app, __FUNCTION__ "(%d, %d, %d, %d)\n", x, y, width, height);

	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(current_draw);
	if (ps != window_surface.end())
	{
		RECT rc;
		GLsurface *s = &(ps->second);
		GetClientRect(s->window, &rc);
		if (rc.right - rc.left != s->width || rc.bottom - rc.top != s->height)
		{
			log_printf(ll_app, "New size: %d x %d\n", rc.right - rc.left, rc.bottom - rc.top);

			EGLSurface old_surf = s->new_surface;
			EGLSurface new_surf = eglCreateWindowSurface(s->dpy, s->config, s->window, s->attrib_list.empty() ? 0 : &(s->attrib_list[0]));
			eglMakeCurrent(current_dpy, new_surf, (current_read == current_draw ? new_surf : current_read), current_ctx);
			eglDestroySurface(s->dpy, old_surf);
			s->new_surface = new_surf;
			s->width = rc.right - rc.left;
			s->height = rc.bottom - rc.top;
			viewport_init = false;
		}
	}

	view_x = x;
	view_y = y;
	view_w = width;
	view_h = height;

	if (viewport_init)
	{
		proj_sx = width / viewport.width;
		proj_sy = height / viewport.height;
		proj_ox = (width - viewport.width + 2*(x - viewport.x)) / viewport.width;
		proj_oy = (height - viewport.height + 2*(y - viewport.y)) / viewport.height;
	}
	else
	{
		proj_sx = proj_sy = 1;
		proj_ox = proj_oy = 0;
		viewport.x = (float)x;
		viewport.y = (float)y;
		viewport.width = (float)width;
		viewport.height = (float)height;
		glViewport(x, y, width, height);
		viewport_init = true;
	}
}

#ifdef GL_OES_matrix_palette
void GL_APIENTRY myglCurrentPaletteMatrixOES(GLuint matrixpaletteindex)
{
	log_printf(ll_all, __FUNCTION__ "(%d)\n", matrixpaletteindex);
	if (matrixpaletteindex >= 32) return;
	curPalMat = matrixpaletteindex;
	if (matMode == GL_MATRIX_PALETTE_OES) matCurrent = MatrixPalette[curPalMat];
}

void GL_APIENTRY myglLoadPaletteFromModelViewMatrixOES(void)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	memcpy(matCurrent, ModelViewMatrix, sizeof(ModelViewMatrix));
}

void GL_APIENTRY myglMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	log_printf(ll_frame, __FUNCTION__ "(%d, %s, %d, 0x%x)\n", size, enumName(type), stride, pointer);

	if (array_buffer)
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(array_buffer);
		if (it != created_buffers.end())
			pointer = (char *)it->second.data + (size_t)pointer;
	}

	shClientAttrib(GL_MATRIX_INDEX_ARRAY_OES, true, size, type, stride, pointer);
}

void GL_APIENTRY myglWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	log_printf(ll_frame, __FUNCTION__ "(%d, %s, %d, 0x%x)\n", size, enumName(type), stride, pointer);

	if (array_buffer)
	{
		std::map<GLuint, GLbuffer>::iterator it = created_buffers.find(array_buffer);
		if (it != created_buffers.end())
			pointer = (char *)it->second.data + (size_t)pointer;
	}

	shClientAttrib(GL_WEIGHT_ARRAY_OES, true, size, type, stride, pointer);
}
#endif

void GL_APIENTRY myglPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer) { log_printf(ll_all, __FUNCTION__ "\n");  }

#ifdef GL_OES_draw_texture
void GL_APIENTRY myglDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
	log_printf(ll_all, __FUNCTION__ "(%d, %d, %d, %d, %d)\n", x, y, z, width, height);
	glRect tex = { (float)(x - 1), (float)(y - 1), (float)width, (float)height };
	glRect ts = texSize[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]];
	glRect sizes = { ts.width, ts.height, viewport.width, viewport.height };
	shDrawTex(tex, texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]], sizes, (float)z, viewZN, viewZF);
}

void GL_APIENTRY myglDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglDrawTexiOES(x, y, z, width, height);
}

void GL_APIENTRY myglDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	glRect tex = { x - 1, y - 1, width, height };
	glRect ts = texSize[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]];
	glRect sizes = { ts.width, ts.height, viewport.width, viewport.height };
	shDrawTex(tex, texCrop[texCurrent[activeTex == GL_TEXTURE0 ? 0 : 1]], sizes, z, viewZN, viewZF);
}

void GL_APIENTRY myglDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	myglDrawTexfOES(FixedToFloat(x), FixedToFloat(y), FixedToFloat(z), FixedToFloat(width), FixedToFloat(height));
}

void GL_APIENTRY myglDrawTexsvOES(GLshort *coords)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!coords) return;
	myglDrawTexsOES(coords[0], coords[1], coords[2], coords[3], coords[4]);
}

void GL_APIENTRY myglDrawTexivOES(GLint *coords)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!coords) return;
	myglDrawTexiOES(coords[0], coords[1], coords[2], coords[3], coords[4]);
}

void GL_APIENTRY myglDrawTexfvOES(GLfloat *coords)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!coords) return;
	myglDrawTexfOES(coords[0], coords[1], coords[2], coords[3], coords[4]);
}

void GL_APIENTRY myglDrawTexxvOES(GLfixed *coords)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (!coords) return;
	myglDrawTexxOES(coords[0], coords[1], coords[2], coords[3], coords[4]);
}
#endif

EGLint GL_APIENTRY myeglGetError(void)
{
	log_printf(ll_all, __FUNCTION__);
	GLenum res = eglGetError();
	log_printf(ll_all, " = %s\n", enumName(res));
	return res;
}

EGLDisplay GL_APIENTRY myeglGetDisplay(NativeDisplayType display)
{
	log_printf(ll_app, __FUNCTION__ "(0x%lx)\n", display);

	if (display == 0)
		display = EGL_DEFAULT_DISPLAY;

	return eglGetDisplay(display);
}

EGLBoolean GL_APIENTRY myeglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
	log_printf(ll_minimal, __FUNCTION__ "(0x%x, %d, %d)\n", dpy, (major ? *major : -1), (minor ? *minor : -1));
	if (major) *major = 0;
	if (minor) *minor = 0;
	return eglInitialize(dpy, major, minor);
}

EGLBoolean GL_APIENTRY myeglTerminate(EGLDisplay dpy)
{
	log_printf(ll_minimal, __FUNCTION__ "\n");
	return eglTerminate(dpy);
}

const char *GL_APIENTRY myeglQueryString(EGLDisplay dpy, EGLint name)
{
	log_printf(ll_app, __FUNCTION__ "(%s)", enumName(name));
	const char *res;
	if (name == EGL_EXTENSIONS)
		res = "EGL_KHR_image EGL_KHR_lock_surface";
	else
		res = eglQueryString(dpy, name);
	log_printf(ll_app, " = %s\n", (res ? res : "(none)"));
	return res;
}

void GL_APIENTRY *myeglGetColorBufferQUALCOMM()
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return NULL;
}

void GL_APIENTRY myglBindFramebuffer(GLenum target, GLuint framebuffer)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d)\n", enumName(target), framebuffer);
	glBindFramebuffer(target, framebuffer);
}

void GL_APIENTRY myglDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
	log_printf(ll_all, __FUNCTION__ "(%d, %d)\n", n, (framebuffers ? *framebuffers : 0));
	glDeleteFramebuffers(n, framebuffers);
}

void GL_APIENTRY myglGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
	log_printf(ll_all, __FUNCTION__ "(%d)");
	glGenFramebuffers(n, framebuffers);
	log_printf(ll_all, " = %d\n", (framebuffers ? *framebuffers : 0));
}

void GL_APIENTRY myglFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s, %d, %d)\n", enumName(target), enumName(attachment), enumName(textarget), texture, level);
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

GLenum GL_APIENTRY myglCheckFramebufferStatus(GLenum target)
{
	log_printf(ll_all, __FUNCTION__ "(%s)", enumName(target));
	GLenum res = glCheckFramebufferStatus(target);
	log_printf(ll_all, " = %s\n", enumName(res));
	return res;
}

void GL_APIENTRY myglBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %d)\n", enumName(target), renderbuffer);
	glBindRenderbuffer(target, renderbuffer);
}

void GL_APIENTRY myglDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
	log_printf(ll_all, __FUNCTION__ "(%d, %d)\n", n, (renderbuffers ? *renderbuffers : 0));
	glDeleteRenderbuffers(n, renderbuffers);
}

void GL_APIENTRY myglGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
	log_printf(ll_all, __FUNCTION__ "(%d)");
	glGenRenderbuffers(n, renderbuffers);
	log_printf(ll_all, " = %d\n", (renderbuffers ? *renderbuffers : 0));
}

void GL_APIENTRY myglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %d, %d)\n", enumName(target), enumName(internalformat), width, height);
	glRenderbufferStorage(target, internalformat, width, height);
}

void GL_APIENTRY myglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	log_printf(ll_all, __FUNCTION__ "(%s, %s, %s, %d)\n", enumName(target), enumName(attachment), enumName(renderbuffertarget), renderbuffer);
	glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void (GL_APIENTRY *myeglGetProcAddress(const char *procname))()
{
	log_printf(ll_all, __FUNCTION__ "(%s)\n", procname);
	if (!procname) return NULL;

	if (!strcmp(procname, "eglGetColorBufferQUALCOMM"))
	{
		log_printf(ll_all, "@0x%x\n", myeglGetColorBufferQUALCOMM);
		return (void (*)(void))myeglGetColorBufferQUALCOMM;
	}

	void (GL_APIENTRY *res)() = eglGetProcAddress(procname);

	if (!res)
	{
		int len = strlen(procname);
		if (len > 3 && len < 256 && !strcmp(procname + len - 3, "OES"))
		{
			if (!strcmp(procname, "glBindFramebufferOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglBindFramebuffer);
				return (void (GL_APIENTRY *)())myglBindFramebuffer;
			}

			if (!strcmp(procname, "glDeleteFramebuffersOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglDeleteFramebuffers);
				return (void (GL_APIENTRY *)())myglDeleteFramebuffers;
			}

			if (!strcmp(procname, "glGenFramebuffersOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglGenFramebuffers);
				return (void (GL_APIENTRY *)())myglGenFramebuffers;
			}

			if (!strcmp(procname, "glFramebufferTexture2DOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglFramebufferTexture2D);
				return (void (GL_APIENTRY *)())myglFramebufferTexture2D;
			}

			if (!strcmp(procname, "glCheckFramebufferStatusOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglCheckFramebufferStatus);
				return (void (GL_APIENTRY *)())myglCheckFramebufferStatus;
			}

			if (!strcmp(procname, "glBindRenderbufferOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglBindRenderbuffer);
				return (void (GL_APIENTRY *)())myglBindRenderbuffer;
			}

			if (!strcmp(procname, "glDeleteRenderbuffersOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglDeleteRenderbuffers);
				return (void (GL_APIENTRY *)())myglDeleteRenderbuffers;
			}

			if (!strcmp(procname, "glGenRenderbuffersOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglGenRenderbuffers);
				return (void (GL_APIENTRY *)())myglGenRenderbuffers;
			}

			if (!strcmp(procname, "glRenderbufferStorageOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglRenderbufferStorage);
				return (void (GL_APIENTRY *)())myglRenderbufferStorage;
			}

			if (!strcmp(procname, "glFramebufferRenderbufferOES"))
			{
				log_printf(ll_all, "@0x%x\n", myglFramebufferRenderbuffer);
				return (void (GL_APIENTRY *)())myglFramebufferRenderbuffer;
			}
		}
	}

	return res;
}

EGLBoolean GL_APIENTRY myeglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
	log_printf(ll_app, __FUNCTION__ "(0x%x, 0x%x, %d, 0x%x (%d))\n", dpy, configs, config_size, num_config, *num_config);
	return eglGetConfigs(dpy, configs, (config_size >= 4096 ? 15 : config_size), num_config);
}

// Workarounds for this routine were discovered by Chainfire and NuShrike.
EGLBoolean GL_APIENTRY myeglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
	log_printf(ll_app, __FUNCTION__ "(\n");
	for (const EGLint *attr = attrib_list; *attr != EGL_NONE; attr += 2)
		log_printf(ll_app, "%s,\t%d,\n", enumName(attr[0]), attr[1]);
	log_printf(ll_app, "%d, 0x%x (%d))\n", config_size, num_config, *num_config);

	EGLBoolean retval = 0;
	EGLint newAttribs[256], *pnew = newAttribs;
	const EGLint *oldAttribs = attrib_list;
	BOOL haveConfigID = FALSE;

	// Check for EGL_CONFIG_ID. We do not do the magic if EGL_CONFIG_ID is present, to allow for testing
	for (oldAttribs = attrib_list; *oldAttribs != EGL_NONE; oldAttribs++)
		if (*oldAttribs == EGL_CONFIG_ID)
		{
			haveConfigID = TRUE;
			break;
		}

	if (haveConfigID) // EGL_CONFIG_ID detected, just pass to original function
		eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
	else
	{
		// Override/add EGL_BLUE, EGL_GREEN and EGL_RED to 5/6/5, copy all other attribs
		*pnew++ = EGL_BLUE_SIZE;
		*pnew++ = 5;

		*pnew++ = EGL_GREEN_SIZE;
		*pnew++ = 6;

		*pnew++ = EGL_RED_SIZE;
		*pnew++ = 5;

		*pnew++ = EGL_DEPTH_SIZE;
		*pnew++ = 24;

		oldAttribs = attrib_list;
		while (*oldAttribs != EGL_NONE)
			if ((*oldAttribs == EGL_BLUE_SIZE) || (*oldAttribs == EGL_GREEN_SIZE) || (*oldAttribs == EGL_RED_SIZE))
				oldAttribs += 2; // skip EGL_BLUE, EGL_GREEN, EGL_RED
			else if (*oldAttribs == EGL_DEPTH_SIZE)
			{
				oldAttribs++;
				if (*oldAttribs++ == 0)
					newAttribs[7] = 0;
			}
			else // copy other attribs
			{
				*pnew++ = *oldAttribs++;
				*pnew++ = *oldAttribs++;
			}
		*pnew++ = EGL_NONE;

		retval = eglChooseConfig(dpy, newAttribs, configs, config_size, num_config);
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	return retval;
}

EGLBoolean GL_APIENTRY myeglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
	log_printf(ll_extra, __FUNCTION__ "(%s)\n", enumName(attribute));
	return eglGetConfigAttrib(dpy, config, attribute, value);
}

EGLSurface GL_APIENTRY myeglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list)
{
	WCHAR name[512];
	GetWindowText(window, name, 512);
	if (!wcscmp(name, L"Tower Defense") || !wcscmp(name, L"Flight Commander"))
		tower_defense = true;

	log_printf(ll_app, __FUNCTION__ "(0x%x, 0x%x (%d))\n", config, attrib_list, (attrib_list ? *attrib_list : 0));
	EGLSurface surf = eglCreateWindowSurface(dpy, config, window, attrib_list);

	GLsurface *s = &(window_surface[surf]);
	s->dpy = dpy;
	s->config = config;
	s->window = window;

	s->attrib_list.clear();
	if (attrib_list)
	{
		for (const EGLint *attr = attrib_list; *attr != EGL_NONE; attr++)
			s->attrib_list.push_back(*attr);
		s->attrib_list.push_back(EGL_NONE);
	}

	RECT rc;
	GetClientRect(window, &rc);
	s->width = rc.right - rc.left;
	s->height = rc.bottom - rc.top;
	s->new_surface = surf;

	log_printf(ll_app, "Initital size: %d x %d\n", s->width, s->height);

	return surf;
}

EGLSurface GL_APIENTRY myeglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint *attrib_list)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
}

EGLSurface GL_APIENTRY myeglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglCreatePbufferSurface(dpy, config, attrib_list);
}

EGLBoolean GL_APIENTRY myeglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(surface);
	if (ps != window_surface.end())
	{
		surface = ps->second.new_surface;
		window_surface.erase(ps);
	}

	return eglDestroySurface(dpy, surface);
}

EGLBoolean GL_APIENTRY myeglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(surface);
	if (ps != window_surface.end())
		surface = ps->second.new_surface;
	return eglQuerySurface(dpy, surface, attribute, value);
}

EGLBoolean GL_APIENTRY myeglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(surface);
	if (ps != window_surface.end())
		surface = ps->second.new_surface;
	return eglSurfaceAttrib(dpy, surface, attribute, value);
}

EGLBoolean GL_APIENTRY myeglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(surface);
	if (ps != window_surface.end())
		surface = ps->second.new_surface;
	return eglBindTexImage(dpy, surface, buffer);
}

EGLBoolean GL_APIENTRY myeglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(surface);
	if (ps != window_surface.end())
		surface = ps->second.new_surface;
	return eglReleaseTexImage(dpy, surface, buffer);
}

EGLBoolean GL_APIENTRY myeglSwapInterval(EGLDisplay dpy, EGLint interval)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglSwapInterval(dpy, interval);
}

EGLContext GL_APIENTRY myeglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list)
{
	log_printf(ll_app, __FUNCTION__ "(\n");
	if (attrib_list)
		for (const EGLint *attr = attrib_list; *attr != EGL_NONE; attr += 2)
			log_printf(ll_app, "%s,\t%d,\n", enumName(attr[0]), attr[1]);
	log_printf(ll_app, "0x%x, 0x%x, 0x%x (%d))\n", config, share_list, attrib_list, (attrib_list ? *attrib_list : 0));

	EGLContext curctx = eglCreateContext(dpy, config, share_list, attrib_list);
	if (!shadersReady) InitMyGLES();
	return curctx;
}

EGLBoolean GL_APIENTRY myeglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	if (shadersReady) CleanupMyGLES();
	return eglDestroyContext(dpy, ctx);
}

EGLBoolean GL_APIENTRY myeglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
	log_printf(ll_app, __FUNCTION__ "(0x%x, 0x%x, 0x%x, 0x%x)\n", dpy, draw, read, ctx);

	viewport_init = false;
	current_dpy = dpy;
	current_draw = draw;
	current_read = read;
	current_ctx = ctx;

	std::map<EGLSurface, GLsurface>::iterator ps;

	ps = window_surface.find(draw);
	if (ps != window_surface.end())
		draw = ps->second.new_surface;

	ps = window_surface.find(read);
	if (ps != window_surface.end())
		read = ps->second.new_surface;

	EGLBoolean res = eglMakeCurrent(dpy, draw, read, ctx);

	return res;
}

EGLContext GL_APIENTRY myeglGetCurrentContext(void)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglGetCurrentContext();
}

EGLSurface GL_APIENTRY myeglGetCurrentSurface(EGLint readdraw)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglGetCurrentSurface(readdraw);
}

EGLDisplay GL_APIENTRY myeglGetCurrentDisplay(void)
{
	log_printf(ll_all, __FUNCTION__);
	EGLDisplay res = eglGetCurrentDisplay();
	log_printf(ll_all, " = 0x%x\n", res);
	return res;
}

EGLBoolean GL_APIENTRY myeglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglQueryContext(dpy, ctx, attribute, value);
}

EGLBoolean GL_APIENTRY myeglWaitGL(void)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglWaitGL();
}

EGLBoolean GL_APIENTRY myeglWaitNative(EGLint engine)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	return eglWaitNative(engine);
}

EGLBoolean GL_APIENTRY myeglSwapBuffers(EGLDisplay dpy, EGLSurface draw)
{
	log_printf(ll_frame, __FUNCTION__ "\n");

	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(draw);
	if (ps != window_surface.end())
		draw = ps->second.new_surface;

	return eglSwapBuffers(dpy, draw);
}

EGLBoolean GL_APIENTRY myeglCopyBuffers(EGLDisplay dpy, EGLSurface surface, NativePixmapType target)
{
	log_printf(ll_all, __FUNCTION__ "\n");
	std::map<EGLSurface, GLsurface>::iterator ps = window_surface.find(surface);
	if (ps != window_surface.end())
		surface = ps->second.new_surface;
	return EGL_FALSE;//eglCopyBuffers(dpy, surface, target);
}

//GLbitfield GL_APIENTRY myglQueryMatrixxOES(GLfixed *mantissa, GLint *exponent) { log_printf(ll_all, __FUNCTION__ "\n"); return glQueryMatrixxOES(mantissa, exponent); }
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls((HMODULE) hModule);
		hZLib = LoadLibrary(L"zlib_mn.dll");
		if (hZLib != NULL)
			uncompress = (_uncompress)GetProcAddress(hZLib, L"uncompress");
		InitQTClookup();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
#ifdef MYGL_LOG
#ifndef HEAVY_LOG
		FILE *flog = fopen("\\My Storage\\gles_log.txt", "w");
		std::list<std::string>::iterator it, end = loglist.end();
		for (it = loglist.begin(); it != end; ++it)
			fputs(it->c_str(), flog);
		fclose(flog);
		loglist.clear();
#endif
#endif
		if (hZLib != NULL)
			FreeLibrary(hZLib);
	}

	return TRUE;
}
