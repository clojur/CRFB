
#include "stdafx.h"
#include "TBar.h"

IMPLEMENT_SERIAL(CBarTSteelBar, CBarSteelBar, 0);
CBarTSteelBar::CBarTSteelBar()
{
}

CBarTSteelBar::CBarTSteelBar(CMainGirder* gdrL, CMainGirder* gdrR,int segL,int segR) :CBarSteelBar(gdrL,gdrR,segL,segR)
{
	m_BarType = CBarSteelBar::T;
}

CBarTSteelBar::~CBarTSteelBar()
{
}



//������ʾ�ø����㼯(������ǰ����������������)
void CBarTSteelBar::createPtArray()
{
	double pLenz, pLeny;      //������ƽ�����Ҫ���ܼ�������γ���(��˺��Ҷ�)
	double sLenz, sLeny;      //�������������Ҫ���ܼ�������γ���(��˺��Ҷ�)
	double rLenz, rLeny;      //������Ҫ���ܼ�������γ������ղ���ֵ(��˺��Ҷ�)

	double x0;                //�������
	double hj;                //�����������ߺ��
	double curX,Ytmp;         //��ʱx,y�������
	int IsAnchor;
	CSegment* seg = nullptr;  //��Ų������Ľڶζ���
	PT3D* pt = nullptr;       //�����ʱ��ά����� 

	/******************************************************************************************************/
	#define CALPT_ADDTOVECTOR hj = -getXAt_p(curX, IsAnchor);\
								  Ytmp = getTopLev(curX) - getYAtX(curX);\
								  pt = new PT3D(hj, Ytmp, curX);\
								  m_BarPtarrayReal.push_back(pt);\
   /******************************************************************************************************/

	for (vector<PT3D*>::iterator iter = m_BarPtarrayReal.begin(); iter != m_BarPtarrayReal.end(); iter++)
		if (nullptr != *iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	m_BarPtarrayFace.clear();

	pLenz = m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P;
	pLeny = m_X8_P + m_X9_P + m_X10_P + m_X11_P + m_X12_P + m_X13_P;

	sLenz = m_X1 + m_X2 + m_X3;
	sLeny = m_X5 + m_X6 + m_X7;

	rLenz = (pLenz > sLenz) ? pLenz : sLenz;
	rLeny = (pLeny > sLeny) ? pLeny : sLeny;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	x0 = m_Lgirder->m_Center + seg->m_AddLength + m_LtoleDis;


	//�����һ���ֵ���
	curX = x0;
	while (curX - x0 < rLenz)
	{
		CALPT_ADDTOVECTOR
		curX += m_DensityBend;
	}
	curX = x0 + rLenz;
	CALPT_ADDTOVECTOR
	curX += m_DensitynoBend;

	//����ڶ����ֵ���
	while (curX < x0 + m_PrjLen - rLeny)
	{
		CALPT_ADDTOVECTOR
		curX += m_DensitynoBend;
	}
	curX = x0 + m_PrjLen - rLeny;
	CALPT_ADDTOVECTOR
	curX += m_DensityBend;

	//����������ֵ���
	while (curX < x0 + m_PrjLen)
	{
		CALPT_ADDTOVECTOR
		curX += m_DensityBend;
	}

	//�������һ��
	curX = x0 + m_PrjLen;
	CALPT_ADDTOVECTOR
}




void CBarTSteelBar::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

