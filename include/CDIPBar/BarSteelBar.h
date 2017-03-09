#pragma once
#include "BarBase.h"
#include "../include/RigidFrameBridge/RigidFrameBridge.h"

/***************************************************************************************


                   纵向钢绞线基类（以束为单位,内含 m_Count 根7芯钢绞线）
				   长度成员变量单位:(米) , 角度成员变量单位:(度)


****************************************************************************************/

class CDIP_BAR_API CBarSteelBar : public CBarBase
{
	DECLARE_SERIAL(CBarSteelBar)

public:
	
	static CRigidFrameBridge* brg;  //桥对象
	static bool b_HasGetinfo;       //标志位:该类是否已经读取过全桥基本信息
	static const double ERR_ELEV;   //计算中错误的高程返回此值 

	enum SteelBarType
	{
		T,	// 顶板束
		X,  // 下弯束
		H,	// 边跨合拢顶板束
		B,	// 边跨合拢底板束
		D,  // 中跨合拢顶板束
		Z   // 中跨合拢底板束
	};

	CBarSteelBar();
	virtual ~CBarSteelBar();
	virtual void Serialize(CArchive& ar);
	
	//获得桥梁基本信息:路线对象,设计标高位置,横坡类型,横坡值,护栏宽度,箱梁顶宽,箱梁底宽
	void GetBridgeInfo(CRigidFrameBridge* brg);
	
	//基本构造函数(传入左右锚固节段所在的主梁对象,以及左右锚固节段编号)
	CBarSteelBar(CMainGirder* GdrL, CMainGirder* GdrR, int Lanchor, int Ranchor);
	//设置钢束根数
	void SetCounts(int ct){ m_Count = ct; }
	//设置钢束张拉方式
	void SetTensionMethod(int tm){ m_StretchMode = tm; }

	//设置本钢束的基本平弯参数
	void SetBasePLanPara(double z0, double z1, double z8, double xz1, double xr2, double Aa, double Ar, double Da, double Dr);
	//设置本钢束的基本平弯参数(多次平弯)+1重载
	void SetBasePLanPara(bool bMultibend,double z0, double z1, double z8, double xz1, double xr2, double Aa, double Ar, double Da, double Dr);

	//设置本钢束的基本竖弯参数(适用于T,X,H,D,Z。如果是Z钢束,h1,h2代表左右齿板高)
	void SetBaseProPara(CString strName, double h1, double h2, double delta, double Aa, double Da, double Br, double Cr, double bj1, double bj2);
	//设置本钢束的基本竖弯参数(适用于B) +1重载
	void SetBaseProPara(CString strName, CString side, double h, double d, double delta, double Aa, double Da, double Br, double Cr, double bj1, double bj2);


	//更新计算平弯具体参数
	void CalDetailPlanPara();
	//更新计算平弯具体参数(多次平弯)
	void CalDetailPlanPara_Multi();

	//更新计算竖弯具体参数(设为虚函数是考虑 X 钢束可能出现"帽筋")
	virtual void CalDetailProPara();
	//更新计算竖弯具体参数(适用于Z钢束)
	void CalDetailProPara_Z();
	//更新计算竖弯具体参数(适用于B钢束，左边跨)
	void CalDetailProPara_BL();
	//更新计算竖弯具体参数(适用于B钢束，右边跨)
	void CalDetailProPara_BR();


	//得到任意里程 X 值时的钢束距离顶板或者底板的距离(本函数为虚函数,T、X、H、D 钢束用基类成员,B、Z钢束会重写成员)
	virtual double getYAtX(double x);
	//得到任意里程x值时的钢束平弯距离(距箱梁中心线)
	double getXAt_p(double x, int& IsAnchor);
	//得到任意里程x值时钢束对应顶板的高程 
	double getTopLev(double x);
	//得到任意里程x值时钢束对应底板的高程 
	double getBottomLev(double x);

	//根据钢束内含钢绞线根数计算【保护层厚度:rType=1 , 钢束最小间距:rType=2 , 波纹管直径:rType=3】
	static double GetParaByBeamct(short count, short calType);


	int m_BarType;           //钢束类型(T,X,H,B,D,Z)
	CString m_Name;          //钢束名称
	int  m_Count;            //每束内含钢绞线根数
	bool m_IsReserveBar;     //是否是备用钢束

