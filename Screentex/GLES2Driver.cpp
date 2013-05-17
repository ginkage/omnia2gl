#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "GLES2Driver.h"

#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "libEGL.lib")

#include "./shader/Sample.frag.h"
#include "./shader/Sample.vert.h"

void matIdentity(float m[16])
{
	for (int i=0; i<16; i++)
		m[i] = (i%5==0) ? 1.0f : 0.0f;  //The first and every fifth element after that is 1.0 other are 0.0
}

bool matInverse(float inverse[16], const float src[16])
{
	float t;
	int i, j, k, swap;
	float tmp[4][4];

	matIdentity(inverse);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			tmp[i][j] = src[i*4+j];

	for (i = 0; i < 4; i++)
	{
		/* look for largest element in column. */
		swap = i;
		for (j = i + 1; j < 4; j++)
			if (fabs(tmp[j][i]) > fabs(tmp[i][i]))
				swap = j;
	    
		if (swap != i)
			/* swap rows. */
			for (k = 0; k < 4; k++)
			{
				t = tmp[i][k];
				tmp[i][k] = tmp[swap][k];
				tmp[swap][k] = t;
	            
				t = inverse[i*4+k];
				inverse[i*4+k] = inverse[swap*4+k];
				inverse[swap*4+k] = t;
			}
	    
		if (tmp[i][i] == 0)
		/* no non-zero pivot.  the matrix is singular, which
		   shouldn't happen.  This means the user gave us a bad
			matrix. */
			return false;
	    
		t = tmp[i][i];
		for (k = 0; k < 4; k++)
		{
			tmp[i][k] /= t;
			inverse[i*4+k] /= t;
		}

		for (j = 0; j < 4; j++)
			if (j != i)
			{
				t = tmp[j][i];
				for (k = 0; k < 4; k++)
				{
					tmp[j][k] -= tmp[i][k]*t;
					inverse[j*4+k] -= inverse[i*4+k]*t;
				}
			}
	}

	return true;
}

// m3=m1*m2
void matMultl(float *m3, const float *m1, const float *m2) 
{
	m3[0] = m2[0]*m1[0] + m2[4]*m1[1] + m2[8]*m1[2] + m2[12]*m1[3];
	m3[1] = m2[1]*m1[0] + m2[5]*m1[1] + m2[9]*m1[2] + m2[13]*m1[3];
	m3[2] = m2[2]*m1[0] + m2[6]*m1[1] + m2[10]*m1[2] + m2[14]*m1[3];
	m3[3] = m2[3]*m1[0] + m2[7]*m1[1] + m2[11]*m1[2] + m2[15]*m1[3];

	m3[4] = m2[0]*m1[4] + m2[4]*m1[5] + m2[8]*m1[6] + m2[12]*m1[7];
	m3[5] = m2[1]*m1[4] + m2[5]*m1[5] + m2[9]*m1[6] + m2[13]*m1[7];
	m3[6] = m2[2]*m1[4] + m2[6]*m1[5] + m2[10]*m1[6] + m2[14]*m1[7];
	m3[7] = m2[3]*m1[4] + m2[7]*m1[5] + m2[11]*m1[6] + m2[15]*m1[7];

	m3[8] = m2[0]*m1[8] + m2[4]*m1[9] + m2[8]*m1[10] + m2[12]*m1[11];
	m3[9] = m2[1]*m1[8] + m2[5]*m1[9] + m2[9]*m1[10] + m2[13]*m1[11];
	m3[10] = m2[2]*m1[8] + m2[6]*m1[9] + m2[10]*m1[10] + m2[14]*m1[11];
	m3[11] = m2[3]*m1[8] + m2[7]*m1[9] + m2[11]*m1[10] + m2[15]*m1[11];

	m3[12] = m2[0]*m1[12] + m2[4]*m1[13] + m2[8]*m1[14] + m2[12]*m1[15];
	m3[13] = m2[1]*m1[12] + m2[5]*m1[13] + m2[9]*m1[14] + m2[13]*m1[15];
	m3[14] = m2[2]*m1[12] + m2[6]*m1[13] + m2[10]*m1[14] + m2[14]*m1[15];
	m3[15] = m2[3]*m1[12] + m2[7]*m1[13] + m2[11]*m1[14] + m2[15]*m1[15];
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
	m[12] = m[0]*x + m[4]*y + m[8]*z + m[12];
	m[13] = m[1]*x + m[5]*y + m[9]*z + m[13];
	m[14] = m[2]*x + m[6]*y + m[10]*z + m[14];
	m[15] = m[3]*x + m[7]*y + m[11]*z + m[15];
}  

