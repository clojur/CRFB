#pragma once
#include "afxwin.h"


// CDlgAutoLayoutYz �Ի���

class CDlgAutoLayoutYz : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgAutoLayoutYz)

public:
	CDlgAutoLayoutYz(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAutoLayoutYz();

// �Ի�������
	enum { IDD = IDD_DLG_LAYOUTYZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_ZQSpanRatioMin;
	CString m_ZQSpanRatioMax;
	CString m_ZQMainSpanMin;
	CString m_ZQMainSpanMax;
	CString m_YQPierHeightMin;
	CString m_YQPierHeightMax;
	CString m_YQStandardSpanMin;
	CString m_YQStandardSpanMax;
	bool m_ProgramDesign;
	bool m_InitialDesign;
	bool m_DeepDesign;

	afx_msg void OnBnClickedProgramDesign();
	afx_msg void OnBnClickedInitialDesign();
	afx_msg void OnBnClickedDeepDesign();

	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedCancel();

	virtual BOOL OnInitDialog();

};
