#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   H�͸�����


****************************************************************************************/

class CDIP_BAR_API CBarHSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarHSteelBar)
public:
	CBarHSteelBar();
	CBarHSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	~CBarHSteelBar();
	virtual void Serialize(CArchive& ar);

	//ê�̳��ȼ��
	void CheckAnchorageLength();

	//������ʾ�ø����㼯
	void createPtArray();

};


