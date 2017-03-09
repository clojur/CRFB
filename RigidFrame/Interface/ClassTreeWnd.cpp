
#include "stdafx.h"
#include "../RigidFrame.h"
#include "ClassTreeWnd.h"
#include "../RigidFrameDoc.h"
#include "../MainFrm.h"
#include "../RigidFrameView.h"
#include "BCGPTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd

CClassTreeWnd::CClassTreeWnd()
{
	BridgeSelected = false;
}

CClassTreeWnd::~CClassTreeWnd()
{
}

void CClassTreeWnd::AddNode(CString name, DWORD_PTR /* object*/)
{
	HTREEITEM root = GetRootItem();
	HTREEITEM hTreeSubitem = InsertItem(name, 1, 1, root);
	//SetItemData(hTreeSubitem, object);

	Invalidate();
}

void CClassTreeWnd::InsertNode(CString name)
{
	HTREEITEM root = GetRootItem();
	HTREEITEM cur = GetSelectedItem();
	HTREEITEM hTreeSubitem = InsertItem(name, 1, 1, root, cur);
	Invalidate();
}

void CClassTreeWnd::DelNode()
{
	HTREEITEM hItem = GetSelectedItem();
	DeleteItem(hItem);
	Invalidate();
}

BEGIN_MESSAGE_MAP(CClassTreeWnd, CBCGPTreeCtrl)
	//{{AFX_MSG_MAP(CClassTreeWnd)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CClassTreeWnd::OnNMDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CClassTreeWnd::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd message handlers

BOOL CClassTreeWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	BOOL bRes = CBCGPTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}



void CClassTreeWnd::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (IsBridgeSelected() == false)
		return;

	HTREEITEM hItem = GetSelectedItem();
	CString str = ((CMainFrame*)AfxGetMainWnd())->Proj->GetProjectPath();
	str += _T("\\");
	str += GetItemText(hItem);
	str += _T(".rfm");
	// 判断是否有同名子窗口

	CString FileName = GetItemText(hItem) + _T(".rfm");

	// 打开项目情况视图
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		CDocTemplate *p = theApp.GetNextDocTemplate(pos);
		POSITION posdoc = p->GetFirstDocPosition();
		while (posdoc != NULL)
		{
			CDocument* pDoc = p->GetNextDoc(posdoc);
			if (pDoc != NULL)
			{
				if (pDoc->GetTitle() == FileName) //判断如果想要打开的文件已经打开对应窗口，则激活
				{
					POSITION posview = pDoc->GetFirstViewPosition();
					if (posview != NULL)
					{
						CView* pV = pDoc->GetNextView(posview);
						pV->GetParentFrame()->ActivateFrame(SW_SHOW);
					}
					return;
				}
			}
		}
	}
	CRigidFrameDoc * newDoc = (CRigidFrameDoc*)theApp.pDocTemplate_Ger->OpenDocumentFile(str);
	POSITION posview = newDoc->GetFirstViewPosition();
	if (posview != NULL)
	{
		CView* pV = newDoc->GetNextView(posview);
	}
	*pResult = 0;
}

void CClassTreeWnd::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	if (GetRootItem() == GetSelectedItem())
	{
		BridgeSelected = false;
	}
	else
	{
		BridgeSelected = true;
	}
	*pResult = 0;
}

bool CClassTreeWnd::IsBridgeSelected()
{
	return BridgeSelected;
}


HTREEITEM CClassTreeWnd::FindItemByNodeName(CString nodeName, HTREEITEM parentItem)
{
	HTREEITEM hFirstRootItem = GetRootItem();
	HTREEITEM hNextRootItem = hFirstRootItem;
	while (hNextRootItem)
	{
		if (hNextRootItem == parentItem)
		{
			HTREEITEM hFirstChildItem = GetChildItem(GetRootItem());
			HTREEITEM hNextChildItem = hFirstChildItem;
			while (hNextChildItem)
			{
				if (GetItemText(hNextChildItem) == nodeName)
				{
					return hNextChildItem;
				}
				hNextChildItem = GetNextSiblingItem(hNextChildItem);
			}
		}
		hNextRootItem = GetNextSiblingItem(hNextRootItem);
	}
	return NULL;
}
