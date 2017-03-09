// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "WorkSpaceBar.h"
#include "../RigidFrameDoc.h"
#include "../RigidFrameView.h"
#include "BCGPDockingControlBar.h"
#include "BCGPPropList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar

BEGIN_MESSAGE_MAP(CWorkSpaceBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
{
	// TODO: add one-time construction code here
	BCGPChartFormatLabel cfl;
	m_AxisFont = cfl.m_textFormat;
}

CWorkSpaceBar::~CWorkSpaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar message handlers

int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndSplitter.CreateStatic(this, 2, 1);

	// 创建 tree 对象
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_EDITLABELS;

	if (!m_wndTreeCtrl.Create(dwViewStyle, rectDummy, &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 0)))
	{
		TRACE0("CControlsContainer: Unable to create controls\n");
		return -1;
	}
	m_wndTreeCtrl.m_bVisualManagerStyle = TRUE;
	m_TreeImages.Create(IDB_TREE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndTreeCtrl.SetImageList(&m_TreeImages, TVSIL_NORMAL);
	m_wndTreeCtrl.SetFont(&theApp.m_fontRegular);

	// 创建 PropList 对象
	rectDummy.SetRectEmpty();
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, &m_wndSplitter, m_wndSplitter.IdFromRowCol(1, 0)))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}
	m_wndPropList.SetFont(&theApp.m_fontRegular);
	InitPropList();

	AdjustLayout();
	return 0;
}

void CWorkSpaceBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndSplitter.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndSplitter.SetRowInfo(0, 200, 0);
}


void CWorkSpaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CWorkSpaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rectClient;
	GetClientRect(rectClient);

	dc.FillRect(rectClient, &globalData.brBarFace);
}


CClassTreeWnd* CWorkSpaceBar::GetTreeCtrl()
{
	return &m_wndTreeCtrl;
}


void CWorkSpaceBar::InitPropList()
{
	// 设置通用外观:
	m_wndPropList.EnableToolBar();
	m_wndPropList.EnableSearchBox();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	//m_wndPropList.EnableContextMenu();

	UpdatePropList_Plan();
}



//void CWorkSpaceBar::SyncActiveView(CRigidFrameView* pView)
//{
//	if (m_wndPropList.IsInternalDlgModal())
//	{
//		return;
//	}
//
//	CString strCaption = _T("Properties");
//	CString strDoc = _T("none");
//	if (pView != NULL)
//	{
//		pView->GetParentFrame()->GetWindowText(strDoc);
//	}
//
//	strCaption += _T(" - ");
//	strCaption += strDoc;
//
//	SetWindowText(strCaption);
//
//	CBCGPChartVisualObject* pChart = pView == NULL ? NULL : pView->GetChart();
//
//	BOOL bEnable = (pChart != NULL);
//
//	m_wndPropList.EnableProperty(ChartTitle, bEnable);
//	m_wndPropList.EnableProperty(ChartCategory, bEnable);
//	m_wndPropList.EnableProperty(Chart3D, bEnable);
//	m_wndPropList.EnableProperty(ChartBackground, bEnable);
//	m_wndPropList.EnableProperty(DiagramBackground, bEnable);
//	m_wndPropList.EnableProperty(LineStyle, FALSE);
//	m_wndPropList.EnableProperty(AxisFont, bEnable);
//
//	if (pChart == NULL)
//	{
//		return;
//	}
//
//	int nCategory = -1;
//
//	switch (pChart->GetChartCategory())
//	{
//	case BCGPChartLine:
//		m_wndPropList.EnableProperty(LineStyle);
//
//	case BCGPChartLine3D:
//		nCategory = 0;
//		break;
//
//	case BCGPChartColumn:
//	case BCGPChartColumn3D:
//		nCategory = 1;
//		break;
//
//	case BCGPChartBar:
//	case BCGPChartBar3D:
//		nCategory = 2;
//		break;
//
//	case BCGPChartArea:
//	case BCGPChartArea3D:
//		nCategory = 3;
//		break;
//
//	case BCGPChartPie:
//	case BCGPChartPie3D:
//		nCategory = 4;
//		m_wndPropList.EnableProperty(AxisFont, FALSE);
//		break;
//
//	case BCGPChartPyramid:
//	case BCGPChartPyramid3D:
//		nCategory = 5;
//		m_wndPropList.EnableProperty(AxisFont, FALSE);
//		break;
//	}
//
//	if (nCategory != -1)
//	{
//		m_wndPropList.SelectPropertyOption(ChartCategory, nCategory);
//	}
//
//	BOOL bIs3D = pChart->IsChart3D() || pChart->GetChartCategory() == BCGPChartPie3D || pChart->GetChartCategory() == BCGPChartPyramid3D;
//
//	m_wndPropList.SelectPropertyOption(Chart3D, bIs3D ? 1 : 0);
//
//	m_wndPropList.UpdateProperty(ChartTitle, (LPCTSTR)pChart->GetChartTitle());
//	m_wndPropList.UpdateBrushProperty(ChartBackground, pChart->GetChartFillColor());
//	m_wndPropList.UpdateBrushProperty(DiagramBackground, pChart->GetDiagramFillColor());
//
//	BCGPChartFormatSeries* pFormat = (BCGPChartFormatSeries*)pChart->GetSeries(0)->GetDataPointFormat(-1, FALSE);
//	if (pFormat != NULL)
//	{
//		m_wndPropList.UpdateProperty(LineStyle,
//			(long)pFormat->m_seriesElementFormat.m_outlineFormat.m_strokeStyle.GetDashStyle());
//	}
//
//	m_wndPropList.UpdateTextFormatProperty(AxisFont, pChart->GetPrimaryAxis(TRUE)->m_axisLabelsFormat.m_textFormat);
//}



void CWorkSpaceBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (point != CPoint(-1, -1))
	{
		CPoint ptTree = point;
		m_wndTreeCtrl.ScreenToClient(&ptTree);

		HTREEITEM hTreeItem = m_wndTreeCtrl.HitTest(ptTree);
		if (hTreeItem != NULL)
		{
			m_wndTreeCtrl.SelectItem(hTreeItem);
		}
	}

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_PROJECT_OPERATOR);
	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPMDIFrameWnd)))
	{
		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CBCGPMDIFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
	}
}


void CWorkSpaceBar::OnSetFocus(CWnd* pOldWnd)
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	m_wndTreeCtrl.SetFocus();
}

//bool CWorkSpaceBar::IsBridgeSelected()
//{
//	return m_wndTreeCtrl.IsBridgeSelected();
//}

//CString CWorkSpaceBar::GetSelBridge()
//{
//	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
//	return m_wndTreeCtrl.GetItemText(hItem);
//}

//void CWorkSpaceBar::RenameBridge(CString newname)
//{
//	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
//	m_wndTreeCtrl.SetItemText(hItem, newname);
//}

//void CWorkSpaceBar::DelBridge()
//{
//	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
//	m_wndTreeCtrl.DeleteItem(hItem);
//}


// 显示模式切换后属性列表区的更新
void CWorkSpaceBar::UpdatePropList(int index)
{
	m_wndPropList.RemoveAll();


	switch (index)
	{
	case 0:
		UpdatePropList_Plan();
		break;
	case 1:
		UpdatePropList_Component();
		break;
	case 2:
		UpdatePropList_Bar();
		break;
	case 3:
		UpdatePropList_Construction();
		break;

	default:
		break;
	}
}



void CWorkSpaceBar::UpdatePropList_Plan()
{


}



void CWorkSpaceBar::UpdatePropList_Component()
{
	CBCGPProp* pGroup1 = new CBCGPProp(_T("UpdatePropList_Component"));
	pGroup1->AddSubItem(new CBCGPProp(_T("收到反映"), (UINT)ChartTitle, _T(""), _T("")));

	CBCGPProp* pCategory = new CBCGPProp(_T("Category"), (UINT)ChartCategory, _T("Line Chart"), _T(""));
	pCategory->AddOption(_T("Line Chart"));
	pCategory->AddOption(_T("Column Chart"));
	pCategory->AddOption(_T("Bar Chart"));
	pCategory->AddOption(_T("Area Chart"));
	pCategory->AddOption(_T("Pie Chart"));
	pCategory->AddOption(_T("Pyramid Chart"));
	pCategory->AllowEdit(FALSE);

	pGroup1->AddSubItem(pCategory);

	pGroup1->AddSubItem(new CBCGPProp(_T("3D Chart"), (UINT)Chart3D, (_variant_t)false));
	pGroup1->AddSubItem(new CBCGPLineStyleProp(_T("Line Style"), (UINT)LineStyle, CBCGPStrokeStyle::BCGP_DASH_STYLE_SOLID));

	CBCGPTextFormatDlgOptions tfOptions;
	tfOptions.m_bShowWordWrap = FALSE;

	pGroup1->AddSubItem(new CBCGPTextFormatProp(_T("Axis Font"), (UINT)AxisFont, m_AxisFont, &tfOptions));

	m_wndPropList.AddProperty(pGroup1);

	CBCGPProp* pGroup2 = new CBCGPProp(_T("Colors"));
	pGroup2->AddSubItem(new CBCGPBrushProp(_T("Chart Background"), (UINT)ChartBackground, CBCGPBrush()));
	pGroup2->AddSubItem(new CBCGPBrushProp(_T("Diagram Background"), (UINT)DiagramBackground, CBCGPBrush()));

	m_wndPropList.AddProperty(pGroup2);
}



