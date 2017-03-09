// MainFrm.h : interface of the CMainFrame class
//

#define  WM_MYMSG WM_USER+100
#pragma once
#include "Interface\WorkSpaceBar.h"
#include "Project.h"

#include <io.h>
#include "RigidFrame.h"
#include "RigidFrameDoc.h"
#include "RigidFrameView.h"
#include "ChildFrm.h"
#include "Interface\RibbonListButton.h"

#include "Dialog objects\DlgNewPrj.h"
#include "Dialog objects\DlgRenameBridge.h"
#include "Dialog objects\DlgDelBridge.h"
class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CProject *Proj;
	CWorkSpaceBar  m_wndWorkSpace;

public:
	CMainFrame();
	virtual ~CMainFrame();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void InitialUpdateFrm(); //关闭打开的窗口并初始化程序界面
	void SavePrj(); //保存工程
	CBCGPRibbonBar* GetRibbonBar(){ return &m_wndRibbonBar; }

protected:
	CBCGPRibbonStatusBar	m_wndStatusBar;
	CBCGPRibbonMainButton	m_MainButton;
	CBCGPRibbonBar	        m_wndRibbonBar;
	CBCGPToolBarImages		m_PanelImages;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
	afx_msg void OnClose();
	afx_msg LRESULT OnRibbonCustomize(WPARAM wp, LPARAM lp);
	afx_msg void OnToolsOptions();
	afx_msg void OnViewWorkspace();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace2();
	afx_msg void OnUpdateViewWorkspace2(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	/*自定义消息处理*/
	afx_msg LRESULT OnMyMsg(WPARAM wp,LPARAM lp);

	virtual BOOL OnShowMDITabContextMenu(CPoint point, DWORD dwAllowedItems, BOOL bDrop);
	virtual CBCGPMDIChildWnd* CreateDocumentWindow(LPCTSTR lpcszDocName, CObject* /*pObj*/);
	BOOL CreateRibbonBar();
	void CreateThemeList();
	void CreateDetailList(CBCGPRibbonPaletteButton*);
	void CreateModelList(CBCGPRibbonPaletteButton*);

	void ShowOptions(int nPage);

	CStringArray	m_IBarThemes, m_HSBarThemes, m_OBarThemes;//体内预应力、横竖向预应力、体外预应力
	CStringArray	m_resDetail;

public:
	afx_msg void OnPrjOpen();
	afx_msg void OnPrjSave();
	afx_msg void OnPrjBarset();
	afx_msg void OnPrjMeterial();
	afx_msg void OnViewFullscreen();
	afx_msg void OnPrjNew();
	afx_msg void OnUpdatePrjSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBrgRename(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBrgDel(CCmdUI *pCmdUI);
	afx_msg void OnBrgRename();
	afx_msg void OnBrgDel();
	afx_msg void OnPrjClose();
	afx_msg void OnUpdatePrjClose(CCmdUI *pCmdUI);
	afx_msg void OnUserFeedback();
	afx_msg void OnSystemUpdate();
	afx_msg void OnHelpManaul();
	afx_msg void OnWindowTileHorz();
	afx_msg void OnWindowTileVert();
	afx_msg void OnPrjCloseall();
	afx_msg void OnPrjClosewndall();
};
