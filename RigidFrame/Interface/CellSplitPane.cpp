
#include "stdafx.h"
#include "../RigidFrame.h"
#include "CellSplitPane.h"
#include "../RigidFrameDoc.h"
#include "../MainFrm.h"
#include "BCGPDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CRigidFrameBridge* Get_Brg();

IMPLEMENT_DYNAMIC(CCellSplitPane, CBCGPDialogBar)

CCellSplitPane::CCellSplitPane()
{
	EnableVisualManagerStyle();
	EnableLayout();

	m_CurT = m_CurGirder = 0;
	isUnifySplit = 1;

	//{{AFX_DATA_INIT(CCellSplitPane)
	//}}AFX_DATA_INIT
}

CCellSplitPane::~CCellSplitPane()
{
}

void CCellSplitPane::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XBLIST, m_wndXb);
	DDX_Control(pDX, IDC_ITEMS, m_wndItems);
	DDX_Control(pDX, IDC_SPLIT_UNIFY, m_chkUnify);
	DDX_Control(pDX, IDC_RESPLIT_PLAN, m_btnResplit); 
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCellSplitPane, CBCGPDialogBar)
	//{{AFX_MSG_MAP(CCellSplitPane)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	ON_CBN_SELCHANGE(IDC_XBLIST, &CCellSplitPane::OnCbnSelchangeXblist)
	ON_BN_CLICKED(IDC_SPLIT_UNIFY, &CCellSplitPane::OnBnClickedSplitUnify)
	ON_LBN_SELCHANGE(IDC_ITEMS, &CCellSplitPane::OnLbnSelchangeItems)
	ON_BN_CLICKED(IDC_RESPLIT_PLAN, &CCellSplitPane::OnBnClickedResplitPlan)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClipboardPane message handlers



