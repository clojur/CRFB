// RigidFrameView.cpp : implementation of the CRigidFrameView class
//

#include "stdafx.h"
#include "RigidFrame.h"
#include "MainFrm.h"
#include "RigidFrameDoc.h"
#include "RigidFrameView.h"
#include "Dialog objects\DlgNewBridge.h"
#include "Dialog objects\DlgLoadRoad.h"
#include "Dialog objects\DlgAutoLayoutYz.h"
#include "Dialog objects\DlgSectInfo.h"
#include "Dialog objects\DlgCellSplitYz.h"
#include "piles/DlgPiles.h"
#include <YjOSG/YjOSG.h>
//#include <DxfMaker/dl_dxf.h>
#include <CDIPBar/BarInc.h>
#include "Interface\WorkSpaceBar.h"
#include <numeric> //accumulate函数用


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRigidFrameView, CView)


BEGIN_MESSAGE_MAP(CRigidFrameView, CView)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_COMMAND(ID_DESIGN_LLAYOUT, &CRigidFrameView::OnDesignLlayout)
	ON_COMMAND(ID_DESIGN_RLAYOUT, &CRigidFrameView::OnDesignRlayout)
	ON_COMMAND(ID_DESIGN_CLAYOUT, &CRigidFrameView::OnDesignClayout)
	ON_COMMAND(ID_DESIGN_CBASESTAKE, &CRigidFrameView::OnDesignBaseStake)
	ON_COMMAND(ID_DESIGN_LMOVE, &CRigidFrameView::OnDesignLmove)
	ON_COMMAND(ID_DESIGN_RMOVE, &CRigidFrameView::OnDesignRmove)
	ON_COMMAND(ID_DESIGN_MOVEDIS, &CRigidFrameView::OnDesignMovedis)
	ON_COMMAND(ID_DESIGN_LAYOUT_MACTION, &CRigidFrameView::OnDesignLayoutMaction)
	ON_COMMAND(ID_DESIGN_DOWNYZ, &CRigidFrameView::OnDesignDownyz)
	ON_COMMAND(ID_DESIGN_DOWNAUTO, &CRigidFrameView::OnDesignDownauto)
	ON_COMMAND(ID_DESIGN_TRANSITCAP, &CRigidFrameView::OnDesignTransitcap)
	ON_COMMAND(ID_DESIGN_TRANSITPILL, &CRigidFrameView::OnDesignTransitpill)
	ON_COMMAND(ID_DESIGN_APR_ABT, &CRigidFrameView::OnDesignAprAbt)
	ON_COMMAND(ID_DESIGN_APR_DOWN, &CRigidFrameView::OnDesignAprDown)
	ON_COMMAND(ID_DESIGN_APR_UP, &CRigidFrameView::OnDesignAprUp)
	ON_COMMAND(ID_OUTPUT_DOCTORBRIDGE, &CRigidFrameView::OnOutputDoctorbridge)
	ON_COMMAND(ID_OUTPUT_MIDAS, &CRigidFrameView::OnOutputMidas)
	ON_COMMAND(ID_DESIGN_BAR_HX, &CRigidFrameView::OnDesignBarHx)
	ON_COMMAND(ID_DESIGN_BARADJUST, &CRigidFrameView::OnDesignBaradjust)
	ON_COMMAND(ID_DESIGN_BARAUTO, &CRigidFrameView::OnDesignBarauto)
	ON_COMMAND(ID_DESIGN_BARCHECK, &CRigidFrameView::OnDesignBarcheck)
	ON_COMMAND(ID_DESIGN_BAROUT, &CRigidFrameView::OnDesignBarout)
	ON_COMMAND(ID_DESIGN_BARYZ, &CRigidFrameView::OnDesignBaryz)
	ON_COMMAND(ID_DESIGN_CAP, &CRigidFrameView::OnDesignCap)
	ON_COMMAND(ID_DESIGN_CELL_0, &CRigidFrameView::OnDesignCell0)
	ON_COMMAND(ID_DESIGN_CELL_BK, &CRigidFrameView::OnDesignCellBk)
	ON_COMMAND(ID_DESIGN_CELL_GER, &CRigidFrameView::OnDesignCellGer)
	ON_COMMAND(ID_DESIGN_CELL_HGB, &CRigidFrameView::OnDesignCellHgb)
	ON_COMMAND(ID_DESIGN_CELLYZ, &CRigidFrameView::OnDesignCellyz)
	ON_COMMAND(ID_DESIGN_CHIBAN, &CRigidFrameView::OnDesignChiban)
	ON_COMMAND(ID_DESIGN_DRAIN, &CRigidFrameView::OnDesignDrain)
	ON_COMMAND(ID_DESIGN_FRAMEWORK, &CRigidFrameView::OnDesignFramework)
	ON_COMMAND(ID_DESIGN_JOINT, &CRigidFrameView::OnDesignJoint)
	ON_COMMAND(ID_DESIGN_LAYOUT_ACTION, &CRigidFrameView::OnDesignLayoutAction)
	ON_COMMAND(ID_DESIGN_LAYOUT_YZ, &CRigidFrameView::OnDesignLayoutYz)
	ON_COMMAND(ID_DESIGN_MAINPIER, &CRigidFrameView::OnDesignMainpier)
	ON_COMMAND(ID_DESIGN_PAVE, &CRigidFrameView::OnDesignPave)
	ON_COMMAND(ID_DESIGN_PILL, &CRigidFrameView::OnDesignPill)
	ON_COMMAND(ID_DESIGN_TRANSIT, &CRigidFrameView::OnDesignTransit)
	ON_COMMAND(ID_OUT_DETAIL, &CRigidFrameView::OnOutDetail)
	ON_COMMAND(ID_OUTPUT_DRAWING, &CRigidFrameView::OnOutputDrawing)
	ON_COMMAND(ID_OUTPUT_MANAGER, &CRigidFrameView::OnOutputManager)
	ON_COMMAND(ID_DESIGN_SHOWCELLLEN, &CRigidFrameView::OnDesignShowcelllen)
	ON_COMMAND(ID_DESIGN_SHOWWEIGHT, &CRigidFrameView::OnDesignShowweight)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SHOWWEIGHT, &CRigidFrameView::OnUpdateDesignShowweight)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SHOWCELLLEN, &CRigidFrameView::OnUpdateDesignShowcelllen)
	ON_COMMAND(ID_PRJ_ROAD, &CRigidFrameView::OnPrjRoad)
	ON_COMMAND(ID_PRJ_SECT, &CRigidFrameView::OnPrjSect)
	ON_COMMAND(ID_PRJ_GEOLOGY, &CRigidFrameView::OnPrjGeology)
	ON_COMMAND(ID_PRJ_HYDROLOGY, &CRigidFrameView::OnPrjHydrology)
	ON_WM_CREATE()

	ON_COMMAND(ID_SHOW_MODEL, &CRigidFrameView::OnShowModel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


CRigidFrameView::CRigidFrameView() :isShowWeight(true), isShowLength(true)
{
}

CRigidFrameView::~CRigidFrameView()
{
}

BOOL CRigidFrameView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}


