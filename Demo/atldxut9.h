#ifndef __ATL_DXUT_H__
#define __ATL_DXUT_H__

//           copyleft
//    http://www.thecodeway.com

//		ver 0.1

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

#ifndef __ATLGDI_H__
	#error atlopengl.h requires atlgdi.h to be included first
#endif

#if (WINVER < 0x0400)
	#error WTL requires Windows version 4.0 or higher
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
class ATL_NO_VTABLE CDXUT9 : public CIdleHandler
{
public:
	// Message map and handlers
	BEGIN_MSG_MAP(CDXUT9<T>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	static HRESULT CALLBACK s_OnCreateDevice(IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->OnCreateDevice(pDevice, pBackBufferSurfaceDesc);
	}
	static HRESULT CALLBACK s_OnResetDevice(IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->OnResetDevice(pDevice, pBackBufferSurfaceDesc);
	}
	static void CALLBACK s_OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnFrameMove(fTime, fElapsedTime);
	}
	static void CALLBACK s_OnFrameRender(IDirect3DDevice9* pDevice, double time, float elapsedTime, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnFrameRender(pDevice, time, elapsedTime);
	}
	static void CALLBACK s_OnLostDevice(void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnLostDevice();
	}
	static void CALLBACK s_OnDestroyDevice(void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->OnDestroyDevice();
	}
	static LRESULT CALLBACK s_MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->DX_MesssgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing);
	}
	static void CALLBACK s_KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		pT->DX_KeyboardProc(nChar, bKeyDown, bAltDown);
	}
	static bool CALLBACK s_IsDeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
	{
		//disable fullscreen mode!
		if(!bWindowed) return false;

		T* pT = static_cast<T*>(pUserContext);
		return pT->IsDeviceAcceptable(pCaps, AdapterFormat, BackBufferFormat, bWindowed);
	}
	static bool CALLBACK s_ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->ModifyDeviceSettings(pDeviceSettings);
	}
	static void CALLBACK s_OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
	{
		T* pT = static_cast<T*>(pUserContext);
		return pT->OnGUIEvent(nEvent, nControlID, pControl);
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);

		//set callback
		DXUTSetCallbackD3D9DeviceCreated(s_OnCreateDevice, pT);
		DXUTSetCallbackD3D9DeviceReset(s_OnResetDevice, pT);
		DXUTSetCallbackD3D9DeviceLost(s_OnLostDevice, pT);
		DXUTSetCallbackD3D9FrameRender(s_OnFrameRender, pT);
		DXUTSetCallbackD3D9DeviceDestroyed(s_OnDestroyDevice, pT);
		DXUTSetCallbackFrameMove(s_OnFrameMove, pT);
		DXUTSetCallbackMsgProc(s_MsgProc, pT);
		DXUTSetCallbackKeyboard(s_KeyboardProc, pT);
		DXUTSetCallbackD3D9DeviceAcceptable(s_IsDeviceAcceptable, pT);
		
		DXUTInit(false, true);
		DXUTSetCursorSettings(true, true);

		DXUTCreateWindow();
		HWND d3dHWND = DXUTGetHWNDDeviceWindowed();

		//FIX STYLE
		LONG style = ::GetWindowLong(d3dHWND, GWL_STYLE);
		style &= ~(WS_OVERLAPPEDWINDOW);
		style |= WS_CHILD|WS_TABSTOP;
		::SetWindowLong(d3dHWND, GWL_STYLE, style);
		::SetParent(d3dHWND, pT->m_hWnd);

		::SetFocus(d3dHWND);
		return 0;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);

		RECT rcClient;
		GetClientRect(pT->m_hWnd, &rcClient);

		int clientWidth = rcClient.right - rcClient.left;
		int clientHeight = rcClient.bottom - rcClient.top;
		MoveWindow(DXUTGetHWNDDeviceWindowed(), 0, 0, clientWidth, clientHeight, true);

		if(clientWidth!=0 && clientHeight!=0)
		{
			if(!m_isDeviceCreated)
			{
				DXUTSetD3DVersionSupport(true, false);
				DXUTCreateDevice(true);

				m_isDeviceCreated = true;
			}
		}
		return 0;
	}

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return TRUE;
	}

	virtual BOOL OnIdle(void)
	{
		DXUTRender3DEnvironment();
		return FALSE;
	}

private:
	bool m_isDeviceCreated;

public:
	CDXUT9() : m_isDeviceCreated(false) { }
	virtual ~CDXUT9() {}
};

}//namespace WTL

#endif //__ATL_DXUT_H__
