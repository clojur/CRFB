// DlgCellSplit.cpp : 实现文件
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "DlgCellSplitYz.h"
#include "BCGPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CDlgCellSplit 对话框

IMPLEMENT_DYNAMIC(CDlgCellSplitYz, CBCGPDialog)

CDlgCellSplitYz::CDlgCellSplitYz(CRigidFrameDoc* pDoc, CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgCellSplitYz::IDD, pParent)
{
	myDoc = pDoc;
	EnableVisualManagerStyle(TRUE, TRUE);
}

CDlgCellSplitYz::~CDlgCellSplitYz()
{
}

void CDlgCellSplitYz::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REMAIN, m_Remain);
	DDX_Text(pDX, IDC_EDIT_INITWEB, m_InitWeb);
	DDX_Text(pDX, IDC_EDIT_25MIN, m_25Min);
	DDX_Text(pDX, IDC_EDIT_30MIN, m_30Min);
	DDX_Text(pDX, IDC_EDIT_35MIN, m_35Min);
	DDX_Text(pDX, IDC_EDIT_40MIN, m_40Min);
	DDX_Text(pDX, IDC_EDIT_45MIN, m_45Min);
	DDX_Text(pDX, IDC_EDIT_50MIN, m_50Min);
	DDX_Text(pDX, IDC_EDIT_55MIN, m_55Min);
	DDX_Text(pDX, IDC_EDIT_25MAX, m_25Max);
	DDX_Text(pDX, IDC_EDIT_30MAX, m_30Max);
	DDX_Text(pDX, IDC_EDIT_35MAX, m_35Max);
	DDX_Text(pDX, IDC_EDIT_40MAX, m_40Max);
	DDX_Text(pDX, IDC_EDIT_45MAX, m_45Max);
	DDX_Text(pDX, IDC_EDIT_50MAX, m_50Max);
	DDX_Text(pDX, IDC_EDIT_55MAX, m_55Max);
}


BEGIN_MESSAGE_MAP(CDlgCellSplitYz, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CDlgCellSplitYz::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCellSplit 消息处理程序


BOOL CDlgCellSplitYz::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 改变对话框上所有控件字体
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	m_Remain = myDoc->RigidFrameBridgeExample->m_MainGirders.at(2)->m_Remain;
	m_InitWeb = 0.6;

	m_25Min = myDoc->RigidFrameBridgeExample->m_DefaultParam.n1B;
	m_30Min = myDoc->RigidFrameBridgeExample->m_DefaultParam.n2B;
	m_35Min = myDoc->RigidFrameBridgeExample->m_DefaultParam.n3B;
	m_40Min = myDoc->RigidFrameBridgeExample->m_DefaultParam.n4B;
	m_45Min = myDoc->RigidFrameBridgeExample->m_DefaultParam.n5B;
	m_50Min = myDoc->RigidFrameBridgeExample->m_DefaultParam.n6B;
	m_55Min = myDoc->RigidFrameBridgeExample->m_DefaultParam.n7B;

	m_25Max = myDoc->RigidFrameBridgeExample->m_DefaultParam.n1E;
	m_30Max = myDoc->RigidFrameBridgeExample->m_DefaultParam.n2E;
	m_35Max = myDoc->RigidFrameBridgeExample->m_DefaultParam.n3E;
	m_40Max = myDoc->RigidFrameBridgeExample->m_DefaultParam.n4E;
	m_45Max = myDoc->RigidFrameBridgeExample->m_DefaultParam.n5E;
	m_50Max = myDoc->RigidFrameBridgeExample->m_DefaultParam.n6E;
	m_55Max = myDoc->RigidFrameBridgeExample->m_DefaultParam.n7E;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgCellSplitYz::OnBnClickedOk()
{
	UpdateData(TRUE);

	myDoc->RigidFrameBridgeExample->m_DefaultParam.n1B = m_25Min;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n2B = m_30Min;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n3B = m_35Min;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n4B = m_40Min;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n5B = m_45Min;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n6B = m_50Min;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n7B = m_55Min;

	myDoc->RigidFrameBridgeExample->m_DefaultParam.n1E = m_25Max;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n2E = m_30Max;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n3E = m_35Max;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n4E = m_40Max;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n5E = m_45Max;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n6E = m_50Max;
	myDoc->RigidFrameBridgeExample->m_DefaultParam.n7E = m_55Max;

	CBCGPDialog::OnOK();
}