void CRigidFrameView::OnDraw(CDC* pDC)
{
}


void CRigidFrameView::OnFilePrintPreview()
{
	BCGPPrintPreview(this);
}


BOOL CRigidFrameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}


void CRigidFrameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


void CRigidFrameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


#ifdef _DEBUG
void CRigidFrameView::AssertValid() const
{
	CView::AssertValid();
}

void CRigidFrameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRigidFrameDoc* CRigidFrameView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRigidFrameDoc)));
	return (CRigidFrameDoc*)m_pDocument;
}
#endif


LRESULT CRigidFrameView::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC)wp);
		ASSERT_VALID(pDC);
		OnDraw(pDC);
	}
	return 0;
}


LRESULT CRigidFrameView::OnChangeVisualManager(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return 0;
}


void CRigidFrameView::OnDesignLlayout()
{
	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit *edit = (CBCGPRibbonEdit *)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_LLAYOUT));
	ASSERT_VALID(edit);

	pDoc->RigidFrameBridgeExample->m_YQLayOutResLeft = edit->GetEditText();
}


void CRigidFrameView::OnDesignRlayout()
{
	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit *edit = (CBCGPRibbonEdit *)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_RLAYOUT));
	ASSERT_VALID(edit);

	pDoc->RigidFrameBridgeExample->m_YQLayOutResRight = edit->GetEditText();
}


void CRigidFrameView::OnDesignClayout()
{
	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit *edit = (CBCGPRibbonEdit *)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_CLAYOUT));
	ASSERT_VALID(edit);

	pDoc->RigidFrameBridgeExample->m_ZQLayOutRes = edit->GetEditText();
}


void CRigidFrameView::OnDesignBaseStake()
{
	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit *edit = (CBCGPRibbonEdit *)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_CBASESTAKE));
	ASSERT_VALID(edit);

	pDoc->RigidFrameBridgeExample->m_CenterStake = edit->GetEditText();
}


void CRigidFrameView::OnDesignLmove()
{
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit* edit_move = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_MOVEDIS));
	ASSERT_VALID(edit_move);
	movedis = edit_move->GetEditText();
	if (movedis == _T(""))
	{
		AfxMessageBox(_T("错误：请输入主桥平移步长！"));
		return;
	}

	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	double m_Center = _wtof(pDoc->RigidFrameBridgeExample->m_CenterStake) - _wtof(movedis);

	/*检验移动是否超限*/
	double allLen = pDoc->RigidFrameBridgeExample->GetBrgAllLen();
	double startStake = m_Center - allLen / 2 + pDoc->RigidFrameBridgeExample->m_BridgeSpanArrange.front().m_CoordinateY;
	double m_TwBoundryLeft(0), m_TwBoundryRight(0);//起始填挖交界点
	pDoc->RigidFrameBridgeExample->m_RoadData.CalcTwBoundry(pDoc->RigidFrameBridgeExample->m_ZQBaseStake, 0, m_TwBoundryLeft, m_TwBoundryRight);//计算填挖交界点
	if (startStake <= m_TwBoundryLeft)
	{
		AfxMessageBox(_T("警告：您已将主墩移出填方区域最小边界，该移动操作无效！"));
		return;
	}
	CString m_CenterStakeTxt = _T("");
	m_CenterStakeTxt.Format(_T("%.3f"), m_Center);
	pDoc->RigidFrameBridgeExample->m_CenterStake = m_CenterStakeTxt;

	UpdateModel(pDoc);
}


void CRigidFrameView::OnDesignMovedis()
{
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit *edit = (CBCGPRibbonEdit *)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_MOVEDIS));
	ASSERT_VALID(edit);
	movedis = edit->GetEditText();
}


