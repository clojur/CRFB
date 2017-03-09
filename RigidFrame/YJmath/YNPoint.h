#pragma once

// double������㣬�����л�

class CYNPoint2D : public CObject
{
public:

	DECLARE_SERIAL(CYNPoint2D)

	CYNPoint2D();
	CYNPoint2D(double CoordinateX, double CoordinateY);
	CYNPoint2D(const CYNPoint2D& src)//�������캯��
	{
		this->m_CoordinateX = src.m_CoordinateX;
		this->m_CoordinateY = src.m_CoordinateY;
	};

	~CYNPoint2D();

	CYNPoint2D& operator = (CYNPoint2D& src);
	virtual void Serialize(CArchive& ar);

	//void SetPtX(double CoordinateX);//���õ�����X
	//void SetPtY(double CoordinateY);//���õ�����Y

	//double GetPtX();//��õ�����X
	//double GetPtY();//��õ�����Y

public:
	double m_CoordinateX;		// ����X
	double m_CoordinateY;		// ����Y
};






class CYNPoint3D : public CObject
{
public:

	DECLARE_SERIAL(CYNPoint3D)

	CYNPoint3D();
	CYNPoint3D(double CoordinateX, double CoordinateY, double CoordinateZ);
	CYNPoint3D(const CYNPoint3D& src)//�������캯��
	{
		this->m_CoordinateX = src.m_CoordinateX;
		this->m_CoordinateY = src.m_CoordinateY;
		this->m_CoordinateZ = src.m_CoordinateZ;
	};

	~CYNPoint3D();

	CYNPoint3D& operator = (CYNPoint3D& src);
	void Serialize(CArchive& ar);

	//void SetPtX(double CoordinateX);//���õ�����X
	//void SetPtY(double CoordinateY);//���õ�����Y
	//void SetPtZ(double CoordinateZ);//���õ�����Y

	//double GetPtX();//��õ�����X
	//double GetPtY();//��õ�����Y
	//double GetPtZ();//��õ�����Z

public:
	double m_CoordinateX;		// ����X
	double m_CoordinateY;		// ����Y
	double m_CoordinateZ;		// ����Z
};
