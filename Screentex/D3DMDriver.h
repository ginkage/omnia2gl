#include "3DDriver.h"

class CD3DMDriver : public C3DDriver
{
public:
	CD3DMDriver();
	~CD3DMDriver();
	HRESULT Initialize(HWND hWnd);
	void Cleanup();
	void BeginDraw();
	void EndDraw();

	void SetViewport(HWND hWnd);
	virtual void PerspectiveMatrix(int width, int height, float fov, float zn, float zf);
	virtual void ApplyMatrix(t_matrix m);
	void SetMatrices();

	virtual void *CreateVertexArray(int num, bool normal, int tc);
	virtual void FillVertexArray(void *va, C3DVertex *v, int num);
	virtual void DrawVertexArray(void *va, t_drawmode mode, int num);

	void *CreateTexture(UINT *width, UINT *height, BITMAPINFOHEADER *bmh, void *data);
	void SetTexture(void *tx);

protected:
	void DeleteVertexArray(void *va);
	void DeleteTexture(void *tx);

protected:
	LPDIRECT3DMOBILE m_pD3DM;
	LPDIRECT3DMOBILEDEVICE m_pd3dmDevice;
};

class CD3DMFxDriver : public CD3DMDriver
{
public:
	void PerspectiveMatrix(int width, int height, float fov, float zn, float zf);
	void ApplyMatrix(t_matrix m);
	void *CreateVertexArray(int num, bool normal, int tc);
	void FillVertexArray(void *va, C3DVertex *v, int num);
	void DrawVertexArray(void *va, t_drawmode mode, int num);
};
