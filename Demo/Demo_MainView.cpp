#include "stdafx.h"
#include "Demo_MainView.h"

using namespace DirectX;

//-------------------------------------------------------------------------------------------------------------------
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct CBChangesEveryFrame
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4X4 mWorld;
	XMFLOAT4 vMeshColor;
};

//-------------------------------------------------------------------------------------------------------------------
const char* m_shaderString = 
"cbuffer cbChangesEveryFrame : register( b0 )\r\n"
"{\r\n"
"    matrix WorldViewProj;\r\n"
"    matrix World;\r\n"
"    float4 vMeshColor;\r\n"
"};\r\n"
"\r\n"
"struct VS_INPUT\r\n"
"{\r\n"
"    float4 Pos : POSITION;\r\n"
"};\r\n"
"\r\n"
"struct PS_INPUT\r\n"
"{\r\n"
"    float4 Pos : SV_POSITION;\r\n"
"};\r\n"
"\r\n"
"PS_INPUT VS( VS_INPUT input )\r\n"
"{\r\n"
"    PS_INPUT output = (PS_INPUT)0;\r\n"
"    output.Pos = mul( input.Pos, WorldViewProj );\r\n"
"    return output;\r\n"
"}\r\n"
"\r\n"
"float4 PS( PS_INPUT input) : SV_Target\r\n"
"{\r\n"
"    return vMeshColor;\r\n"
"}\r\n";


//-------------------------------------------------------------------------------------------------------------------
CMainView::CMainView()
	: m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pCBChangesEveryFrame(nullptr)
	, m_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f)
	, m_colorIndex(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
CMainView::~CMainView()
{
}

//-------------------------------------------------------------------------------------------------------------------
bool CMainView::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed)
{
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
bool CMainView::ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings)
{
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
HRESULT CMainView::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr = S_OK;

	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	V_RETURN(D3DCompile(m_shaderString, strlen(m_shaderString) + 1, nullptr, nullptr, nullptr, "VS", "vs_4_0", dwShaderFlags, 0, &pVSBlob, 0));

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pVSBlob);
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_pVertexLayout);
	SAFE_RELEASE(pVSBlob);
	if (FAILED(hr))
		return hr;

	// Set the input layout
	pd3dImmediateContext->IASetInputLayout(m_pVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	V_RETURN(D3DCompile(m_shaderString, strlen(m_shaderString) + 1, nullptr, nullptr, nullptr, "PS", "ps_4_0", dwShaderFlags, 0, &pPSBlob, 0));

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	SAFE_RELEASE(pPSBlob);
	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	V_RETURN(pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer));

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DWORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = indices;
	V_RETURN(pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer));

	// Set index buffer
	pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffers
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	V_RETURN(pd3dDevice->CreateBuffer(&bd, nullptr, &m_pCBChangesEveryFrame));

	// Initialize the world matrices
	m_World = XMMatrixIdentity();

	// Initialize the view matrix
	static const XMVECTORF32 s_Eye = { 0.0f, 3.0f, -6.0f, 0.f };
	static const XMVECTORF32 s_At = { 0.0f, 1.0f, 0.0f, 0.f };
	static const XMVECTORF32 s_Up = { 0.0f, 1.0f, 0.0f, 0.f };
	m_View = XMMatrixLookAtLH(s_Eye, s_At, s_Up);

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------------
HRESULT CMainView::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	// Setup the projection parameters
	float fAspect = static_cast<float>(pBackBufferSurfaceDesc->Width) / static_cast<float>(pBackBufferSurfaceDesc->Height);
	m_Projection = XMMatrixPerspectiveFovLH(XM_PI * 0.25f, fAspect, 0.1f, 100.0f);

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnFrameMove(double fTime, float fElapsedTime)
{
	// Rotate cube around the origin
	m_World = XMMatrixRotationY(60.0f * XMConvertToRadians((float)fTime));

	// Modify the color
	if (m_colorIndex == 0) {
		m_vMeshColor.x = (sinf((float)fTime * 1.0f) + 1.0f) * 0.5f;
		m_vMeshColor.y = (cosf((float)fTime * 3.0f) + 1.0f) * 0.5f;
		m_vMeshColor.z = (sinf((float)fTime * 5.0f) + 1.0f) * 0.5f;
	}
	else if(m_colorIndex == 1) {
		m_vMeshColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		m_vMeshColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	}

}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
	double fTime, float fElapsedTime)
{
	//
	// Clear the back buffer
	//
	auto pRTV = DXUTGetD3D11RenderTargetView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, Colors::MidnightBlue);

	//
	// Clear the depth stencil
	//
	auto pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	XMMATRIX mWorldViewProjection = m_World * m_View * m_Projection;

	// Update constant buffer that changes once per frame
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	V(pd3dImmediateContext->Map(m_pCBChangesEveryFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	auto pCB = reinterpret_cast<CBChangesEveryFrame*>(MappedResource.pData);
	XMStoreFloat4x4(&pCB->mWorldViewProj, XMMatrixTranspose(mWorldViewProjection));
	XMStoreFloat4x4(&pCB->mWorld, XMMatrixTranspose(m_World));
	pCB->vMeshColor = m_vMeshColor;
	pd3dImmediateContext->Unmap(m_pCBChangesEveryFrame, 0);

	//
	// Render the cube
	//
	pd3dImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &m_pCBChangesEveryFrame);
	pd3dImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &m_pCBChangesEveryFrame);
	pd3dImmediateContext->DrawIndexed(36, 0, 0);
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnD3D11ReleasingSwapChain(void)
{
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnD3D11DestroyDevice(void)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pCBChangesEveryFrame);
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainView::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	bool* pbNoFurtherProcessing)
{
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown)
{
	if (bKeyDown)
	{
		switch (nChar)
		{
		case VK_F1: // Change as needed                
			break;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
bool CMainView::OnDeviceRemoved(void)
{
	return true;
}
