#pragma once


// CDlgNewBridge �Ի���

class CDlgNewBridge : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNewBridge)

public:
	CDlgNewBridge(int index, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewBridge();

// �Ի�������
	enum { IDD = IDD_DLG_NEWBRIDGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_Newbridge;
	virtual BOOL OnInitDialog();
	double m_CenterPos;
};
