// DlgDelBridge.cpp : 实现文件
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "DlgDelBridge.h"
#include "BCGPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CDlgDelBridge 对话框

IMPLEMENT_DYNAMIC(CDlgDelBridge, CBCGPDialog)

CDlgDelBridge::CDlgDelBridge(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgDelBridge::IDD, pParent)
{
	m_strOp = _T("");
	EnableVisualManagerStyle(TRUE, TRUE);
}

CDlgDelBridge::~CDlgDelBridge()
{
}

void CDlgDelBridge::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDelBridge, CBCGPDialog)
	ON_BN_CLICKED(ID_REMOVE, &CDlgDelBridge::OnBnClickedRemove)
	ON_BN_CLICKED(ID_DEL, &CDlgDelBridge::OnBnClickedDel)
END_MESSAGE_MAP()


// CDlgDelBridge 消息处理程序


void CDlgDelBridge::OnBnClickedRemove()
{
	m_strOp = _T("remove");
	CBCGPDialog::OnOK();
}


void CDlgDelBridge::OnBnClickedDel()
{
	m_strOp = _T("del");
	CBCGPDialog::OnOK();
}


BOOL CDlgDelBridge::OnInitDialog()
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
	// 异常:  OCX 属性页应返回 FALSE
}
