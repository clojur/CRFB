// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "RigidFrame.h"
#include "RigidFrameView.h" //加
#include "ChildFrm.h"
#include <RigidFrameBridge/RigidFrameBridge.h>
#include "RigidFrameDoc.h"

#include "MainFrm.h" //加
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_DESIGN_CELL, &CChildFrame::OnDesignCell)
	ON_COMMAND(IDC_SPLIT_UNIFY, &CChildFrame::OnSplitUnify)
	ON_COMMAND(IDC_RESPLIT_PLAN, &CChildFrame::OnResplitPlan)
	ON_WM_CHILDACTIVATE()
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{

}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if (!CBCGPMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_SYSMENU;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers

void CChildFrame::ActivateFrame(int nCmdShow)
{
	CBCGPMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	//(一) 创建节段划分方案列表DockPane

	if (!m_wndCellSplitPane.Create(_T("节段划分方案"), this, TRUE, IDD_DLG_CELLSPLIT_PLAN,
		WS_VISIBLE | WS_CHILD | CBRS_RIGHT, 10143))
	{
		TRACE0("Failed to create pane\n");
		return -1;
	}

	m_wndCellSplitPane.EnableVisualManagerStyle();
	m_wndCellSplitPane.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndCellSplitPane);
	m_wndCellSplitPane.ShowControlBar(FALSE, FALSE, TRUE);


	//(二) ............................

	return 0;
}




// 点击“智能划分节段”按钮
void CChildFrame::OnDesignCell()
{
	// 0、判断该桥是否有统一采用一种节段划分方案的条件
	bool findUnifyPoint = false;
	double beamLen = 0;

	// 1、遍历主梁对象找到T构压入CString数组
	vector<CString> m_GirderStr; // T构对象名称数组(1号T,2号T，3号T........)
	CRigidFrameDoc* pDoc = (CRigidFrameDoc*)GetActiveDocument();
	CRigidFrameBridge* brg = pDoc->RigidFrameBridgeExample;

	vector<CMainGirder*>::iterator iter;
	m_GirderStr.clear();

	for (iter = brg->m_MainGirders.begin(); iter != brg->m_MainGirders.end(); ++iter)
	{
		if ((*iter)->m_Type == CMainGirder::T_CONSTRUCTION)
		{
			m_GirderStr.push_back((*iter)->m_Id);
			if (fabs(((*iter)->m_AllLen - (*iter)->m_BottomLen - 2 * (*iter)->m_Remain) - beamLen) > 0.01 && beamLen != 0 && !findUnifyPoint)
			{
				m_wndCellSplitPane.m_chkUnify.EnableWindow(0);
				beamLen = (*iter)->m_AllLen - (*iter)->m_BottomLen - 2 * (*iter)->m_Remain;
				findUnifyPoint = true;
			}
		}
	}

	//2、将CString数组填入下拉列表
	m_wndCellSplitPane.addXbEnum(m_GirderStr);

	//3、显示节段划分DockPane
	m_wndCellSplitPane.ShowControlBar(TRUE, FALSE, TRUE);
}


//BOOL CChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
//{
//	if (!CBCGPMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
//	{
//		return FALSE;
//	}
//	return TRUE;
//}




void CChildFrame::OnSplitUnify()
{
	// TODO:  在此添加命令处理程序代码
}


void CChildFrame::OnResplitPlan()
{
	// TODO:  在此添加命令处理程序代码
}


void CChildFrame::OnChildActivate()
{
	CBCGPMDIChildWnd::OnChildActivate();

	//填满菜单栏布孔编辑框
	CRigidFrameView* pCurView = (CRigidFrameView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive()->GetActiveView();
	CRigidFrameDoc *doc = pCurView->GetDocument();
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit* edit_1 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_LLAYOUT));
	CBCGPRibbonEdit* edit_2 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_CLAYOUT));
	CBCGPRibbonEdit* edit_3 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_RLAYOUT));
	CBCGPRibbonEdit* edit_4 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_CBASESTAKE));
	ASSERT_VALID(edit_1);
	ASSERT_VALID(edit_2);
	ASSERT_VALID(edit_3);
	ASSERT_VALID(edit_4);
	edit_1->SetEditText(doc->RigidFrameBridgeExample->m_YQLayOutResLeft);
	edit_2->SetEditText(doc->RigidFrameBridgeExample->m_ZQLayOutRes);
	edit_3->SetEditText(doc->RigidFrameBridgeExample->m_YQLayOutResRight);
	edit_4->SetEditText(doc->RigidFrameBridgeExample->m_CenterStake);
}
