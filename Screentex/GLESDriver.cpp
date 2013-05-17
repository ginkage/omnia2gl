#include <GLES/gl.h>
#include <EGL/egl.h>
//#include "../libgles_cm/libgles_cm.h"

#include "GLESDriver.h"

#pragma comment(lib, "libGLES_CM.lib")
//#pragma comment(lib, "libGLESv1_CM.lib")
//#pragma comment(lib, "libEGL.lib")

C3DDriver *CreateGLESDriver()
{
	return new CGLESDriver;
}

CGLESDriver::CGLESDriver()
{
	m_EGLSurface = EGL_NO_SURFACE;
	m_EGLContext = EGL_NO_CONTEXT;
	m_EGLDisplay = EGL_NO_DISPLAY;
}

CGLESDriver::~CGLESDriver()
{
	Cleanup();
}

HRESULT CGLESDriver::Initialize(HWND hWnd)
{
	EGLConfig eglConfig;
	EGLint numConfigs;	

	const EGLint configAttribs[] =
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     0,
		EGL_DEPTH_SIZE,     16,
		EGL_STENCIL_SIZE,   EGL_DONT_CARE,
		EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
		EGL_NONE,           EGL_NONE
	};

	m_EGLDisplay = eglGetDisplay((NativeDisplayType)GetWindowDC(hWnd));
	if (m_EGLDisplay == EGL_NO_DISPLAY)
	{
		ASSERTMSG(L"eglGetDisplay() failed", 0);
		return NULL;
	}

	EGLint minor, major;
	if (eglInitialize(m_EGLDisplay, &major, &minor) == EGL_FALSE || eglGetError() != EGL_SUCCESS)
	{
		ASSERTMSG(L"eglInitialize() failed", 0);
		return NULL;
	}

	if (eglChooseConfig(m_EGLDisplay, configAttribs, &eglConfig, 1, &numConfigs) == EGL_FALSE || eglGetError() != EGL_SUCCESS)
	{
		ASSERTMSG(L"eglChooseConfig() failed", 0);
		return NULL;
	}

	if (numConfigs < 1)
	{
		ASSERTMSG(L"numConfigs < 1 failed", 0);
		return NULL;
	}

	m_EGLSurface = eglCreateWindowSurface(m_EGLDisplay, eglConfig, hWnd, NULL);
	if (m_EGLSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
	{
		ASSERTMSG(L"eglCreateWindowSurface() failed", 0);
		return NULL;
	}

	m_EGLContext = eglCreateContext(m_EGLDisplay, eglConfig, NULL, NULL);
	if (m_EGLContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
	{
		ASSERTMSG(L"eglCreateContext() failed", 0);
		return NULL;
	}

	if (!eglMakeCurrent(m_EGLDisplay, m_EGLSurface, m_EGLSurface, m_EGLContext) || eglGetError() != EGL_SUCCESS)
	{
		ASSERTMSG(L"eglMakeCurrent() failed", 0);
		return NULL;
	}

	glClearColorx(ZERO, ZERO, ZERO, ONE);
	glShadeModel(GL_FLAT);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);

	SetViewport(hWnd);

	return S_OK;
}

void CGLESDriver::Cleanup()
{
	if (m_EGLDisplay != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(m_EGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (m_EGLSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(m_EGLDisplay, m_EGLSurface);
			m_EGLSurface = EGL_NO_SURFACE;
		}

		if (m_EGLContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(m_EGLDisplay, m_EGLContext);
			m_EGLContext = EGL_NO_CONTEXT;
		}

		eglTerminate(m_EGLDisplay);
		m_EGLDisplay = EGL_NO_DISPLAY;
	}
}

void CGLESDriver::BeginDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CGLESDriver::EndDraw()
{
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	eglSwapBuffers(m_EGLDisplay, m_EGLSurface);
}

void CGLESDriver::PerspectiveMatrix(int width, int height, float fov, float zn, float zf)
{
	GLfloat zNear = 1.0f, zFar = 100.0f, aspect = ((GLfloat)width) / height;
	GLfloat ym = zNear * (GLfloat)tan(fov * 0.5f), xm = ym * aspect;
	GLfixed xmax = FixedFromFloat(xm), ymax = FixedFromFloat(ym);
	glFrustumx(-xmax, xmax, -ymax, ymax, FixedFromFloat(zNear), FixedFromFloat(zFar));
}

void CGLESDriver::SetViewport(HWND hWnd)
{
	RECT r;
	GetWindowRect(hWnd, &r);
	glViewport(0, 0, r.right - r.left, r.bottom - r.top);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	PerspectiveMatrix(r.right - r.left, r.bottom - r.top, PI*0.25f, 1, 100);
}

void CGLESDriver::ApplyMatrix(t_matrix m)
{
	if (m > mtx_count) return;
	C3DTransform& w = matrix[m];

	float x, y, z, a;
	C3DDriver::AxisAngle(m, &x, &y, &z, &a);

	glTranslatex(FixedFromFloat(-w.vx), FixedFromFloat(-w.vy), FixedFromFloat(-w.vz));

	if (fabs(a) < 1.0e-5)
		return;
	else
		glRotatex(FixedFromFloat(a*180/PI), FixedFromFloat(x), FixedFromFloat(y), FixedFromFloat(z));
}

void CGLESDriver::SetMatrices()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ApplyMatrix(mtx_view);
	ApplyMatrix(mtx_world);
}

