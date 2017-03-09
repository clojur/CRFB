// DlgLoadRoad.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "DlgLoadRoad.h"
#include "BCGPDialog.h"
#include "../RigidFrameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgLoadRoad �Ի���

IMPLEMENT_DYNAMIC(CDlgLoadRoad, CBCGPDialog)

CDlgLoadRoad::CDlgLoadRoad(CRoadData& road, CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgLoadRoad::IDD, pParent)
, m_DesignFile(_T("C:\\XXRoad\\γ������\\YyLc_CS_2016.04.20_A1.ZDM"))
, m_GroundFile(_T("C:\\XXRoad\\γ������\\YyLc_CS_2016.04.20_A1.zmx"))
, m_Resized(false)
, m_Road(road)
, m_CrossFile(_T("C:\\XXRoad\\γ������\\YyLc_CS_2016.04.20_A1.HDM"))
, m_WaterLevel(_T("660"))
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CDlgLoadRoad::~CDlgLoadRoad()
{
}

void CDlgLoadRoad::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_DESIGN, m_DesignFile);
	DDX_Text(pDX, IDC_EDT_GROUND, m_GroundFile);
	DDX_Text(pDX, IDC_EDT_CROSS, m_CrossFile);
	DDX_Text(pDX, IDC_EDT_WATER, m_WaterLevel);
}


BEGIN_MESSAGE_MAP(CDlgLoadRoad, CBCGPDialog)
	ON_BN_CLICKED(IDC_BTN_DESIGN, &CDlgLoadRoad::OnBnClickedBtnDesign)
	ON_BN_CLICKED(IDC_BTN_GROUND, &CDlgLoadRoad::OnBnClickedBtnGround)
	ON_BN_CLICKED(IDOK, &CDlgLoadRoad::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CROSS, &CDlgLoadRoad::OnBnClickedBtnCross)
END_MESSAGE_MAP()


// CDlgLoadRoad ��Ϣ�������


void CDlgLoadRoad::OnBnClickedBtnDesign()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("γ��������ļ�(*.ZDM)|*.ZDM| �����ļ�(*.*) |*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		m_DesignFile = dlg.GetPathName();
		UpdateData(false);
	}
}


void CDlgLoadRoad::OnBnClickedBtnGround()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("γ�ص������ļ�(*.zmx)|*.zmx|�����ļ�(*.*) |*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		m_GroundFile = dlg.GetPathName();
		UpdateData(false);
	}
}


void CDlgLoadRoad::OnBnClickedBtnCross()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("γ�ص������������ļ�(*.HDM)|*.HDM|�����ļ�(*.*) |*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		m_CrossFile = dlg.GetPathName();
		UpdateData(false);
	}
}


void CDlgLoadRoad::OnBnClickedOk()
{
	//DestroyWindow();
	m_Road.LoadDesignLine(m_DesignFile);

	//��ȡ����ߵ㼯
	for (int i = 0; i < m_Road.m_DesignPoints.GetCount(); i++)
		m_DesignLine.push_back(CPoint2d(m_Road.m_DesignPoints[i].GetStake(), m_Road.m_DesignPoints[i].GetElevation()));

	m_Road.LoadGroundLine(m_GroundFile);

	//��ȡ������
	for (int i = 0; i < m_Road.m_GroundPoints.GetCount(); i++)
		m_GroundLine.push_back(CPoint2d(m_Road.m_GroundPoints[i].GetStake(), m_Road.m_GroundPoints[i].GetElevation()));
	
	m_Road.LoadCrossSection(m_CrossFile);
	
	//��ȡˮλ
	UpdateData();
	if (m_WaterLevel == "" || m_WaterLevel == "-1.0")
		m_Road.m_WaterLevel=0.0;
	else
	{
		m_Road.m_WaterLevel=_wtof(m_WaterLevel.GetBuffer());
	}
	CBCGPDialog::OnOK();
}


BOOL CDlgLoadRoad::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	return TRUE;
}
