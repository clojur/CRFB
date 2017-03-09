// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* CStringToPConstChar(CString str)
{
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	char* pAscii = new char[len + 1];
	len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), pAscii, len + 1, NULL, NULL);
	pAscii[len] = 0;
	return (const char*)pAscii;
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_GROUP, OnMdiNewVertGroup)
	ON_COMMAND(ID_MDI_CANCEL, OnMdiCancel)
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_PRJ_OPEN, &CMainFrame::OnPrjOpen)
	ON_COMMAND(ID_PRJ_SAVE, &CMainFrame::OnPrjSave)
	ON_COMMAND(ID_PRJ_BARSET, &CMainFrame::OnPrjBarset)
	ON_COMMAND(ID_PRJ_METERIAL, &CMainFrame::OnPrjMeterial)
	ON_COMMAND(ID_VIEW_FULLSCREEN, &CMainFrame::OnViewFullscreen)
	ON_COMMAND(ID_PRJ_NEW, &CMainFrame::OnPrjNew)
	ON_UPDATE_COMMAND_UI(ID_PRJ_SAVE, &CMainFrame::OnUpdatePrjSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CMainFrame::OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_BRG_RENAME, &CMainFrame::OnUpdateBrgRename)
	ON_UPDATE_COMMAND_UI(ID_BRG_DEL, &CMainFrame::OnUpdateBrgDel)
	ON_COMMAND(ID_BRG_RENAME, &CMainFrame::OnBrgRename)
	ON_COMMAND(ID_BRG_DEL, &CMainFrame::OnBrgDel)
	ON_COMMAND(ID_PRJ_CLOSE, &CMainFrame::OnPrjClose)
	ON_UPDATE_COMMAND_UI(ID_PRJ_CLOSE, &CMainFrame::OnUpdatePrjClose)
	ON_COMMAND(ID_USER_FEEDBACK, &CMainFrame::OnUserFeedback)
	ON_COMMAND(ID_SYSTEM_UPDATE, &CMainFrame::OnSystemUpdate)
	ON_COMMAND(ID_HELP_MANAUL, &CMainFrame::OnHelpManaul)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, &CMainFrame::OnWindowTileHorz)
	ON_COMMAND(ID_WINDOW_TILE_VERT, &CMainFrame::OnWindowTileVert)
	ON_COMMAND(ID_PRJ_CLOSEALL, &CMainFrame::OnPrjCloseall)
	ON_COMMAND(ID_PRJ_CLOSEWNDALL, &CMainFrame::OnPrjClosewndall)
	/*自定义消息映射*/
	ON_MESSAGE(WM_MYMSG,&CMainFrame::OnMyMsg)
END_MESSAGE_MAP()


// CMainFrame construction/destruction
/*自定义消息处理函数*/
LRESULT CMainFrame::OnMyMsg(WPARAM wp, LPARAM lp)
{
	CRigidFrameDoc* Cfd=(CRigidFrameDoc*)this->GetActiveFrame()->GetActiveDocument();
	double wl=Cfd->RigidFrameBridgeExample->m_RoadData.m_WaterLevel;
	char str[20];
	sprintf_s(str, "%.2f", wl);
	m_wndWorkSpace.m_wndPropList.RemoveAll();
	CBCGPProp* pGroup0 = new CBCGPProp(_T("基础资料"));
	pGroup0->AddSubItem(new CBCGPProp(_T("水位(m)"), (UINT)m_wndWorkSpace.ChartTitle, str, _T("")));
	m_wndWorkSpace.m_wndPropList.AddProperty(pGroup0);

	auto info = (std::vector<CStringPair>*)lp;
	if (info != nullptr)
	{
		CBCGPProp* pGroup1 = new CBCGPProp(_T("工程属性"));
		std::vector<CStringPair>::iterator iter = info->begin();
		for (; iter != info->end(); ++iter)
		{
			pGroup1->AddSubItem(new CBCGPProp(iter->key, (UINT)m_wndWorkSpace.ChartTitle, iter->value.GetString(), _T("")));
		}
		m_wndWorkSpace.m_wndPropList.AddProperty(pGroup1);
	}
		
	return 0;
}
CMainFrame::CMainFrame()
{
	Proj = nullptr;
	CreateThemeList();
}

CMainFrame::~CMainFrame()
{
	if (Proj != nullptr)
	{
		delete Proj;
		Proj = nullptr;
	}
}


