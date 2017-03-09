#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   B�͸�����


****************************************************************************************/

class CDIP_BAR_API CBarBSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarBSteelBar)
public:
	CBarBSteelBar();
	CBarBSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	virtual ~CBarBSteelBar();
	virtual void Serialize(CArchive& ar);

	//�õ�������� X ֵʱ�ĸ�������߿�װ�ľ���
	virtual double getYAtX(double x);

	//ê�̳��ȼ��
	void CheckAnchorageLength();

	//������ʾ�ø����㼯
	void createPtArray();


};


