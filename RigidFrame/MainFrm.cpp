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
	/*�Զ�����Ϣӳ��*/
	ON_MESSAGE(WM_MYMSG,&CMainFrame::OnMyMsg)
END_MESSAGE_MAP()


// CMainFrame construction/destruction
/*�Զ�����Ϣ������*/
LRESULT CMainFrame::OnMyMsg(WPARAM wp, LPARAM lp)
{
	CRigidFrameDoc* Cfd=(CRigidFrameDoc*)this->GetActiveFrame()->GetActiveDocument();
	double wl=Cfd->RigidFrameBridgeExample->m_RoadData.m_WaterLevel;
	char str[20];
	sprintf_s(str, "%.2f", wl);
	m_wndWorkSpace.m_wndPropList.RemoveAll();
	CBCGPProp* pGroup0 = new CBCGPProp(_T("��������"));
	pGroup0->AddSubItem(new CBCGPProp(_T("ˮλ(m)"), (UINT)m_wndWorkSpace.ChartTitle, str, _T("")));
	m_wndWorkSpace.m_wndPropList.AddProperty(pGroup0);

	auto info = (std::vector<CStringPair>*)lp;
	if (info != nullptr)
	{
		CBCGPProp* pGroup1 = new CBCGPProp(_T("��������"));
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

	m_IBarThemes.Add(_T("������"));
	m_IBarThemes.Add(_T("������"));
	m_IBarThemes.Add(_T("�߿����������"));
	m_IBarThemes.Add(_T("�߿�����װ���"));
	m_IBarThemes.Add(_T("�п����������"));
	m_IBarThemes.Add(_T("�п�����װ���"));

	m_HSBarThemes.Add(_T("����ԤӦ����"));
	m_HSBarThemes.Add(_T("����ԤӦ����"));
	m_OBarThemes.Add(_T("����ԤӦ����"));
}

void CMainFrame::CreateDetailList(CBCGPRibbonPaletteButton* pBtn)
{
	pBtn->SetItemToolTip(0, _T("���˵��"));
	pBtn->SetItemToolTip(1, _T("ȫ�Ź���������"));
	pBtn->SetItemToolTip(2, _T("��λƽ��ͼ"));
	pBtn->SetItemToolTip(3, _T("���̵����ݶ���ͼ"));
	pBtn->SetItemToolTip(4, _T("���岼��ͼ"));
	pBtn->SetItemToolTip(5, _T("����һ�㹹��ͼ"));
};


void CMainFrame::CreateModelList(CBCGPRibbonPaletteButton* pBtn)
{
	pBtn->SetItemToolTip(0, _T("�������ģʽ"));
	pBtn->SetItemToolTip(1, _T("�����༭ģʽ"));
	pBtn->SetItemToolTip(2, _T("��������ģʽ"));
	pBtn->SetItemToolTip(3, _T("����ʩ��ģʽ"));
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

	if (!m_wndWorkSpace.Create(_T("�����б�"), this, CRect(0, 0, 200, 200),
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
		int answer = MessageBox(_T("��ǰ�����ѱ��޸�,�Ƿ񱣴�?"), _T("��ʾ"), MB_YESNOCANCEL | MB_ICONINFORMATION);
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


	/*  ---------------   ���ϱ�ǩ --------------- */
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("��������"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// ��Ŀ�������:
	CBCGPRibbonPanel* pPanelPrjManager = pCategory->AddPanel(_T("���̹���"), m_PanelImages.ExtractIcon(1));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_NEW, _T("�½�����"), 0, 0));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_OPEN, _T("�򿪹���"), 0, 1));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_SAVE, _T("���湤��"), 0, 2));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_PRJ_CLOSE, _T("�رչ���"), 0, 3));
	pPanelPrjManager->Add(new CBCGPRibbonSeparator);
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_FILE_NEW, _T("��������"), 0, 4));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_BRG_RENAME, _T("������"), 0));
	pPanelPrjManager->Add(new CBCGPRibbonButton(ID_BRG_DEL, _T("ɾ������"), 0));
	pPanelPrjManager->SetKeys(_T("prjmanager"));
	// ������Ϣ���:
	CBCGPRibbonPanel* pPanelAll = pCategory->AddPanel(_T("������Ϣ"), m_PanelImages.ExtractIcon(1));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_ROAD, _T("·������"), 0, 5));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_SECT, _T("��������"), 0, 6));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_GEOLOGY, _T("��������"), 0, 7));
	pPanelAll->Add(new CBCGPRibbonButton(ID_PRJ_HYDROLOGY, _T("ˮ������"), 0, 8));

	// ��������:
	CBCGPRibbonPanel* pPanelSetting = pCategory->AddPanel(_T("���Ϲ��"), m_PanelImages.ExtractIcon(1));
	pPanelSetting->Add(new CBCGPRibbonButton(ID_PRJ_METERIAL, _T("����ê�߹��"), 0, 9));


	/* ---------------  �ϲ���Ʊ�ǩ ---------------  */
	CBCGPRibbonCategory* pCategory1 = m_wndRibbonBar.AddCategory(_T("�����ϲ����"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// ���Ͳ������:
	CBCGPRibbonPanel* pPanelLayout = pCategory1->AddPanel(_T("���Ͳ���"), m_PanelImages.ExtractIcon(1));
	pPanelLayout->Add(new CBCGPRibbonButton(ID_DESIGN_LAYOUT_YZ, _T("ԭ��"), 0, 10));
	pPanelLayout->Add(new CBCGPRibbonButton(ID_DESIGN_LAYOUT_ACTION, _T("���ܲ���"), 0, 11));
	pPanelLayout->AddSeparator();


	CBCGPRibbonEdit* pBtnLlayout = new CBCGPRibbonEdit(ID_DESIGN_LLAYOUT, 120, _T("������:"), 28);
	pPanelLayout->Add(pBtnLlayout);
	CBCGPRibbonEdit* pBtnClayout = new CBCGPRibbonEdit(ID_DESIGN_CLAYOUT, 120, _T("�չ���:"), 28);
	pPanelLayout->Add(pBtnClayout);
	CBCGPRibbonEdit* pBtnRlayout = new CBCGPRibbonEdit(ID_DESIGN_RLAYOUT, 120, _T("������:"), 28);
	pPanelLayout->Add(pBtnRlayout);

	CBCGPRibbonEdit* pBtnBaseStake = new CBCGPRibbonEdit(ID_DESIGN_CBASESTAKE, 80, _T("����׮��:"), 28);
	pPanelLayout->Add(pBtnBaseStake);

	pPanelLayout->Add(new CBCGPRibbonButton(ID_DESIGN_LAYOUT_MACTION, _T("�����벼��"), 10));

	CBCGPRibbonButtonsGroup* pButtonsGroup1 = new CBCGPRibbonButtonsGroup;
	pButtonsGroup1->AddButton(new CBCGPRibbonButton(ID_DESIGN_LMOVE, _T(""), 17));
	CBCGPRibbonEdit* pEdit = new CBCGPRibbonEdit(ID_DESIGN_MOVEDIS, 80);
	pEdit->SetPrompt(_T("�ƶ�����(m)"));
	pButtonsGroup1->AddButton(pEdit);
	pButtonsGroup1->AddButton(new CBCGPRibbonButton(ID_DESIGN_RMOVE, _T(""), 18));
	pPanelLayout->Add(pButtonsGroup1);



	// �ڶλ������:
	CBCGPRibbonPanel* pPanelCell = pCategory1->AddPanel(_T("�ڶλ���"), m_PanelImages.ExtractIcon(1));
	pPanelCell->Add(new CBCGPRibbonButton(ID_DESIGN_CELLYZ, _T("ԭ��"), 0, 10));
	pPanelCell->Add(new CBCGPRibbonButton(ID_DESIGN_CELL, _T("���ܻ���"), 0, 12));
	pPanelCell->Add(new CBCGPRibbonCheckBox(ID_DESIGN_SHOWWEIGHT, _T("��ʾ����")));
	pPanelCell->Add(new CBCGPRibbonCheckBox(ID_DESIGN_SHOWCELLLEN, _T("��ʾ����")));
	// �ڶι������:
	CBCGPRibbonPanel* pPanelCellDetail = pCategory1->AddPanel(_T("�ڶι��켰�ֽ�"), m_PanelImages.ExtractIcon(1));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_GER, _T("��ͨ�ڶ�"), 0, 13));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_0, _T("0�ſ�"), 0, 14));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_BK, _T("�߿��ֽ���"), 0, 15));
	pPanelCellDetail->Add(new CBCGPRibbonButton(ID_DESIGN_CELL_HGB, _T("���и���"), 0, 16));
	// ����������:
	CBCGPRibbonPanel* pPanelBar = pCategory1->AddPanel(_T("�������"), m_PanelImages.ExtractIcon(1));
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_BARYZ, _T("ԭ��"), 0, 10));
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_BARAUTO, _T("���ܲ���"), 0, 17));
	CRibbonListButton* pBtnBarAdjust = new CRibbonListButton(ID_DESIGN_BARADJUST, _T("��������"), 0, 18);
	pPanelBar->Add(pBtnBarAdjust);
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_BARCHECK, _T("�������"), 0, 19));
	pPanelBar->Add(new CBCGPRibbonButton(ID_DESIGN_CHIBAN, _T("�ݰ����"), 0, 20));
	// �����ϲ����:
	CBCGPRibbonPanel* pPanelOther = pCategory1->AddPanel(_T("�����ϲ�"), m_PanelImages.ExtractIcon(1));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_FRAMEWORK, _T("�����Ǽ�"), 0, 21));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_JOINT, _T("������"), 0, 22));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_PAVE, _T("��װ"), 0, 23));
	pPanelOther->Add(new CBCGPRibbonButton(ID_DESIGN_DRAIN, _T("��ˮ"), 0, 24));



	/* �²���Ʊ�ǩ */
	CBCGPRibbonCategory* pCategory2 = m_wndRibbonBar.AddCategory(_T("�����²����"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// ����������:
	CBCGPRibbonPanel* pPanelDownAuto = pCategory2->AddPanel(_T("�������"), m_PanelImages.ExtractIcon(1));
	pPanelDownAuto->Add(new CBCGPRibbonButton(ID_DESIGN_DOWNYZ, _T("ԭ��"), 0, 10));
	pPanelDownAuto->Add(new CBCGPRibbonButton(ID_DESIGN_DOWNAUTO, _T("�������"), 5, 25));
	// ����������:
	CBCGPRibbonPanel* pPanelMainT = pCategory2->AddPanel(_T("�������"), m_PanelImages.ExtractIcon(1));
	pPanelMainT->Add(new CBCGPRibbonButton(ID_DESIGN_MAINPIER, _T("����"), 1, 26));
	pPanelMainT->Add(new CBCGPRibbonButton(ID_DESIGN_CAP, _T("��̨"), 0, 27));
	pPanelMainT->Add(new CBCGPRibbonButton(ID_DESIGN_PILL, _T("׮��"), 0, 28));
	// ���ɶ�������:
	CBCGPRibbonPanel* pPanelTransit = pCategory2->AddPanel(_T("���ɶ����"), m_PanelImages.ExtractIcon(1));
	pPanelTransit->Add(new CBCGPRibbonButton(ID_DESIGN_TRANSIT, _T("���ɶ�"), 2, 29));
	pPanelTransit->Add(new CBCGPRibbonButton(ID_DESIGN_TRANSITCAP, _T("��̨"), 0, 30));
	pPanelTransit->Add(new CBCGPRibbonButton(ID_DESIGN_TRANSITPILL, _T("׮��"), 0, 31));



	/* �²���Ʊ�ǩ */
	CBCGPRibbonCategory* pCategory3 = m_wndRibbonBar.AddCategory(_T("�������"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// ����������:
	CBCGPRibbonPanel* pPanelApproach = pCategory3->AddPanel(_T("�������"), m_PanelImages.ExtractIcon(1));
	pPanelApproach->Add(new CBCGPRibbonButton(ID_DESIGN_APR_UP, _T("�ϲ�"), 2, 32));
	pPanelApproach->Add(new CBCGPRibbonButton(ID_DESIGN_APR_DOWN, _T("�Ŷ�"), 0, 33));
	pPanelApproach->Add(new CBCGPRibbonButton(ID_DESIGN_APR_ABT, _T("��̨"), 0, 34));




	/* �����ǩ */
	CBCGPRibbonCategory* pCategory4 = m_wndRibbonBar.AddCategory(_T("����ɹ�"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	// ͼ��������:
	CBCGPRibbonPanel* pPanelResmanager = pCategory4->AddPanel(_T("�ɹ�����"), m_PanelImages.ExtractIcon(1));
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("ͼ�����"), 0, 35));

	CBCGPRibbonButton*bntZt = new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("����ͼֽ"), 0, 36);
	pPanelResmanager->Add(bntZt);
	bntZt->SetMenu(IDR_OUTPUT_ALL,TRUE);

	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("�ϲ�ͼֽ"), 0, 37));
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_MANAGER, _T("�²�ͼֽ"), 0, 38));


	//CBCGPRibbonPaletteButton* pBtndetail = new CBCGPRibbonPaletteButton(ID_OUT_DETAIL, _T("ͼֽ��ϸ�༭"), 0, 0, IDB_OUTMANAGER, 64);
	//CreateDetailList(pBtndetail);
	//pBtndetail->SetIconsInRow(8);
	//pPanelResmanager->Add(pBtndetail);
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_DRAWING, _T("������Ƴ�ͼ"), 0, 39));
	pPanelResmanager->Add(new CBCGPRibbonButton(ID_OUTPUT_DRAWING, _T("ʩ����ͼ"), 0, 40));
	// ����ģ�����:
	CBCGPRibbonPanel* pPanelExpModel = pCategory4->AddPanel(_T("�����ṹ����ģ��"), m_PanelImages.ExtractIcon(1));
	pPanelExpModel->Add(new CBCGPRibbonButton(ID_OUTPUT_DOCTORBRIDGE, _T("������ʿ"), 0, 41));
	pPanelExpModel->Add(new CBCGPRibbonButton(ID_OUTPUT_MIDAS, _T("����˹"), 0, 42));


	/* ʩ����ǩ */
	//CBCGPRibbonCategory* pCategory4 = m_wndRibbonBar.AddCategory(_T("ʩ��Ӧ��"), IDB_RIBBON_FILESMALL, IDB_RIBBON_ALL);

	/*���߼���ͼ��ǩ*/
	CBCGPRibbonCategory* pCategory5 = m_wndRibbonBar.AddCategory(_T("���߼���ͼ"), IDB_LAYOUTSMALL, IDB_RIBBON_ALL);
	CBCGPRibbonPanel* pPanelSystem = pCategory5->AddPanel(_T("ϵͳ����"));
	pPanelSystem->Add(new CBCGPRibbonButton(ID_USER_FEEDBACK, _T("�û�����"), 0, 43));
	pPanelSystem->Add(new CBCGPRibbonButton(ID_SYSTEM_UPDATE, _T("ϵͳ����"), 0, 44));

	CBCGPRibbonPanel* pPanelHelp = pCategory5->AddPanel(_T("����"));
	pPanelHelp->Add(new CBCGPRibbonButton(ID_HELP_MANAUL, _T("�����ĵ�"), 0, 45));

	CBCGPRibbonPanel* pPanelWindow = pCategory5->AddPanel(_T("���ڹ���"));
	pPanelWindow->Add(new CBCGPRibbonButton(ID_WINDOW_CLOSEALL, _T("�ر�����"), 0, 46));
	pPanelWindow->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_HORZ, _T("Hƽ��"), 0, 47));
	pPanelWindow->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_VERT, _T("Vƽ��"), 0, 48));

	CBCGPRibbonPanel* pPanelShowModel = pCategory5->AddPanel(_T("��ʾģʽ"));
	CBCGPRibbonPaletteButton* pBtnShowModel = new CBCGPRibbonPaletteButton(ID_SHOW_MODEL, _T("��ʾģʽ"), 0, 0, IDB_SHOW_MODEL, 90);
	CreateModelList(pBtnShowModel);
	pBtnShowModel->SetIconsInRow(4);
	pBtnShowModel->SetInitialColumns(4);
	pPanelShowModel->Add(pBtnShowModel);

	//CBCGPRibbonPanel* pPanelViewModel = pCategory5->AddPanel(_T("��ʾģʽ"));
	//pPanelViewModel->Add(new CBCGPRibbonButton(ID_PRJ_CLOSEWNDALL, _T("�ر����д���"), 0, 36));
	//pPanelViewModel->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_HORZ, _T("ˮƽƽ��"), 0, 36));
	//pPanelViewModel->Add(new CBCGPRibbonButton(ID_WINDOW_TILE_VERT, _T("��ֱƽ��"), 0, 36));


	/* �滻��尴ť */
	CRibbonListButton galleryTheme;
	galleryTheme.SetRightAlignedText(FALSE);
	galleryTheme.AddGroup(_T("����������"), IDB_BARADJUST_THEME, 64, m_IBarThemes);
	galleryTheme.AddGroup(_T("������������"), IDB_BARADJUST_THEME, 64, m_HSBarThemes);
	galleryTheme.AddGroup(_T("����������"), IDB_BARADJUST_THEME, 64, m_OBarThemes);
	galleryTheme.AddSubItem(new CBCGPRibbonButton(444, _T("����ѡ��...")));
	galleryTheme.SetIconsInRow(3);
	galleryTheme.EnableMenuResize();
	m_wndRibbonBar.ReplaceRibbonElementByID(ID_DESIGN_BARADJUST, galleryTheme, FALSE);


	// Ribbon��ʼ���ð�ť����
	CBCGPRibbonQATDefaultState qatState;
	qatState.AddCommand(ID_PRJ_SAVE);
	qatState.AddCommand(ID_DESIGN_CELL_GER);
	qatState.AddCommand(ID_DESIGN_BARADJUST);
	m_wndRibbonBar.SetQuickAccessDefaultState(qatState);

	// ��ӳ��ò˵���ť
	CBCGPRibbonButton* pFullscreenButton = new CBCGPRibbonButton(ID_VIEW_FULLSCREEN, _T("ȫ����ʾ"), 0, 0, 1);
	m_wndRibbonBar.AddToTabs(pFullscreenButton);

	CBCGPRibbonButton* pStyleButton = new CBCGPRibbonButton(-1, _T("������ʽ"), -1, -1);
	pStyleButton->SetMenu(IDR_THEME_MENU, TRUE);
	m_wndRibbonBar.AddToTabs(pStyleButton);

	m_wndRibbonBar.AddToTabs(new CBCGPRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon(0)));

	// ʹ���Զ�������
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

	pageCustomizeQAT.AddCustomCategory(_T("��������"), lstPopular);
	pageCustomizeQAT.AddCustomCategory(_T("Ribbon���������� "), lstHidden);
	pageCustomizeQAT.AddCustomCategory(_T("������������"), lstAll);

	// Create "Customize Ribbon" page:
	CBCGPRibbonCustomizeRibbonPage pageCustomizeRibbon(&m_wndRibbonBar);

	pageCustomizeRibbon.AddCustomCategory(_T("Popular Commands"), lstPopular);
	pageCustomizeRibbon.AddCustomCategory(_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeRibbon.AddCustomCategory(_T("All Commands"), lstAll);

	// Create property sheet:
	CBCGPPropertySheet propSheet(_T("ѡ��"), this, nPage);
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
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("�չ������ļ�(*.rfms)|*.rfms|�����ļ�(*.*) |*.*||"));
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
		str = _T("CDIP�����չ����ϵͳ ") + theApp.tmp_StrProjectpath + theApp.tmp_StrProjectname + _T(".rfms");
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
	AfxMessageBox(_T(" ������Ӹֽ�������öԻ���"));
}


