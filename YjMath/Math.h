#pragma once
#include<vector>
//typedef struct tag2POINTd{
//	double x;
//	double y;
//}POINT2d,*lpPOINT2d;

class YJMATH_API CPoint2d{
public:
	~CPoint2d(){};
	CPoint2d(){ x = 0; y = 0; }
	CPoint2d(double x, double y){ this->x = x; this->y = y; }
	CPoint2d(const CPoint2d& src){ this->x = src.x; this->y = src.y; }
	void operator=(const CPoint2d& src){ this->x = src.x; this->y = src.y; }
	double x;
	double y;
};
typedef std::vector<CPoint2d> POINT2dArray;
class YJMATH_API CYjMath
{
public:
	CYjMath();
	~CYjMath();
public:
	//计算面积
	double GetArea(int n, POINT2dArray  points);
	//手工实例化点
	double instance();
};

