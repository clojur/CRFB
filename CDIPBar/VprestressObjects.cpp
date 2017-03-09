
#include "stdafx.h"
#include "VprestressObjects.h"



CVprestressObjects::CVprestressObjects()
{
	m_Space = 0.5;
	m_Limit = 4.0;
}

CVprestressObjects::~CVprestressObjects()
{
	//删除钢筋点集vector释放内存
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


//自动设计竖向预应力钢筋
void CVprestressObjects::AutoDesign(CRigidFrameBridge* brg)
{
	PT3D* pt = nullptr;                          //临时3D点对象 
	CVprestressBar* bar = nullptr;               //单根钢筋对象

	CString slptype = brg->m_StdSect.m_SlpType;  //横坡类型
	double slp = brg->m_StdSect.m_SlpVal;        //横坡值
	double dk = brg->m_StdSect.m_DboxWidth;      //箱梁底板宽(m)

	double ys;                                   //每节段扣除标准段后的余数(每端)
	double oldBH = 1;                            //最后一根钢筋的编号(不断累加)

	double cellLength;                           //节段长度(cm)
	double sp = m_Space * 100;                   //排布间距(cm)
	double tmpY;


	//删除钢筋点集
	for (vector<CVprestressBar*>::iterator iter = m_bars.begin(); iter != m_bars.end(); iter++)
	{
		if (nullptr != *iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	}
	m_bars.clear();


	//主梁对象大循环
	for (vector<CMainGirder*>::iterator giter = brg->m_MainGirders.begin(); giter != brg->m_MainGirders.end(); giter++)
	{
		//节段对象小循环
		for (vector<CSegment*>::iterator siter = (*giter)->segments.begin(); siter != (*giter)->segments.end(); siter++)
		{
			//0#块由4个单元组成，这里必须做特殊处理(合为一个大单元设计)
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
					bar->m_Discol = 0.6 - 0.3;//这里需要重写获得任意x腹板宽度函数

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					brg->m_RoadData.GetDesignElevation(pt->z, tmpY);//这里需要重写获得顶板实际高程
					pt->y = tmpY;
					bar->m_BarPtarrayReal.push_back(pt);

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					pt->y = tmpY - (*giter)->getH_AtX(pt->z - (*giter)->m_Center);//这里需要重写获得实际的减量
					bar->m_BarPtarrayReal.push_back(pt);

					//计算钢筋总长度
					bar->m_Alength = tmpY - pt->y;

					//定义钢筋类型
					bar->m_BarType = (tmpY - pt->y) >= m_Limit ? _T("预应力钢绞线") : _T("精轧螺纹钢筋");

					//写"首尾中"位置
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
					bar->m_Discol = 0.6 - 0.3;//这里需要重写获得任意x腹板宽度函数

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					brg->m_RoadData.GetDesignElevation(pt->z, tmpY);//这里需要重写获得顶板实际高程
					pt->y = tmpY;
					bar->m_BarPtarrayReal.push_back(pt);

					pt = new PT3D();
					pt->z = bar->m_Stake;
					pt->x = -dk / 2 - 0.15;
					pt->y = tmpY - (*giter)->getH_AtX(pt->z - (*giter)->m_Center);//这里需要重写获得实际的减量
					bar->m_BarPtarrayReal.push_back(pt);

					//计算钢筋总长度
					bar->m_Alength = tmpY - pt->y;

					//定义钢筋类型
					bar->m_BarType = (tmpY - pt->y) >= m_Limit ? _T("预应力钢绞线") : _T("精轧螺纹钢筋");

					//写"首尾中"位置
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



//自定义排序函数(按钢筋长度)  
bool SortByH(const CVprestressBar* v1, const CVprestressBar* v2)
{
	return v1->m_Alength < v2->m_Alength;  
}



//重新改写力筋标注编号
void CVprestressObjects::RewriteBarname()
{
	double oldH;   //老长度
	int oldBH = 1; //老编号

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



