// RigidFrameDoc.h : interface of the CRigidFrameDoc class
//


#pragma once
#include <RigidFrameBridge/RigidFrameBridge.h>
#include <YjOSG/YjOSG.h>

class DocCallback;

class CRigidFrameDoc : public CDocument
{
protected: // create from serialization only
	CRigidFrameDoc();
	DECLARE_DYNCREATE(CRigidFrameDoc)

// Attributes
public:
	CRigidFrameBridge* RigidFrameBridgeExample;  //∏’ππ«≈∂‘œÛ

	// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	DocCallback* getUpdateCallback() { return _callback; }
	osg::Group* getRoot() const { return _root; }

// Implementation
public:
	virtual ~CRigidFrameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	osg::ref_ptr<osg::Group> _root;
	DocCallback* _callback;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};



class DocCallback : public ComponentCallback
{
public:
	DocCallback(CRigidFrameDoc* doc)
		: ComponentCallback(doc->getRoot())
	{
		_doc = doc;
		_changed = false;
	}

	void DoUpdate()
	{
		_root->addChild(_doc->RigidFrameBridgeExample->getUpdateCallback()->getRoot());
		_doc->RigidFrameBridgeExample->getUpdateCallback()->setView(getView());
	}

private:
	CRigidFrameDoc* _doc;
};