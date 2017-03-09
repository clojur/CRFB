
/* ######################################################################################

                                  类名: 标准截面

                                  描述: 用来构建标准截面对象,包含了桥宽、箱底宽、设计高位置等信息

                                  包含的下级对象: 无


########################################################################################*/

#pragma once
#include <YjOSG/YjOSG.h>
#include "macro.h"

class STDSECTION_API CStdSection :public CObject
{
	DECLARE_SERIAL(CStdSection)
public:
	CStdSection();
	CStdSection(CStdSection& section);
    ~CStdSection();
	void Serialize(CArchive& ar);
	osg::ref_ptr<osg::Geometry> Draw(double stake, double centerY, double centerZ, double height);
	osg::ref_ptr<osg::Geometry> Draw(double stake, double centerY, double centerZ, double height, osg::Vec3Array* outer, osg::Vec3Array* inner);

private:
	void CalcOuter(double stake, double centerY, double centerZ, double height, osg::Vec3Array* outer);
	void CalcInner(double stake, double centerY, double centerZ, double height, osg::Vec3Array* inner);

public:
	CString  m_LocElevation; //设计标高位置
	CString  m_SlpType;      //横坡类型
	double   m_SlpVal;       //横坡值 左低为负、右低为正
	double   m_Guardrail;    //护栏宽度
	double   m_TboxWidth;    //桥宽(箱梁顶宽)
	double	 m_DboxWidth;    //底宽(箱梁底宽)
	double	 m_A, m_B, m_H,  //横截面参数
		     m_A1, m_A2, m_A3, m_A4, m_A5,	//其中A2不在界面输入，A3、B3、B6表示斜率 
			 m_B1, m_B2, m_B3, m_B4, m_B5, m_B6, m_B7, m_B8, m_B9;
	
};


