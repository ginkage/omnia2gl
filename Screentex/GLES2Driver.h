#include "3DDriver.h"

class CGLES2Driver : public C3DDriver
{
public:
	CGLES2Driver();
	~CGLES2Driver();
	HRESULT Initialize(HWND hWnd);
	void Cleanup();
	void BeginDraw();
	void EndDraw();

	void SetViewport(HWND hWnd);
	void PerspectiveMatrix(int width, int height, float fov, float zn, float zf);
	void RotateMatrix(t_matrix m, float x, float y, float z, float a) { C3DDriver::RotateMatrix(m, x, y, z, -a); }
	void AxisAngle(t_matrix m, float *x, float *y, float *z, float *a)
	{
		C3DDriver::AxisAngle(m, x, y, z, a);
		*a = - *a;
	}
	void ApplyMatrix(t_matrix m);
	void SetMatrices();

	void *CreateVertexArray(int num, bool normal, int tc);
	void FillVertexArray(void *va, C3DVertex *v, int num);
	void DrawVertexArray(void *va, t_drawmode mode, int num);

	void *CreateTexture(UINT *width, UINT *height, BITMAPINFOHEADER *bmh, void *data);
	void SetTexture(void *tx);

protected:
	void DeleteVertexArray(void *va);
	void DeleteTexture(void *tx);

private:
	EGLDisplay m_EGLDisplay;
	EGLSurface m_EGLSurface;
	EGLContext m_EGLContext;
	GLuint vs, fs, prog;
	float matProj[16], mvp[16];
};