void CRigidFrameView::OnDesignRmove()
{
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit* edit_move = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_MOVEDIS));
	ASSERT_VALID(edit_move);
	movedis = edit_move->GetEditText();
	if (movedis == _T(""))
	{
		AfxMessageBox(_T("错误：请输入主桥平移步长！"));
		return;
	}

	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	double m_Center = _wtof(pDoc->RigidFrameBridgeExample->m_CenterStake) + _wtof(movedis);
	/*检验移动是否超限*/
	double allLen = pDoc->RigidFrameBridgeExample->GetBrgAllLen();
	double endStake = m_Center + allLen / 2 - pDoc->RigidFrameBridgeExample->m_BridgeSpanArrange.back().m_CoordinateY;
	double m_TwBoundryLeft(0), m_TwBoundryRight(0);//起始填挖交界点
	pDoc->RigidFrameBridgeExample->m_RoadData.CalcTwBoundry(pDoc->RigidFrameBridgeExample->m_ZQBaseStake, 0, m_TwBoundryLeft, m_TwBoundryRight);//计算填挖交界点
	if (endStake >= m_TwBoundryRight)
	{
		AfxMessageBox(_T("警告：您已将主墩移出填方区域最大边界，该移动操作无效！"));
		return;
	}

	CString m_CenterStakeTxt = _T("");
	m_CenterStakeTxt.Format(_T("%.3f"), m_Center);
	pDoc->RigidFrameBridgeExample->m_CenterStake = m_CenterStakeTxt;

	UpdateModel(pDoc);
}


void CRigidFrameView::OnDesignLayoutMaction()
{
	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	UpdateModel(pDoc);
}


void CRigidFrameView::OnDesignDownyz()
{
}


void CRigidFrameView::OnDesignDownauto()
{
}


void CRigidFrameView::OnDesignTransitcap()
{
}


void CRigidFrameView::OnDesignTransitpill()
{
}


void CRigidFrameView::OnDesignAprAbt()
{
}


void CRigidFrameView::OnDesignAprDown()
{
}


void CRigidFrameView::OnDesignAprUp()
{
}


void CRigidFrameView::OnOutputDoctorbridge()
{
}


void CRigidFrameView::OnOutputMidas()
{
}


void CRigidFrameView::OnDesignBarHx()
{
}


void CRigidFrameView::OnDesignBaradjust()
{
	CString str;
	str.Format(_T("调整钢束: %d"), CBCGPRibbonPaletteButton::GetLastSelectedItem(ID_DESIGN_BARADJUST));
	AfxMessageBox(str);
}


void CRigidFrameView::OnDesignBarauto()
{
}


void CRigidFrameView::OnDesignBarcheck()
{
}


void CRigidFrameView::OnDesignBarout()
{
}


void CRigidFrameView::OnDesignBaryz()
{
}


void CRigidFrameView::OnDesignCap()
{
}


void CRigidFrameView::OnDesignCell0()
{
}


void CRigidFrameView::OnDesignCellBk()
{
}


void CRigidFrameView::OnDesignCellGer()
{
}


void CRigidFrameView::OnDesignCellHgb()
{
}


void CRigidFrameView::OnDesignCellyz()
{
	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDlgCellSplitYz dlg(pDoc);
	dlg.DoModal();
}


void CRigidFrameView::OnDesignChiban()
{
}


