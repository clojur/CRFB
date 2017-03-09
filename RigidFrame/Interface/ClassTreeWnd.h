
#if !defined(AFX_CLASSTREEWND_H__0517A828_D18F_11D3_A727_009027900694__INCLUDED_)
#define AFX_CLASSTREEWND_H__0517A828_D18F_11D3_A727_009027900694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CClassTreeWnd : public CBCGPTreeCtrl
{
public:

protected:

private:
	bool BridgeSelected;

public:
	CClassTreeWnd();
	virtual ~CClassTreeWnd();
	void AddNode(CString name, DWORD_PTR object = NULL);
	void InsertNode(CString name);
	void DelNode();
	HTREEITEM GetCurrentNode() const { return GetSelectedItem(); };
	CString GetNodeName(HTREEITEM item) const { return GetItemText(item); }
	void SetNodeName(HTREEITEM item, CString name) { SetItemText(item, name); Invalidate(); }
	bool IsBridgeSelected();
	HTREEITEM FindItemByNodeName(CString nodeName, HTREEITEM parentItem);

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif
