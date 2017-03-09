#pragma once


// CDlgRenameBridge 对话框

class CDlgRenameBridge : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgRenameBridge)

public:
	CDlgRenameBridge(CString oldname, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRenameBridge();

// 对话框数据
	enum { IDD = IDD_DLG_RENAMEBRG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strName;
};
