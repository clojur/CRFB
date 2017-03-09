/* ######################################################################################

                                    ����: Ĭ�ϲ���

									����: ��������ṹ��ƹ����е�Ĭ�ϲ���

									�������¼�����: ·������

########################################################################################*/

#pragma once

#include <YjMath/YjMath.h>
#include "macro.h"

class DEFAULTPARAM_API CDefaultParam : public CObject
{
public:
	/*��������ַ�����ڷ���Χ�ľ���*/
	double m_TwBoundryExceedDis;

	/*�Զ����׿��Ʋ���*/
	int m_AutoLayOutMode;              //�Զ�����ģʽ(������ơ�������ơ�ʩ��ͼ���)
	CYNPoint2D m_ZQMainSpanLimit;      //��������羶��Χ
	CYNPoint2D m_ZQSpanRatioLimit;     //���ű��п��ֵ��Χ
	CYNPoint2D m_YQStandardSpanLimit;  //���ű�׼�羶��Χ
	CYNPoint2D m_YQPierHeightLimit;    //���Ŷո߷�Χ

	/*�ߴ�����ⶨ���Ʋ���*/
	CYNPoint3D m_TGirderRootHeightRatio; //����X��Y��ʾ������T�������п�羶Lz���������h�ı�ֵ��Χ��������T���ԳƲ��ã�������Z��ʾ��T���߿�羶Lb���������h1�ı�ֵ��СT���������߸���2*h-h1��ȷ�������ڴ�СT���ǶԳƲ��õ���������մ�СT��������������ݽṹ������ȷ����
	CYNPoint3D m_TGirderEndHeightRatio;  //����X��Y��ʾ������T�������п�羶L���������h�ı�ֵ��Χ��������T���ԳƲ��õ������������Z��ʾ������T�������п�羶�������С���ߵı�ֵ�����ڴ�СT���ķǶԳ�����������ߡ�2m

	/*��£������*/
	double m_ClosureLen;

	/*��£����С����*/
	double m_ClosureHeightMin;

	/*�ڶλ���*/
	int n1B, n2B, n3B, n4B, n5B, n6B, n7B;//�Զ����ֽڶε�Ԫʱ�ڶε���С��Ŀ
	int n1E, n2E, n3E, n4E, n5E, n6E, n7E;//�Զ����ֽڶε�Ԫʱ�ڶε������Ŀ


public:

	DECLARE_SERIAL(CDefaultParam)

	CDefaultParam();

	~CDefaultParam();

	void Serialize(CArchive& ar);

};



