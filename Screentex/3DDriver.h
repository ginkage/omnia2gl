#include <windows.h>
#include <list>

#define PRECISION 16
#define ONE	(1 << PRECISION)
#define ZERO 0
#define PI ((FLOAT) 3.141592654f)
inline int FixedFromInt(int value) {return value << PRECISION;};
inline int FixedFromFloat(float value) { return static_cast<int> (value * static_cast<float>(ONE) + 0.5f);};
inline int MultiplyFixed(int op1, int op2) { return (op1 * op2) >> PRECISION;};

class C3DTransform
{
public:
	float qw, qx, qy, qz;
	float vx, vy, vz;
	float sx, sy, sz;
};

class C3DVertex
{
public:
	float vx, vy, vz;
	float nx, ny, nz;
	float tu0, tv0;

	void Pos(float x, float y, float z) { vx = x; vy = y; vz = z; }
	void Normal(float x, float y, float z) { nx = x; ny = y; nz = z; }
	void TC0(float u, float v) { tu0 = u; tv0 = v; }
};

enum t_renderstate { rs_culling, rs_lighting, rs_zbuffer };
enum t_cullmode { cull_none, cull_cw, cull_ccw };
enum t_matrix { mtx_world, mtx_view, mtx_projection, mtx_count };
enum t_drawmode { dm_list, dm_strip, dm_fan };

class C3DDriver
{
public:
	C3DDriver() { ResetMatrix(mtx_world); ResetMatrix(mtx_view); ResetMatrix(mtx_projection); }
	virtual ~C3DDriver() { Cleanup(); }
	virtual HRESULT Initialize(HWND hWnd) { return S_OK; }
	virtual void Cleanup();
	virtual void BeginDraw() {}
	virtual void EndDraw() {}
	virtual void Enable(t_renderstate rs) {}
	virtual void Disable(t_renderstate rs) {}
	virtual void Set(t_renderstate rs, int val) {}
	virtual void SetViewport(HWND hWnd) {}

	void ResetMatrix(t_matrix m);
	virtual void RotateMatrix(t_matrix m, float x, float y, float z, float a);
	virtual void AxisAngle(t_matrix m, float *x, float *y, float *z, float *a);
	void TranslateMatrix(t_matrix m, float dx, float dy, float dz);
	void ScaleMatrix(t_matrix m, float sx, float sy, float sz);
	virtual void PerspectiveMatrix(float width, float height, float fov, float zn, float zf) {}
	virtual void SetMatrices() {}

	virtual void *CreateVertexArray(int num, bool normal, int tc) { return NULL; }
	virtual void FillVertexArray(void *va, C3DVertex *v, int num) {}
	virtual void DrawVertexArray(void *va, t_drawmode mode, int num) {}

	virtual void *CreateTexture(UINT *width, UINT *height, BITMAPINFOHEADER *bmh, void *data) { return NULL; }
	virtual void SetTexture(void *tx) {}

protected:
	virtual void DeleteVertexArray(void *va) {}
	virtual void DeleteTexture(void *tx) {}

protected:
	C3DTransform matrix[mtx_count];
	std::list<void *> vertex_array;
	std::list<void *> texture;
};

C3DDriver *CreateD3DMDriver();
C3DDriver *CreateD3DMFxDriver();
C3DDriver *CreateGLESDriver();
C3DDriver *CreateGLES2Driver();
