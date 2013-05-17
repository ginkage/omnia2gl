#include <d3dm.h>
#include <d3dmx.h>
#include "D3DMDriver.h"

#pragma comment(lib, "d3dm.lib")
#pragma comment(lib, "d3dmx.lib")

C3DDriver *CreateD3DMDriver()
{
	return new CD3DMDriver;
}

CD3DMDriver::CD3DMDriver()
{
	m_pD3DM = NULL;
	m_pd3dmDevice = NULL;
}

CD3DMDriver::~CD3DMDriver()
{
	C3DDriver::Cleanup(); // We need to release everything prior to device release
	Cleanup();
}

HRESULT CD3DMDriver::Initialize(HWND hWnd)
{
	if (!(m_pD3DM = Direct3DMobileCreate(D3DM_SDK_VERSION)))
	{
		MessageBox(hWnd, L"Failed to init D3DM", L"Error", MB_OK);
		return E_FAIL;
	}

	HRESULT hr;
	D3DMPRESENT_PARAMETERS d3dmpp; 
	memset(&d3dmpp, 0, sizeof(d3dmpp));
	d3dmpp.Windowed = TRUE;
	d3dmpp.SwapEffect = D3DMSWAPEFFECT_DISCARD;
	d3dmpp.BackBufferFormat = D3DMFMT_R5G6B5;
	d3dmpp.EnableAutoDepthStencil = FALSE;
	d3dmpp.AutoDepthStencilFormat = D3DMFMT_D16;

	if (FAILED(hr = m_pD3DM->CreateDevice(D3DMADAPTER_DEFAULT, D3DMDEVTYPE_DEFAULT, hWnd, 0, &d3dmpp, &m_pd3dmDevice)))
	{
		WCHAR buffer[256];
		wsprintf(buffer, L"Unable to create a D3DM device: %s",
			(hr == D3DMERR_INVALIDCALL ? L"D3DMERR_INVALIDCALL" :
			(hr == D3DMERR_INVALIDDEVICE ? L"D3DMERR_INVALIDDEVICE" :
			(hr == D3DMERR_NOTAVAILABLE ? L"D3DMERR_NOTAVAILABLE" :
			(hr == E_OUTOFMEMORY ? L"E_OUTOFMEMORY" : L"Unknown error")))));
		MessageBox(hWnd, buffer, L"Error", MB_OK);
		return E_FAIL;
	}

	m_pd3dmDevice->SetRenderState(D3DMRS_CULLMODE, D3DMCULL_NONE);
	m_pd3dmDevice->SetRenderState(D3DMRS_LIGHTING, FALSE);
//	m_pd3dmDevice->SetRenderState(D3DMRS_ZENABLE, TRUE);
	m_pd3dmDevice->SetRenderState(D3DMRS_TEXTUREPERSPECTIVE, TRUE);

	m_pd3dmDevice->SetTextureStageState(0, D3DMTSS_COLOROP, D3DMTOP_MODULATE);
	m_pd3dmDevice->SetTextureStageState(0, D3DMTSS_COLORARG1, D3DMTA_TEXTURE);
	m_pd3dmDevice->SetTextureStageState(0, D3DMTSS_COLORARG2, D3DMTA_DIFFUSE);
	m_pd3dmDevice->SetTextureStageState(0, D3DMTSS_ALPHAOP, D3DMTOP_DISABLE);

	SetViewport(hWnd);

	return S_OK;
}

void CD3DMDriver::Cleanup()
{
	if (m_pd3dmDevice != NULL)
		m_pd3dmDevice->Release();

	if (m_pD3DM != NULL)
		m_pD3DM->Release();
}

void CD3DMDriver::BeginDraw()
{
	m_pd3dmDevice->Clear(0, NULL, D3DMCLEAR_TARGET | D3DMCLEAR_ZBUFFER, D3DMCOLOR_XRGB(0,0,0), 1.0f, 0);
	m_pd3dmDevice->BeginScene();
}

void CD3DMDriver::EndDraw()
{
	m_pd3dmDevice->EndScene();
	m_pd3dmDevice->Present(NULL, NULL, NULL, NULL);
}

void CD3DMDriver::PerspectiveMatrix(int width, int height, float fov, float zn, float zf)
{
	D3DMXMATRIX matProj;
	D3DMXMatrixPerspectiveFovLH(&matProj, fov, ((float)width)/height, zn, zf);
	m_pd3dmDevice->SetTransform(D3DMTS_PROJECTION, (D3DMMATRIX*)&matProj, D3DMFMT_D3DMVALUE_FLOAT);
}

