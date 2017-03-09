// DlgNewBridge.cpp : ʵ���ļ�
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

// CDlgNewBridge �Ի���

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


// CDlgNewBridge ��Ϣ�������




void CDlgNewBridge::OnBnClickedOk()
{
	CBCGPDialog::OnOK();
	
	// ��ʱ���������洢��App����ȫ����
//	UpdateData(TRUE);
	theApp.tmp_StrNewbridge = m_Newbridge;
	theApp.tmp_CenterMileage = m_CenterPos;

	//CDocument* pDocument = pChild->GetActiveDocument();
}


BOOL CDlgNewBridge::OnInitDialog()
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
}
