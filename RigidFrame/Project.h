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
	unsigned int m_NextIndex;   //�½��չ��ŷ���ʱ����ʼ��ţ���ʼ��ʱ��1��ʼ

	char PrjName[256];				//��Ŀ����
	char PrjDirectory[256];		    //��Ŀ����·��
	CString PrjPath;		        //������Ŀ·������Ŀȫ��������.rfms��׺
	char PrjFilePathName[256];	    //������Ŀ·������Ŀȫ������.rfms��׺
};