//测试建立钢束的行为
void CRigidFrameView::OnDesignDrain()
{
	CMainGirder* grdL = nullptr;
	CMainGirder* grdR = nullptr;

	//(1)获得桥对象
	CRigidFrameBridge* brg;
	CRigidFrameDoc* pDoc = (CRigidFrameDoc*)GetDocument();
	brg = pDoc->RigidFrameBridgeExample;

	//(2)打开文件待写
	FILE *fp = _wfopen(L"D:\\1.scr", L"w");


	//(3)构造T钢束
	grdL = (CMainGirder*)brg->m_MainGirders[2];
	grdR = (CMainGirder*)brg->m_MainGirders[2];

	CBarTSteelBar* barT;
	for (int i = 106; i <= 118; i++)
	{
		barT = new CBarTSteelBar(grdL, grdR, -i, i);
		if (!CBarSteelBar::b_HasGetinfo)
		{
			barT->GetBridgeInfo(brg);
		}
		barT->SetBasePLanPara(3.3 + (i - 101)*0.17, 3.3, 3.3, 2, 2, 4, 10, 4, 10);
		barT->SetBaseProPara(_T("T"), 0.62, 0.62, 0.14, 11, 11, 8, 8, 0.15, 0.15);
		barT->CalDetailPlanPara();
		barT->CalDetailProPara();

		barT->createPtArray();
		double x, y, z;
		fwprintf(fp, L"3DPOLY");

		for (vector<PT3D*>::iterator it = barT->m_BarPtarrayReal.begin(); it != barT->m_BarPtarrayReal.end(); it++)
		{
			x = (*it)->x;
			y = (*it)->y;
			z = (*it)->z;
			fwprintf(fp, L"\n%.4lf,%.4lf,%.4lf", x, y, z);
		}
		fwprintf(fp, L"\n\n");
		delete barT;
		barT = nullptr;
	}


	//(4)构造X钢束
	grdL = (CMainGirder*)brg->m_MainGirders[2];
	grdR = (CMainGirder*)brg->m_MainGirders[2];

	CBarXSteelBar* barX;
	for (int i = 101; i <= 118; i++)
	{
		barX = new CBarXSteelBar(grdL, grdR, -i, i);
		if (!CBarSteelBar::b_HasGetinfo)
		{
			barX->GetBridgeInfo(brg);
		}
		barX->SetBasePLanPara(2.95, 2.95, 2.95, 2, 2, 0, 10, 0, 10);
		barX->SetBaseProPara(_T("X"), 7.8 - (i - 101)*0.35, 7.8 - (i - 101)*0.35, 2.5 - (i - 101)*0.15, 35 - (i - 101)*1.2, 35 - (i - 101)*1.2, 10, 10, 0.15, 0.15);
		barX->CalDetailPlanPara();
		barX->CalDetailProPara();

		barX->createPtArray();
		double x, y, z;
		fwprintf(fp, L"3DPOLY");
		for (vector<PT3D*>::iterator it = barX->m_BarPtarrayReal.begin(); it != barX->m_BarPtarrayReal.end(); it++)
		{
			x = (*it)->x;
			y = (*it)->y;
			z = (*it)->z;
			fwprintf(fp, L"\n%.4lf,%.4lf,%.4lf", x, y, z);
		}
		fwprintf(fp, L"\n\n");
		delete barX;
		barX = nullptr;
	}

	//(5)构造D钢束
	grdL = (CMainGirder*)brg->m_MainGirders[2];
	grdR = (CMainGirder*)brg->m_MainGirders[4];

	CBarDSteelBar* barD;
	for (int i = 115; i <= 117; i++)
	{
		barD = new CBarDSteelBar(grdL, grdR, i, -i);
		if (!CBarSteelBar::b_HasGetinfo)
		{
			barD->GetBridgeInfo(brg);
		}
		barD->SetBasePLanPara(1.52 + (i - 115)*0.21, 2.15, 2.15, 2.5, 2.5, 4, 20, 4, 20);
		barD->SetBaseProPara(_T("D"), 0.92, 0.92, 0.3, 12, 12, 10, 10, 0.2, 0.2);
		barD->CalDetailPlanPara();
		barD->CalDetailProPara();

		barD->createPtArray();
		double x, y, z;
		fwprintf(fp, L"3DPOLY");
		for (vector<PT3D*>::iterator it = barD->m_BarPtarrayReal.begin(); it != barD->m_BarPtarrayReal.end(); it++)
		{
			x = (*it)->x;
			y = (*it)->y;
			z = (*it)->z;
			fwprintf(fp, L"\n%.4lf,%.4lf,%.4lf", x, y, z);
		}
		fwprintf(fp, L"\n\n");
		delete barD;
		barD = nullptr;
	}

	//(6)构造H钢束
	grdL = (CMainGirder*)brg->m_MainGirders[0];
	grdR = (CMainGirder*)brg->m_MainGirders[2];
	CBarHSteelBar* barH;
	for (int i = 115; i <= 117; i++)
	{
		barH = new CBarHSteelBar(grdL, grdR, 101, -i);
		if (!CBarSteelBar::b_HasGetinfo)
		{
			barH->GetBridgeInfo(brg);
		}
		barH->SetBasePLanPara(0.8 + (i - 115)*0.45, 0.8 + (i - 115)*0.45, 2.15, 2.5, 2.5, 0, 12, 6, 12);
		barH->SetBaseProPara(_T("H"), 0.7, 0.92, 0.3, 8, 12, 10, 10, 0.15, 0.2);
		barH->CalDetailPlanPara();
		barH->CalDetailProPara();

		barH->createPtArray();
		double x, y, z;
		fwprintf(fp, L"3DPOLY");
		for (vector<PT3D*>::iterator it = barH->m_BarPtarrayReal.begin(); it != barH->m_BarPtarrayReal.end(); it++)
		{
			x = (*it)->x;
			y = (*it)->y;
			z = (*it)->z;
			fwprintf(fp, L"\n%.4lf,%.4lf,%.4lf", x, y, z);
		}
		fwprintf(fp, L"\n\n");
		delete barH;
		barH = nullptr;
	}


	//(7)构造Z钢束
	grdL = (CMainGirder*)brg->m_MainGirders[2];
	grdR = (CMainGirder*)brg->m_MainGirders[4];

	CBarZSteelBar* barZ;
	for (int i = 111; i <= 117; i++)
	{
		barZ = new CBarZSteelBar(grdL, grdR, i, -i);
		if (!CBarSteelBar::b_HasGetinfo)
		{
			barZ->GetBridgeInfo(brg);
		}
		barZ->SetBasePLanPara(0.7 + (i - 111)*0.21, 2.17, 2.17, 2.5, 2.5, 4, 15, 4, 15);
		barZ->SetBaseProPara(_T("Z"), 0.35, 0.35, 0.15, 8, 8, 10, 10, 0.2, 0.2);

		barZ->CalDetailPlanPara();
		barZ->CalDetailProPara();
		barZ->createPtArray();
		double x, y, z;
		fwprintf(fp, L"3DPOLY");
		for (vector<PT3D*>::iterator it = barZ->m_BarPtarrayReal.begin(); it != barZ->m_BarPtarrayReal.end(); it++)
		{
			x = (*it)->x;
			y = (*it)->y;
			z = (*it)->z;
			fwprintf(fp, L"\n%.4lf,%.4lf,%.4lf", x, y, z);
		}
		fwprintf(fp, L"\n\n");
		delete barZ;
		barZ = nullptr;
	}

	//(8)构造B钢束
	grdL = (CMainGirder*)brg->m_MainGirders[0];
	grdR = (CMainGirder*)brg->m_MainGirders[2];
	CBarBSteelBar* barB;
	for (int i = 114; i <= 116; i++)
	{
		barB = new CBarBSteelBar(grdL, grdR, 101, -i);
		if (!CBarSteelBar::b_HasGetinfo)
		{
			barB->GetBridgeInfo(brg);
		}
		barB->SetBasePLanPara(0.85 + (i - 114)*0.45, 0.85 + (i - 114)*0.45, 2.2, 2.5, 2.5, 0, 0, 8, 15);
		barB->SetBaseProPara(_T("B"), _T("左"), 0.8, 0.35, 0.14, 8, 8, 10, 10, 0.2, 0.2);
		barB->CalDetailPlanPara();
		barB->CalDetailProPara();

		barB->createPtArray();
		double x, y, z;
		fwprintf(fp, L"3DPOLY");
		for (vector<PT3D*>::iterator it = barB->m_BarPtarrayReal.begin(); it != barB->m_BarPtarrayReal.end(); it++)
		{
			x = (*it)->x;
			y = (*it)->y;
			z = (*it)->z;
			fwprintf(fp, L"\n%.4lf,%.4lf,%.4lf", x, y, z);
		}
		fwprintf(fp, L"\n\n");
		delete barZ;
		barZ = nullptr;
	}
	fclose(fp);
	AfxMessageBox(L"OK");


	//测试竖向预应力
	CString str;

	fp = _wfopen(L"D:\\2.scr", L"w");
	CVprestressObjects* sl = new CVprestressObjects();

	str.Format(_T("%d"), sl->m_bars.size());

	sl->AutoDesign(brg);
	str.Format(_T("%d"), sl->m_bars.size());
	AfxMessageBox(str);
	sl->RewriteBarname();
	str.Format(_T("%d"), sl->m_bars.size());
	AfxMessageBox(str);

	PT3D* pt = nullptr;
	int tt = -1;
	double y1, y2, z;

	for (vector<CVprestressBar*>::iterator iter = sl->m_bars.begin(); iter != sl->m_bars.end(); iter++)
	{
		fwprintf(fp, L"\n3dpoly");
		for (vector<PT3D*>::iterator piter = (*iter)->m_BarPtarrayReal.begin(); piter != (*iter)->m_BarPtarrayReal.end(); piter++)
		{
			fwprintf(fp, L"\n%.4lf,%.4lf,%.4lf", (*piter)->z, (*piter)->y, (*piter)->x);
			if (tt == -1)
				y1 = (*piter)->y;
			else
				y2 = (*piter)->y;
			tt *= -1;
			z = (*piter)->z;
		}
		fwprintf(fp, L"\n");
		fwprintf(fp, L"\ntext");
		fwprintf(fp, L"\n%.4lf,%.4lf", z - 0.02, y1 + 0.1);
		fwprintf(fp, L"\n0.1");
		fwprintf(fp, L"\n0");
		fwprintf(fp, L"\n%s", (*iter)->m_Name);

		fwprintf(fp, L"\ntext");
		fwprintf(fp, L"\n%.4lf,%.4lf", z - 0.05, y2 - 0.2);
		fwprintf(fp, L"\n0.05");
		fwprintf(fp, L"\n0");
		fwprintf(fp, L"\n%.4lf", (*iter)->m_Alength);

	}

	fclose(fp);
	delete sl;
	sl = nullptr;
	AfxMessageBox(L"SLOK");
}


