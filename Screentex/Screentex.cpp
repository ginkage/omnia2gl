#pragma comment(linker, "/nodefaultlib:oldnames.lib")
#pragma comment(lib, "aygshell.lib")
//#pragma comment(lib, "SamsungMobileSDK_2.lib")

#include <math.h>
#include <aygshell.h>
#include "3DDriver.h"
//#include "smiAccelerometer.h"

typedef struct { float t, x, y; } time_coord;
typedef std::list<time_coord> coord_list;
coord_list g_Moves;

//HWND g_hWndMenuBar;
LONG g_ScrWidth, g_ScrHeight, g_TexWidth = 1, g_TexHeight = 1;
void *g_Texture = NULL;
void *g_Quad = NULL;

float g_xPress = 0, g_yPress = 0, g_DX = 0, g_DY = 0, g_dDX = 0, g_dDY = 0, g_prevDX = 0, g_prevDY = 0;
float g_curTime = 0, g_startTime = 0, g_endTime = 0;
DWORD LastTickCount = 0;

float g_x = 0, g_y = 0, g_z = 0, g_a = 0;

HRESULT InitGeometry(C3DDriver *driver)
{
	HDC hdc = GetDC(0);
	HDC m_hdcMem = CreateCompatibleDC(hdc);

	BITMAPINFO m_DIBinfo;
	ZeroMemory(&m_DIBinfo, sizeof(m_DIBinfo));
	BITMAPINFOHEADER& bmih = m_DIBinfo.bmiHeader;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = g_ScrWidth;
	bmih.biHeight = g_ScrHeight;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biClrUsed = 0;
	bmih.biSizeImage = (((bmih.biWidth * bmih.biBitCount + 31) & (~31)) >> 3) * g_ScrHeight;

	PVOID m_ppvBits = NULL;
	HBITMAP m_hBitmap = CreateDIBSection(NULL, &m_DIBinfo, DIB_RGB_COLORS, &m_ppvBits, NULL, 0);

	if (m_hBitmap != NULL && m_ppvBits != NULL)
	{
		SelectObject(m_hdcMem, m_hBitmap);
		if (BitBlt(m_hdcMem, 0, 0, bmih.biWidth, bmih.biHeight, hdc, 0, 0, SRCCOPY))
		{
			g_TexWidth = 1;
			while (g_TexWidth < bmih.biWidth && g_TexWidth < 512)
				g_TexWidth <<= 1;

			g_TexHeight = 1;
			while (g_TexHeight < bmih.biHeight && g_TexHeight < 512)
				g_TexHeight <<= 1;

			g_Texture = driver->CreateTexture((UINT*)&g_TexWidth, (UINT*)&g_TexHeight, &bmih, m_ppvBits);
		}

		DeleteObject(m_hBitmap);
	}

	DeleteDC(m_hdcMem);
	DeleteDC(hdc);

	g_Quad = driver->CreateVertexArray(4, false, 1);

	float u0 = 0.5f/g_TexWidth;
	float v0 = 0.5f/g_TexHeight;
	float u1 = (g_ScrWidth > g_TexWidth ? (1 - u0) : ((float)g_ScrWidth - 0.5f)/g_TexWidth);
	float v1 = (g_ScrHeight > g_TexHeight ? (1 - v0) : ((float)g_ScrHeight - 0.5f)/g_TexHeight);

	float aspect = ((float)g_ScrWidth)/g_ScrHeight;
	float pos = 20*aspect;

	C3DVertex vertices[4];
	vertices[0].Pos(-pos, 20, 0);
	vertices[0].TC0(u0, v0);
	vertices[1].Pos(-pos, -20, 0);
	vertices[1].TC0(u0, v1);
	vertices[2].Pos(pos, 20, 0);
	vertices[2].TC0(u1, v0);
	vertices[3].Pos(pos, -20, 0);
	vertices[3].TC0(u1, v1);

	driver->FillVertexArray(g_Quad, vertices, 4);

	return S_OK;
}

VOID NormalizePos()
{
	while (g_DX < -(int)g_ScrWidth)
		g_DX += 2*g_ScrWidth;
	while (g_DX > (int)g_ScrWidth)
		g_DX -= 2*g_ScrWidth;
	while (g_DY < -(int)g_ScrWidth)
		g_DY += 2*g_ScrWidth;
	while (g_DY > (int)g_ScrWidth)
		g_DY -= 2*g_ScrWidth;
}