void CMainFrame::OnPrjMeterial()
{
	AfxMessageBox(_T(" �������ԤӦ�������������öԻ���"));
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
		proj->SetTreeCtrl(m_wndWorkSpace.GetTreeCtrl()); //�ڶ�proj������в���ǰ���ȿ������β˵�
		if (!proj->Create())
		{
			delete proj;
			AfxMessageBox(_T("������Ŀʧ��"));
			return;
		}
		Proj = proj;
		//Proj->Save(); //Proj�½��ɹ�֮�󼴱���

		theApp.hasProject = true;
		theApp.projectModified = true;
		CString str(_T("CDIP�����չ����ϵͳ ") + theApp.tmp_StrProjectpath + theApp.tmp_StrProjectname + _T(".rfms"));
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
			/*���������οؼ�ѡ�нڵ����ƶ�Ӧdoc������·����*/
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
	AfxMessageBox(_T("��������ѡ�����β˵��е���Ŀ��"));
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
					if (pDoc->GetTitle() == (name + _T(".rfm")) || pDoc->GetTitle() == name) //�ж������Ҫ�򿪵��ļ��Ѿ��򿪶�Ӧ���ڣ��򼤻�
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
	AfxMessageBox(_T("��������ѡ�����β˵��е���Ŀ��"));
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
	// TODO:  �ڴ���������������
}


