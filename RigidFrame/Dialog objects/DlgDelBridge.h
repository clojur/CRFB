#pragma once


// CDlgDelBridge �Ի���

class CDlgDelBridge : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDelBridge)

public:
	CDlgDelBridge(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDelBridge();

// �Ի�������
	enum { IDD = IDD_DLG_DELBRG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_strOp;	//ѡ��Ĳ�����ɾ�����Ƴ�
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedDel();
	virtual BOOL OnInitDialog();
};
