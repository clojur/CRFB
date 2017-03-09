
/* ######################################################################################

                                 类名: 节段

                                 描述: 用来构建节段对象

                                 包含的下级对象: 无


########################################################################################*/

#pragma once

#include <YjMath/YjMath.h>
#include <StdSection/StdSection.h>
#include <RoadData/RoadData.h>
//#include "../RigidFrame/MainFrm.h"
#include "macro.h"


class CSegmentCallback;

class SEGMENT_API CSegment : public CPickObject
{
	friend class CSegmentCallback;
public:
	_DECLARE_DYNCREATE(CSegment)      \
	AFX_EXT_API friend CArchive& AFXAPI \
	operator>>(CArchive& ar, CSegment* &pOb);

	CSegment();
	~CSegment();

	CYjMath Yjmath;         //数学库

	int m_Id;           //节段编号
	bool	m_Selected;     //节段是否被选中
	double  m_LeftStake;
	double	m_RightStake;
	double	m_Length;       //长度
	double  m_AddLength;    //累计长度
	double	m_LlHeight;     //左截面左梁高
	double	m_LrHeight;     //左截面右梁高
	double	m_RlHeight;     //右截面左梁高
	double	m_RrHeight;     //右截面右梁高
	double	m_LplateThick;  //左底板厚
	double	m_RplateThick;  //右底板厚
	double	m_LwebThick;    //左腹板厚
	double	m_RwebThick;    //右腹板厚
	double	m_LdesignElev;  //左设计高程
	double	m_RdesignElev;  //右设计高程
	double	m_LlwebElev;    //左截面左上高程
	double	m_LrwebElev;    //左截面右上高程
	double	m_RlwebElev;    //右截面左上高程
	double	m_RrwebElev;    //右截面右上高程
	double	m_LbottomElev1; //左截面下高程1
	double	m_LbottomElev2; //左截面下高程2
	double	m_RbottomElev1; //右截面下高程1
	double	m_RbottomElev2; //右截面下高程2
	double	m_Volume;       //体积
	double	anti_crackRebar;//防裂钢筋

	POINT2dArray Slo_points; //左外轮廓面点集
	POINT2dArray Sli_points; //左内轮廓面点集
	POINT2dArray  Sro_points;//右外轮廓面点集
	POINT2dArray Sri_points; //右内轮廓面点集
	CStdSection crossSection;//横截面类

	void Serialize(CArchive& ar);//序列化
	double  GetVolume();//计算节段体积
	double	GetSlo();//获取左外轮廓面积
	double	GetSli();//获取左内轮廓面积
	double	GetSro();//获取右外轮廓面积
	double	GetSri();//获取右内轮廓面积
	void SetRoadData(CRoadData *road);
	void SetSection(CStdSection* sect);
	osg::Group* getRoot() const { return _root; }
	CSegmentCallback* getUpdateCallback() const { return _callback; }
	void GetModelAttrib();
private:
	osg::ref_ptr<osg::Group> _root;
	CSegmentCallback* _callback;
	CRoadData* m_Road;
	CStdSection* m_Section;
};



class SEGMENT_API CSegmentCallback : public ComponentCallback
{
public:
	CSegmentCallback(CSegment* segment)
		: ComponentCallback(segment->getRoot())
		, _segment(segment)
	{
	}

	void DoUpdate() override;

private:
	CClassGeode *UpdateScene(double leftStake, double leftElev, double leftHeight, double rightStake, double rightElev, double rightHeight);

private:
	CSegment* _segment;
};