void CMainFrame::OnPrjClosewndall()
{
	// TODO:  �ڴ���������������
}


/*�رմ򿪵Ĵ��ڲ���ʼ���������*/
void CMainFrame::InitialUpdateFrm()
{
	if (theApp.projectModified)
	{
		int answer = MessageBox(_T("��ǰ�����ѱ��޸�,�Ƿ񱣴�?"), _T("��ʾ"), MB_YESNOCANCEL | MB_ICONINFORMATION);
		if (answer == IDYES)
		{
			SavePrj();
		}
		else if (answer == IDCANCEL)
		{
			return;
		}
	}
	/*������β˵�*/
	m_wndWorkSpace.GetTreeCtrl()->DeleteAllItems();
	/*�ر����м�����Ӵ���*/
	while (((CMainFrame*)AfxGetMainWnd())->GetActiveFrame() != AfxGetMainWnd())
	{
		((CChildFrame*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame())->DestroyWindow();
	}
	/*����δ�Ķ���ǩ*/
	theApp.projectModified = false;
}


void CMainFrame::OnUserFeedback()
{
	// TODO:  �ڴ���������������
}


void CMainFrame::OnSystemUpdate()
{
	// TODO:  �ڴ���������������
}


void CMainFrame::OnHelpManaul()
{
	// TODO:  �ڴ���������������
}
 

/*���湤��*/
void CMainFrame::SavePrj()
{
	/*���湤���ļ�*/
	Proj->Save();
	/*�����������иչ��Ź���*/
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

	/*����δ�Ķ���ǩ*/
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