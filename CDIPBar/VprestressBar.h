#pragma once
#include "BarBase.h"
#include "../include/RigidFrameBridge/RigidFrameBridge.h"

/***************************************************************************************


                      ����ԤӦ���ֽ��ࣨ�Ը�Ϊ��λ��


****************************************************************************************/

class CDIP_BAR_API CVprestressBar : public CBarBase
{
	DECLARE_SERIAL(CVprestressBar)

public:

	CVprestressBar();
	virtual ~CVprestressBar();
	virtual void Serialize(CArchive& ar);


	CString m_BarType;       //�ֽ�����(�������Ƹֽ� or ԤӦ���ֽ���)
	int m_Id;                //�ֽ���(ȫ��Ψһ,�ڲ�������)
	CString m_Name;          //�ֽ�����(ȫ�Ų�Ψһ,����ʾ�ü�����������)
	CString m_Site;          //�ֽ�λ��("head" or "center" or "tail")

	double m_Stake;          //���
	int m_Column;            //����
	double m_Discol;         //�ž�
	double m_Extdis;         //�쳤ֵ(�ն��ڶε�����ԤӦ�����쳤������ê��) 
	double m_AnchorDepth;    //ê�̶˲ۿ���;

	CMainGirder* m_girder;   //������������
	int m_Seg;               //�����ڶζ�����
	
	bool m_Isoutput;         //�Ƿ������Ϊ���Ͷ���
	double m_Ctrlstresses;   //��������Ӧ��(Mpa)
};


