#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   D�͸�����


****************************************************************************************/

class CDIP_BAR_API CBarDSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarDSteelBar)
public:
	CBarDSteelBar();
	CBarDSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	~CBarDSteelBar();
	virtual void Serialize(CArchive& ar);

	//ê�̳��ȼ��
	void CheckAnchorageLength();

	//������ʾ�ø����㼯
	void createPtArray();

};


