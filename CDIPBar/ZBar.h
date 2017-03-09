#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   Z�͸�����


****************************************************************************************/

class CDIP_BAR_API CBarZSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarZSteelBar)
public:
	CBarZSteelBar();
	CBarZSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	virtual ~CBarZSteelBar();
	virtual void Serialize(CArchive& ar);

	//�õ�������� X ֵʱ�ĸ�������װ����ľ���
	virtual double getYAtX(double x);

	//ê�̳��ȼ��
	void CheckAnchorageLength();

	//������ʾ�ø����㼯
	void createPtArray();

};