void CRigidFrameView::OnDesignFramework()
{
}


void CRigidFrameView::OnDesignJoint()
{
}


void CRigidFrameView::OnDesignLayoutAction()
{
	CRigidFrameDoc *doc = GetDocument();
	ASSERT_VALID(doc);

	CRigidFrameBridge *brg = doc->RigidFrameBridgeExample;

	if (brg->m_BridgeSpanArrange.size() != 0)
	{
		int answer = MessageBox(_T("该操作将覆盖您当前正在编辑的刚构桥方案,是否继续?"), _T("提示"), MB_YESNO | MB_ICONINFORMATION);
		if (answer == IDNO)
		{
			return;
		}
	}

	brg->PreAutoLayOut(0); //暂定第一个主桥自动布孔方案为当前打开方案

	UpdateModel(doc);

	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	pFrame->Proj->SetTreeCtrl(pFrame->m_wndWorkSpace.GetTreeCtrl());
	CString ZQParentName = brg->m_ZQName;
	CString ZQCurrentName = brg->m_ZQName + _T("-自动布孔方案_1");
	pFrame->Proj->RenameBridge(ZQParentName, ZQCurrentName);
	brg->m_ZQName = ZQCurrentName;
	doc->DoSave(pFrame->Proj->GetProjectPath() + _T("\\") + brg->m_ZQName + _T(".rfm"));

	int num = 1;
	unsigned int num_1 = brg->GetBridgeSpanArrangeAll().size();
	for (unsigned int i = 0; i < num_1; i++)
	{
		if (i == brg->m_ZQID)
			continue;
		CRigidFrameDoc *doc_x = (CRigidFrameDoc*)theApp.pDocTemplate_Ger->CreateNewDocument();
		doc_x->RigidFrameBridgeExample = new CRigidFrameBridge(*brg);
		CString str = _T("");
		CString title = _T("");
		title.Format(_T("%s-自动布孔方案_%d"), ZQParentName, ++num);
		doc_x->RigidFrameBridgeExample->m_ZQName = title;
		doc_x->RigidFrameBridgeExample->m_BridgeSpanArrange = brg->GetBridgeSpanArrangeAll().at(i);
		doc_x->RigidFrameBridgeExample->GetBridgeLayOutText(); //更新主桥自动布孔字符串
		doc_x->RigidFrameBridgeExample->YQAutoSetSpan(5, 10); //引桥自动布孔并更新布孔字符串
		doc_x->RigidFrameBridgeExample->CreateMainGirder();
		pFrame->Proj->AddBridge(title);
		doc_x->DoSave(pFrame->Proj->GetProjectPath() + _T("\\") + title + _T(".rfm"));
		delete doc_x;
		doc_x = nullptr;
	}
	return;
}


