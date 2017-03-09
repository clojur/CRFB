#pragma once
#include "VprestressBar.h"

/***************************************************************************************


                  竖向预应力钢筋集合类（全桥只需拿一个集合对象即可完成设计）


****************************************************************************************/

class CDIP_BAR_API CVprestressObjects : public CObject
{
public:
	CVprestressObjects();
	virtual ~CVprestressObjects();

	//自动设计竖向预应力钢筋
	void AutoDesign(CRigidFrameBridge* brg);
	//重新改写力筋标注编号
	void RewriteBarname();

	vector<CVprestressBar*> m_bars;

	double m_Space;   //竖向预应力自动设计间距
	double m_Limit;   //粗钢筋和预应力钢绞线采用的分界长度
};


