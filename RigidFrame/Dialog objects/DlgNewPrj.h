#pragma once
#include "afxwin.h"


// CDlgNewPrj 对话框

class CDlgNewPrj : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNewPrj)

public:
	CDlgNewPrj(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewPrj();

// 对话框数据
	enum { IDD = IDD_DLG_NEWPRJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_projectName;
	CString m_projectPath;

	afx_msg void OnBnClickedOk();
	CBCGPEdit m_wndProjectPath;
	virtual BOOL OnInitDialog();
	
};
