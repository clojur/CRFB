// RigidFrame.h : main header file for the RigidFrame application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <YjOSG/YjOSG.h>


// CRigidFrameApp:
// See RigidFrame.cpp for the implementation of this class
//

class CRigidFrameApp : public CBCGPWinApp
{
public:
	CRigidFrameApp();

	virtual void PreLoadState ();
    
	CFont  m_fontRegular;  //ȫ��ϵͳ����
	CMultiDocTemplate* pDocTemplate_Ger;

	bool hasProject;       //�Ƿ��Ѵ򿪹���
	bool projectModified;  //�����Ƿ��ѱ��޸�

	CString tmp_StrProjectname;
	CString tmp_StrProjectpath;
	CString tmp_StrNewbridge;
	double tmp_CenterMileage;

private:
	ViewManager* _viewMgr;


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual BOOL SaveAllModified();

	ViewManager* GetViewManager() const { return _viewMgr; }
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
	afx_msg void OnWindowCloseall();
};

extern CRigidFrameApp theApp;