void CRigidFrameView::OnDesignLayoutYz()
{
	CDlgAutoLayoutYz dlg;
	if (dlg.DoModal() == IDOK)
	{
		CRigidFrameDoc *pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		int layoutMode = 0;
		if (dlg.m_ProgramDesign)
		{
			layoutMode = 1;
		}
		if (dlg.m_InitialDesign)
		{
			layoutMode = 2;
		}
		if (dlg.m_DeepDesign)
		{
			layoutMode = 3;
		}
	}
}


void CRigidFrameView::OnDesignMainpier()
{
	// TODO:  在此添加命令处理程序代码
}


void CRigidFrameView::OnDesignPave()
{
	// TODO:  在此添加命令处理程序代码
}


void CRigidFrameView::OnDesignPill()
{
	// TODO:  在此添加命令处理程序代码
	//AfxMessageBox(_T("hello,piles"));
	CDlgPiles p;
	p.DoModal();
}


void CRigidFrameView::OnDesignTransit()
{
	// TODO:  在此添加命令处理程序代码
}


void CRigidFrameView::OnOutDetail()
{
	// TODO:  在此添加命令处理程序代码
}


void CRigidFrameView::OnOutputDrawing()
{
	// TODO:  在此添加命令处理程序代码
}


void CRigidFrameView::OnOutputManager()
{
#if 0
	// TODO:  在此添加命令处理程序代码
	DL_Dxf dxf;
	DL_WriterA* dw = dxf.out("C:\\Users\\yanwen\\Desktop\\TestDXF.dxf", DL_Codes::AC1015);
	// section header:
	dxf.writeHeader(*dw);
	dw->sectionEnd();
	// section tables:
	dw->sectionTables();
	// VPORT:
	dxf.writeVPort(*dw);
	//dw->dxfInt(76, 1);
	// LTYPE:
	dw->tableLinetypes(1);
	dxf.writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYLAYER", "", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "", 0, 0, 0.0));
	dw->tableEnd();
	// LAYER:
	dw->tableLayers(1);
	dxf.writeLayer(
		*dw,
		DL_LayerData("0", 0),
		DL_Attributes("", 4, 0, "CONTINUOUS", 1.0)
		);
	dw->tableEnd();
	// STYLE:
	dw->tableStyle(1);
	DL_StyleData style("Standard", 0, 0.0, 0.8, 0.0, 0, 2.5, "fst", "chnfont");
	style.bold = false;
	style.italic = true;
	dxf.writeStyle(*dw, style);
	dw->tableEnd();
	// VIEW:
	dxf.writeView(*dw);
	// UCS:
	dxf.writeUcs(*dw);
	// APPID:
	dw->tableAppid(1);
	dxf.writeAppid(*dw, "ACAD");
	dw->tableEnd();
	// DIMSTYLE:
	dxf.writeDimStyle(*dw, 2.0, 1.5, 0, 0.625, 2.0);
	// BLOCK_RECORD:
	dxf.writeBlockRecord(*dw);
	dw->tableEnd();
	dw->sectionEnd();
	// BLOCK:
	dw->sectionBlocks();
	dxf.writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Model_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space0");
	dw->sectionEnd();
	// ENTITIES:
	dw->sectionEntities();
	DL_Attributes attributes("0", 4, 0, "CONTINUOUS", 1.0);
	// start hatch with one loop:
	DL_HatchData data(1, false, 10.0, 0.0, "ANSI31", 0.0, 0.0);
	dxf.writeHatch1(*dw, data, attributes);
	// start loop:
	DL_HatchLoopData lData(1);
	dxf.writeHatchLoop1(*dw, lData);
	// write edge:
	DL_HatchEdgeData eData(
		200.0,
		200.0,
		10.0,
		0.0,
		M_PI * 2,
		true
		);
	dxf.writeHatchEdge(*dw, eData);
	// end loop:
	dxf.writeHatchLoop2(*dw, lData);
	// end hatch:
	dxf.writeHatch2(*dw, data, attributes);
	//改变颜色
	attributes.setColor(2);
	//绘制直线
	DL_LineData dataline(10, 10, 0, 80, 80, 0);
	dxf.writeLine(*dw, dataline, attributes);
	//绘制圆弧
	DL_ArcData dataarc(100.0, 100.0, 0.0, 100, 10, 80);
	dxf.writeArc(*dw, dataarc, attributes);
	//绘制圆
	DL_CircleData datacir(100, 0, 0, 50);
	dxf.writeCircle(*dw, datacir, attributes);
	//单行文本
	DL_TextData datatext(100, 50, 0, 105, 55, 0, 4, 0.8, 0, 0, 0, "test", "Standard", 0);
	dxf.writeText(*dw, datatext, attributes);
	//绘制多义线
	attributes.setColor(1);
	DL_PolylineData datapolyline(5, 1, 5, 0);
	dxf.writePolyline(*dw, datapolyline, attributes);
	DL_VertexData dataver(1, 1, 0);
	dxf.writeVertex(*dw, dataver);
	dataver.x = 10;
	dataver.y = 10;
	dxf.writeVertex(*dw, dataver);
	dataver.x = 100;
	dataver.y = 30;
	dxf.writeVertex(*dw, dataver);
	dataver.x = 80;
	dataver.y = 40;
	dxf.writeVertex(*dw, dataver);
	dxf.writePolylineEnd(*dw);
	//标注
	attributes.setColor(3);
	DL_DimensionData datadim(1, 1, 0, 1, 20, 0, 1, 1, 1, 1, "888", "Standard", 0, 1, 1);
	//对齐标注
	DL_DimAlignedData datadimali(3, 3, 0, 22, 2, 0);
	dxf.writeDimAligned(*dw, datadim, datadimali, attributes);
	//角度标注
	DL_DimensionData datadim1(0, 0, 0, 1, 20, 0, 2, 1, 1, 1, "角度标注%%d", "Standard", 0, 1, 1);
	DL_DimAngularData datadimang(0, 0, 0, 10, 15, 0, 10, 2, 0, 10, 8, 0);
	dxf.writeDimAngular(*dw, datadim1, datadimang, attributes);
	//多行文本
	DL_MTextData datamtext(50.0, 150.0, 0.0, 50.0, 150.0, 0.0, 4.0, 100.0, 1, 1, 1, 1.0,
		"\\A1;注\\P\\pxi-1,l1,t0,1;1.^I本图尺寸除标高、里程以米计外，其余均以厘米计；\\P2.^I荷载等级：公路 - Ⅰ级；桥面净宽：2×净11m；\\P3.^I本桥上部结构采用3联(4×20+3×20+4×20)先简支后结构连续T形梁；下部结构桥台采用U台配扩大基础和肋板台配承台桩基础，桥墩采用桩基础, 5～10号墩采用端承桩，其余按摩擦桩设计;\\P4.^I本桥在5、10、15号桥墩及桥台处设置80型伸缩缝，其余墩顶处均为结构连续；5、10、15号桥墩及桥台处采用GYZF{\\H0.1x;\\S^ 4;}350×87型支座；其余桥墩处采用GYZ500×110型支座； \\P5.^I立面图中虚线为左侧地面线，点划线为中线地面线，实线为右侧地面线；中线立面展示中所示数值为靠近路基中心线一侧之值；为便于查看，部分钻孔位置做了适当平移；\\P6.^I若发现实际地形、地质与设计文件不符时，请与设计代表或设计单位联系协商解决。",
		"Standard", 0);
	dxf.writeMText(*dw, datamtext, attributes);
	// end section ENTITIES:
	dw->sectionEnd();
	dxf.writeObjects(*dw, "MY_OBJECTS");
	dxf.writeObjectsEnd(*dw);
	dw->dxfEOF();
	dw->close();
	delete dw;
	return;
#endif
}


