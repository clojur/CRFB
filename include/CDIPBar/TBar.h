#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   T�͸�����


****************************************************************************************/

class CDIP_BAR_API CBarTSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarTSteelBar)
public:
	CBarTSteelBar();
	CBarTSteelBar(CMainGirder* gdrL, CMainGirder* grdR, int segL, int segR);
	~CBarTSteelBar();
	virtual void Serialize(CArchive& ar);

	//ê�̳��ȼ��
	void CheckAnchorageLength();

	//������ʾ�ø����㼯
	void createPtArray();

protected:

};


