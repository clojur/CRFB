#pragma once


// CDlgPiles 对话框
#include "Resource.h"
#include "macro.h"
class PILESBAR_API CDlgPiles : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPiles)

public:
	CDlgPiles(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPiles();

// 对话框数据
	enum { IDD = IDD_DLG_PILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
