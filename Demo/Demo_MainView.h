#pragma once

#include "atldxut11.h"

class CMainView : public CWindowImpl<CMainView>, public CDXUT11<CMainView>
{
public:
	DECLARE_WND_CLASS_EX(NULL, 0, -1)

	BEGIN_MSG_MAP(CMainView)
		CHAIN_MSG_MAP(CDXUT11<CMainView>);
	END_MSG_MAP()

public:
	void	SetColor(int index) { m_colorIndex = index; }
	int		GetColor(void) const { return m_colorIndex; }

public:
	bool	IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed);
	bool	ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings);
	HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	HRESULT	OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	void	OnFrameMove(double fTime, float fElapsedTime);
	void	OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);
	void	OnD3D11ReleasingSwapChain(void);
	void	OnD3D11DestroyDevice(void);
	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void	OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown);
	bool	OnDeviceRemoved(void);

private:
	ID3D11VertexShader*         m_pVertexShader;
	ID3D11PixelShader*          m_pPixelShader;
	ID3D11InputLayout*          m_pVertexLayout;
	ID3D11Buffer*               m_pVertexBuffer;
	ID3D11Buffer*               m_pIndexBuffer;
	ID3D11Buffer*               m_pCBChangesEveryFrame;
	DirectX::XMMATRIX           m_World;
	DirectX::XMMATRIX           m_View;
	DirectX::XMMATRIX           m_Projection;
	DirectX::XMFLOAT4           m_vMeshColor;

	int							m_colorIndex;

public:
	CMainView();
	virtual ~CMainView();
};