VOID SetupMatrices(C3DDriver *driver)
{
	DWORD CurTickCount = GetTickCount(), TimePassed = CurTickCount - LastTickCount;
	g_curTime = CurTickCount / 1000.0f;

	if (g_endTime > 0)
	{
		g_dDX = 0;
		g_dDY = 0;
	}

	LastTickCount = CurTickCount;
	float dt = TimePassed / 1000.0f, ddx = -0.375f*g_ScrWidth, ddy = -0.375f*g_ScrWidth;

	if (g_dDX == 0) ddx = 0;
	else if (g_dDX < 0) ddx = -ddx;
	if (g_dDY == 0) ddy = 0;
	else if (g_dDY < 0) ddy = -ddy;

	if (g_dDX * (g_dDX + ddx*dt) < 0)
		g_dDX = 0;
	else
		g_dDX += ddx*dt;

	if (g_dDY * (g_dDY + ddy*dt) < 0)
		g_dDY = 0;
	else
		g_dDY += ddy*dt;

	g_DX += g_dDX*dt;
	g_DY += g_dDY*dt;

	NormalizePos();

	float dd = -0.5857864376269049511983112757903;
	if (g_curTime - g_startTime < 1)
		dd *= (1 - (FLOAT)cos((g_curTime - g_startTime) * PI)) * 0.5f;
	if (g_endTime > 0)
	{
		if (g_endTime > g_curTime)
			dd *= (1 - (FLOAT)cos((g_endTime - g_curTime) * PI)) * 0.5f;
		else
		{
			dd = 0;
			PostQuitMessage(0);
		}
	}
	dd -= 2.4142135623730950488016887242097f;

	driver->ResetMatrix(mtx_view);
	driver->TranslateMatrix(mtx_view, 0, 0, -dd*20);

/*	SmiAccelerometerVector accel;
	if (SmiAccelerometerGetVector(&accel) == SMI_SUCCESS)
	{
		float x1 = 0, y1 = 0, z1 = 1;
		float x2 = -accel.x, y2 = accel.y, z2 = accel.z, l2 = sqrtf(x2*x2 + y2*y2 + z2*z2);
		float dx = (y1*z2 - y2*z1), dy = (x2*z1 - x1*z2), dz = (x1*y2 - x2*y1), dl = sqrtf(dx*dx + dy*dy + dz*dz);
		float theta = (float)atan2(dl / l2, (x1*x2 + y1*y2 + z1*z2) / l2);
		if (fabsf(dl) > FLT_EPSILON)
		{
			driver->ResetMatrix(mtx_world);
			driver->RotateMatrix(mtx_world, dx / dl, dy / dl, dz / dl, -theta);
		}
	}
	else*/ if (g_endTime > 0)
	{
		if (g_x == 0 && g_y == 0 && g_z == 0)
			driver->AxisAngle(mtx_world, &g_x, &g_y, &g_z, &g_a);

		driver->ResetMatrix(mtx_world);
		if (g_endTime > g_curTime)
			driver->RotateMatrix(mtx_world, g_x, g_y, g_z, g_a*(g_endTime - g_curTime));
	}
	else
	{
		float xx = g_DX - g_prevDX;
		float yy = g_DY - g_prevDY;

		while (xx < -(int)g_ScrWidth)
			xx += 2*g_ScrWidth;
		while (xx > (int)g_ScrWidth)
			xx -= 2*g_ScrWidth;
		while (yy < -(int)g_ScrWidth)
			yy += 2*g_ScrWidth;
		while (yy > (int)g_ScrWidth)
			yy -= 2*g_ScrWidth;

		float ll = (float)_hypot(xx, yy);
		if (ll > 1.0e-5)
		{
			float aa = ll*PI/g_ScrWidth;
			ll = 1 / ll;
			driver->RotateMatrix(mtx_world, yy*ll, xx*ll, 0, aa);
		}

		g_prevDX = g_DX;
		g_prevDY = g_DY;
	}

	driver->SetMatrices();
}

VOID Render(C3DDriver *driver)
{
	driver->BeginDraw();
	SetupMatrices(driver);
	driver->SetTexture(g_Texture);
	driver->DrawVertexArray(g_Quad, dm_strip, 4);
	driver->EndDraw();
}

