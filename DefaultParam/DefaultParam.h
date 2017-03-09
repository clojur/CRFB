/* ######################################################################################

                                    类名: 默认参数

									描述: 用来管理结构设计过程中的默认参数

									包含的下级对象: 路线数据

########################################################################################*/

#pragma once

#include <YjMath/YjMath.h>
#include "macro.h"

class DEFAULTPARAM_API CDefaultParam : public CObject
{
public:
	/*允许超过桥址所在挖方范围的距离*/
	double m_TwBoundryExceedDis;

	/*自动布孔控制参数*/
	int m_AutoLayOutMode;              //自动布孔模式(方案设计、初步设计、施工图设计)
	CYNPoint2D m_ZQMainSpanLimit;      //主桥主跨跨径范围
	CYNPoint2D m_ZQSpanRatioLimit;     //主桥边中跨比值范围
	CYNPoint2D m_YQStandardSpanLimit;  //引桥标准跨径范围
	CYNPoint2D m_YQPierHeightLimit;    //引桥墩高范围

	/*尺寸初步拟定控制参数*/
	CYNPoint3D m_TGirderRootHeightRatio; //坐标X和Y表示相邻两T构所夹中跨跨径Lz与根部梁高h的比值范围（相邻两T构对称布置）；坐标Z表示大T构边跨跨径Lb与根部梁高h1的比值，小T构根部梁高根据2*h-h1来确定（对于大、小T构非对称布置的情况，最终大小T构根部梁高需根据结构计算来确定）
	CYNPoint3D m_TGirderEndHeightRatio;  //坐标X和Y表示相邻两T构所夹中跨跨径L与跨中梁高h的比值范围（对于两T构对称布置的情况）；坐标Z表示相邻两T构所夹中跨跨径与跨中最小梁高的比值（对于大、小T构的非对称情况）；梁高≥2m

	/*合拢段梁长*/
	double m_ClosureLen;

	/*合拢段最小梁高*/
	double m_ClosureHeightMin;

	/*节段划分*/
	int n1B, n2B, n3B, n4B, n5B, n6B, n7B;//自动划分节段单元时节段的最小数目
	int n1E, n2E, n3E, n4E, n5E, n6E, n7E;//自动划分节段单元时节段的最大数目


public:

	DECLARE_SERIAL(CDefaultParam)

	CDefaultParam();

	~CDefaultParam();

	void Serialize(CArchive& ar);

};



