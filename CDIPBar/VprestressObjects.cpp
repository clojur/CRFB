
#include "stdafx.h"
#include "VprestressObjects.h"



CVprestressObjects::CVprestressObjects()
{
	m_Space = 0.5;
	m_Limit = 4.0;
}

CVprestressObjects::~CVprestressObjects()
{
	//ɾ���ֽ�㼯vector�ͷ��ڴ�
	for (vector<CVprestressBar*>::iterator iter = m_bars.begin(); iter != m_bars.end(); iter++)
	{
		if (nullptr != *iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	}
	m_bars.clear();
}


//�Զ��������ԤӦ���ֽ�
void CVprestressObjects::AutoDesign(CRigidFrameBridge* brg)
{
	PT3D* pt = nullptr;                          //��ʱ3D����� 
	CVprestressBar* bar = nullptr;               //�����ֽ����

	CString slptype = brg->m_StdSect.m_SlpType;  //��������
	double slp = brg->m_StdSect.m_SlpVal;        //����ֵ
	double dk = brg->m_StdSect.m_DboxWidth;      //�����װ��(m)

	double ys;                                   //ÿ�ڶο۳���׼�κ������(ÿ��)
	double oldBH = 1;                            //���һ���ֽ�ı��(�����ۼ�)

	double cellLength;                           //�ڶγ���(cm)
	double sp = m_Space * 100;                   //�Ų����(cm)
	double tmpY;


	//ɾ���ֽ�㼯
	for (vector<CVprestressBar*>::iterator iter = m_bars.begin(); iter != m_bars.end(); iter++)
	{
		if (nullptr != *iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	}
	m_bars.clear();


	//���������ѭ��
	for (vector<CMainGirder*>::iterator giter = brg->m_MainGirders.begin(); giter != brg->m_MainGirders.end(); giter++)
	{
		//�ڶζ���Сѭ��
		for (vector<CSegment*>::iterator siter = (*giter)->segments.begin(); siter != (*giter)->segments.end(); siter++)
		{
			//0#����4����Ԫ��ɣ�������������⴦��(��Ϊһ����Ԫ���)
			if ((*siter)->m_Id == -100)
				cellLength = ((*giter)->m_BottomLen + 2 * (*giter)->m_Remain) * 100;
			else if ((*siter)->m_Id == -99 || (*siter)->m_Id == 99 || (*siter)->m_Id == 100)
				goto nextStep;
			else
				cellLength = (*siter)->m_Length * 100;


			ys = fmod(cellLength,sp);
			if (ys < ACC_NORMAL) 
			{
				for (int i = oldBH; i <= oldBH + round(cellLength / sp) - 1; i++)
				{
					bar = new CVprestressBar();
					bar->m_Id = i;
					bar->m_Name.Format(_T("%d"), i);
					bar->m_Stake = (*siter)->m_LeftStake + sp / 2 / 100 + (i - oldBH) * sp / 100;
					bar->m_Extdis = 0;
					bar->m_Column = 2;
					bar->m_Discol = 0.6 - 0.3;//������Ҫ��д�������x�����Ⱥ���

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					brg->m_RoadData.GetDesignElevation(pt->z, tmpY);//������Ҫ��д��ö���ʵ�ʸ߳�
					pt->y = tmpY;
					bar->m_BarPtarrayReal.push_back(pt);

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					pt->y = tmpY - (*giter)->getH_AtX(pt->z - (*giter)->m_Center);//������Ҫ��д���ʵ�ʵļ���
					bar->m_BarPtarrayReal.push_back(pt);

					//����ֽ��ܳ���
					bar->m_Alength = tmpY - pt->y;

					//����ֽ�����
					bar->m_BarType = (tmpY - pt->y) >= m_Limit ? _T("ԤӦ���ֽ���") : _T("�������Ƹֽ�");

					//д"��β��"λ��
					if (i == oldBH)
						bar->m_Site = _T("head");
					else if (i == oldBH + round(cellLength / sp) - 1)
						bar->m_Site = _T("tail");
					else
						bar->m_Site = _T("center");
					m_bars.push_back(bar);
				}
				oldBH += round(cellLength / sp);

			}
			else
			{
				for (int i = oldBH; i <= oldBH + floor(cellLength / sp); i++)
				{
					bar = new CVprestressBar();
					bar->m_Id = i;
					bar->m_Name.Format(_T("%d"), i);
					bar->m_Stake = (*siter)->m_LeftStake + ys / 2 / 100 + (i - oldBH) * sp / 100;
					bar->m_Extdis = 0;
					bar->m_Column = 2;
					bar->m_Discol = 0.6 - 0.3;//������Ҫ��д�������x�����Ⱥ���

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					brg->m_RoadData.GetDesignElevation(pt->z, tmpY);//������Ҫ��д��ö���ʵ�ʸ߳�
					pt->y = tmpY;
					bar->m_BarPtarrayReal.push_back(pt);

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					pt->y = tmpY - (*giter)->getH_AtX(pt->z - (*giter)->m_Center);//������Ҫ��д���ʵ�ʵļ���
					bar->m_BarPtarrayReal.push_back(pt);

					//����ֽ��ܳ���
					bar->m_Alength = tmpY - pt->y;

					//����ֽ�����
					bar->m_BarType = (tmpY - pt->y) >= m_Limit ? _T("ԤӦ���ֽ���") : _T("�������Ƹֽ�");

					//д"��β��"λ��
					if (i == oldBH)
						bar->m_Site = _T("head");
					else if (i == oldBH + floor(cellLength / sp))
						bar->m_Site = _T("tail");
					else
						bar->m_Site = _T("center");
					m_bars.push_back(bar);
				}
				oldBH = oldBH + floor(cellLength / sp) + 1;
			}
		nextStep:;
		}
	}
}



//�Զ���������(���ֽ��)  
bool SortByH(const CVprestressBar* v1, const CVprestressBar* v2)
{
	return v1->m_Alength < v2->m_Alength;  
}



//���¸�д�����ע���
void CVprestressObjects::RewriteBarname()
{
	double oldH;   //�ϳ���
	int oldBH = 1; //�ϱ��

	sort(m_bars.begin(), m_bars.end(), SortByH);

	oldH = m_bars[0]->m_Alength;

	for (vector<CVprestressBar*>::iterator it = m_bars.begin(); it != m_bars.end(); it++)
	{
		if (fabs((*it)->m_Alength - oldH) > ACC_VPRE)
		{
			oldBH ++;
			oldH = (*it)->m_Alength;
		}
		(*it)->m_Name.Format(_T("%d"), oldBH);
	}
}



