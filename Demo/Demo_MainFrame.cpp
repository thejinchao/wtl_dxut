#include "stdafx.h"
#include "Demo_MainFrame.h"

//-------------------------------------------------------------------------------------------------------------------
CMainFrame::CMainFrame()
{
}

//-------------------------------------------------------------------------------------------------------------------
CMainFrame::~CMainFrame()
{
}

//-------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

//-------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::OnIdle(void)
{
	UISetCheck(ID_BUTTON_LIGHT1, m_view.getLightCounts()==1);
	UISetCheck(ID_BUTTON_LIGHT2, m_view.getLightCounts()==2);
	UISetCheck(ID_BUTTON_LIGHT3, m_view.getLightCounts()==3);

	UIUpdateToolBar();
	return FALSE;
}

//-------------------------------------------------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT /*lpCreateStruct*/)
{
	//remove old menu
	SetMenu(NULL);	

	// create toolbar and rebar
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	UIAddToolBar(hWndToolBar);

	// create view window
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	//add idle filter
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	pLoop->AddIdleHandler(&m_view);

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnButtonLight(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_view.setLightCounts(wID-ID_BUTTON_LIGHT1+1);
	return 0;
}