void CMainFrame::CreateThemeList()
{
	m_IBarThemes.RemoveAll();

	m_IBarThemes.Add(_T("顶板束"));
	m_IBarThemes.Add(_T("下弯束"));
	m_IBarThemes.Add(_T("边跨合龙顶板束"));
	m_IBarThemes.Add(_T("边跨合龙底板束"));
	m_IBarThemes.Add(_T("中跨合龙顶板束"));
	m_IBarThemes.Add(_T("中跨合龙底板束"));

	m_HSBarThemes.Add(_T("横向预应力束"));
	m_HSBarThemes.Add(_T("竖向预应力束"));
	m_OBarThemes.Add(_T("体外预应力束"));
}

void CMainFrame::CreateDetailList(CBCGPRibbonPaletteButton* pBtn)
{
	pBtn->SetItemToolTip(0, _T("设计说明"));
	pBtn->SetItemToolTip(1, _T("全桥工程数量表"));
	pBtn->SetItemToolTip(2, _T("桥位平面图"));
	pBtn->SetItemToolTip(3, _T("工程地质纵断面图"));
	pBtn->SetItemToolTip(4, _T("总体布置图"));
	pBtn->SetItemToolTip(5, _T("箱梁一般构造图"));
};


void CMainFrame::CreateModelList(CBCGPRibbonPaletteButton* pBtn)
{
	pBtn->SetItemToolTip(0, _T("方案设计模式"));
	pBtn->SetItemToolTip(1, _T("构件编辑模式"));
	pBtn->SetItemToolTip(2, _T("钢束调整模式"));
	pBtn->SetItemToolTip(3, _T("建设施工模式"));
};


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPMDITabParams mdiTabParams;

	mdiTabParams.m_bTabIcons = TRUE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_SCROLLED;
	mdiTabParams.m_closeButtonMode = CBCGPTabWnd::TAB_CLOSE_BUTTON_HIGHLIGHTED_COMPACT;
	EnableMDITabbedGroups(TRUE, mdiTabParams);
	EnableTearOffMDIChildren();

	if (!CreateRibbonBar())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: add your status bar panes here:
	m_wndStatusBar.AddElement(new CBCGPRibbonStatusBarPane(
		ID_STATUSBAR_PANE1, _T("Pane 1"), TRUE), _T("Pane 1"));
	m_wndStatusBar.AddExtendedElement(new CBCGPRibbonStatusBarPane(
		ID_STATUSBAR_PANE2, _T("Pane 2"), TRUE), _T("Pane 2"));

	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize(CSize(16, 16));
	imagesWorkspace.Load(IDB_WORKSPACE);
	imagesWorkspace.SmoothResize(globalData.GetRibbonImageScale());

	if (!m_wndWorkSpace.Create(_T("桥梁列表"), this, CRect(0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return -1;      // fail to create
	}
	m_wndWorkSpace.SetIcon(imagesWorkspace.ExtractIcon(0), FALSE);


	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndWorkSpace);

	// Enable windows manager:
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Enable windows navigator (activated by Ctrl+Tab/Ctrl+Shift+Tab):
	EnableWindowsNavigator();
	EnableFullScreenMode(ID_VIEW_FULLSCREEN);
	EnableFullScreenMainMenu(TRUE);
	EnableFullScreenAutoHideTaskbar();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CBCGPMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}



