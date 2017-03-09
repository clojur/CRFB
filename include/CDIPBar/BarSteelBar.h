#pragma once
#include "BarBase.h"
#include "../include/RigidFrameBridge/RigidFrameBridge.h"

/***************************************************************************************


                   ����ֽ��߻��ࣨ����Ϊ��λ,�ں� m_Count ��7о�ֽ��ߣ�
				   ���ȳ�Ա������λ:(��) , �Ƕȳ�Ա������λ:(��)


****************************************************************************************/

class CDIP_BAR_API CBarSteelBar : public CBarBase
{
	DECLARE_SERIAL(CBarSteelBar)

public:
	
	static CRigidFrameBridge* brg;  //�Ŷ���
	static bool b_HasGetinfo;       //��־λ:�����Ƿ��Ѿ���ȡ��ȫ�Ż�����Ϣ
	static const double ERR_ELEV;   //�����д���ĸ̷߳��ش�ֵ 

	enum SteelBarType
	{
		T,	// ������
		X,  // ������
		H,	// �߿��£������
		B,	// �߿��£�װ���
		D,  // �п��£������
		Z   // �п��£�װ���
	};

	CBarSteelBar();
	virtual ~CBarSteelBar();
	virtual void Serialize(CArchive& ar);
	
	//�������������Ϣ:·�߶���,��Ʊ��λ��,��������,����ֵ,�������,��������,�����׿�
	void GetBridgeInfo(CRigidFrameBridge* brg);
	
	//�������캯��(��������ê�̽ڶ����ڵ���������,�Լ�����ê�̽ڶα��)
	CBarSteelBar(CMainGirder* GdrL, CMainGirder* GdrR, int Lanchor, int Ranchor);
	//���ø�������
	void SetCounts(int ct){ m_Count = ct; }
	//���ø���������ʽ
	void SetTensionMethod(int tm){ m_StretchMode = tm; }

	//���ñ������Ļ���ƽ�����
	void SetBasePLanPara(double z0, double z1, double z8, double xz1, double xr2, double Aa, double Ar, double Da, double Dr);
	//���ñ������Ļ���ƽ�����(���ƽ��)+1����
	void SetBasePLanPara(bool bMultibend,double z0, double z1, double z8, double xz1, double xr2, double Aa, double Ar, double Da, double Dr);

	//���ñ������Ļ����������(������T,X,H,D,Z�������Z����,h1,h2�������ҳݰ��)
	void SetBaseProPara(CString strName, double h1, double h2, double delta, double Aa, double Da, double Br, double Cr, double bj1, double bj2);
	//���ñ������Ļ����������(������B) +1����
	void SetBaseProPara(CString strName, CString side, double h, double d, double delta, double Aa, double Da, double Br, double Cr, double bj1, double bj2);


	//���¼���ƽ��������
	void CalDetailPlanPara();
	//���¼���ƽ��������(���ƽ��)
	void CalDetailPlanPara_Multi();

	//���¼�������������(��Ϊ�麯���ǿ��� X �������ܳ���"ñ��")
	virtual void CalDetailProPara();
	//���¼�������������(������Z����)
	void CalDetailProPara_Z();
	//���¼�������������(������B��������߿�)
	void CalDetailProPara_BL();
	//���¼�������������(������B�������ұ߿�)
	void CalDetailProPara_BR();


	//�õ�������� X ֵʱ�ĸ������붥����ߵװ�ľ���(������Ϊ�麯��,T��X��H��D �����û����Ա,B��Z��������д��Ա)
	virtual double getYAtX(double x);
	//�õ��������xֵʱ�ĸ���ƽ�����(������������)
	double getXAt_p(double x, int& IsAnchor);
	//�õ��������xֵʱ������Ӧ����ĸ߳� 
	double getTopLev(double x);
	//�õ��������xֵʱ������Ӧ�װ�ĸ߳� 
	double getBottomLev(double x);

	//���ݸ����ں��ֽ��߸������㡾��������:rType=1 , ������С���:rType=2 , ���ƹ�ֱ��:rType=3��
	static double GetParaByBeamct(short count, short calType);


	int m_BarType;           //��������(T,X,H,B,D,Z)
	CString m_Name;          //��������
	int  m_Count;            //ÿ���ں��ֽ��߸���
	bool m_IsReserveBar;     //�Ƿ��Ǳ��ø���

	CMainGirder* m_Lgirder;  //��ê�̽ڶ���������
	CMainGirder* m_Rgirder;  //��ê�̽ڶ���������
	int m_Lseg;              //��ê�̽ڶα��
	int m_Rseg;              //��ê�̽ڶα��
	double m_DisL;           //��ê�̽ڶ����(�ӷ����)
	double m_DisR;           //��ê�̽ڶ����(�ӷ����) 
	double m_DisLtoR;        //����ê�̽ڶμ�ĳ���(�ӷ�����)
	double m_PrjLen;         //����ͶӰ����
	CString m_side;          // �����߿�����("��"or "��")          


	/********************************  ���䲿����ز���  ***************************************/

	double m_H1, m_H2;                      //�����˲��ඥ��ľ���(�����Ǿ�װ�ľ���)
	double m_Delta;                         //����ƽֱ�ξඥ��ľ���(������ƽֱ�ξ�װ�ľ���)

	double m_Aa_S, m_Ba_S, m_Ca_S, m_Da_S;  //����������(A,B,C,D)ƫ��
	double m_Ar_S, m_Br_S, m_Cr_S, m_Dr_S;  //����������(A,B,C,D)�뾶
	double m_At_S, m_Bt_S, m_Ct_S, m_Dt_S;  //����������(A,B,C,D)T��
	double m_Ae_S, m_Be_S, m_Ce_S, m_De_S;  //����������(A,B,C,D)���
	double m_Al_S, m_Bl_S, m_Cl_S, m_Dl_S;  //����������(A,B,C,D)������

	double m_X1, m_X2, m_X3, m_X4, m_X5, m_X6, m_X7,m_X8,m_X9;  //��������������ˮƽ�ֶγ���
	double m_Y1, m_Y2, m_Y3, m_Y4, m_Y5, m_Y6;                  //������������������ֶγ���  

	double m_LtoleDis;                      //��߾�(ê�̵㵽�ڶνӷ�ľ���)
	double m_RtoleDis;                      //�ұ߾�(ê�̵㵽�ڶνӷ�ľ���)

	double m_LtplateH;                      //���ݰ��
	double m_RtplateH;                      //�Ҳ�ݰ�� 


	/********************************  ƽ�䲿����ز���  ***************************************/

	double m_Z0, m_Z1, m_Z2, m_Z3, m_Z4, m_Z5, m_Z6, m_Z7, m_Z8;   //����ƽ������������ֶγ���
	double m_XZ1, m_XZ2, m_XR1, m_XR2;                             //����ƽ��������ˮƽ���볤��
	double m_X1_P, m_X2_P, m_X3_P, m_X4_P, m_X5_P, m_X6_P, m_X7_P; //����ƽ��������ˮƽ�ֶγ���
	double m_X8_P, m_X9_P, m_X10_P, m_X11_P, m_X12_P, m_X13_P;     //����ƽ��������ˮƽ�ֶγ���

	bool m_MultiBend;                       //�����Ƿ���ƽ��

	double m_Aa_P, m_Ba_P, m_Ca_P, m_Da_P;  //����������(A,B,C,D)ƽ���
	double m_Ar_P, m_Br_P, m_Cr_P, m_Dr_P;  //����������(A,B,C,D)ƽ��뾶
	double m_At_P, m_Bt_P, m_Ct_P, m_Dt_P;  //����������(A,B,C,D)T��
	double m_Ae_P, m_Be_P, m_Ce_P, m_De_P;  //����������(A,B,C,D)���
	double m_Al_P, m_Bl_P, m_Cl_P, m_Dl_P;  //����������(A,B,C,D)������


	static CString  m_LocElevation;         //��Ʊ��λ��
	static CString  m_SlpType;              //��������
	static double   m_SlpVal;               //����ֵ ���Ϊ�����ҵ�Ϊ��
	static double   m_Guardrail;            //�������
	static double   m_TboxWidth;            //�ſ�(��������)
	static double	m_DboxWidth;            //�׿�(�����׿�)
	static double   m_Pavement;             //��װ���

	static double   m_DensityBend;          //�����������ֵĵ㼯�ܶ�(��)
	static double   m_DensitynoBend;        //����ƽֱ���ֵĵ㼯�ܶ�(��)

};


