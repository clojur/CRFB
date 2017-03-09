/* ######################################################################################

����: ׮�����

����: ��������׮������,������׮�������Ϣ



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
		FRICTION,//Ħ��׮
		END_BEARING   //�˳�׮
	};
	enum ConcreteType
	{
		C10,C15,C20,C25,
		C30,C40,C45,C50,C55,
		C60,C65,C70,C80
	};
	osg::Vec3 m_PileBottomCenter;//��׮�Ķ�λ��Ϣ���������ģ�
	float m_Height;//����׮��
	float m_Diameter;//����׮ֱ��
	unsigned int m_PileCount;//׮����
	float m_YDistance;//˳�ż��
	float m_XDistance;//���ż��
	std::vector<PileBar*> m_PilesBar;//׮�ڸֽ�
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
	/*ÿ�ָֽ������*/
	char m_BarID[20];//�ֽ���
	std::vector<std::string> m_BarUse;//�ֽ���;
	float m_BarDiameter;//�ֽ�ֱ��
	float m_PerBarLength;//ÿ���ֽ��(cm)
	unsigned int m_BarCount;//�ֽ����
	float m_PerMeterBarWeight;//�ֽ�ÿ����(kg/m)

	double m_BarAllLength;//�øֽ��ȫ��
	double m_BarAllWeight;//�øֽ������

	float m_Axial;//���ָֽ�֮���������(���ȵ�λ)
	float m_Circle;//���ָֽ�֮���Բ�ܼ��(�Ƕȵ�λ)
	

	void draw(const char *barID){}
	void SetBarAttrib(const char *barID, float barDiameter, float barLenth, unsigned int barCount){}
};