BOOL CMainFrame::OnShowMDITabContextMenu(CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(bDrop ? IDR_POPUP_DROP_MDITABS : IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
	{
		pPopup->DeleteMenu(ID_MDI_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
	{
		pPopup->DeleteMenu(ID_MDI_NEW_VERT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
	{
		pPopup->DeleteMenu(ID_MDI_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
	{
		pPopup->DeleteMenu(ID_MDI_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->SetAutoDestroy(FALSE);
	pPopupMenu->Create(this, point.x, point.y, pPopup->GetSafeHmenu());

	return TRUE;
}

void CMainFrame::OnMdiMoveToNextGroup()
{
	MDITabMoveToNextGroup();
}

void CMainFrame::OnMdiMoveToPrevGroup()
{
	MDITabMoveToNextGroup(FALSE);
}

void CMainFrame::OnMdiNewHorzTabGroup()
{
	MDITabNewGroup(FALSE);
}

void CMainFrame::OnMdiNewVertGroup()
{
	MDITabNewGroup();
}

void CMainFrame::OnMdiCancel()
{
	// TODO: Add your command handler code here

}

CBCGPMDIChildWnd* CMainFrame::CreateDocumentWindow(LPCTSTR lpcszDocName, CObject* /*pObj*/)
{
	if (lpcszDocName != NULL && lpcszDocName[0] != '\0')
	{
		CDocument* pDoc = AfxGetApp()->OpenDocumentFile(lpcszDocName);

		if (pDoc != NULL)
		{
			POSITION pos = pDoc->GetFirstViewPosition();

			if (pos != NULL)
			{
				CView* pView = pDoc->GetNextView(pos);
				if (pView == NULL)
				{
					return NULL;
				}

				return DYNAMIC_DOWNCAST(CBCGPMDIChildWnd, pView->GetParent());
			}
		}
	}

	return NULL;
}

void CMainFrame::OnClose()
{
	if (theApp.projectModified)
	{
		int answer = MessageBox(_T("当前工程已被修改,是否保存?"), _T("提示"), MB_YESNOCANCEL | MB_ICONINFORMATION);
		if (answer == IDYES)
		{
			SavePrj();
		}
		else if (answer == IDCANCEL)
		{
			return;
		}
	}
	SaveMDIState(theApp.GetRegSectionPath());
	CBCGPMDIFrameWnd::OnClose();
}


BOOL CMainFrame::CreateRibbonBar()
{
	m_wndRibbonBar.EnableCustomization();

	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	m_wndRibbonBar.SetTabIconsStyle(BCGPRibbonIconStyle_Default);
	m_wndRibbonBar.SetQuickAccessIconsStyle(BCGPRibbonIconStyle_Default);

	//Load panel images :
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_RIBBON_ICONS);


	/*  ---------------   资料标签 --------------- */
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("资料设置"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// 项目管理面板:
	CBCGPRibbonPanel* pPanelPrjManager = pCategory->AddPanel(_T("工程管理"), m_PanelImages.ExtractIcon(1));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_NEW, _T("新建工程"), 0, 0));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_OPEN, _T("打开工程"), 0, 1));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_SAVE, _T("保存工程"), 0, 2));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_CLOSE, _T("关闭工程"), 0, 3));
	pPanelPrjManager->Add(new CBCGPRibbonSeparator);
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_FILE_NEW, _T("新增桥梁"), 0, 4));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_BRG_RENAME, _T("重命名"), 0));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_BRG_DEL, _T("删除桥梁"), 0));
	pPanelPrjManager->SetKeys(_T("prjmanager"));
	// 总体信息面板:
	CBCGPRibbonPanel* pPanelAll = pCategory->AddPanel(_T("总体信息"), m_PanelImages.ExtractIcon(1));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_ROAD, _T("路线资料"), 0, 5));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_SECT, _T("桥梁断面"), 0, 6));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_GEOLOGY, _T("地质资料"), 0, 7));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_HYDROLOGY, _T("水文资料"), 0, 8));

	// 工程设置:
	CBCGPRibbonPanel* pPanelSetting = pCategory->AddPanel(_T("材料规格"), m_PanelImages.ExtractIcon(1));
	pPanelSetting->Add(new CBCGPRibbonButton(ID_PRJ_METERIAL, _T("钢束锚具规格"), 0, 9));


	/* ---------------  上部设计标签 ---------------  */
	CBCGPRibbonCategory* pCategory1 = m_wndRibbonBar.AddCategory(_T("主桥上部设计"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// 桥型布置面板:
	CBCGPRibbonPanel* pPanelLayout = pCategory1->AddPanel(_T("桥型布置"), m_PanelImages.ExtractIcon(1));
	pPanelLayout->Add(new CBCGPRibbonButton(ID_DESIGN_LAYOUT_YZ, _T("原则"), 0, 10));
	pPanelLayout->Add(new CBCGPRibbonButton(ID_DESIGN_LAYOUT_ACTION, _T("智能布跨"), 0, 11));
	pPanelLayout->AddSeparator();


	CBCGPRibbonEdit* pBtnLlayout = new CBCGPRibbonEdit(ID_DESIGN_LLAYOUT, 120, _T("左引桥:"), 28);
	pPanelLayout->Add(pBtnLlayout);
	CBCGPRibbonEdit* pBtnClayout = new CBCGPRibbonEdit(ID_DESIGN_CLAYOUT, 120, _T("刚构桥:"), 28);
	pPanelLayout->Add(pBtnClayout);
	CBCGPRibbonEdit* pBtnRlayout = new CBCGPRibbonEdit(ID_DESIGN_RLAYOUT, 120, _T("右引桥:"), 28);
	pPanelLayout->Add(pBtnRlayout);

	CBCGPRibbonEdit* pBtnBaseStake = new CBCGPRibbonEdit(ID_DESIGN_CBASESTAKE, 80, _T("中心桩号:"), 28);
	pPanelLayout->Add(pBtnBaseStake);

	pPanelLayout->Add(new CBCGPRibbonButton(ID_DESIGN_LAYOUT_MACTION, _T("按输入布跨"), 10));

	CBCGPRibbonButtonsGroup* pButtonsGroup1 = new CBCGPRibbonButtonsGroup;
	pButtonsGroup1->AddButton(new CBCGPRibbonButton(ID_DESIGN_LMOVE, _T(""), 17));
	CBCGPRibbonEdit* pEdit = new CBCGPRibbonEdit(ID_DESIGN_MOVEDIS, 80);
	pEdit->SetPrompt(_T("移动增量(m)"));
	pButtonsGroup1->AddButton(pEdit);
	pButtonsGroup1->AddButton(new CBCGPRibbonButton(ID_DESIGN_RMOVE, _T(""), 18));
	pPanelLayout->Add(pButtonsGroup1);



	// 节段划分面板:
	CBCGPRibbonPanel* pPanelCell = pCategory1->AddPanel(_T("节段划分"), m_PanelImages.ExtractIcon(1));
	pPanelCell->Add(new CBCGPRibbonButton(ID_DESIGN_CELLYZ, _T("原则"), 0, 10));
	pPanelCell->Add(new CBCGPRibbonButton(ID_DESIGN_CELL, _T("智能划分"), 0, 12));
	pPanelCell->Add(new CBCGPRibbonCheckBox(ID_DESIGN_SHOWWEIGHT, _T("显示重量")));
	pPanelCell->Add(new CBCGPRibbonCheckBox(ID_DESIGN_SHOWCELLLEN, _T("显示长度")));
	// 节段构造面板:
	CBCGPRibbonPanel* pPanelCellDetail = pCategory1->AddPanel(_T("节段构造及钢筋"), m_PanelImages.ExtractIcon(1));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_GER, _T("普通节段"), 0, 13));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_0, _T("0号块"), 0, 14));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_BK, _T("边跨现浇段"), 0, 15));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_HGB, _T("跨中隔板"), 0, 16));
	// 钢束设计面板:
	CBCGPRibbonPanel* pPanelBar = pCategory1->AddPanel(_T("钢束设计"), m_PanelImages.ExtractIcon(1));
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_BARYZ, _T("原则"), 0, 10));
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_BARAUTO, _T("智能布束"), 0, 17));
	CRibbonListButton* pBtnBarAdjust = new CRibbonListButton(ID_DESIGN_BARADJUST, _T("钢束调整"), 0, 18);
	pPanelBar->Add(pBtnBarAdjust);
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_BARCHECK, _T("钢束检查"), 0, 19));
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_CHIBAN, _T("齿板设计"), 0, 20));
	// 其它上部面板:
	CBCGPRibbonPanel* pPanelOther = pCategory1->AddPanel(_T("其它上部"), m_PanelImages.ExtractIcon(1));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_FRAMEWORK, _T("合龙骨架"), 0, 21));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_JOINT, _T("伸缩缝"), 0, 22));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_PAVE, _T("铺装"), 0, 23));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_DRAIN, _T("排水"), 0, 24));



	/* 下部设计标签 */
	CBCGPRibbonCategory* pCategory2 = m_wndRibbonBar.AddCategory(_T("主桥下部设计"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// 智能设计面板:
	CBCGPRibbonPanel* pPanelDownAuto = pCategory2->AddPanel(_T("智能设计"), m_PanelImages.ExtractIcon(1));
	pPanelDownAuto->Add(new CBCGPRibbonButton(ID_DESIGN_DOWNYZ, _T("原则"), 0, 10));
	pPanelDownAuto->Add(new CBCGPRibbonButton(ID_DESIGN_DOWNAUTO, _T("智能设计"), 5, 25));
	// 主墩设计面板:
	CBCGPRibbonPanel* pPanelMainT = pCategory2->AddPanel(_T("主墩设计"), m_PanelImages.ExtractIcon(1));
	pPanelMainT->Add(new CBCGPRibbonButton(ID_DESIGN_MAINPIER, _T("主墩"), 1, 26));
	pPanelMainT->Add(new CBCGPRibbonButton(ID_DESIGN_CAP, _T("承台"), 0, 27));
	pPanelMainT->Add(new CBCGPRibbonButton(ID_DESIGN_PILL, _T("桩基"), 0, 28));
	// 过渡墩设计面板:
	CBCGPRibbonPanel* pPanelTransit = pCategory2->AddPanel(_T("过渡墩设计"), m_PanelImages.ExtractIcon(1));
	pPanelTransit->Add(new CBCGPRibbonButton(ID_DESIGN_TRANSIT, _T("过渡墩"), 2, 29));
	pPanelTransit->Add(new CBCGPRibbonButton(ID_DESIGN_TRANSITCAP, _T("承台"), 0, 30));
	pPanelTransit->Add(new CBCGPRibbonButton(ID_DESIGN_TRANSITPILL, _T("桩基"), 0, 31));



	/* 下部设计标签 */
	CBCGPRibbonCategory* pCategory3 = m_wndRibbonBar.AddCategory(_T("引桥设计"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// 引桥设计面板:
	CBCGPRibbonPanel* pPanelApproach = pCategory3->AddPanel(_T("引桥设计"), m_PanelImages.ExtractIcon(1));
	pPanelApproach->Add(new CBCGPRibbonButton(ID_DESIGN_APR_UP, _T("上部"), 2, 32));
	pPanelApproach->Add(new CBCGPRibbonButton(ID_DESIGN_APR_DOWN, _T("桥墩"), 0, 33));
	pPanelApproach->Add(new CBCGPRibbonButton(ID_DESIGN_APR_ABT, _T("桥台"), 0, 34));




	/* 输出标签 */
	CBCGPRibbonCategory* pCategory4 = m_wndRibbonBar.AddCategory(_T("输出成果"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// 图册管理面板:
	CBCGPRibbonPanel* pPanelResmanager = pCategory4->AddPanel(_T("成果管理"), m_PanelImages.ExtractIcon(1));
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("图册管理"), 0, 35));

	CBCGPRibbonButton*bntZt = new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("总体图纸"), 0, 36);
	pPanelResmanager->Add(bntZt);
	bntZt->SetMenu(IDR_OUTPUT_ALL,TRUE);

	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("上部图纸"), 0, 37));
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("下部图纸"), 0, 38));


	//CBCGPRibbonPaletteButton* pBtndetail = new CBCGPRibbonPaletteButton(ID_OUT_DETAIL, _T("图纸详细编辑"), 0, 0, IDB_OUTMANAGER, 64);
	//CreateDetailList(pBtndetail);
	//pBtndetail->SetIconsInRow(8);
	//pPanelResmanager->Add(pBtndetail);
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_DRAWING, _T("初步设计出图"), 0, 39));
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_DRAWING, _T("施工出图"), 0, 40));
	// 计算模型面板:
	CBCGPRibbonPanel* pPanelExpModel = pCategory4->AddPanel(_T("导出结构计算模型"), m_PanelImages.ExtractIcon(1));
	pPanelExpModel->Add(new CBCGPRibbonButton(ID_OUTPUT_DOCTORBRIDGE, _T("桥梁博士"), 0, 41));
	pPanelExpModel->Add(new CBCGPRibbonButton(ID_OUTPUT_MIDAS, _T("迈达斯"), 0, 42));


	/* 施工标签 */
	//CBCGPRibbonCategory* pCategory4 = m_wndRibbonBar.AddCategory(_T("施工应用"), IDB_RIBBON_FILESMALL, IDB_RIBBON_ALL);

	/*工具及视图标签*/
	CBCGPRibbonCategory* pCategory5 = m_wndRibbonBar.AddCategory(_T("工具及视图"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	CBCGPRibbonPanel* pPanelSystem = pCategory5->AddPanel(_T("系统管理"));
	pPanelSystem->Add(new CBCGPRibbonButton(ID_USER_FEEDBACK, _T("用户反馈"), 0, 43));
	pPanelSystem->Add(new CBCGPRibbonButton(ID_SYSTEM_UPDATE, _T("系统更新"), 0, 44));

	CBCGPRibbonPanel* pPanelHelp = pCategory5->AddPanel(_T("帮助"));
	pPanelHelp->Add(new CBCGPRibbonButton(ID_HELP_MANAUL, _T("帮助文档"), 0, 45));

	CBCGPRibbonPanel* pPanelWindow = pCategory5->AddPanel(_T("窗口管理"));
	pPanelWindow->Add(new CBCGPRibbonButton(ID_WINDOW_CLOSEALL, _T("关闭所有"), 0, 46));
	pPanelWindow->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_HORZ, _T("H平铺"), 0, 47));
	pPanelWindow->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_VERT, _T("V平铺"), 0, 48));

	CBCGPRibbonPanel* pPanelShowModel = pCategory5->AddPanel(_T("显示模式"));
	CBCGPRibbonPaletteButton* pBtnShowModel = new CBCGPRibbonPaletteButton(ID_SHOW_MODEL, _T("显示模式"), 0, 0, IDB_SHOW_MODEL, 90);
	CreateModelList(pBtnShowModel);
	pBtnShowModel->SetIconsInRow(4);
	pBtnShowModel->SetInitialColumns(4);
	pPanelShowModel->Add(pBtnShowModel);

	//CBCGPRibbonPanel* pPanelViewModel = pCategory5->AddPanel(_T("显示模式"));
	//pPanelViewModel->Add(new CBCGPRibbonButton(ID_PRJ_CLOSEWNDALL, _T("关闭所有窗口"), 0, 36));
	//pPanelViewModel->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_HORZ, _T("水平平铺"), 0, 36));
	//pPanelViewModel->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_VERT, _T("垂直平铺"), 0, 36));


	/* 替换面板按钮 */
	CRibbonListButton galleryTheme;
	galleryTheme.SetRightAlignedText(FALSE);
	galleryTheme.AddGroup(_T("纵向钢束设计"), IDB_BARADJUST_THEME, 64, m_IBarThemes);
	galleryTheme.AddGroup(_T("横竖向钢束设计"), IDB_BARADJUST_THEME, 64, m_HSBarThemes);
	galleryTheme.AddGroup(_T("体外钢束设计"), IDB_BARADJUST_THEME, 64, m_OBarThemes);
	galleryTheme.AddSubItem(new CBCGPRibbonButton(444, _T("调整选项...")));
	galleryTheme.SetIconsInRow(3);
	galleryTheme.EnableMenuResize();
	m_wndRibbonBar.ReplaceRibbonElementByID(ID_DESIGN_BARADJUST, galleryTheme, FALSE);


	// Ribbon起始常用按钮设置
	CBCGPRibbonQATDefaultState qatState;
	qatState.AddCommand(ID_PRJ_SAVE);
	qatState.AddCommand(ID_DESIGN_CELL_GER);
	qatState.AddCommand(ID_DESIGN_BARADJUST);
	m_wndRibbonBar.SetQuickAccessDefaultState(qatState);

	// 添加常用菜单按钮
	CBCGPRibbonButton* pFullscreenButton = new CBCGPRibbonButton(ID_VIEW_FULLSCREEN, _T("全屏显示"), 0, 0, 1);
	m_wndRibbonBar.AddToTabs(pFullscreenButton);

	CBCGPRibbonButton* pStyleButton = new CBCGPRibbonButton(-1, _T("界面样式"), -1, -1);
	pStyleButton->SetMenu(IDR_THEME_MENU, TRUE);
	m_wndRibbonBar.AddToTabs(pStyleButton);

	m_wndRibbonBar.AddToTabs(new CBCGPRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon(0)));

	// 使用自定义字体
	m_wndRibbonBar.SetFont(&theApp.m_fontRegular);

	return TRUE;
}

