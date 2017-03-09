// DlgRenameBridge.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "DlgRenameBridge.h"
#include "BCGPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CDlgRenameBridge �Ի���

IMPLEMENT_DYNAMIC(CDlgRenameBridge, CBCGPDialog)

CDlgRenameBridge::CDlgRenameBridge(CString oldname, CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgRenameBridge::IDD, pParent)
, m_strName(oldname)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CDlgRenameBridge::~CDlgRenameBridge()
{
}

void CDlgRenameBridge::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BRIDGENAME, m_strName);
}


BEGIN_MESSAGE_MAP(CDlgRenameBridge, CBCGPDialog)
//	ON_BN_CLICKED(IDOK, &CDlgRenameBridge::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgRenameBridge ��Ϣ�������


//void CDlgRenameBridge::OnBnClickedOk()
//{
//	
//}


BOOL CDlgRenameBridge::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// �ı�Ի��������пؼ�����
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
