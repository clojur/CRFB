#pragma once

// 3D点类
class PT3D{
public:
	PT3D(){	x = y = z = 0;}
	~PT3D(){};
	
	PT3D(double x, double y, double z)
	{ this->x = x; this->y = y; this->z = z; }
	PT3D(const PT3D& pt)
	{ this->x = pt.x; this->y = pt.y; this->z = z; }
	void operator=(const PT3D& pt)
	{ this->x = pt.x; this->y = pt.y; this->z = z; }

	double x;
	double y;
	double z;
};


/***************************************************************************************


                                 抽象钢筋基类


****************************************************************************************/


class CDIP_BAR_API CBarBase : public CObject
{
	DECLARE_SERIAL(CBarBase)
public:
	enum BarType
	{
		RE_BAR,			           //普通钢筋
		PRE_BAR,                   //预应力钢筋
		STEEL_BAR,		           //预应力钢绞线
		WIRE_BAR,		           //预应力钢丝
		SHAPE_STEEL_BAR,           //型钢
	};

public:
	CBarBase();
	virtual ~CBarBase();
	virtual void Serialize(CArchive& ar);

	//获得钢筋总长度(m)
	double getAlength();
	//获得钢筋总重量(m)
	double getAweight();

	PT3D m_LocBase;                //钢筋定位基点
	vector<PT3D*> m_BarPtarrayReal;//钢筋外形点集(真实)
	vector<PT3D*> m_BarPtarrayFace;//钢筋外形点集(相对)

	int   m_SteelClass;            //钢材类型
	short m_StretchMode;           //张拉方式(0:不张拉 2:两端张拉 -1:点集首点为张拉端 1:点集末点为张拉端)

	double m_Modulus;              //弹性模量
	double m_Tstrength;            //抗拉强度
	double m_Uweight;              //容重

	double m_Alength;              //总长度
	double m_Aweight;              //总重量
	double m_WorkLen;              //工作长度(如:张拉工作长度)

protected:
	//由相对点集生成实际点集
	void setRealBarPtarray();

	//求两直线交点(两点两斜率式)
	void getInters(double x1, double y1, double x2, double y2, double k1, double k2, double &x, double &y);

};