LRESULT CMainFrame::OnRibbonCustomize(WPARAM wp, LPARAM /*lp*/)
{
	ShowOptions(wp == 0 ? 1 : 0);
	return 1;
}

void CMainFrame::OnToolsOptions()
{
	ShowOptions(0);
}

void CMainFrame::ShowOptions(int nPage)
{
	// Create custom categories:

	// "Popular" items:

	CList<UINT, UINT> lstPopular;

	lstPopular.AddTail(ID_FILE_NEW);
	lstPopular.AddTail(ID_FILE_OPEN);
	lstPopular.AddTail(ID_FILE_SAVE);
	lstPopular.AddTail(ID_FILE_PRINT_PREVIEW);
	lstPopular.AddTail(ID_FILE_PRINT_DIRECT);
	lstPopular.AddTail(ID_EDIT_UNDO);

	// Hidden commands:
	CList<UINT, UINT> lstHidden;
	m_wndRibbonBar.GetItemIDsList(lstHidden, TRUE);

	// All commands:
	CList<UINT, UINT> lstAll;
	m_wndRibbonBar.GetItemIDsList(lstAll);

	// Create "Customize QAT" page:
	CBCGPRibbonCustomizeQATPage pageCustomizeQAT(&m_wndRibbonBar);

	pageCustomizeQAT.AddCustomCategory(_T("常用命令"), lstPopular);
	pageCustomizeQAT.AddCustomCategory(_T("Ribbon的所有命令 "), lstHidden);
	pageCustomizeQAT.AddCustomCategory(_T("程序所有命令"), lstAll);

	// Create "Customize Ribbon" page:
	CBCGPRibbonCustomizeRibbonPage pageCustomizeRibbon(&m_wndRibbonBar);

	pageCustomizeRibbon.AddCustomCategory(_T("Popular Commands"), lstPopular);
	pageCustomizeRibbon.AddCustomCategory(_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeRibbon.AddCustomCategory(_T("All Commands"), lstAll);

	// Create property sheet:
	CBCGPPropertySheet propSheet(_T("选项"), this, nPage);
	propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	propSheet.EnableLayout();

	propSheet.SetLook(CBCGPPropertySheet::PropSheetLook_List, 150 /* List width */);

	propSheet.AddPage(&pageCustomizeRibbon);

	propSheet.AddPage(&pageCustomizeQAT);

	// TODO: add your option pages here:
	// COptionsPage1 pageOptions1;
	// propSheet.AddPage (&pageOptions1);
	//
	// COptionsPage1 pageOptions2;
	// propSheet.AddPage (&pageOptions2);

	if (propSheet.DoModal() != IDOK)
	{
		return;
	}
}




void CMainFrame::OnPrjOpen()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("刚构工程文件(*.rfms)|*.rfms|所有文件(*.*) |*.*||"));
	if (IDOK == dlg.DoModal())
	{
		InitialUpdateFrm();
		Proj = new CProject();
		Proj->SetTreeCtrl(m_wndWorkSpace.GetTreeCtrl());
		Proj->Open(dlg.GetPathName());
		CString str(dlg.GetFileName());
		theApp.tmp_StrProjectname = str.Left(str.GetLength() - 5);
		theApp.tmp_StrProjectpath = dlg.GetFolderPath();
		theApp.hasProject = true;
		str = _T("CDIP连续刚构设计系统 ") + theApp.tmp_StrProjectpath + theApp.tmp_StrProjectname + _T(".rfms");
		SetTitle(str);
		SetWindowText(str);
		m_wndRibbonBar.SetWindowTextW(str);
	}
}


