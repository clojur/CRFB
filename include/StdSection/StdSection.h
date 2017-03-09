
/* ######################################################################################

                                  ����: ��׼����

                                  ����: ����������׼�������,�������ſ���׿���Ƹ�λ�õ���Ϣ

                                  �������¼�����: ��


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
	CString  m_LocElevation; //��Ʊ��λ��
	CString  m_SlpType;      //��������
	double   m_SlpVal;       //����ֵ ���Ϊ�����ҵ�Ϊ��
	double   m_Guardrail;    //�������
	double   m_TboxWidth;    //�ſ�(��������)
	double	 m_DboxWidth;    //�׿�(�����׿�)
	double	 m_A, m_B, m_H,  //��������
		     m_A1, m_A2, m_A3, m_A4, m_A5,	//����A2���ڽ������룬A3��B3��B6��ʾб�� 
			 m_B1, m_B2, m_B3, m_B4, m_B5, m_B6, m_B7, m_B8, m_B9;
	
};


