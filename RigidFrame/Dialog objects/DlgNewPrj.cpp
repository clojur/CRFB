// DlgNewPrj.cpp : 实现文件
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "DlgNewPrj.h"
#include "BCGPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CDlgNewPrj 对话框

IMPLEMENT_DYNAMIC(CDlgNewPrj, CBCGPDialog)

CDlgNewPrj::CDlgNewPrj(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgNewPrj::IDD, pParent)
, m_projectName(_T("XXRoad"))
, m_projectPath(_T("C:\\"))
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CDlgNewPrj::~CDlgNewPrj()
{
}

void CDlgNewPrj::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEWPRJ_EDIT1, m_projectName);
	DDX_Control(pDX, IDC_EDIT_PRJPATH, m_wndProjectPath);
	DDX_Text(pDX, IDC_EDIT_PRJPATH, m_projectPath);
}


BEGIN_MESSAGE_MAP(CDlgNewPrj, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CDlgNewPrj::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewPrj 消息处理程序


void CDlgNewPrj::OnBnClickedOk()
{
	UpdateData(TRUE);
	theApp.tmp_StrProjectname = m_projectName;
	theApp.tmp_StrProjectpath = m_projectPath;

	CDlgNewPrj::OnOK();
}


BOOL CDlgNewPrj::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 指定文件夹浏览的缺省提示
	m_wndProjectPath.EnableFolderBrowseButton(_T("选择工程存放路径"));

	// 改变对话框上所有控件字体
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	return TRUE;
}
