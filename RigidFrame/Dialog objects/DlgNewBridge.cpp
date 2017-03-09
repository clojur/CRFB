// DlgNewBridge.cpp : 实现文件
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "DlgNewBridge.h"
#include "BCGPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgNewBridge 对话框

IMPLEMENT_DYNAMIC(CDlgNewBridge, CBCGPDialog)

CDlgNewBridge::CDlgNewBridge(int index, CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgNewBridge::IDD, pParent)
	, m_CenterPos(10350)
{
	m_Newbridge.Format(_T("%s_%d"), _T("RigidFrameBridge"), index);
	EnableVisualManagerStyle(TRUE, TRUE);
}

CDlgNewBridge::~CDlgNewBridge()
{
}

void CDlgNewBridge::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEWBRIDGE, m_Newbridge);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS, m_CenterPos);
	DDV_MinMaxDouble(pDX, m_CenterPos, 0, 1000000);
}


BEGIN_MESSAGE_MAP(CDlgNewBridge, CBCGPDialog)

	ON_BN_CLICKED(IDOK, &CDlgNewBridge::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewBridge 消息处理程序




void CDlgNewBridge::OnBnClickedOk()
{
	CBCGPDialog::OnOK();
	
	// 临时将新桥名存储于App对象全局区
//	UpdateData(TRUE);
	theApp.tmp_StrNewbridge = m_Newbridge;
	theApp.tmp_CenterMileage = m_CenterPos;

	//CDocument* pDocument = pChild->GetActiveDocument();
}


BOOL CDlgNewBridge::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 改变对话框上所有控件字体
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}
