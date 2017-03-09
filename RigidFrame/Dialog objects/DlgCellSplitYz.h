#pragma once

#include "../RigidFrameDoc.h"

// CDlgCellSplit 对话框

class CDlgCellSplitYz : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgCellSplitYz)

public:
	CDlgCellSplitYz(CRigidFrameDoc*, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCellSplitYz();

// 对话框数据
	enum { IDD = IDD_DLG_CELLSPLIT_YZ };
	CRigidFrameDoc* myDoc;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	double m_Remain;
	double m_InitWeb;
	int    m_25Min;
	int    m_30Min;
	int    m_35Min;
	int    m_40Min;
	int    m_45Min;
	int    m_50Min;
	int    m_55Min;
	int    m_25Max;
	int    m_30Max;
	int    m_35Max;
	int    m_40Max;
	int    m_45Max;
	int    m_50Max;
	int    m_55Max;
	afx_msg void OnBnClickedOk();
};
