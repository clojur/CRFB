/* ######################################################################################

                                 类名: 主墩

                                 描述: 用来构建主墩对象,包含了长宽等信息

                                 包含的下级对象: 墩身、承台、桩基组


########################################################################################*/

#pragma once

#include <RoadData/RoadData.h>
#include "PierBody.h"
#include "Cap.h"
#include "Pile.h"
#include "macro.h"

class CMainPierCallback;
class CRoadData;

class MAINPIER_API CMainPier : public CObject
{
	friend class CMainPierCallback;
	DECLARE_SERIAL(CMainPier)
public:
	std::vector<CPierBody*> m_PierBodys;  //墩身对象
	CCap* m_Cap;            //承台对象
	std::vector<CPile*> m_Piles;        //桩基组对象

	double m_Width;
	double m_Length;

	float  m_Lspace;//顺桥向间距
	float  m_Cspace;//横桥向间距

private:
	CRoadData *m_Road;
	double m_CenterX;			//主墩中心线的X坐标
	double m_CenterY;			//主墩中心线的Y坐标
	double m_RootHeight;		//0号块根部高
	double m_PierThickness;		//墩壁厚
	double m_CapThickness;		//承台厚度
	double m_CapPadding;		//承台襟边
	double m_PileDiameter;		//桩径
	double m_PileLength;		//桩长
	osg::ref_ptr<osg::Group> _root;
	CMainPierCallback* _callback;

public:
	CMainPier();
	CMainPier(CRoadData *road, double cx, double cy, double bottomLen, double width);
	~CMainPier();
	void Serialize(CArchive& ar);

	double getConcreteVolume();

	void SetPierParam(double thickness, double rootHeight);
	void SetCapParam(double thickness, double padding);
	void SetPileParam(double diameter, double length);

	CMainPierCallback* getUpdateCallback() const { return _callback; }
	osg::Group* getRoot() const { return _root; }

	void Create();

	void CreatePierBody();
	void CreateCap();
	void CreatePills();
};



class CMainPierCallback : public ComponentCallback
{
public:
	CMainPierCallback(CMainPier* pier)
		: ComponentCallback(pier->getRoot())
	{
		_pier = pier;
	}

	void DoUpdate() override;
	
private:
	CMainPier* _pier;
};