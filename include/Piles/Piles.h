/* ######################################################################################

类名: 桩基配筋

描述: 用来构建桩基对象,包含了桩基配筋信息



########################################################################################*/
#pragma once
#include "afx.h"
#include<iostream>
#include<vector>
#include "Yjosg/YjOSG.h"
#include "macro.h"
class PileBar;
class PILESBAR_API CPiles :public CObject
{
public:
	CPiles();
	~CPiles();
	enum PileType
	{
		FRICTION,//摩擦桩
		END_BEARING   //端承桩
	};
	enum ConcreteType
	{
		C10,C15,C20,C25,
		C30,C40,C45,C50,C55,
		C60,C65,C70,C80
	};
	osg::Vec3 m_PileBottomCenter;//该桩的定位信息（底面中心）
	float m_Height;//单棵桩长
	float m_Diameter;//单棵桩直径
	unsigned int m_PileCount;//桩棵数
	float m_YDistance;//顺桥间距
	float m_XDistance;//横桥间距
	std::vector<PileBar*> m_PilesBar;//桩内钢筋
};

class PileBar:public CObject
{
public:
	PileBar();
	~PileBar();
	enum BarType
	{
		HPB300,
		HRB335,
		HRBF335,
		HRB400,
		HRBF400,
		RRB400,
		HRB400E,
		HRB500,
		HRBF500
	};
	/*每种钢筋共有属性*/
	char m_BarID[20];//钢筋编号
	std::vector<std::string> m_BarUse;//钢筋用途
	float m_BarDiameter;//钢筋直径
	float m_PerBarLength;//每根钢筋长度(cm)
	unsigned int m_BarCount;//钢筋根数
	float m_PerMeterBarWeight;//钢筋每米重(kg/m)

	double m_BarAllLength;//该钢筋的全长
	double m_BarAllWeight;//该钢筋的总重

	float m_Axial;//该种钢筋之间的轴向间距(长度单位)
	float m_Circle;//该种钢筋之间的圆周间距(角度单位)
	

	void draw(const char *barID){}
	void SetBarAttrib(const char *barID, float barDiameter, float barLenth, unsigned int barCount){}
};