void CD3DMDriver::SetViewport(HWND hWnd)
{
	RECT r;
	GetWindowRect(hWnd, &r);
	PerspectiveMatrix(r.right - r.left, r.bottom - r.top, D3DMX_PI*0.25f, 1, 100);
}

void CD3DMDriver::ApplyMatrix(t_matrix m)
{
	if (m > mtx_count) return;
	C3DTransform& w = matrix[m];
	float n = (float)sqrt(w.qx*w.qx + w.qy*w.qy + w.qz*w.qz);
	float a = 2 * (float)atan2(n, w.qw);

    D3DMXMATRIX mat, rot, pos;
	D3DMXVECTOR3 axis(1, 0, 0);

	if (fabs(a) < 1.0e-5)
		D3DMXMatrixIdentity(&rot);
	if (fabs(a - PI) < 1.0e-5)
		D3DMXMatrixRotationAxis(&rot, &axis, D3DMX_PI);
	else
	{
		n = 1 / (1 - w.qw*w.qw);
		axis = D3DMXVECTOR3(w.qx * n, w.qy * n, w.qz * n);
		D3DMXMatrixRotationAxis(&rot, &axis, a);
	}

	D3DMXMatrixTranslation(&pos, w.vx, w.vy, w.vz);
	D3DMXMatrixMultiply(&mat, &rot, &pos);

	switch (m)
	{
	case mtx_world:
		m_pd3dmDevice->SetTransform(D3DMTS_WORLD, (D3DMMATRIX*)&mat, D3DMFMT_D3DMVALUE_FLOAT);
		break;
	case mtx_view:
		m_pd3dmDevice->SetTransform(D3DMTS_VIEW, (D3DMMATRIX*)&mat, D3DMFMT_D3DMVALUE_FLOAT);
		break;
	}
}

void CD3DMDriver::SetMatrices()
{
	ApplyMatrix(mtx_world);
	ApplyMatrix(mtx_view);
}

struct CUSTOMVERTEX
{
    D3DMXVECTOR3 position;
    D3DMXVECTOR3 normal;
    float tu, tv;
};

#define D3DMFVF_CUSTOMVERTEX (D3DMFVF_XYZ_FLOAT | D3DMFVF_NORMAL_FLOAT | D3DMFVF_TEX1 | D3DMFVF_TEXCOORDSIZE2(0) | D3DMFVF_TEXCOORDFLOAT(0))

struct D3DMVertexArray
{
	LPDIRECT3DMOBILEVERTEXBUFFER vb;
};

void *CD3DMDriver::CreateVertexArray(int num, bool normal, int tc)
{
	D3DMVertexArray *arr = new D3DMVertexArray;
	memset(arr, 0, sizeof(D3DMVertexArray));

	D3DMCAPS caps;
	if (FAILED(m_pd3dmDevice->GetDeviceCaps(&caps)))
		return NULL;
	D3DMPOOL vbpool = ((caps.SurfaceCaps & D3DMSURFCAPS_VIDVERTEXBUFFER) ? D3DMPOOL_VIDEOMEM : D3DMPOOL_SYSTEMMEM);

	if (FAILED(m_pd3dmDevice->CreateVertexBuffer(num*sizeof(CUSTOMVERTEX), 0, D3DMFVF_CUSTOMVERTEX, vbpool, &(arr->vb))))
		return NULL;

	vertex_array.push_back(arr);
	return arr;
}

void CD3DMDriver::FillVertexArray(void *va, C3DVertex *v, int num)
{
	D3DMVertexArray *arr = (D3DMVertexArray *)va;
	if (!arr || !arr->vb) return;

	CUSTOMVERTEX* pVertices;
	if (SUCCEEDED(arr->vb->Lock(0, 0, (void**)&pVertices, 0)))
	{
		int i;
		for (i = 0; i < num; i++)
		{
			pVertices[i].position = D3DMXVECTOR3(v[i].vx, v[i].vy, v[i].vz);
			pVertices[i].normal = D3DMXVECTOR3(v[i].nx, v[i].ny, v[i].nz);
			pVertices[i].tu = v[i].tu0;
			pVertices[i].tv = v[i].tv0;
		}

		arr->vb->Unlock();
	}
}