	CMainGirder* m_Lgirder;  //左锚固节段所在主梁
	CMainGirder* m_Rgirder;  //右锚固节段所在主梁
	int m_Lseg;              //左锚固节段编号
	int m_Rseg;              //右锚固节段编号
	double m_DisL;           //左锚固节段里程(接缝里程)
	double m_DisR;           //右锚固节段里程(接缝里程) 
	double m_DisLtoR;        //左右锚固节段间的长度(接缝间距离)
	double m_PrjLen;         //钢束投影长度
	CString m_side;          // 所属边跨类型("左"or "右")          


	/********************************  竖弯部分相关参数  ***************************************/

	double m_H1, m_H2;                      //钢束端部距顶板的距离(或者是距底板的距离)
	double m_Delta;                         //钢束平直段距顶板的距离(或者是平直段距底板的距离)

	double m_Aa_S, m_Ba_S, m_Ca_S, m_Da_S;  //钢束特征点(A,B,C,D)偏角
	double m_Ar_S, m_Br_S, m_Cr_S, m_Dr_S;  //钢束特征点(A,B,C,D)半径
	double m_At_S, m_Bt_S, m_Ct_S, m_Dt_S;  //钢束特征点(A,B,C,D)T长
	double m_Ae_S, m_Be_S, m_Ce_S, m_De_S;  //钢束特征点(A,B,C,D)外距
	double m_Al_S, m_Bl_S, m_Cl_S, m_Dl_S;  //钢束特征点(A,B,C,D)处弧长

	double m_X1, m_X2, m_X3, m_X4, m_X5, m_X6, m_X7,m_X8,m_X9;  //钢束竖弯特征点水平分段长度
	double m_Y1, m_Y2, m_Y3, m_Y4, m_Y5, m_Y6;                  //钢束竖弯特征点竖向分段长度  

	double m_LtoleDis;                      //左边距(锚固点到节段接缝的距离)
	double m_RtoleDis;                      //右边距(锚固点到节段接缝的距离)

	double m_LtplateH;                      //左侧齿板高
	double m_RtplateH;                      //右侧齿板高 


	/********************************  平弯部分相关参数  ***************************************/

	double m_Z0, m_Z1, m_Z2, m_Z3, m_Z4, m_Z5, m_Z6, m_Z7, m_Z8;   //钢束平弯特征点竖向分段长度
	double m_XZ1, m_XZ2, m_XR1, m_XR2;                             //钢束平弯特征点水平输入长度
	double m_X1_P, m_X2_P, m_X3_P, m_X4_P, m_X5_P, m_X6_P, m_X7_P; //钢束平弯特征点水平分段长度
	double m_X8_P, m_X9_P, m_X10_P, m_X11_P, m_X12_P, m_X13_P;     //钢束平弯特征点水平分段长度

	bool m_MultiBend;                       //钢束是否多次平弯

	double m_Aa_P, m_Ba_P, m_Ca_P, m_Da_P;  //钢束特征点(A,B,C,D)平弯角
	double m_Ar_P, m_Br_P, m_Cr_P, m_Dr_P;  //钢束特征点(A,B,C,D)平弯半径
	double m_At_P, m_Bt_P, m_Ct_P, m_Dt_P;  //钢束特征点(A,B,C,D)T长
	double m_Ae_P, m_Be_P, m_Ce_P, m_De_P;  //钢束特征点(A,B,C,D)外距
	double m_Al_P, m_Bl_P, m_Cl_P, m_Dl_P;  //钢束特征点(A,B,C,D)处弧长


	static CString  m_LocElevation;         //设计标高位置
	static CString  m_SlpType;              //横坡类型
	static double   m_SlpVal;               //横坡值 左低为负、右低为正
	static double   m_Guardrail;            //护栏宽度
	static double   m_TboxWidth;            //桥宽(箱梁顶宽)
	static double	m_DboxWidth;            //底宽(箱梁底宽)
	static double   m_Pavement;             //铺装厚度

	static double   m_DensityBend;          //钢束弯曲部分的点集密度(米)
	static double   m_DensitynoBend;        //钢束平直部分的点集密度(米)

};