struct GLESVertexArray
{
	GLfixed *vertices;
	GLfixed *normals;
	GLfixed *tex0;
};

void *CGLESDriver::CreateVertexArray(int num, bool normal, int tc)
{
	GLESVertexArray *arr = new GLESVertexArray;
	memset(arr, 0, sizeof(GLESVertexArray));
	arr->vertices = new GLfixed[num*3*sizeof(GLfixed)];
	if (normal)
		arr->normals = new GLfixed[num*3*sizeof(GLfixed)];
	if (tc >= 1)
		arr->tex0 = new GLfixed[num*2*sizeof(GLfixed)];
	vertex_array.push_back(arr);
	return arr;
}

void CGLESDriver::FillVertexArray(void *va, C3DVertex *v, int num)
{
	GLESVertexArray *arr = (GLESVertexArray *)va;
	if (!arr) return;

	int i;
	GLfixed *vertices = arr->vertices;
	GLfixed *normals = arr->normals;
	GLfixed *tex0 = arr->tex0;

	if (vertices)
		for (i = 0; i < num; i++)
		{
			*vertices++ = FixedFromFloat(v[i].vx);
			*vertices++ = FixedFromFloat(v[i].vy);
			*vertices++ = FixedFromFloat(v[i].vz);
		}

	if (normals)
		for (i = 0; i < num; i++)
		{
			*normals++ = FixedFromFloat(v[i].nx);
			*normals++ = FixedFromFloat(v[i].ny);
			*normals++ = FixedFromFloat(v[i].nz);
		}

	if (tex0)
		for (i = 0; i < num; i++)
		{
			*tex0++ = FixedFromFloat(v[i].tu0);
			*tex0++ = FixedFromFloat(v[i].tv0);
		}
}

void CGLESDriver::DrawVertexArray(void *va, t_drawmode mode, int num)
{
	GLESVertexArray *arr = (GLESVertexArray *)va;
	if (!arr) return;

	if (arr->vertices)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FIXED, 0, arr->vertices);
	}

	if (arr->tex0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FIXED, 0, arr->tex0);
	}

	switch (mode)
	{
	case dm_list:
		glDrawArrays(GL_TRIANGLES, 0, num);
		break;
	case dm_strip:
		glDrawArrays(GL_TRIANGLE_STRIP, 0, num);
		break;
	case dm_fan:
		glDrawArrays(GL_TRIANGLE_FAN, 0, num);
		break;
	}
}

void CGLESDriver::DeleteVertexArray(void *va)
{
	GLESVertexArray *arr = (GLESVertexArray *)va;
	if (!arr) return;

	if (arr->vertices)
		delete[] arr->vertices;
	if (arr->normals)
		delete[] arr->normals;
	if (arr->tex0)
		delete[] arr->tex0;
	delete arr;
}

struct GLESTexture
{
	GLuint tex;
	int width;
	int height;
};

void *CGLESDriver::CreateTexture(UINT *width, UINT *height, BITMAPINFOHEADER *bih, void *data)
{
	GLESTexture *tex = new GLESTexture;
	glGenTextures(1, &(tex->tex));
	tex->width = *width;
	tex->height = *height;
	texture.push_back(tex);

	int bpitch = (((bih->biWidth * bih->biBitCount + 31) & (~31)) >> 3);
	int tpitch = ((tex->width * 3 + 3) & (~3)), ty = 0;
	BYTE *dst = new BYTE[tex->height*tpitch], *pd = dst;
	BYTE *src = (BYTE *)data + bih->biHeight*bpitch;

	memset(dst, 0, tex->height*tpitch);
	for (LONG y = 0; y < bih->biHeight; y++)
	{
		src -= bpitch;
		for (int x = 0; x < bpitch && x < tpitch; x += 3)
		{
			pd[x+0] = src[x+2];
			pd[x+1] = src[x+1];
			pd[x+2] = src[x+0];
		}

		if (y * tex->height >= ty * bih->biHeight) {
			pd += tpitch;
			ty++;
		}
	}

	glBindTexture(GL_TEXTURE_2D, tex->tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, dst);

	delete[] dst;

	return tex;
}

void CGLESDriver::SetTexture(void *tx)
{
	GLESTexture *tex = (GLESTexture *)tx;
	if (!tex) return;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->tex);
}

void CGLESDriver::DeleteTexture(void *tx)
{
	GLESTexture *tex = (GLESTexture *)tx;
	if (!tex) return;
	delete tex;
}
