#pragma once

// double型坐标点，可序列化

class CYNPoint2D : public CObject
{
public:

	DECLARE_SERIAL(CYNPoint2D)

	CYNPoint2D();
	CYNPoint2D(double CoordinateX, double CoordinateY);
	CYNPoint2D(const CYNPoint2D& src)//拷贝构造函数
	{
		this->m_CoordinateX = src.m_CoordinateX;
		this->m_CoordinateY = src.m_CoordinateY;
	};

	~CYNPoint2D();

	CYNPoint2D& operator = (CYNPoint2D& src);
	virtual void Serialize(CArchive& ar);

	//void SetPtX(double CoordinateX);//设置点坐标X
	//void SetPtY(double CoordinateY);//设置点坐标Y

	//double GetPtX();//获得点坐标X
	//double GetPtY();//获得点坐标Y

public:
	double m_CoordinateX;		// 坐标X
	double m_CoordinateY;		// 坐标Y
};






class CYNPoint3D : public CObject
{
public:

	DECLARE_SERIAL(CYNPoint3D)

	CYNPoint3D();
	CYNPoint3D(double CoordinateX, double CoordinateY, double CoordinateZ);
	CYNPoint3D(const CYNPoint3D& src)//拷贝构造函数
	{
		this->m_CoordinateX = src.m_CoordinateX;
		this->m_CoordinateY = src.m_CoordinateY;
		this->m_CoordinateZ = src.m_CoordinateZ;
	};

	~CYNPoint3D();

	CYNPoint3D& operator = (CYNPoint3D& src);
	void Serialize(CArchive& ar);

	//void SetPtX(double CoordinateX);//设置点坐标X
	//void SetPtY(double CoordinateY);//设置点坐标Y
	//void SetPtZ(double CoordinateZ);//设置点坐标Y

	//double GetPtX();//获得点坐标X
	//double GetPtY();//获得点坐标Y
	//double GetPtZ();//获得点坐标Z

public:
	double m_CoordinateX;		// 坐标X
	double m_CoordinateY;		// 坐标Y
	double m_CoordinateZ;		// 坐标Z
};
