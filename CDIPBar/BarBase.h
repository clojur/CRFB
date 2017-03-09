#pragma once

// 3D����
class PT3D{
public:
	PT3D(){	x = y = z = 0;}
	~PT3D(){};
	
	PT3D(double x, double y, double z)
	{ this->x = x; this->y = y; this->z = z; }
	PT3D(const PT3D& pt)
	{ this->x = pt.x; this->y = pt.y; this->z = z; }
	void operator=(const PT3D& pt)
	{ this->x = pt.x; this->y = pt.y; this->z = z; }

	double x;
	double y;
	double z;
};


/***************************************************************************************


                                 ����ֽ����


****************************************************************************************/


class CDIP_BAR_API CBarBase : public CObject
{
	DECLARE_SERIAL(CBarBase)
public:
	enum BarType
	{
		RE_BAR,			           //��ͨ�ֽ�
		PRE_BAR,                   //ԤӦ���ֽ�
		STEEL_BAR,		           //ԤӦ���ֽ���
		WIRE_BAR,		           //ԤӦ����˿
		SHAPE_STEEL_BAR,           //�͸�
	};

public:
	CBarBase();
	virtual ~CBarBase();
	virtual void Serialize(CArchive& ar);

	//��øֽ��ܳ���(m)
	double getAlength();
	//��øֽ�������(m)
	double getAweight();

	PT3D m_LocBase;                //�ֽλ����
	vector<PT3D*> m_BarPtarrayReal;//�ֽ����ε㼯(��ʵ)
	vector<PT3D*> m_BarPtarrayFace;//�ֽ����ε㼯(���)

	int   m_SteelClass;            //�ֲ�����
	short m_StretchMode;           //������ʽ(0:������ 2:�������� -1:�㼯�׵�Ϊ������ 1:�㼯ĩ��Ϊ������)

	double m_Modulus;              //����ģ��
	double m_Tstrength;            //����ǿ��
	double m_Uweight;              //����

	double m_Alength;              //�ܳ���
	double m_Aweight;              //������
	double m_WorkLen;              //��������(��:������������)

protected:
	//����Ե㼯����ʵ�ʵ㼯
	void setRealBarPtarray();

	//����ֱ�߽���(������б��ʽ)
	void getInters(double x1, double y1, double x2, double y2, double k1, double k2, double &x, double &y);

};