void CMainFrame::OnPrjSave()
{
	SavePrj();
}



void CMainFrame::OnPrjBarset()
{
	AfxMessageBox(_T(" 这里添加钢筋参数设置对话框"));
}


void CMainFrame::OnPrjMeterial()
{
	AfxMessageBox(_T(" 这里添加预应力钢束参数设置对话框"));
}


void CMainFrame::OnViewFullscreen()
{
	ShowFullScreen();
}


void CMainFrame::OnPrjNew()
{
	CDlgNewPrj dlg;
	if (dlg.DoModal() == IDOK)
	{
		InitialUpdateFrm();
		CProject *proj = new CProject(dlg.m_projectName, dlg.m_projectPath);
		proj->SetTreeCtrl(m_wndWorkSpace.GetTreeCtrl()); //在对proj对象进行操作前请先控制树形菜单
		if (!proj->Create())
		{
			delete proj;
			AfxMessageBox(_T("创建项目失败"));
			return;
		}
		Proj = proj;
		//Proj->Save(); //Proj新建成功之后即保存

		theApp.hasProject = true;
		theApp.projectModified = true;
		CString str(_T("CDIP连续刚构设计系统 ") + theApp.tmp_StrProjectpath + theApp.tmp_StrProjectname + _T(".rfms"));
		SetTitle(str);
		SetWindowText(str);
		m_wndRibbonBar.SetWindowTextW(str);
	}
}


