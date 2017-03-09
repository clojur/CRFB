#pragma once


// CDlgNewBridge 对话框

class CDlgNewBridge : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNewBridge)

public:
	CDlgNewBridge(int index, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewBridge();

// 对话框数据
	enum { IDD = IDD_DLG_NEWBRIDGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_Newbridge;
	virtual BOOL OnInitDialog();
	double m_CenterPos;
};