LRESULT CCellSplitPane::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CBCGPDialogBar::HandleInitDialog(wParam, lParam);

	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout ();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(IDC_RESPLIT_PLAN, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
		pLayout->AddAnchor(IDC_XBLIST, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
		pLayout->AddAnchor (IDC_ITEMS, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
		pLayout->AddAnchor(IDC_SPLIT_UNIFY, CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeNone);
	}
	
	m_chkUnify.SetCheck(isUnifySplit);

	return lRes;
}


void CCellSplitPane::addXbEnum(vector<CString>& girderStr)
{
	vector<CString>::iterator iter;

	m_wndXb.ResetContent();

	for (iter = girderStr.begin(); iter != girderStr.end(); ++iter)
	{
		m_wndXb.AddString(*iter);
	}
	m_wndXb.SetCurSel(0);
}



// �л�T������
void CCellSplitPane::OnCbnSelchangeXblist()
{
	if (m_GirderPlanStr.size() == 0) return;

	CString selectedStr;
	m_wndXb.GetWindowTextW(selectedStr);

	CRigidFrameBridge* brg = Get_Brg();

	vector<CMainGirder*>::iterator iter;
	unsigned int Tcount = m_wndXb.GetCurSel();
	m_CurT = m_wndXb.GetCurSel();
	m_CurGirder = 0;

	for (iter = brg->m_MainGirders.begin(); iter != brg->m_MainGirders.end(); ++iter)
	{
		if ((*iter)->m_Id == selectedStr)
		{
			m_wndItems.CleanUp();
			vector<CString>::iterator iter1;
			int i = 0;
			for (iter1 = m_GirderPlanStr[Tcount].begin(); iter1 != m_GirderPlanStr[Tcount].end(); ++iter1)
			{
				m_wndItems.AddString(*iter1);
				if ((*iter1) == (*iter)->m_ResSplit)
				{
					m_wndItems.SetCheck(i, 1);
					m_wndItems.SetCurSel(i);
				}
				i++;
			}

			//���� DockPane ������ʾ�����͵����۳���
			CString str;
			str.Format(_T("������=%.1lf��,����=%d��"),((*iter)->m_AllLen - (*iter)->m_BottomLen - 2 * (*iter)->m_Remain) / 2, m_GirderPlanStr[Tcount].size());
			SetWindowText(str);
			break;
		}
		m_CurGirder++;
	}
}


//�����ͳһ���֡���ѡ��
void CCellSplitPane::OnBnClickedSplitUnify()
{
	isUnifySplit = m_chkUnify.GetCheck();
}



//�л���ǰT������Ļ��ַ���)
void CCellSplitPane::OnLbnSelchangeItems()
{
	// 1����ѡ�з����򹳣���������ȡ����;
	CString resStr;
	int ct = m_wndItems.GetCurSel();
	m_wndItems.GetText(ct, resStr);

	for (int i = 0; i < m_wndItems.GetCount(); i++)
	{
		m_wndItems.SetCheck(i, 0);
	}
	m_wndItems.SetCheck(ct, 1);
	
	
	//2���ı��T�������ջ��ַ���m_ResSplit,����ѡ�����ַ��������ڶμ���
	CRigidFrameBridge* brg = Get_Brg();

	if (isUnifySplit != 0)
	{
		unsigned int ct = brg->m_MainGirders.size();
		for (unsigned int i = 0; i < ct; i++)
		{
			if (brg->m_MainGirders[i]->m_Type == CMainGirder::T_CONSTRUCTION) //�˴�������T���Ļ���,���浥�ڶ������ڡ��������л��ֽڶΡ�ʱ�Ѿ����ֹ�
				brg->m_MainGirders[i]->CreateSegmentsByStr(resStr);
		}
	}
	else
	{
		brg->m_MainGirders[m_CurGirder]->CreateSegmentsByStr(resStr);
	}
	

	//�����Ϣ���

	//vector<CMainGirder*>::iterator iter;
	//for (iter = brg->m_MainGirders.begin(); iter != brg->m_MainGirders.end(); ++iter)
	//{
	//	TRACE(_T("\n��������:%s,�����ڶ���:%d��"), (*iter)->m_Id, (*iter)->segments.size());
	//	vector<CSegment*>::iterator iter1;
	//	unsigned int tt = 0;
	//	for (iter1 = (*iter)->segments.begin(); iter1 != (*iter)->segments.end(); ++iter1,tt++)
	//	{
	//		TRACE(_T("\n��%d���ڶ�,ID=%s,����=%.1lf,��װ��=%.3lf,�ҵװ��=%.3lf,��������=%.3lf,������Ҹ�=%.3lf"), 
	//			tt + 1, (*iter1)->m_Id, (*iter1)->m_Length, (*iter1)->m_LplateThick, (*iter1)->m_RplateThick, 
	//			(*iter1)->m_LlHeight, (*iter1)->m_LrHeight);
	//	}
	//}


}



//���»�������T���Ľڶη���
void CCellSplitPane::OnBnClickedResplitPlan()
{
	CRigidFrameBridge* brg = Get_Brg();
	if (brg->m_MainGirders.size() == 0)
	{
		AfxMessageBox(L"���Ż�δ���п׿粼��,���Ȳ��ú��ٻ���!", MB_ICONINFORMATION);
		return;
	}

	int answer = MessageBox(_T("���ܻ��ֽڶλḲ��֮ǰ�Ļ��ֽ��,��ȷ����?"), _T("��ʾ"), MB_YESNO | MB_ICONEXCLAMATION);
	if (answer == IDNO)	return;


	//�Ȼ�������T���ķ���
	vector<CMainGirder*>::iterator iter;
	unsigned int Tcount = (brg->m_MainGirders.size()-3)/2;
	m_GirderPlanStr.clear();
	m_GirderPlanStr.resize(Tcount);

	unsigned int i = 0;
	for (iter = brg->m_MainGirders.begin(); iter != brg->m_MainGirders.end(); ++iter)
	{
		if ((*iter)->m_Type == CMainGirder::T_CONSTRUCTION)
		{
			m_GirderPlanStr[i] = (*iter)->AutoCellsplit();
			(*iter)->m_ResSplit = m_GirderPlanStr[i].at(0);
			(*iter)->CreateSegmentsByStr((*iter)->m_ResSplit);
			i++;
		}
		else
		{
			(*iter)->CreateSegmentsByStr(_T("Standard"));
		}
		(*iter)->getUpdateCallback()->Lock();
		(*iter)->getUpdateCallback()->Update();
		(*iter)->getUpdateCallback()->Unlock();
	}

	//����ʾ��ǰT���ķ����б�
	m_wndItems.CleanUp();
	vector<CString>::iterator iter1;
	for (iter1 = m_GirderPlanStr[m_CurT].begin(); iter1 != m_GirderPlanStr[m_CurT].end(); ++iter1)
	{
		m_wndItems.AddString(*iter1);
	}
	m_wndItems.SetCheck(0, 1);
	m_wndItems.SetCurSel(0);

	//���� DockPane ������ʾ�����͵����۳���
	OnCbnSelchangeXblist();
}


void CCellSplitPane::OnPressCloseButton()
{
	//AfxMessageBox(L"closed!");
	CBCGPDialogBar::OnPressCloseButton();
}
