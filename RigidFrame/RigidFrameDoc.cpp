// RigidFrameDoc.cpp : implementation of the CRigidFrameDoc class
//

#include "stdafx.h"
#include "RigidFrame.h"
#include "RigidFrameDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MainFrm.h"

// CRigidFrameDoc

IMPLEMENT_DYNCREATE(CRigidFrameDoc, CDocument)

BEGIN_MESSAGE_MAP(CRigidFrameDoc, CDocument)
	
END_MESSAGE_MAP()


// CRigidFrameDoc construction/destruction

CRigidFrameDoc::CRigidFrameDoc()
{
	// TODO: add one-time construction code here
	RigidFrameBridgeExample = nullptr;
	_root = new osg::Group;
	_callback = new DocCallback(this);
}

CRigidFrameDoc::~CRigidFrameDoc()
{
	if (RigidFrameBridgeExample != nullptr)
	{
		delete RigidFrameBridgeExample;
		RigidFrameBridgeExample = nullptr;
	}
}

BOOL CRigidFrameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	RigidFrameBridgeExample = new CRigidFrameBridge(theApp.tmp_StrNewbridge, theApp.tmp_CenterMileage);

	SetTitle(theApp.tmp_StrNewbridge);
	SetModifiedFlag(TRUE);
	return TRUE;
}




// CRigidFrameDoc serialization

void CRigidFrameDoc::Serialize(CArchive& ar)
{
// 	RigidFrameBridgeExample->Serialize(ar);
	if (ar.IsLoading())
	{
		ar >> RigidFrameBridgeExample;
		getUpdateCallback()->Update();
	}
	else
	{
		ar << RigidFrameBridgeExample;

	}

}


// CRigidFrameDoc diagnostics

#ifdef _DEBUG
void CRigidFrameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRigidFrameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRigidFrameDoc commands


