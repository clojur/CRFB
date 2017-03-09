#pragma once
#include "Interface\tinyxml2.h"
#include "Interface/ClassTreeWnd.h"
#include <vector>
#include <string>
using namespace std;

class CProject : public CObject
{
public:
	CProject();
	CProject(CString projectName, CString directory);
	virtual ~CProject();
	
	void SetTreeCtrl(CClassTreeWnd* tree);

	bool Create();
	bool Save();
	void Open(CString FilePathName);

	bool AddBridge(CString bridgeName);
	bool DelBridge(CString bridgeName);
	bool RenameBridge(CString oldname, CString newname);
	bool CopyBridgeAs(CString origName, CString newName);

	CString GetProjectName();
	CString GetProjectPath();
	unsigned int GetNextIndex() const { return m_NextIndex; }

private:
	CImageList m_ImageList;
	CClassTreeWnd* m_Tree;
	tinyxml2::XMLDocument *Doc;
	unsigned int m_NextIndex;   //新建刚构桥方案时的起始编号，初始化时从1开始

	char PrjName[256];				//项目名称
	char PrjDirectory[256];		    //项目所在路径
	CString PrjPath;		        //包含项目路径的项目全名，不含.rfms后缀
	char PrjFilePathName[256];	    //包含项目路径的项目全名，含.rfms后缀
};


