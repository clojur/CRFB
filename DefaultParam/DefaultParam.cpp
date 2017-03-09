#include "stdafx.h"
#include "DefaultParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CDefaultParam, CObject, 0);

CDefaultParam::CDefaultParam()
{
	m_TwBoundryExceedDis = 50.0;

	CYNPoint2D m_Pt_1(80, 180);
	m_ZQMainSpanLimit = m_Pt_1;      //主桥主跨跨径范围
	CYNPoint2D m_Pt_2(0.52, 0.58);
	m_ZQSpanRatioLimit = m_Pt_2;     //主桥边中跨比值范围
	CYNPoint2D m_Pt_3(20, 40);
	m_YQStandardSpanLimit = m_Pt_3;  //引桥标准跨径范围
	CYNPoint2D m_Pt_4(1.5, 60);
	m_YQPierHeightLimit = m_Pt_4;    //引桥墩高范围

	CYNPoint3D m_Pt_5(16, 17, 10);
	m_TGirderRootHeightRatio = m_Pt_5;
	CYNPoint3D m_Pt_6(45, 60, 40);
	m_TGirderEndHeightRatio = m_Pt_6;

	m_ClosureLen = 2;

	m_ClosureHeightMin = 2;

	n1B = n2B = n3B = n4B = n5B = n6B = n7B = 2;  //自动划分节段单元时节段的最小数目
	n1E = 0;
	n2E = 6;
	n3E = 6;
	n4E = 8;
	n5E = 8;
	n6E = 0;
	n7E = 0;  //自动划分节段单元时节段的最大数

}

CDefaultParam::~CDefaultParam()
{
}

void CDefaultParam::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_TwBoundryExceedDis << m_AutoLayOutMode << m_ClosureLen;
		ar << n1B << n2B << n3B << n4B << n5B << n6B << n7B;//自动划分节段单元时节段的最小数目
		ar << n1E << n2E << n3E << n4E << n5E << n6E << n7E;//自动划分节段单元时节段的最大数目
	}
	else
	{	// loading code
		ar >> m_TwBoundryExceedDis >> m_AutoLayOutMode >> m_ClosureLen;
		ar >> n1B >> n2B >> n3B >> n4B >> n5B >> n6B >> n7B;//自动划分节段单元时节段的最小数目
		ar >> n1E >> n2E >> n3E >> n4E >> n5E >> n6E >> n7E;//自动划分节段单元时节段的最大数目
	}
	m_ZQMainSpanLimit.Serialize(ar);
	m_ZQSpanRatioLimit.Serialize(ar);
	m_YQStandardSpanLimit.Serialize(ar);
	m_YQPierHeightLimit.Serialize(ar);
	m_TGirderRootHeightRatio.Serialize(ar);
	m_TGirderEndHeightRatio.Serialize(ar);
}
