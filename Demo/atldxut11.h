#ifndef __ATL_DXUT_H__
#define __ATL_DXUT_H__

//           copyleft
//    http://www.thecodeway.com
//		ver 2.0

#pragma once

#ifndef __cplusplus
#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
#error atldxut.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
#error atlwin.h requires atlwin.h to be included first
#endif

#if (_WIN32_WINNT < _WIN32_WINNT_VISTA )
#error WTL requires Windows version 6.0 or higher
#endif

namespace WTL
{

/////////////////////////////////////////////////////////////////////////////
// CDXUTMessageLoop
class CDXUTMessageLoop : public CMessageLoop
{
public:
	BOOL OnIdle(int nIdleCount) {
		return !CMessageLoop::OnIdle(nIdleCount);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDXUT

template <class T>
class ATL_NO_VTABLE CDXUT11 : public CIdleHandler
{
public:
	// Message map and handlers
	BEGIN_MSG_MAP(CDXUT11<T>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

	static bool CALLBACK s_IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
		DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->IsD3D11DeviceAcceptable(AdapterInfo, Output, DeviceInfo, BackBufferFormat, bWindowed);
	}

	static bool CALLBACK s_ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->ModifyDeviceSettings(pDeviceSettings);
	}

	static HRESULT CALLBACK s_OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc);
	}

	static HRESULT CALLBACK s_OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc);
	}

	static void CALLBACK s_OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnFrameMove(fTime, fElapsedTime);
	}

	static void CALLBACK s_OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
		double fTime, float fElapsedTime, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnD3D11FrameRender(pd3dDevice, pd3dImmediateContext, fTime, fElapsedTime);
	}

	static void CALLBACK s_OnD3D11ReleasingSwapChain(void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnD3D11ReleasingSwapChain();
	}

	static void CALLBACK s_OnD3D11DestroyDevice(void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnD3D11DestroyDevice();
	}

	static LRESULT CALLBACK s_MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);

		LRESULT nResult = pT->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing);
		if (*pbNoFurtherProcessing)
			return nResult;
		
		if (uMsg == WM_NCDESTROY)
		{
			// clear out window handle
			pT->OnD3D11DestroyDevice();
			DXUTSetCallbackD3D11DeviceDestroyed(nullptr);
			pT->m_hWnd = NULL;
			*pbNoFurtherProcessing = true;
		}

		return 0;
	}

	static void CALLBACK s_OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnKeyboard(nChar, bKeyDown, bAltDown);
	}

	static bool CALLBACK s_OnDeviceRemoved(void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->OnDeviceRemoved();
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);

		// Set general DXUT callbacks
		DXUTSetCallbackFrameMove(s_OnFrameMove, pT);
		DXUTSetCallbackKeyboard(s_OnKeyboard, pT);
		DXUTSetCallbackMsgProc(s_MsgProc, pT);
		DXUTSetCallbackDeviceChanging(s_ModifyDeviceSettings, pT);
		DXUTSetCallbackDeviceRemoved(s_OnDeviceRemoved, pT);

		// Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
		DXUTSetCallbackD3D11DeviceAcceptable(s_IsD3D11DeviceAcceptable, pT);
		DXUTSetCallbackD3D11DeviceCreated(s_OnD3D11CreateDevice, pT);
		DXUTSetCallbackD3D11SwapChainResized(s_OnD3D11ResizedSwapChain, pT);
		DXUTSetCallbackD3D11FrameRender(s_OnD3D11FrameRender, pT);
		DXUTSetCallbackD3D11SwapChainReleasing(s_OnD3D11ReleasingSwapChain, pT);
		DXUTSetCallbackD3D11DeviceDestroyed(s_OnD3D11DestroyDevice, pT);

		DXUTInit(true, true, nullptr); // Parse the command line, show msgboxes on error, no extra command line params
		DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen
		DXUTSetWindow(pT->m_hWnd, pT->m_hWnd, pT->m_hWnd);
		// Only require 10-level hardware or later
		DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, 800, 600);

		return 0;
	}

	virtual BOOL OnIdle(void)
	{
		DXUTRender3DEnvironment();
		return FALSE;
	}

private:
	bool m_isDeviceCreated;

public:
	CDXUT11() : m_isDeviceCreated(false) { }
	virtual ~CDXUT11() {}
};

}

#endif
