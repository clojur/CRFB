#pragma once
#include "afxwin.h"


// CDlgNewPrj �Ի���

class CDlgNewPrj : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNewPrj)

public:
	CDlgNewPrj(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewPrj();

// �Ի�������
	enum { IDD = IDD_DLG_NEWPRJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_projectName;
	CString m_projectPath;

	afx_msg void OnBnClickedOk();
	CBCGPEdit m_wndProjectPath;
	virtual BOOL OnInitDialog();
	
};
