// D:\CRFB_Git\CRFB_Git\Dialog objects\DlgAutoLayoutYz.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\RigidFrame.h"
#include "DlgAutoLayoutYz.h"
#include "BCGPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgAutoLayoutYz �Ի���

IMPLEMENT_DYNAMIC(CDlgAutoLayoutYz, CBCGPDialog)

CDlgAutoLayoutYz::CDlgAutoLayoutYz(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgAutoLayoutYz::IDD, pParent)
, m_ZQSpanRatioMin(_T("0.52"))
, m_ZQSpanRatioMax(_T("0.58"))
, m_ZQMainSpanMin(_T("80"))
, m_ZQMainSpanMax(_T("200"))
, m_YQPierHeightMin(_T("4.0"))
, m_YQPierHeightMax(_T("40.0"))
, m_YQStandardSpanMin(_T("20"))
, m_YQStandardSpanMax(_T("40"))
, m_ProgramDesign(false)
, m_InitialDesign(false)
, m_DeepDesign(false)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CDlgAutoLayoutYz::~CDlgAutoLayoutYz()
{
}

void CDlgAutoLayoutYz::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ZQSpanRatioMin);
	DDX_Text(pDX, IDC_EDIT2, m_ZQSpanRatioMax);
	DDX_Text(pDX, IDC_EDIT3, m_ZQMainSpanMin);
	DDX_Text(pDX, IDC_EDIT4, m_ZQMainSpanMax);
	DDX_Text(pDX, IDC_EDIT5, m_YQPierHeightMin);
	DDX_Text(pDX, IDC_EDIT6, m_YQPierHeightMax);
	DDX_Text(pDX, IDC_EDIT7, m_YQStandardSpanMin);
	DDX_Text(pDX, IDC_EDIT8, m_YQStandardSpanMax);
}


BEGIN_MESSAGE_MAP(CDlgAutoLayoutYz, CBCGPDialog)
	ON_BN_CLICKED(IDC_PROGRAM_DESIGN, &CDlgAutoLayoutYz::OnBnClickedProgramDesign)
	ON_BN_CLICKED(IDC_INITIAL_DESIGN, &CDlgAutoLayoutYz::OnBnClickedInitialDesign)
	ON_BN_CLICKED(IDC_DEEP_DESIGN, &CDlgAutoLayoutYz::OnBnClickedDeepDesign)
	ON_BN_CLICKED(IDOK, &CDlgAutoLayoutYz::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAutoLayoutYz ��Ϣ�������


void CDlgAutoLayoutYz::OnBnClickedProgramDesign()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDlgAutoLayoutYz::OnBnClickedInitialDesign()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDlgAutoLayoutYz::OnBnClickedDeepDesign()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDlgAutoLayoutYz::OnBnClickedOk()
{
	CBCGPDialog::OnOK();
	UpdateData(true);
}


BOOL CDlgAutoLayoutYz::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	//m_ZQSpanRatioMax = _T("0.60");
	UpdateData(false);

	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}