bool g_bActive = false;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static SHACTIVATEINFO s_sai;
	WORD xPos, yPos;
	time_coord tc;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		g_xPress = xPos;
		g_yPress = yPos;
		g_dDX = 0;
		g_dDY = 0;
		while (!g_Moves.empty())
			g_Moves.pop_front();
		break;

	case WM_MOUSEMOVE:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		tc.t = g_curTime;
		tc.x = xPos;
		tc.y = yPos;
		g_DX += g_xPress - xPos;
		g_DY += g_yPress - yPos;
		NormalizePos();
		g_xPress = xPos;
		g_yPress = yPos;
		g_Moves.push_back(tc);

		while (!g_Moves.empty())
		{
			if (g_curTime - g_Moves.front().t > 0.5)
				g_Moves.pop_front();
			else
				break;
		}
		break;

	case WM_LBUTTONUP:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		if (g_Moves.size() > 1)
		{
			g_dDX = (g_Moves.front().x - g_Moves.back().x)/(g_Moves.back().t - g_Moves.front().t);
			g_dDY = (g_Moves.front().y - g_Moves.back().y)/(g_Moves.back().t - g_Moves.front().t);
		}
//        PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;

	case WM_KEYDOWN:
	case WM_LBUTTONDBLCLK:
		g_endTime = g_curTime + 1;
//		PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;

	case WM_CREATE:
/*		SHMENUBARINFO mbi;
		memset(&mbi, 0, sizeof(SHMENUBARINFO));
		mbi.cbSize     = sizeof(SHMENUBARINFO);
		mbi.hwndParent = hWnd;
		mbi.nToolBarId = 0;
		mbi.hInstRes   = 0;

		if (!SHCreateMenuBar(&mbi)) 
			g_hWndMenuBar = NULL;
		else
			g_hWndMenuBar = mbi.hwndMB;
*/
		memset(&s_sai, 0, sizeof (s_sai));
		s_sai.cbSize = sizeof (s_sai);
		break;

	case WM_CLOSE:
		g_bActive = false;
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		g_bActive = false;
		PostQuitMessage(0);
		return 0;

	case WM_ACTIVATE:
		g_bActive = (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE);
        SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
		break;

	case WM_SETTINGCHANGE:
        SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
		break;

	case WM_WINDOWPOSCHANGED:
//		SetViewport(hWnd);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPTSTR szCmd, INT )
{
	HWND hWnd = FindWindow(TEXT("Screentex"), TEXT("Screenshot as texture"));
	if (hWnd) 
	{
		SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
		return 0;
	}

	WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, MsgProc, 0L, 0L, hInst, NULL,
		NULL, NULL, NULL, TEXT("Screentex") };
	RegisterClass(&wc);

	g_ScrWidth = GetSystemMetrics(SM_CXSCREEN);
	g_ScrHeight = GetSystemMetrics(SM_CYSCREEN);

	hWnd = CreateWindow(TEXT("Screentex"), TEXT("Screenshot as texture"),
		WS_VISIBLE | WS_SYSMENU, 0, 0, g_ScrWidth, g_ScrHeight, // CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, wc.hInstance, NULL);

/*	if (g_hWndMenuBar)
	{
		RECT rc;
		RECT rcMenuBar;

		GetWindowRect(hWnd, &rc);
		GetWindowRect(g_hWndMenuBar, &rcMenuBar);
		rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
		MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
	}
*/
	SHFullScreen(hWnd, SHFS_HIDESIPBUTTON | SHFS_HIDETASKBAR);

	C3DDriver *driver = CreateGLESDriver();
	if (driver && SUCCEEDED(driver->Initialize(hWnd)))
	{
		if (SUCCEEDED(InitGeometry(driver)))
		{
			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
			g_startTime = GetTickCount() / 1000.0f;

			MSG msg;
			memset(&msg, 0, sizeof(msg));
			do
			{
				while (g_bActive ? PeekMessage(&msg, 0, 0, 0, PM_REMOVE) : GetMessage(&msg, 0, 0, 0))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if (g_bActive)
					Render(driver);
				else
					PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
			while (WM_QUIT != msg.message);
		}
	}

	delete driver;
	DestroyWindow(hWnd);
	UnregisterClass(TEXT("Screentex"), hInst);

	return 0;
}
