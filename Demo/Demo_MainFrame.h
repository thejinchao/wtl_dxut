#pragma once

#include "resource.h"
#include "Demo_MainView.h"

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>, public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle(void);

	/******************************************************
		Message Handle
	*******************************************************/
	BEGIN_MSG_MAP_EX(CMainFrame)
		MSG_WM_CREATE(OnCreate)

		COMMAND_RANGE_HANDLER(ID_BUTTON_LIGHT1, ID_BUTTON_LIGHT3, OnButtonLight)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	/******************************************************
		Update UI
	*******************************************************/
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_BUTTON_LIGHT1, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BUTTON_LIGHT2, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BUTTON_LIGHT3, UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()

	/******************************************************
		Message Handle Function
	*******************************************************/
	int OnCreate(LPCREATESTRUCT /*lpCreateStruct*/);
	LRESULT OnButtonLight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
private:
	CMainView m_view;		//Canvas View

public:
	CMainFrame();
	virtual ~CMainFrame();
};


