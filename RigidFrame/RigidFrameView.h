// RigidFrameView.h : interface of the CRigidFrameView class
//


#pragma once

#include <YjOSG/YjOSG.h>
#include "RigidFrameDoc.h"
class CRigidFrameView : public CView
{
private:
	CString movedis = _T("");

protected: // create from serialization only
	CRigidFrameView();
	DECLARE_DYNCREATE(CRigidFrameView)

// Attributes
public:
	CRigidFrameDoc* GetDocument() const;

	bool isShowWeight;
	bool isShowLength;

	// 设置当前显示模式
	void setCurShowModel(int index);

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRigidFrameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeVisualManager(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDesignLlayout();
	afx_msg void OnDesignRlayout();
	afx_msg void OnDesignClayout();
	afx_msg void OnDesignBaseStake();
	afx_msg void OnDesignLmove();
	afx_msg void OnDesignRmove();
	afx_msg void OnDesignMovedis();
	afx_msg void OnDesignLayoutMaction();
	afx_msg void OnDesignDownyz();
	afx_msg void OnDesignDownauto();
	afx_msg void OnDesignTransitcap();
	afx_msg void OnDesignTransitpill();
	afx_msg void OnDesignAprAbt();
	afx_msg void OnDesignAprDown();
	afx_msg void OnDesignAprUp();
	afx_msg void OnOutputDoctorbridge();
	afx_msg void OnOutputMidas();
	afx_msg void OnDesignBarHx();
	afx_msg void OnDesignBaradjust();
	afx_msg void OnDesignBarauto();
	afx_msg void OnDesignBarcheck();
	afx_msg void OnDesignBarout();
	afx_msg void OnDesignBaryz();
	afx_msg void OnDesignCap();
	afx_msg void OnDesignCell0();
	afx_msg void OnDesignCellBk();
	afx_msg void OnDesignCellGer();
	afx_msg void OnDesignCellHgb();
	afx_msg void OnDesignCellyz();
	afx_msg void OnDesignChiban();
	afx_msg void OnDesignDrain();
	afx_msg void OnDesignFramework();
	afx_msg void OnDesignJoint();
	afx_msg void OnDesignLayoutAction();
	afx_msg void OnDesignLayoutYz();
	afx_msg void OnDesignMainpier();
	afx_msg void OnDesignPave();
	afx_msg void OnDesignPill();
	afx_msg void OnDesignTransit();
	afx_msg void OnOutDetail();
	afx_msg void OnOutputDrawing();
	afx_msg void OnOutputManager();
	afx_msg void OnDesignShowcelllen();
	afx_msg void OnDesignShowweight();
	afx_msg void OnUpdateDesignShowweight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDesignShowcelllen(CCmdUI* pCmdUI);
	afx_msg void OnPrjRoad();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPrjSect();
	afx_msg void OnPrjGeology();
	afx_msg void OnPrjHydrology();
	afx_msg void OnShowModel();

	private:
		bool UpdateModel(CRigidFrameDoc* pDoc);//更新模型
public:
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // debug version in RigidFrameView.cpp
inline CRigidFrameDoc* CRigidFrameView::GetDocument() const
   { return reinterpret_cast<CRigidFrameDoc*>(m_pDocument); }
#endif