void CMainFrame::OnUpdatePrjSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.hasProject);
}


void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.hasProject);
}


void CMainFrame::OnUpdateBrgRename(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_wndWorkSpace.GetTreeCtrl()->IsBridgeSelected());
}


void CMainFrame::OnUpdateBrgDel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_wndWorkSpace.GetTreeCtrl()->IsBridgeSelected());
}


void CMainFrame::OnBrgRename()
{
	HTREEITEM item = m_wndWorkSpace.GetTreeCtrl()->GetCurrentNode();
	if (item)
	{
		CString oldname = m_wndWorkSpace.GetTreeCtrl()->GetNodeName(item);
		Proj->SetTreeCtrl(m_wndWorkSpace.GetTreeCtrl());
		CDlgRenameBridge dlg(oldname);
		if (dlg.DoModal() == IDOK)
		{
			UpdateData(true);
			Proj->RenameBridge(oldname, dlg.m_strName);
			CString path = Proj->GetProjectPath();
			/*更新与树形控件选中节点名称对应doc的完整路径名*/
			POSITION pos = theApp.GetFirstDocTemplatePosition();
			CString brgFilePathName = path + _T("\\") + oldname + _T(".rfm");
			while (pos != NULL)
			{
				CDocTemplate *p = theApp.GetNextDocTemplate(pos);
				POSITION posdoc = p->GetFirstDocPosition();
				while (posdoc != NULL)
				{
					CRigidFrameDoc *pDoc = (CRigidFrameDoc*)(p->GetNextDoc(posdoc));
					while (pDoc->RigidFrameBridgeExample->m_ZQName == oldname)
					{
						pDoc->RigidFrameBridgeExample->m_ZQName = dlg.m_strName;
						pDoc->SetTitle(dlg.m_strName + _T(".rfm"));
						theApp.projectModified = true;
					}
				}
			}
			CFile::Rename(brgFilePathName, path + _T("\\") + dlg.m_strName + _T(".rfm"));
		}
		return;
	}
	AfxMessageBox(_T("错误：请先选中树形菜单中的项目！"));
}