void CRigidFrameView::OnDesignShowcelllen()
{
	isShowLength = !isShowLength;
}


void CRigidFrameView::OnDesignShowweight()
{
	isShowWeight = !isShowWeight;
}


void CRigidFrameView::OnUpdateDesignShowweight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isShowWeight);
}


void CRigidFrameView::OnUpdateDesignShowcelllen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isShowLength);
}


void CRigidFrameView::OnPrjRoad()
{
	CRigidFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRoadData& road = pDoc->RigidFrameBridgeExample->m_RoadData;
	CDlgLoadRoad dlg(road);
	if (dlg.DoModal() == IDOK)
	{
		double &leftStake = pDoc->RigidFrameBridgeExample->m_RoadData.m_TwBoundryLeft;
		double &rightStake = pDoc->RigidFrameBridgeExample->m_RoadData.m_TwBoundryRight;
		double outSpace = pDoc->RigidFrameBridgeExample->m_DefaultParam.m_TwBoundryExceedDis;
		pDoc->RigidFrameBridgeExample->m_RoadData.CalcTwBoundry(pDoc->RigidFrameBridgeExample->m_ZQBaseStake, outSpace, leftStake, rightStake);
		pDoc->getUpdateCallback()->Update();
		pDoc->RigidFrameBridgeExample->getUpdateCallback()->Lock();
		pDoc->RigidFrameBridgeExample->getUpdateCallback()->Update();
		pDoc->RigidFrameBridgeExample->getUpdateCallback()->Unlock();
		//OsgRender(doc->RigidFrameBridgeExample->m_RoadData.Draw());
	}
}


int CRigidFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//m_Osg = new cOSG(this);
	//m_Osg->CreateLight();//在cOSG初始化调用
	theApp.GetViewManager()->AddView(m_hWnd);
	CRigidFrameDoc* doc = static_cast<CRigidFrameDoc*>(GetDocument());
	theApp.GetViewManager()->SetSceneData(m_hWnd, doc->getUpdateCallback());
	theApp.GetViewManager()->Display(m_hWnd);
	return 0;
}

void CRigidFrameView::OnDestroy()
{
	theApp.GetViewManager()->RemoveView(m_hWnd);
	CView::OnDestroy();
}

void CRigidFrameView::OnPrjSect()
{
	// TODO:  在此添加命令处理程序代码
	CRigidFrameDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDlgSectInfo dlg(pDoc);
	if (dlg.DoModal() == IDOK)
	{
		pDoc->RigidFrameBridgeExample->m_ZQWidth = dlg.m_Wt;
		//更新是否保存标签
		theApp.projectModified = true;
		pDoc->SetModifiedFlag(TRUE);
		if (pDoc->RigidFrameBridgeExample->m_MainGirders.size() > 0)
		{
			UpdateModel(pDoc);
		}
	}
}

