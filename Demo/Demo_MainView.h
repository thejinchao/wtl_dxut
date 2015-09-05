#pragma once

#include "atldxut9.h"

class CMainView : public CWindowImpl<CMainView>, public CDXUT9<CMainView>
{
public:
	int getLightCounts(void) const;
	void setLightCounts(int counts);

public:
	DECLARE_WND_CLASS_EX(NULL, 0, -1)

	BEGIN_MSG_MAP(CMainView)
		CHAIN_MSG_MAP(CDXUT9<CMainView>);
	END_MSG_MAP()

	
	/******************************************************
		DXUT Callback
	*******************************************************/
	HRESULT OnCreateDevice(IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	HRESULT OnResetDevice(IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	void OnFrameMove(double fTime, float fElapsedTime);
	void OnFrameRender(IDirect3DDevice9* pDevice, double time, float fElapsedTime);
	void OnLostDevice(void);
	void OnDestroyDevice(void);
	bool IsDeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed);
	bool ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings);
	LRESULT DX_MesssgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void DX_KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown);
	void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl);

public:
	CMainView();
	virtual ~CMainView();
};