void CMainFrame::OnBrgDel()
{
	CDlgDelBridge dlg;
	HTREEITEM item = m_wndWorkSpace.GetTreeCtrl()->GetCurrentNode();
	if (item)
	{
		if (dlg.DoModal() == IDOK)
		{
			Proj->SetTreeCtrl(m_wndWorkSpace.GetTreeCtrl());
			CString name = m_wndWorkSpace.GetTreeCtrl()->GetNodeName(item);
			POSITION pos = theApp.GetFirstDocTemplatePosition();
			Proj->DelBridge(name);
			theApp.projectModified = true;
			if (dlg.m_strOp == _T("del"))
			{
				CString str = Proj->GetProjectPath() + _T("\\") + name + _T(".rfm");
				if (access(CStringToPConstChar(str), 0) == 0)
				{
					CFile::Remove(Proj->GetProjectPath() + _T("\\") + name + _T(".rfm"));
				}
			}
			while (pos != NULL)
			{
				CDocTemplate *p = theApp.GetNextDocTemplate(pos);
				POSITION posdoc = p->GetFirstDocPosition();
				while (posdoc != NULL)
				{
					CDocument* pDoc = p->GetNextDoc(posdoc);
					if (pDoc->GetTitle() == (name + _T(".rfm")) || pDoc->GetTitle() == name) //判断如果想要打开的文件已经打开对应窗口，则激活
					{
						POSITION posview = pDoc->GetFirstViewPosition();
						if (posview != NULL)
						{
							CView* pV = pDoc->GetNextView(posview);
							pV->GetParentFrame()->DestroyWindow();
						}
					}
				}
			}
		}
		return;
	}
	AfxMessageBox(_T("错误：请先选中树形菜单中的项目！"));
}


