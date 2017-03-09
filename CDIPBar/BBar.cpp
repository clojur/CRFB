
#include "stdafx.h"
#include "BBar.h"


IMPLEMENT_SERIAL(CBarBSteelBar, CBarSteelBar, 0);
CBarBSteelBar::CBarBSteelBar()
{
	m_BarType = CBarSteelBar::B;
}


CBarBSteelBar::CBarBSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR) :CBarSteelBar(gdrL, gdrR, segL, segR)
{
	m_BarType = CBarSteelBar::B;
}


CBarBSteelBar::~CBarBSteelBar()
{
}



//�õ�������� X ֵʱ�ĸ�������߿�װ�ľ��� [(��֤���� 2016.10.28)]
double CBarBSteelBar::getYAtX(double x)
{
	double elev, elev1, elev2;
	double rB, rC, A, D, Th1, Th2, newX, iah, idh, tmp;
	double lDG, rDG; //ǰ��ê�̶������װ�ױ��
	CSegment* seg = nullptr;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	Th1 = seg->m_LplateThick;
	iah = (seg->m_RbottomElev1 - seg->m_LbottomElev1) / seg->m_Length;
	lDG = seg->m_LbottomElev2;

	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	Th2 = seg->m_RplateThick;
	idh = (seg->m_RbottomElev1 - seg->m_LbottomElev1) / seg->m_Length;
	rDG = seg->m_RbottomElev2;

	if (m_DisL - x > ACC_NORMAL || x - m_DisR > ACC_NORMAL)
	{
		return ERR_ELEV;
	}

	rB = m_Br_S;
	rC = m_Cr_S;
	A = m_Aa_S * PI / 180;
	D = m_Da_S * PI / 180;

	//���¶���newXֵ
	newX = x - m_DisL - m_X1;

	//�����������ê�̵�߳�
	if (m_side == _T("��"))
	{
		elev1 = lDG + m_H1;
		elev2 = rDG + Th2 + m_RtoleDis * fabs(idh) + m_RtplateH * cos(D);
	}
	else
	{
		elev1 = lDG + Th1 + m_LtoleDis * fabs(iah) + m_LtplateH * cos(A);
		elev2 = rDG + m_H2;
	}

	//����x��ĵװ�߳�
	elev = getBottomLev(x);

	if (newX <= 0 && newX >= -m_X1)
	{
		return elev1 - elev;
	}
	else if (newX >= 0 && newX < m_X2)
	{
		return elev1 - elev - newX * tan(A);
	}
	else if (newX >= m_X2 && newX < m_X2 + m_X3 + m_X4)
	{
		tmp = newX - m_X2;
		return elev1 - elev - m_Y3 - (sqrt(rB*rB - pow(tmp - rB * sin(A),2)) - rB * cos(A));
	}
	else if (newX >= m_X2 + m_X3 + m_X4 && newX < m_X2 + m_X3 + m_X4 + m_X5)
	{
		return  m_Delta;
	}
	else if (newX >= m_X2 + m_X3 + m_X4 + m_X5 && newX < m_X2 + m_X3 + m_X4 + m_X5 + m_X6 + m_X7)
	{
		tmp = m_DisR - m_DisL - m_X8 - m_X9 - m_X1 - newX;
		return elev2 - elev - m_Y6 - (sqrt(rC * rC - pow(rC * sin(D) - tmp,2)) - rC * cos(D));
	}
	else if (newX >= m_X2 + m_X3 + m_X4 + m_X5 + m_X6 + m_X7 && newX < m_X2 + m_X3 + m_X4 + m_X5 + m_X6 + m_X7 + m_X8)
	{
		tmp = m_X2 + m_X3 + m_X4 + m_X5 + m_X6 + m_X7 + m_X8 - newX;
		return elev2 - elev - tmp * tan(D);
	}
	else
	{
		return elev2 - elev;
	}
}



//������ʾ�ø����㼯(������ǰ����������������)
void CBarBSteelBar::createPtArray()
{
	double pLenz, pLeny;      //������ƽ�����Ҫ���ܼ�������γ���(��˺��Ҷ�)
	double sLenz, sLeny;      //�������������Ҫ���ܼ�������γ���(��˺��Ҷ�)
	double rLenz, rLeny;      //������Ҫ���ܼ�������γ������ղ���ֵ(��˺��Ҷ�)

	double x0;                //�������
	double hj;                //�����������ߺ��
	double curX, Ytmp;         //��ʱx,y�������
	int IsAnchor;
	CSegment* seg = nullptr;  //��Ų������Ľڶζ���
	PT3D* pt = nullptr;       //�����ʱ��ά����� 

	/******************************************************************************************************/
#define CALPT_ADDTOVECTOR hj = -getXAt_p(curX, IsAnchor);\
								  Ytmp = getBottomLev(curX) + getYAtX(curX);\
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

	sLenz = m_X2 + m_X3 + m_X4;
	sLeny = m_X6 + m_X7 + m_X8;

	rLenz = (pLenz > sLenz) ? pLenz : sLenz;
	rLeny = (pLeny > sLeny) ? pLeny : sLeny;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	x0 = seg->m_LeftStake + m_X1;


	//�����һ���ֵ���
	curX = x0;
	while (curX - x0 < rLenz)
	{
		//CALPT_ADDTOVECTOR
		hj = -getXAt_p(curX, IsAnchor);
		double aa = getBottomLev(curX);
		double bb = getYAtX(curX);
		Ytmp = aa + bb;
		pt = new PT3D(hj, Ytmp, curX);
		m_BarPtarrayReal.push_back(pt);

		curX += m_DensityBend;
	}
	curX = x0 + rLenz;
	CALPT_ADDTOVECTOR

		if (curX + m_DensitynoBend > x0 + m_PrjLen - rLeny)
			curX = x0 + (m_PrjLen + rLenz - rLeny) / 2;
		else
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



void CBarBSteelBar::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