void CD3DMDriver::DrawVertexArray(void *va, t_drawmode mode, int num)
{
	D3DMVertexArray *arr = (D3DMVertexArray *)va;
	if (!arr || !arr->vb) return;

	m_pd3dmDevice->SetStreamSource(0, arr->vb, sizeof(CUSTOMVERTEX));

	switch (mode)
	{
	case dm_list:
		m_pd3dmDevice->DrawPrimitive(D3DMPT_TRIANGLESTRIP, 0, num / 3);
		break;
	case dm_strip:
		m_pd3dmDevice->DrawPrimitive(D3DMPT_TRIANGLESTRIP, 0, num - 2);
		break;
	case dm_fan:
		m_pd3dmDevice->DrawPrimitive(D3DMPT_TRIANGLESTRIP, 0, num - 2);
		break;
	}
}

void CD3DMDriver::DeleteVertexArray(void *va)
{
	D3DMVertexArray *arr = (D3DMVertexArray *)va;
	if (!arr) return;
	if (arr->vb)
		arr->vb->Release();
	delete arr;
}

struct D3DMTexture
{
	LPDIRECT3DMOBILETEXTURE texture;
	int width;
	int height;
};

void *CD3DMDriver::CreateTexture(UINT *width, UINT *height, BITMAPINFOHEADER *bih, void *data)
{
	D3DMTexture *tex = new D3DMTexture;

	D3DMCAPS caps;
	if (FAILED(m_pd3dmDevice->GetDeviceCaps(&caps)))
		return NULL;
	D3DMPOOL texpool = ((caps.SurfaceCaps & D3DMSURFCAPS_VIDTEXTURE) ? D3DMPOOL_VIDEOMEM : D3DMPOOL_SYSTEMMEM);        
	D3DMXCreateTexture(m_pd3dmDevice, *width, *height, 1, 0, D3DMFMT_R8G8B8, texpool, &(tex->texture));
	texture.push_back(tex);

	BITMAPFILEHEADER bfh;
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfOffBits = sizeof(bfh) + bih->biSize;
	bfh.bfSize = bfh.bfOffBits + bih->biSizeImage;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;

	BYTE *memFile = new BYTE[bfh.bfSize], *pf = memFile;
	memcpy(pf, &bfh, sizeof(bfh));
	pf += sizeof(bfh);
	memcpy(pf, bih, bih->biSize);
	pf += bih->biSize;
	memcpy(pf, data, bih->biSizeImage);

	LPDIRECT3DMOBILESURFACE pSurf = NULL;
	tex->texture->GetSurfaceLevel(0, &pSurf);

	D3DMSURFACE_DESC desc;
	pSurf->GetDesc(&desc);

	tex->width = *width = desc.Width;
	tex->height = *height = desc.Height;

	RECT rc, *prc = &rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = (bih->biWidth > tex->width ? tex->width : bih->biWidth);
	rc.bottom = (bih->biHeight > tex->height ? tex->height : bih->biHeight);

	D3DMXLoadSurfaceFromFileInMemory(pSurf, NULL, prc, memFile, bfh.bfSize, NULL, D3DMX_FILTER_POINT, 0, NULL);

	pSurf->Release();
	delete[] memFile;

	return tex;
}

void CD3DMDriver::SetTexture(void *tx)
{
	D3DMTexture *tex = (D3DMTexture *)tx;
	if (!tex || !tex->texture) return;
	m_pd3dmDevice->SetTexture(0, tex->texture);
}

void CD3DMDriver::DeleteTexture(void *tx)
{
	D3DMTexture *tex = (D3DMTexture *)tx;
	if (!tex) return;
	if (tex->texture)
		tex->texture->Release();
	delete tex;
}


C3DDriver *CreateD3DMFxDriver()
{
	return new CD3DMFxDriver;
}

void CD3DMFxDriver::PerspectiveMatrix(int width, int height, float fov, float zn, float zf)
{
	D3DMXMATRIXFXD matProj;
	D3DMXMatrixPerspectiveFovLHFxd(&matProj, D3DMXToFixed(fov), D3DMXToFixed(((float)width)/height), D3DMXToFixed(zn), D3DMXToFixed(zf));
	m_pd3dmDevice->SetTransform(D3DMTS_PROJECTION, (D3DMMATRIX*)&matProj, D3DMFMT_D3DMVALUE_FIXED);
}