void CWorkSpaceBar::UpdatePropList_Bar()
{
	CBCGPProp* pGroup1 = new CBCGPProp(_T("箱梁总体尺寸"));
	pGroup1->AddSubItem(new CBCGPProp(_T("端部高(m)"), (UINT)ChartTitle, _T(""), _T("")));
	pGroup1->AddSubItem(new CBCGPProp(_T("根部高(m)"), (UINT)ChartTitle, _T(""), _T("")));
	pGroup1->AddSubItem(new CBCGPProp(_T("端部底板厚(m)"), (UINT)ChartTitle, _T(""), _T("")));
	pGroup1->AddSubItem(new CBCGPProp(_T("根部底板厚(m)"), (UINT)ChartTitle, _T(""), _T("")));
	pGroup1->AddSubItem(new CBCGPProp(_T("箱梁底曲线方次"), (UINT)ChartTitle, _T(""), _T("")));
	pGroup1->AddSubItem(new CBCGPProp(_T("箱梁底板厚度曲线方次"), (UINT)ChartTitle, _T(""), _T("")));

	CBCGPProp* pCategory = new CBCGPProp(_T("Category"), (UINT)ChartCategory, _T("Line Chart"), _T(""));
	pCategory->AddOption(_T("Line Chart"));
	pCategory->AddOption(_T("Column Chart"));
	pCategory->AddOption(_T("Bar Chart"));
	pCategory->AddOption(_T("Area Chart"));
	pCategory->AddOption(_T("Pie Chart"));
	pCategory->AddOption(_T("Pyramid Chart"));
	pCategory->AllowEdit(FALSE);

	pGroup1->AddSubItem(pCategory);

	pGroup1->AddSubItem(new CBCGPProp(_T("3D Chart"), (UINT)Chart3D, (_variant_t)false));
	pGroup1->AddSubItem(new CBCGPLineStyleProp(_T("Line Style"), (UINT)LineStyle, CBCGPStrokeStyle::BCGP_DASH_STYLE_SOLID));

	CBCGPTextFormatDlgOptions tfOptions;
	tfOptions.m_bShowWordWrap = FALSE;

	pGroup1->AddSubItem(new CBCGPTextFormatProp(_T("Axis Font"), (UINT)AxisFont, m_AxisFont, &tfOptions));

	m_wndPropList.AddProperty(pGroup1);

	CBCGPProp* pGroup2 = new CBCGPProp(_T("Colors"));
	pGroup2->AddSubItem(new CBCGPBrushProp(_T("Chart Background"), (UINT)ChartBackground, CBCGPBrush()));
	pGroup2->AddSubItem(new CBCGPBrushProp(_T("Diagram Background"), (UINT)DiagramBackground, CBCGPBrush()));

	m_wndPropList.AddProperty(pGroup2);
}





void CWorkSpaceBar::UpdatePropList_Construction()
{
	CBCGPProp* pGroup1 = new CBCGPProp(_T("UpdatePropList_Construction"));
	pGroup1->AddSubItem(new CBCGPProp(_T("收到反映"), (UINT)ChartTitle, _T(""), _T("")));

	CBCGPProp* pCategory = new CBCGPProp(_T("Category"), (UINT)ChartCategory, _T("Line Chart"), _T(""));
	pCategory->AddOption(_T("Line Chart"));
	pCategory->AddOption(_T("Column Chart"));
	pCategory->AddOption(_T("Bar Chart"));
	pCategory->AddOption(_T("Area Chart"));
	pCategory->AddOption(_T("Pie Chart"));
	pCategory->AddOption(_T("Pyramid Chart"));
	pCategory->AllowEdit(FALSE);

	pGroup1->AddSubItem(pCategory);

	pGroup1->AddSubItem(new CBCGPProp(_T("3D Chart"), (UINT)Chart3D, (_variant_t)false));
	pGroup1->AddSubItem(new CBCGPLineStyleProp(_T("Line Style"), (UINT)LineStyle, CBCGPStrokeStyle::BCGP_DASH_STYLE_SOLID));

	CBCGPTextFormatDlgOptions tfOptions;
	tfOptions.m_bShowWordWrap = FALSE;

	pGroup1->AddSubItem(new CBCGPTextFormatProp(_T("Axis Font"), (UINT)AxisFont, m_AxisFont, &tfOptions));

	m_wndPropList.AddProperty(pGroup1);

	CBCGPProp* pGroup2 = new CBCGPProp(_T("Colors"));
	pGroup2->AddSubItem(new CBCGPBrushProp(_T("Chart Background"), (UINT)ChartBackground, CBCGPBrush()));
	pGroup2->AddSubItem(new CBCGPBrushProp(_T("Diagram Background"), (UINT)DiagramBackground, CBCGPBrush()));

	m_wndPropList.AddProperty(pGroup2);
}
