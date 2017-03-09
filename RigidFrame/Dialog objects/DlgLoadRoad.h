#pragma once

#include <YjOSG/YjOSG.h>
#include <RoadData/RoadData.h>
#include <YjMath/YjMath.h>

// CDlgLoadRoad �Ի���

class CDlgLoadRoad : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgLoadRoad)

public:
	CDlgLoadRoad(CRoadData& road, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLoadRoad();

// �Ի�������
	enum { IDD = IDD_DLG_LOADROAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDesign();
	afx_msg void OnBnClickedBtnGround();
	afx_msg void OnBnClickedBtnCross();
	afx_msg void OnBnClickedOk();

public:
	CString m_DesignFile;
	CString m_GroundFile;
	CString m_CrossFile;

private:
	bool m_Resized;
	POINT2dArray m_DesignLine;
	POINT2dArray m_GroundLine;
	CRoadData& m_Road;
public:
	// ˮλ�߶�
	CString m_WaterLevel;
};
