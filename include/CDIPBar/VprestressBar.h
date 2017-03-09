#pragma once
#include "BarBase.h"
#include "../include/RigidFrameBridge/RigidFrameBridge.h"

/***************************************************************************************


                      竖向预应力钢筋类（以根为单位）


****************************************************************************************/

class CDIP_BAR_API CVprestressBar : public CBarBase
{
	DECLARE_SERIAL(CVprestressBar)

public:

	CVprestressBar();
	virtual ~CVprestressBar();
	virtual void Serialize(CArchive& ar);


	CString m_BarType;       //钢筋类型(精轧螺纹钢筋 or 预应力钢绞线)
	int m_Id;                //钢筋编号(全桥唯一,内部计算用)
	CString m_Name;          //钢筋名称(全桥不唯一,供显示用及算量分类用)
	CString m_Site;          //钢筋位置("head" or "center" or "tail")

	double m_Stake;          //里程
	int m_Column;            //排数
	double m_Discol;         //排距
	double m_Extdis;         //伸长值(墩顶节段的竖向预应力筋伸长到墩内锚固) 
	double m_AnchorDepth;    //锚固端槽口深;

	CMainGirder* m_girder;   //所属主梁对象
	int m_Seg;               //所属节段对象编号
	
	bool m_Isoutput;         //是否输出作为典型断面
	double m_Ctrlstresses;   //张拉控制应力(Mpa)
};


