// DlgPiles.cpp : 实现文件
//

#include "stdafx.h"
#include "Piles.h"
#include "DlgPiles.h"
#include "afxdialogex.h"

// CDlgPiles 对话框

IMPLEMENT_DYNAMIC(CDlgPiles, CDialogEx)

CDlgPiles::CDlgPiles(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPiles::IDD, pParent)
{

}

CDlgPiles::~CDlgPiles()
{
}

void CDlgPiles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPiles, CDialogEx)
END_MESSAGE_MAP()


// CDlgPiles 消息处理程序
