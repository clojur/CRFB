#pragma once


// CDlgPiles �Ի���
#include "Resource.h"
#include "macro.h"
class PILESBAR_API CDlgPiles : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPiles)

public:
	CDlgPiles(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPiles();

// �Ի�������
	enum { IDD = IDD_DLG_PILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
