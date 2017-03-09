#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   X�͸�����


****************************************************************************************/

class CDIP_BAR_API CBarXSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarXSteelBar)
public:
	CBarXSteelBar();
	CBarXSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	~CBarXSteelBar();
	virtual void Serialize(CArchive& ar);

	//������ʾ�ø����㼯(������ǰ�������󸹰�����)
	void CBarXSteelBar::createPtArray();

	//���ݻ���������������ϸ������
	virtual void CalDetailProPara();
	void CalDetailProPara_Mbar();

	//�õ�������� X ֵʱ�ĸ��� Y ֵ(YΪ���붥�����)
	double getYAtX(double x);
	//�õ�������� X ֵʱ�ĸ��� Y ֵ(YΪ���붥�����)
	double getYAtX_M(double x);

	//ê�̳��ȼ��
	void CheckAnchorageLength();

	bool m_IsXmbar;

};