//rotate about arbit axis
BOOL matRotate(float *m, float DEGAngle, float x, float y, float z)
{
	float Sq = (float)sqrt(x*x + y*y + z*z);
	if (Sq > -FLT_EPSILON && Sq < FLT_EPSILON) //chk for divide by zero......
		return false;

	float inv = 1.0f/Sq;	
	x = x * inv;   
	y = y * inv;
	z = z * inv;

	float radian = (float)(PI/180) * DEGAngle;
	float f32c = (float)cos(radian); //TODO!! Optimize me
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

//will update the current matrix' m' with the tranalation vector x,y,z
void matTranslatel(float *m, const float x, const float y, const float z)
{
	m[3] = m[0] * x + m[1] * y + m[2] * z + m[3];
	m[7] = m[4] * x + m[5] * y + m[6] * z + m[7];
	m[10] = m[8] * x + m[9] * y + m[10] * z + m[11];
	m[15] = m[12] * x + m[13] * y + m[14] * z + m[15];
}  

void matScale(float *m, float x, float y, float z)
{
	m[0] *= x; m[4] *= y; m[8] *= z;// m[12] *= x ;  
	m[1] *= x; m[5] *= y; m[9] *= z; //m[13] *= y ;  
	m[2] *= x; m[6] *= y; m[10] *= z; //m[14] *= z ;  
}

//like glfrustum
bool matFrustum(float *m, float f32Left, float f32Right, float f32Bottom, float f32Top, float f32ZNear, float f32ZFar)
{
	float diff = f32Right - f32Left;
	if (diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
		return false;

	diff = f32Top - f32Bottom;
	if (diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
		return false;

	diff = f32ZFar - f32ZNear;
	if (diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
		return false;	   

	m[0] = float(2.0 * f32ZNear / (f32Right - f32Left));
	m[1] = m[2] = m[3] = 0;

	m[4] = 0;
	m[5] = float(2.0 * f32ZNear / (f32Top - f32Bottom));
	m[6] = m[7] = 0;

	m[8] = (f32Right + f32Left) / (f32Right - f32Left);
	m[9] = (f32Top + f32Bottom) / (f32Top - f32Bottom);
	m[10] = -((f32ZNear + f32ZFar) / (f32ZFar - f32ZNear) );
	m[11] = -1;

	m[12] = m[13] =0;
	m[14] = -((2 * f32ZNear * f32ZFar) / (f32ZFar - f32ZNear));
	m[15] = 0;

	return true;
}

C3DDriver *CreateGLES2Driver()
{
	return new CGLES2Driver;
}

CGLES2Driver::CGLES2Driver()
{
	m_EGLSurface = EGL_NO_SURFACE;
	m_EGLContext = EGL_NO_CONTEXT;
	m_EGLDisplay = EGL_NO_DISPLAY;
	vs = 0;
	fs = 0;
	prog = 0;
}

CGLES2Driver::~CGLES2Driver()
{
	Cleanup();
}

HRESULT CGLES2Driver::Initialize(HWND hWnd)
{
	EGLConfig eglConfig;
	EGLint numConfigs;

	const EGLint configAttribs[] =
	{
		EGL_BUFFER_SIZE,	EGL_DONT_CARE,
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     0,
		EGL_DEPTH_SIZE,     16,
		EGL_STENCIL_SIZE,   EGL_DONT_CARE,
		EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
		EGL_NONE
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

/*	EGLConfig configs[2];
	EGLint config_count;
	eglGetConfigs(m_EGLDisplay, configs, 2, &config_count);
	eglChooseConfig(m_EGLDisplay, configAttribs, configs, 2, &config_count);
*/
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

	m_EGLContext = eglCreateContext(m_EGLDisplay, eglConfig, EGL_NO_CONTEXT, NULL);
	if (m_EGLContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
	{
		ASSERTMSG(L"eglCreateContext() failed", 0);
		return NULL;
	}

	if (!eglMakeCurrent(m_EGLDisplay, m_EGLSurface, EGL_NO_SURFACE, m_EGLContext) || eglGetError() != EGL_SUCCESS)
	{
		ASSERTMSG(L"eglMakeCurrent() failed", 0);
		return NULL;
	}

	glClearColor(ZERO, ZERO, ZERO, ONE);

//	glClearColorx(ZERO, ZERO, ZERO, ONE);
//	glShadeModel(GL_FLAT);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);

	SetViewport(hWnd);

	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	prog = glCreateProgram();

	glAttachShader(prog, fs);
	glAttachShader(prog, vs);

	glShaderBinary(1, &vs, (GLenum)0, SampleVert, sizeof(int)*SampleVertLength);
	glShaderBinary(1, &fs, (GLenum)0, SampleFrag, sizeof(int)*SampleFragLength);

	glBindAttribLocation(prog, 0, "vertexPos");
	glBindAttribLocation(prog, 1, "texCoord");

	glLinkProgram(prog);
	glUseProgram(prog);

	return S_OK;
}

void CGLES2Driver::Cleanup()
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

		if (prog)
		{
			glDetachShader(prog, vs);
			glDetachShader(prog, fs);
		}

		if (vs)
		{
			glDeleteShader(vs);
			vs = 0;
		}

		if (fs)
		{
			glDeleteShader(fs);
			fs = 0;
		}

		if (prog)
		{
			glDeleteProgram(prog);
			prog = 0;
		}
	}
}

void CGLES2Driver::BeginDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CGLES2Driver::EndDraw()
{
//	glDisable(GL_TEXTURE_2D);
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	eglSwapBuffers(m_EGLDisplay, m_EGLSurface);
}

void CGLES2Driver::PerspectiveMatrix(int width, int height, float fov, float zn, float zf)
{
	GLfloat zNear = 1.0f, zFar = 100.0f, aspect = ((GLfloat)width) / height;
	GLfloat ym = zNear * (GLfloat)tan(fov * 0.5f), xm = ym * aspect;
//	GLfixed xmax = FixedFromFloat(xm), ymax = FixedFromFloat(ym);
//	glFrustumx(-xmax, xmax, -ymax, ymax, FixedFromFloat(zNear), FixedFromFloat(zFar));
	matFrustum(matProj, -xm, xm, -ym, ym, zNear, zFar);
}

void CGLES2Driver::SetViewport(HWND hWnd)
{
	RECT r;
	GetWindowRect(hWnd, &r);
	glViewport(0, 0, r.right - r.left, r.bottom - r.top);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
	PerspectiveMatrix(r.right - r.left, r.bottom - r.top, PI*0.25f, 1, 100);
}

void CGLES2Driver::ApplyMatrix(t_matrix m)
{
	if (m > mtx_count) return;
	C3DTransform& w = matrix[m];

	float x, y, z, a;
	C3DDriver::AxisAngle(m, &x, &y, &z, &a);

//	glTranslatex(FixedFromFloat(-w.vx), FixedFromFloat(-w.vy), FixedFromFloat(-w.vz));
	matTranslate(mvp, -w.vx, -w.vy, -w.vz);

	if (fabs(a) < 1.0e-5)
		return;
	else
//		glRotatex(FixedFromFloat(a*180/PI), FixedFromFloat(x), FixedFromFloat(y), FixedFromFloat(z));
		matRotate(mvp, a*180/PI, x, y, z);
}

void CGLES2Driver::SetMatrices()
{
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();

	matIdentity(mvp);
	ApplyMatrix(mtx_view);
	ApplyMatrix(mtx_world);

	float res[16];
	matMult(res, matProj, mvp);
	glUniformMatrix4fv(glGetUniformLocation(prog, "mvp"), 1, GL_FALSE, res);
}

struct GLESVertexArray
{
	GLfixed *vertices;
	GLfixed *normals;
	GLfixed *tex0;
};

void *CGLES2Driver::CreateVertexArray(int num, bool normal, int tc)
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

void CGLES2Driver::FillVertexArray(void *va, C3DVertex *v, int num)
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

void CGLES2Driver::DrawVertexArray(void *va, t_drawmode mode, int num)
{
	GLESVertexArray *arr = (GLESVertexArray *)va;
	if (!arr) return;

	if (arr->vertices)
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FIXED, GL_FALSE, 0, arr->vertices);
	}

	if (arr->tex0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FIXED, GL_FALSE, 0, arr->tex0);
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

void CGLES2Driver::DeleteVertexArray(void *va)
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

void *CGLES2Driver::CreateTexture(UINT *width, UINT *height, BITMAPINFOHEADER *bih, void *data)
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

void CGLES2Driver::SetTexture(void *tx)
{
	GLESTexture *tex = (GLESTexture *)tx;
	if (!tex) return;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->tex);
}

void CGLES2Driver::DeleteTexture(void *tx)
{
	GLESTexture *tex = (GLESTexture *)tx;
	if (!tex) return;
	delete tex;
}
