

#if _MSC_VER > 1000
#pragma once
#endif 

#include "MainGirder/MainGirder.h"
#include <vector>
using namespace std;

class CCellSplitPane : public CBCGPDialogBar
{
	DECLARE_DYNAMIC(CCellSplitPane)

public:
	CCellSplitPane();

// Dialog Data
	//{{AFX_DATA(CCellSplitPane)
	enum { IDD = IDD_DLG_CELLSPLIT_PLAN };
	
	CBCGPComboBox m_wndXb;
	CBCGPCheckListBox m_wndItems;
	CBCGPButton	m_btnResplit;
	CBCGPButton m_chkUnify;

	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void addXbEnum(vector<CString>&);

//	virtual void AdjustControlsLayout();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellSplitPane)
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	virtual void OnPressCloseButton();

	// Implementation
public:
	virtual ~CCellSplitPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClipboardPane)
	//}}AFX_MSG
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeXblist();
	afx_msg void OnBnClickedSplitUnify();
	afx_msg void OnLbnSelchangeItems();
private:
	int isUnifySplit;                        //�Ƿ�ͳһ���ֽڶ�

	unsigned int m_CurGirder;                //��ǰT��(����)����������
	unsigned int m_CurT;                     //��ǰT������������

	vector<vector<CString> > m_GirderPlanStr;//T������ڶλ��ַ�������
	
public:
	afx_msg void OnBnClickedResplitPlan();
};

