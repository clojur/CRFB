#pragma once


// CDlgDelBridge 对话框

class CDlgDelBridge : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDelBridge)

public:
	CDlgDelBridge(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDelBridge();

// 对话框数据
	enum { IDD = IDD_DLG_DELBRG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strOp;	//选择的操作，删除或移除
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedDel();
	virtual BOOL OnInitDialog();
};