void CMainFrame::OnPrjClose()
{
	InitialUpdateFrm();
}


void CMainFrame::OnUpdatePrjClose(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.hasProject);
}


void CMainFrame::OnPrjCloseall()
{
	// TODO:  在此添加命令处理程序代码
}


void CMainFrame::OnPrjClosewndall()
{
	// TODO:  在此添加命令处理程序代码
}


/*关闭打开的窗口并初始化程序界面*/
void CMainFrame::InitialUpdateFrm()
{
	if (theApp.projectModified)
	{
		int answer = MessageBox(_T("当前工程已被修改,是否保存?"), _T("提示"), MB_YESNOCANCEL | MB_ICONINFORMATION);
		if (answer == IDYES)
		{
			SavePrj();
		}
		else if (answer == IDCANCEL)
		{
			return;
		}
	}
	/*清空树形菜单*/
	m_wndWorkSpace.GetTreeCtrl()->DeleteAllItems();
	/*关闭所有激活的子窗口*/
	while (((CMainFrame*)AfxGetMainWnd())->GetActiveFrame() != AfxGetMainWnd())
	{
		((CChildFrame*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame())->DestroyWindow();
	}
	/*重置未改动标签*/
	theApp.projectModified = false;
}


void CMainFrame::OnUserFeedback()
{
	// TODO:  在此添加命令处理程序代码
}


void CMainFrame::OnSystemUpdate()
{
	// TODO:  在此添加命令处理程序代码
}


void CMainFrame::OnHelpManaul()
{
	// TODO:  在此添加命令处理程序代码
}
 

/*保存工程*/
void CMainFrame::SavePrj()
{
	/*保存工程文件*/
	Proj->Save();
	/*遍历保存所有刚构桥工程*/
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		CDocTemplate *p = theApp.GetNextDocTemplate(pos);
		POSITION posdoc = p->GetFirstDocPosition();
		while (posdoc != NULL)
		{
			CRigidFrameDoc *pDoc = (CRigidFrameDoc*)(p->GetNextDoc(posdoc));
			CString BrgFilePathName = Proj->GetProjectPath() + _T("\\") + pDoc->RigidFrameBridgeExample->m_ZQName + _T(".rfm");
			pDoc->DoSave(BrgFilePathName);
		}
	}

	/*重置未改动标签*/
	theApp.projectModified = false;
}

void CMainFrame::OnWindowTileHorz()
{
	MDITabNewGroup(FALSE);
}

void CMainFrame::OnWindowTileVert()
{
	MDITabNewGroup(TRUE);
}