void CD3DMFxDriver::ApplyMatrix(t_matrix m)
{
	if (m > mtx_count) return;
	C3DTransform& w = matrix[m];

	float x, y, z, a;
	AxisAngle(m, &x, &y, &z, &a);

    D3DMXMATRIXFXD mat, rot, pos;
	D3DMXVECTOR3FXD axis(x, y, z);

	if (fabs(a) < 1.0e-5)
		D3DMXMatrixIdentityFxd(&rot);
	else
		D3DMXMatrixRotationAxisFxd(&rot, &axis, D3DMXToFixed(a));

	D3DMXMatrixTranslationFxd(&pos, D3DMXToFixed(w.vx), D3DMXToFixed(w.vy), D3DMXToFixed(w.vz));
	D3DMXMatrixMultiplyFxd(&mat, &rot, &pos);

	switch (m)
	{
	case mtx_world:
		m_pd3dmDevice->SetTransform(D3DMTS_WORLD, (D3DMMATRIX*)&mat, D3DMFMT_D3DMVALUE_FIXED);
		break;
	case mtx_view:
		m_pd3dmDevice->SetTransform(D3DMTS_VIEW, (D3DMMATRIX*)&mat, D3DMFMT_D3DMVALUE_FIXED);
		break;
	}
}

struct CUSTOMVERTEX_FX
{
    D3DMXVECTOR3FXD position;
    D3DMXVECTOR3FXD normal;
    FIXED tu, tv;
};

#define D3DMFVF_CUSTOMVERTEX_FX (D3DMFVF_XYZ_FIXED | D3DMFVF_NORMAL_FIXED | D3DMFVF_TEX1 | D3DMFVF_TEXCOORDSIZE2(0) | D3DMFVF_TEXCOORDFIXED(0))

void *CD3DMFxDriver::CreateVertexArray(int num, bool normal, int tc)
{
	D3DMVertexArray *arr = new D3DMVertexArray;
	memset(arr, 0, sizeof(D3DMVertexArray));

	D3DMCAPS caps;
	if (FAILED(m_pd3dmDevice->GetDeviceCaps(&caps)))
		return NULL;
	D3DMPOOL vbpool = ((caps.SurfaceCaps & D3DMSURFCAPS_VIDVERTEXBUFFER) ? D3DMPOOL_VIDEOMEM : D3DMPOOL_SYSTEMMEM);

	if (FAILED(m_pd3dmDevice->CreateVertexBuffer(num*sizeof(CUSTOMVERTEX_FX), 0, D3DMFVF_CUSTOMVERTEX_FX, vbpool, &(arr->vb))))
		return NULL;

	vertex_array.push_back(arr);
	return arr;
}

void CD3DMFxDriver::FillVertexArray(void *va, C3DVertex *v, int num)
{
	D3DMVertexArray *arr = (D3DMVertexArray *)va;
	if (!arr || !arr->vb) return;

	CUSTOMVERTEX_FX* pVertices;
	if (SUCCEEDED(arr->vb->Lock(0, 0, (void**)&pVertices, 0)))
	{
		int i;
		for (i = 0; i < num; i++)
		{
			pVertices[i].position = D3DMXVECTOR3FXD(v[i].vx, v[i].vy, v[i].vz);
			pVertices[i].normal = D3DMXVECTOR3FXD(v[i].nx, v[i].ny, v[i].nz);
			pVertices[i].tu = D3DMXToFixed(v[i].tu0);
			pVertices[i].tv = D3DMXToFixed(v[i].tv0);
		}

		arr->vb->Unlock();
	}
}

void CD3DMFxDriver::DrawVertexArray(void *va, t_drawmode mode, int num)
{
	D3DMVertexArray *arr = (D3DMVertexArray *)va;
	if (!arr || !arr->vb) return;

	m_pd3dmDevice->SetStreamSource(0, arr->vb, sizeof(CUSTOMVERTEX_FX));

	switch (mode)
	{
	case dm_list:
		m_pd3dmDevice->DrawPrimitive(D3DMPT_TRIANGLESTRIP, 0, num / 3);
		break;
	case dm_strip:
		m_pd3dmDevice->DrawPrimitive(D3DMPT_TRIANGLESTRIP, 0, num - 2);
		break;
	case dm_fan:
		m_pd3dmDevice->DrawPrimitive(D3DMPT_TRIANGLESTRIP, 0, num - 2);
		break;
	}
}
