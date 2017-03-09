// Project.cpp : 实现文件
//

#include "stdafx.h"
#include "RigidFrame.h"
#include "Project.h"
#include <string>
#include <stdio.h>
#include "Interface/ClassTreeWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;
using namespace tinyxml2;
// CProject

void Convert(const wchar_t *src, char* dst)
{
	DWORD dwSize = WideCharToMultiByte(CP_OEMCP, NULL, src, -1, NULL, 0, NULL, FALSE);
	WideCharToMultiByte(CP_OEMCP, NULL, src, -1, dst, dwSize, NULL, FALSE);
}

CProject::CProject()
{
	m_NextIndex = 1;
	Doc = nullptr;
	m_Tree = nullptr;
}

CProject::CProject(CString projectName, CString directory)
{
	m_NextIndex = 1;
	Doc = nullptr;
	m_Tree = nullptr;
	Convert((LPCTSTR)projectName, PrjName);
	Convert((LPCTSTR)directory, PrjDirectory);
	CString path = directory + projectName;
	PrjPath = path;
	path = path + _T("\\") + projectName + _T(".rfms");
	Convert((LPCTSTR)path, PrjFilePathName);
}

CProject::~CProject()
{
	if (Doc != NULL)
		delete Doc;
}

void CProject::SetTreeCtrl(CClassTreeWnd* tree)
{
	m_Tree = tree;
}

bool CProject::Create()
{
	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
	XMLElement *root = doc->NewElement("project");
	root->SetAttribute("name", PrjName);
	root->SetAttribute("path", PrjDirectory);
	doc->InsertEndChild(root); //XMLDocument对象文件末尾添加新的根节点
	Doc = doc;

	m_Tree->InsertItem(CString(PrjName), TVI_ROOT);	//向项目树中添加根节点
	m_Tree->Invalidate(); //更新显示项目树

	CreateDirectory(PrjPath, NULL); //创建文件夹目录
	//Save();//保存doc到指定目录文件
	theApp.projectModified = true;
	return true;
}

bool CProject::Save()
{
	Doc->SaveFile(PrjFilePathName);
	return true;
}

void CProject::Open(CString filePathName)
{
	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
	char path[256];
	Convert((LPCTSTR)filePathName, path);
	doc->LoadFile(path);
	Doc = doc;

	XMLElement* root = doc->RootElement();
	strncpy(PrjName, root->Attribute("name"), sizeof(PrjName) - 1);
	PrjName[sizeof(PrjName) - 1] = '\0';
	strncpy(PrjDirectory, root->Attribute("path"), sizeof(PrjDirectory) - 1);
	PrjDirectory[sizeof(PrjDirectory) - 1] = '\0';

	PrjPath = CString(PrjDirectory)+ CString(PrjName);
	Convert((LPCTSTR)filePathName, PrjFilePathName);

	m_Tree->DeleteAllItems(); //清空项目树
	HTREEITEM hRootItem = m_Tree->InsertItem(CString(PrjName), TVI_ROOT);	//向项目树中添加根节点

	XMLElement* elem = root->FirstChildElement();
	while (elem)
	{
		CString bridgeName = CString(elem->Attribute("name"));
		m_Tree->InsertItem(CString(bridgeName), hRootItem);	//向项目树中添加一级子节点（刚构桥对象）
		elem = elem->NextSiblingElement();
	}
	m_Tree->Invalidate(); //更新显示项目树
}





bool CProject::AddBridge(CString bridgeName)
{
	HTREEITEM isItemSelected = m_Tree->GetSelectedItem();
	if (isItemSelected = NULL)
		return false;

	XMLElement *element = Doc->NewElement("bridge");
	char name[256];
	char path[256];
	Convert((LPCTSTR)bridgeName, name);
	element->SetAttribute("name", name);
	CString BrgFilePathName = PrjPath + _T("\\") + bridgeName + _T(".rfm");
	Convert((LPCTSTR)BrgFilePathName, path);
	element->SetAttribute("path", path);

	XMLElement *root = Doc->RootElement();
	root->InsertEndChild(element);
	//Doc->SaveFile(PrjFilePathName);
	theApp.projectModified = true;
	m_NextIndex++;

	HTREEITEM hRootItem = m_Tree->GetRootItem();
	m_Tree->InsertItem(bridgeName, 4, 3, hRootItem);	//向项目树中添加一级子节点（刚构桥对象），选中时显示图标4，未选中时显示图标3

	return true;
}

bool CProject::DelBridge(CString bridgeName)
{
	HTREEITEM isItemSelected = m_Tree->GetSelectedItem();
	if (isItemSelected = NULL)
		return false;

	char name[256] = { 0 };
	Convert((LPCTSTR)bridgeName, name);
	XMLElement* root = Doc->RootElement();
	XMLElement* elem = root->FirstChildElement();
	while (elem)
	{
		if (!strcmp(elem->Attribute("name"), name))
		{
			root->DeleteChild(elem);
			break;
		}
		elem = elem->NextSiblingElement();
	}
	//Doc->SaveFile(PrjFilePathName);
	theApp.projectModified = true;
	m_Tree->DeleteItem(m_Tree->GetSelectedItem());
	m_Tree->Invalidate(); //更新显示项目树
	return true;
}

bool CProject::RenameBridge(CString oldname, CString newname)
{
	char o[256] = { 0 };
	char n[256] = { 0 };
	char path[256] = { 0 };
	Convert(oldname, o);
	Convert(newname, n);

	XMLElement* root = Doc->RootElement();
	XMLElement* elem = root->FirstChildElement();
	while (elem)
	{
		if (!strcmp(elem->Attribute("name"), o))
		{
			elem->SetAttribute("name", n);
			CString brgFilePathName = PrjPath + _T("\\") + newname + _T(".rfm");
			Convert(brgFilePathName, path);
			elem->SetAttribute("path", path);
			/*更新项目树*/
			HTREEITEM item = m_Tree->FindItemByNodeName(oldname, m_Tree->GetRootItem());
			m_Tree->SetNodeName(item, newname);
			break;
		}
		elem = elem->NextSiblingElement();
	}
	//Doc->SaveFile(PrjFilePathName);
	theApp.projectModified = true;
	return true;
}



bool CProject::CopyBridgeAs(CString origName, CString newName)
{
	HTREEITEM isItemSelected = m_Tree->GetSelectedItem();
	if (isItemSelected = NULL)
		return false;

	char o[256] = { 0 };
	char n[256] = { 0 };
	char path[256] = { 0 };
	Convert(origName, o);
	Convert(newName, n);

	XMLElement* root = Doc->RootElement();
	XMLElement* elem = root->FirstChildElement();
	while (elem)
	{
		if (!strcmp(elem->Attribute("name"), o))
		{
			CString brgFilePathName = PrjPath + _T("\\") + newName + _T(".rfm");
			Convert(brgFilePathName, path);
			XMLElement *newelem = Doc->NewElement("bridge");
			newelem->SetAttribute("name", n);
			newelem->SetAttribute("path", path);
			root->InsertAfterChild(elem, newelem);
			m_Tree->InsertItem(newName, 1, 1, m_Tree->GetRootItem(), isItemSelected); //在选中的一级子节点（刚构桥对象）下面复制一个新的节点
			m_Tree->Invalidate(); //更新显示项目树
			break;
		}
		elem = elem->NextSiblingElement();
	}
	return true;
}





CString CProject::GetProjectName()
{
	CString str(PrjName);
	return str;
}

CString CProject::GetProjectPath()
{
	return PrjPath;
}

