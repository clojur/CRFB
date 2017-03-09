// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ClassTreeWnd.h"

class CRigidFrameView;
class CRigidFrameDoc;

class CWorkSpaceBar : public CBCGPDockingControlBar
{
public:
	enum PropID
	{
		ChartTitle,
		ChartCategory,
		Chart3D,
		LineStyle,
		AxisFont,
		ChartBackground,
		DiagramBackground,
	};

public:
	CWorkSpaceBar();
	virtual ~CWorkSpaceBar();
	void AdjustLayout();
	void SyncActiveView(CRigidFrameView* pView);
	CClassTreeWnd* GetTreeCtrl();


	// 显示模式切换后属性列表区的更新
	void UpdatePropList(int);



	//bool IsBridgeSelected();
//	CString GetSelBridge();
	void RenameBridge(CString newname);
	//void DelBridge();
	CBCGPPropList	  m_wndPropList;
// Attributes
protected:
	CClassTreeWnd     m_wndTreeCtrl;
	CImageList		  m_TreeImages;
	
	CBCGPSplitterWnd  m_wndSplitter;
	CBCGPTextFormat		m_AxisFont;
	void InitPropList();
	void UpdatePropList_Plan();
	void UpdatePropList_Component();
	void UpdatePropList_Bar();
	void UpdatePropList_Construction();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////
