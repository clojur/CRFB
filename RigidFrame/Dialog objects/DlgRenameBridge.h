#pragma once


// CDlgRenameBridge �Ի���

class CDlgRenameBridge : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgRenameBridge)

public:
	CDlgRenameBridge(CString oldname, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRenameBridge();

// �Ի�������
	enum { IDD = IDD_DLG_RENAMEBRG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strName;
};