void CRigidFrameView::OnPrjGeology()
{
	// TODO:  在此添加命令处理程序代码
}


void CRigidFrameView::OnPrjHydrology()
{
	// TODO:  在此添加命令处理程序代码
}


bool CRigidFrameView::UpdateModel(CRigidFrameDoc * pDoc)
{
	CRigidFrameBridge *brg = pDoc->RigidFrameBridgeExample;

	if (brg->m_CenterStake == _T(""))
	{
		AfxMessageBox(_T("错误：请输入主桥布孔中心桩号！"));
		return false;
	}
	if (brg->m_ZQLayOutRes == _T(""))
	{
		AfxMessageBox(_T("错误：请输入主桥布孔方案！"));
		return false;
	}
	if (brg->m_RoadData.m_GroundFile == _T(""))
	{
		AfxMessageBox(_T("错误：请载入地面线数据！"));
		return false;
	}
	if (brg->m_RoadData.m_DesignFile == _T(""))
	{
		AfxMessageBox(_T("错误：请载入设计线数据！"));
		return false;
	}
	if (brg->m_RoadData.m_CrossFile == _T(""))
	{
		AfxMessageBox(_T("错误：请载入横断面数据！"));
		return false;
	}

	vector<double> m_ZQSpanVec = brg->ResolveStrToSpan(brg->m_ZQLayOutRes);
	double allLen = accumulate(m_ZQSpanVec.begin(), m_ZQSpanVec.end(), 0);
	double startStake = _wtof(brg->m_CenterStake) - allLen / 2 + m_ZQSpanVec.front();
	if (startStake <= brg->m_RoadData.m_TwBoundryLeft)
	{
		AfxMessageBox(_T("警告：您已将主墩移出填方区域最小边界，该移动操作无效！"));
		return false;
	}
	double endStake = _wtof(brg->m_CenterStake) + allLen / 2 - m_ZQSpanVec.back();
	if (endStake >= brg->m_RoadData.m_TwBoundryRight)
	{
		AfxMessageBox(_T("警告：您已将主墩移出填方区域最大边界，该移动操作无效！"));
		return false;
	}

	brg->SetBridgeSpan(brg->m_YQLayOutResLeft, brg->m_ZQLayOutRes, brg->m_YQLayOutResRight, _wtof(brg->m_CenterStake)); //主桥指定布孔
	if (_T("") == brg->m_YQLayOutResLeft || _T("") == brg->m_YQLayOutResRight)
	{
		brg->YQAutoSetSpan(5, 10); //引桥自动布孔并更新布孔字符串
	}
	brg->CreateMainGirder(); //创建主梁对象

	pDoc->RigidFrameBridgeExample->getUpdateCallback()->Lock();
	pDoc->RigidFrameBridgeExample->getUpdateCallback()->Update();
	pDoc->RigidFrameBridgeExample->getUpdateCallback()->Unlock();


	//更新菜单栏布孔编辑框
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();//获得主框架指针
	CBCGPRibbonEdit* edit_1 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_LLAYOUT));
	CBCGPRibbonEdit* edit_2 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_CLAYOUT));
	CBCGPRibbonEdit* edit_3 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_RLAYOUT));
	CBCGPRibbonEdit* edit_4 = (CBCGPRibbonEdit*)(pFrame->GetRibbonBar()->FindByID(ID_DESIGN_CBASESTAKE));
	ASSERT_VALID(edit_1);
	ASSERT_VALID(edit_2);
	ASSERT_VALID(edit_3);
	ASSERT_VALID(edit_4);
	edit_1->SetEditText(brg->m_YQLayOutResLeft);
	edit_2->SetEditText(brg->m_ZQLayOutRes);
	edit_3->SetEditText(brg->m_YQLayOutResRight);
	edit_4->SetEditText(brg->m_CenterStake);

	//更新是否保存标签
	theApp.projectModified = true;
	pDoc->SetModifiedFlag(TRUE);

	return true;
}



// 显示模式切换函数
void CRigidFrameView::setCurShowModel(int index)
{
	CMainFrame* mainFrm = (CMainFrame*)AfxGetMainWnd();
	CBCGPRibbonBar* rbar = (CBCGPRibbonBar*)mainFrm->GetRibbonBar();
	if (rbar != nullptr)
	{
		CBCGPRibbonCategory* cat = (CBCGPRibbonCategory*)rbar->GetCategory(6);
		CBCGPRibbonPanel* panel = (CBCGPRibbonPanel*)cat->GetPanel(3);
		CString str(_T("显示模式： "));
		switch (index)
		{
		case 0:
			str += _T("(方案设计模式)");
			break;
		case 1:
			str += _T("(构件编辑模式)");
			break;
		case 2:
			str += _T("(钢束调整模式)");
			break;
		case 3:
			str += _T("(建设施工模式)");
			break;
		default:
			break;
		}

		CBCGPRibbonPaletteButton* bnt = (CBCGPRibbonPaletteButton*)panel->GetElement(0);
		bnt->SelectItem(index);
		panel->SetName(str);

		// 显示模式切换后属性列表区的更新
		mainFrm->m_wndWorkSpace.UpdatePropList(index);
	}
}



void CRigidFrameView::OnShowModel()
{
	setCurShowModel(CBCGPRibbonPaletteButton::GetLastSelectedItem(ID_SHOW_MODEL));
}
