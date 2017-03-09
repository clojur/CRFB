#include "stdafx.h"
#include "Math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CYjMath::CYjMath()
{
	instance();
}


CYjMath::~CYjMath()
{
}


double CYjMath::GetArea(int n, POINT2dArray  points)//得到任意多边形面积
{
	double s = 0, a, b, c, p, s0;
	if (n == 2)
	{
		AfxMessageBox(_T("输入的点太少不能构成面积"));
		return 0;
	}
	for (int i = 1; i < n - 1; ++i)
	{
		a = sqrt(pow((points[0].x - points[i].x), 2) + pow((points[0].y - points[i].y), 2));
		b = sqrt(pow((points[0].x - points[i + 1].x), 2) + pow((points[0].y - points[i + 1].y), 2));
		c = sqrt(pow((points[i].x - points[i + 1].x), 2) + pow((points[i].y - points[i + 1].y), 2));
		p = (a + b + c) / 2.0;
		s0 = sqrt(p*(p - a)*(p - b)*(p - c));
		s += s0;
	}
	return s;
}


double CYjMath::instance()
{
	CPoint2d op[11];
	//外轮廓点
	op[0].x = 0; op[0].y = 0;
	op[1].x = -6.0; op[1].y = 0;
	op[2].x = -6.0; op[2].y = -0.4;
	op[3].x = -4.5; op[3].y = -0.4;
	op[4].x = -3.5; op[4].y = -0.8;
	op[5].x = -3.5; op[5].y = -6.8;
	op[6].x = 3.5; op[6].y = -6.8;
	op[7].x = 3.5; op[7].y = -0.6;
	op[8].x = 4.5; op[8].y = -0.2;
	op[9].x = 6.0; op[9].y = -0.2;
	op[10].x = 6.0; op[10].y = 0.2;
	//内轮廓点
	CPoint2d ip[10];
	ip[0].x = 2.111; ip[0].y = -5.728;
	ip[1].x = -2.111; ip[1].y = -5.728;
	ip[2].x = -2.323; ip[2].y = -5.516;
	ip[3].x = -2.323; ip[3].y = -1.616;
	ip[4].x = -1.933; ip[4].y = -1.227;
	ip[5].x = -1.062; ip[5].y = -0.878;
	ip[6].x = 1.062; ip[6].y = -0.878;
	ip[7].x = 1.933; ip[7].y = -1.227;
	ip[8].x = 2.232; ip[8].y = -1.616;
	ip[9].x = 2.232; ip[9].y = -5.516;
	return 0;
}



