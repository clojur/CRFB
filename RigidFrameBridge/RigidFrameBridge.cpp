
#include "stdafx.h"
#include "RigidFrameBridge.h"
#include <osg\ShapeDrawable>
#include <numeric> //accumulate������

IMPLEMENT_SERIAL(CRigidFrameBridge, CObject, 1)

CRigidFrameBridge::CRigidFrameBridge()
{
	_root = new osg::Group;
	_callback = new CRigidFrameBridgeCallback(this);
}

CRigidFrameBridge::CRigidFrameBridge(CString brgName, double CentPos)
{
	m_ZQWidth = 12;
	m_ZQName = brgName;
	m_ZQBaseStake = CentPos;

	_root = new osg::Group;
	_callback = new CRigidFrameBridgeCallback(this);
	for (vector<CMainGirder*>::iterator it = m_MainGirders.begin(); it != m_MainGirders.end(); it++)
		*it = nullptr;
}

CRigidFrameBridge::CRigidFrameBridge(CRigidFrameBridge& brg)
	: m_ZQName(brg.m_ZQName)
	, m_ZQWidth(brg.m_ZQWidth)
	, m_ZQBaseStake(brg.m_ZQBaseStake)
	, m_RoadData(brg.m_RoadData)
	, m_StdSect(brg.m_StdSect)
	, m_BridgeSpanArrange(brg.m_BridgeSpanArrange)
	, m_YQLeft(brg.m_YQLeft)
	, m_YQRight(brg.m_YQRight)
{
	_root = new osg::Group;
	_callback = new CRigidFrameBridgeCallback(this);
}

CRigidFrameBridge::~CRigidFrameBridge()
{
	//ɾ������ָ��vector��ֹ�ڴ�й¶
	for (vector<CMainGirder*>::iterator it = m_MainGirders.begin(); it != m_MainGirders.end(); it++)
		if (nullptr != *it)
		{
			delete *it;
			*it = nullptr;
		}
	m_MainGirders.clear();
}

CRigidFrameBridge& CRigidFrameBridge::operator=(CRigidFrameBridge& src)
{
	return *this;
}

/*���л�*/
void CRigidFrameBridge::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_ZQName << m_ZQID << m_ZQWidth << m_ZQBaseStake << m_YQLayOutResLeft << m_ZQLayOutRes << m_YQLayOutResRight << m_CenterStake;
	}
	else
	{	// loading code
		ar >> m_ZQName >> m_ZQID >> m_ZQWidth >> m_ZQBaseStake >> m_YQLayOutResLeft >> m_ZQLayOutRes >> m_YQLayOutResRight >> m_CenterStake;
	}
	//////////////////////////////////////////////////////////////////////////
	m_DefaultParam.Serialize(ar);
	m_YQLeft.Serialize(ar);
	m_YQRight.Serialize(ar);
	m_StdSect.Serialize(ar);
	//////////////////////////////////////////////////////////////////////////
	if (ar.IsStoring())
	{
		m_RoadData.Serialize(ar);
	}
	else
	{
		m_RoadData.Serialize(ar);
		double outSpace = m_DefaultParam.m_TwBoundryExceedDis;
		m_RoadData.CalcTwBoundry(m_ZQBaseStake, outSpace, m_RoadData.m_TwBoundryLeft, m_RoadData.m_TwBoundryRight);
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned int count = 0;
	if (ar.IsStoring())
	{
		count = m_BridgeSpanArrange.size();
		ar << count;
	}
	else
	{
		ar >> count;
		m_BridgeSpanArrange.resize(count);
	}
	for (unsigned int i = 0; i < count; i++)
	{
		m_BridgeSpanArrange[i].Serialize(ar);
	}
	//////////////////////////////////////////////////////////////////////////
	count = 0;
	if (ar.IsStoring())
	{
		count = m_MainGirders.size();
		ar << count;
		for (unsigned int i = 0; i < count; i++)
		{
			m_MainGirders[i]->Serialize(ar);
		}
	}
	else
	{
		ar >> count;
		for (unsigned int i = 0; i < count; i++)
		{
			CMainGirder* girder = new CMainGirder;
			girder->Serialize(ar);
			girder->SetRoadData(&m_RoadData);
			girder->CalcOutline();
			girder->CreatePier();
			girder->SetSection(&m_StdSect);
			m_MainGirders.push_back(girder);
		}
	}
}


/*�����Զ����׺���*/
void CRigidFrameBridge::PreAutoLayOut(int index)
{
	m_BridgeSpanArrangeAll.clear();

	double m_ZQMainSpanDouble(0);
	double m_ZQSpanRatio = (m_DefaultParam.m_ZQSpanRatioLimit.m_CoordinateX + m_DefaultParam.m_ZQSpanRatioLimit.m_CoordinateY) / 2; //��������С���п��ƽ��ֵ����
	double m_TwBoundry = m_RoadData.m_TwBoundryRight - m_RoadData.m_TwBoundryLeft;//����׮������
	double m_RigidFrameBridgeBoundry = (m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX)*(m_DefaultParam.m_ZQSpanRatioLimit.m_CoordinateX * 2 + 1);//���ż�����Сȫ��

	if (m_TwBoundry < m_RigidFrameBridgeBoundry)
	{
		AfxMessageBox(_T("���棺��ַ�����·������С�������չ�����ȫ��������Сֵ�������Զ����׿���ԭ��������߲����������ͷ�����"));
	}
	else
	{
		double m_YQPierHeght = m_DefaultParam.m_YQPierHeightLimit.m_CoordinateY;//ָ���������ո�
		vector<double> m_Stake = m_RoadData.GetStakeByTwHeight(m_YQPierHeght);
		if (!m_Stake.empty())
		{
			vector<double>::iterator m_iter_begin, m_iter_end; //���������
			m_iter_begin = m_Stake.begin(); //ָ��ʼԪ�صĵ�����
			m_iter_end = m_Stake.end() - 1; //ָ�����Ԫ�صĵ�����
			m_YQLeft.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX;  //���ڼ�С���Ź�ģ���ǣ��������ű�׼�羶�������ֵ����Ϊ�������������ոߵ����ƣ��˴�Ӧȡ��СֵΪ�ˣ��������ų�����Χ(20~40)����ֵ����������Ӱ�췶ΧΪ2*(40-20)/(1+2*0.58)=18.5m���ۺͿ��ǣ�ȡ��Сֵ��
			m_YQRight.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX; //���ڼ�С���Ź�ģ���ǣ��������ű�׼�羶�������ֵ����Ϊ�������������ոߵ����ƣ��˴�Ӧȡ��СֵΪ�ˣ��������ų�����Χ(20~40)����ֵ����������Ӱ�췶ΧΪ(40-20)/(1+2*0.58)=18.5m���ۺͿ��ǣ�ȡ��Сֵ��
			m_ZQStartPt.m_CoordinateX = *m_iter_begin + m_YQLeft.m_CoordinateX;
			m_ZQEndPt.m_CoordinateX = *m_iter_end - m_YQRight.m_CoordinateX;
			double m_ZQBoundary = m_ZQEndPt.m_CoordinateX - m_ZQStartPt.m_CoordinateX; //����ȫ��
			double m_YQBoundaryLeft = m_ZQStartPt.m_CoordinateX - m_RoadData.m_TwBoundryLeft; //������ȫ��
			double m_YQBoundaryRight = m_RoadData.m_TwBoundryRight - m_ZQEndPt.m_CoordinateX; //������ȫ��
			//��ʼ���ף�//
			m_ZQMainSpanNum = 1;
			m_ZQMainSpanDouble = m_ZQBoundary / (m_ZQMainSpanNum + m_ZQSpanRatio * 2);
			if (m_ZQMainSpanDouble < m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX)
			{
				//����ֻ��һ������
				//�ϲ����������׿�����,���в���
				m_ZQBoundary += m_YQLeft.m_CoordinateX + m_YQRight.m_CoordinateX;
				m_YQBoundaryLeft -= m_YQLeft.m_CoordinateX;
				m_YQBoundaryRight -= m_YQRight.m_CoordinateX;
				m_ZQStartPt.m_CoordinateX -= m_YQLeft.m_CoordinateX;
				m_ZQEndPt.m_CoordinateX += m_YQRight.m_CoordinateX;
				m_ZQMainSpanNum = 1;
				m_ZQMainSpanDouble = m_ZQBoundary / (m_ZQMainSpanNum + m_ZQSpanRatio * 2);
				if (m_ZQMainSpanDouble < m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX) //���ϲ����������׿����ź��������绹��С��������Сֵ
				{
					if (m_ZQMainSpanDouble <= m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateY) //������羶С�����ſ羶��Χ���ֵ
					{
						AfxMessageBox(_T("���棺��������羶�����������ţ��������Ų���ͬ���ŵ����Ͳ��ף�"));
					}
					else //������羶�������ſ羶�������ֵ����������羶������Сֵ֮��ʱ
					{
						if (m_YQBoundaryLeft <= m_YQLeft.m_CoordinateX && m_YQBoundaryRight <= m_YQRight.m_CoordinateX) //���������ſ羶��С�ڱ�׼�羶ʱ,�ɶ϶�����������,��ֱ�ӽ���̨
						{
							m_ZQBoundary = m_RoadData.m_TwBoundryRight - m_RoadData.m_TwBoundryLeft; //����ȫ��
							m_ZQMainSpanDouble = m_ZQBoundary / (m_ZQMainSpanNum + m_ZQSpanRatio * 2);
						}
						else //���������ſ羶������һ����ڱ�׼�羶ʱ
						{
							m_ZQStartPt.m_CoordinateX -= (m_RigidFrameBridgeBoundry - m_ZQBoundary) / 2; //���ü�����С�羶����,�޸������������
							m_ZQEndPt.m_CoordinateX += (m_RigidFrameBridgeBoundry - m_ZQBoundary) / 2; //���ü�����С�羶����,�޸������յ�����
							m_ZQBoundary = m_RigidFrameBridgeBoundry; //���ü�����С�羶����
							if (m_ZQStartPt.m_CoordinateX <= m_RoadData.m_TwBoundryLeft && m_ZQEndPt.m_CoordinateX < m_RoadData.m_TwBoundryRight) //���ҽ���������㳬�����ڽ���㷶Χ
							{
								m_ZQStartPt.m_CoordinateX = m_RoadData.m_TwBoundryLeft;
								m_ZQMainSpanDouble = m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX;
								m_ZQEndPt.m_CoordinateX = m_ZQStartPt.m_CoordinateX + m_ZQBoundary;
							}
							else if (m_ZQStartPt.m_CoordinateX > m_RoadData.m_TwBoundryLeft && m_ZQEndPt.m_CoordinateX >= m_RoadData.m_TwBoundryRight) //���ҽ��������յ㳬�����ڽ���㷶Χ
							{
								m_ZQEndPt.m_CoordinateX = m_RoadData.m_TwBoundryRight;
								m_ZQMainSpanDouble = m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX;
								m_ZQStartPt.m_CoordinateX = m_ZQEndPt.m_CoordinateX - m_ZQBoundary;
							}
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////
				CYNPoint2D m_Span(0, 0);
				m_ZQMainSpan = (int)ceil(m_ZQMainSpanDouble); //��������羶����ȡ��
				m_ZQMainSpanNum = m_ZQMainSpanNum; //�������
				m_ZQSideSpanLeft = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //С׮�Ų�߿�羶����ȡ��
				m_ZQSideSpanRight = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //��׮�Ų�߿�羶����ȡ��
				m_Span.m_CoordinateY = m_ZQSideSpanLeft;
				m_Span.m_CoordinateX = m_ZQStartPt.m_CoordinateX + 0.5 * m_Span.m_CoordinateY;
				m_BridgeSpanArrange.push_back(m_Span);
				m_Span.m_CoordinateX = m_ZQStartPt.m_CoordinateX + m_Span.m_CoordinateY;
				for (int i = 0; i < m_ZQMainSpanNum; i++)
				{
					m_Span.m_CoordinateY = m_ZQMainSpan;
					m_Span.m_CoordinateX += 0.5 * m_Span.m_CoordinateY;
					m_BridgeSpanArrange.push_back(m_Span);
					m_Span.m_CoordinateX += 0.5 * m_Span.m_CoordinateY;
				}
				m_Span.m_CoordinateY = m_ZQSideSpanRight;
				m_Span.m_CoordinateX += 0.5 * m_Span.m_CoordinateY;
				m_BridgeSpanArrange.push_back(m_Span);
				m_BridgeSpanArrangeAll.push_back(m_BridgeSpanArrange);
				m_BridgeSpanArrange.clear();
				//////////////////////////////////////////////////////////////////////////
			}
			else
			{
				while (m_ZQMainSpanDouble >= m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX)
				{
					//////////////////////////////////////////////////////////////////////////
					if (m_ZQMainSpanDouble > m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX && m_ZQMainSpanDouble < m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateY)
					{
						CYNPoint2D m_Span(0, 0);
						m_ZQMainSpan = (int)ceil(m_ZQMainSpanDouble); //��������羶����ȡ��
						m_ZQMainSpanNum = m_ZQMainSpanNum; //�������
						m_ZQSideSpanLeft = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //С׮�Ų�߿�羶����ȡ��
						m_ZQSideSpanRight = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //��׮�Ų�߿�羶����ȡ��
						m_Span.m_CoordinateY = m_ZQSideSpanLeft;
						m_Span.m_CoordinateX = m_ZQStartPt.m_CoordinateX + 0.5 * m_Span.m_CoordinateY;
						m_BridgeSpanArrange.push_back(m_Span);
						m_Span.m_CoordinateX = m_ZQStartPt.m_CoordinateX + m_Span.m_CoordinateY;
						for (int i = 0; i < m_ZQMainSpanNum; i++)
						{
							m_Span.m_CoordinateY = m_ZQMainSpan;
							m_Span.m_CoordinateX += 0.5 * m_Span.m_CoordinateY;
							m_BridgeSpanArrange.push_back(m_Span);
							m_Span.m_CoordinateX += 0.5 * m_Span.m_CoordinateY;
						}
						m_Span.m_CoordinateY = m_ZQSideSpanRight;
						m_Span.m_CoordinateX += 0.5 * m_Span.m_CoordinateY;
						m_BridgeSpanArrange.push_back(m_Span);
						m_BridgeSpanArrangeAll.push_back(m_BridgeSpanArrange);
						m_BridgeSpanArrange.clear();
					}
					//////////////////////////////////////////////////////////////////////////
					m_ZQMainSpanNum += 1;
					m_ZQMainSpanDouble = m_ZQBoundary / (m_ZQMainSpanNum + m_ZQSpanRatio * 2);
				}
			}
		}
		else
		{
			AfxMessageBox(_T("����ָ���������ո߳�����ַ�����·�η�Χ�������߶ȣ�"));
		}
	}
	m_BridgeSpanArrange = m_BridgeSpanArrangeAll[index];
	GetBridgeLayOutText();
	m_ZQID = index;
}


/*���Ž������Ų��׺���*/
void CRigidFrameBridge::YQAutoSetSpan(int m_ExceedSpanLimit, int m_StandardSpanInterval)
{
	m_ZQStartPt.m_CoordinateX = m_BridgeSpanArrange[0].m_CoordinateX - 0.5 * m_BridgeSpanArrange[0].m_CoordinateY;
	m_ZQEndPt.m_CoordinateX = m_BridgeSpanArrange.back().m_CoordinateX + 0.5 * m_BridgeSpanArrange.back().m_CoordinateY;
	double m_ExceedSpanLeft(0), m_ExceedSpanRight(0);
	if (m_ZQStartPt.m_CoordinateX > m_RoadData.m_TwBoundryLeft)
	{
		double m_YQBoundaryLeft = m_ZQStartPt.m_CoordinateX - m_RoadData.m_TwBoundryLeft;
		m_YQLeft.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateY; //�����ű�׼�羶
		m_ExceedSpanLeft = m_YQLeft.m_CoordinateX * (int)ceil(m_YQBoundaryLeft / m_YQLeft.m_CoordinateX) - m_YQBoundaryLeft;
		while (m_ExceedSpanLeft > m_ExceedSpanLimit && (m_YQLeft.m_CoordinateX - m_StandardSpanInterval) >= m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX)
		{
			m_YQLeft.m_CoordinateX = m_YQLeft.m_CoordinateX - m_StandardSpanInterval;
			m_ExceedSpanLeft = m_YQLeft.m_CoordinateX * (int)ceil(m_YQBoundaryLeft / m_YQLeft.m_CoordinateX) - m_YQBoundaryLeft;
		}
		m_YQLeft.m_CoordinateY = (int)ceil(m_YQBoundaryLeft / m_YQLeft.m_CoordinateX); //�����ű�׼����
	}
	if (m_ZQEndPt.m_CoordinateX < m_RoadData.m_TwBoundryRight)
	{
		double m_YQBoundaryRight = m_RoadData.m_TwBoundryRight - m_ZQEndPt.m_CoordinateX;
		m_YQRight.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateY; //�����ű�׼�羶
		m_ExceedSpanRight = m_YQLeft.m_CoordinateX * (int)ceil(m_YQBoundaryRight / m_YQRight.m_CoordinateX) - m_YQBoundaryRight;
		while (m_ExceedSpanRight > m_ExceedSpanLimit && (m_YQRight.m_CoordinateX - m_StandardSpanInterval) >= m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX)
		{
			m_YQRight.m_CoordinateX = m_YQRight.m_CoordinateX - m_StandardSpanInterval;
			m_ExceedSpanRight = m_YQRight.m_CoordinateX * (int)ceil(m_YQBoundaryRight / m_YQRight.m_CoordinateX) - m_YQBoundaryRight;
		}
		m_YQRight.m_CoordinateY = (int)ceil(m_YQBoundaryRight / m_YQRight.m_CoordinateX); //�����ű�׼����
	}
	if (m_ZQStartPt.m_CoordinateX <= m_RoadData.m_TwBoundryLeft)
	{
		m_YQLeft = CYNPoint2D(0, 1);
	}
	if (m_ZQEndPt.m_CoordinateX >= m_RoadData.m_TwBoundryRight)
	{
		m_YQRight = CYNPoint2D(0, 1);
	}
	CString m_txt = _T("");
	m_txt.Format(_T("%.0fx%.1f"), m_YQLeft.m_CoordinateY, m_YQLeft.m_CoordinateX);
	m_YQLayOutResLeft = m_txt;
	m_txt.Format(_T("%.0fx%.1f"), m_YQRight.m_CoordinateY, m_YQRight.m_CoordinateX);
	m_YQLayOutResRight = m_txt;
}

/*��Ϊָ�����׷���*/
void CRigidFrameBridge::SetBridgeSpan(CString m_YQLeftSpanText, CString m_ZQSpanText, CString m_YQSpanRightText, double m_BridgeCenterStake)
{
	vector<double> m_YQLeftSpanVec, m_ZQSpanVec, m_YQRightSpanVec;
	m_YQLeftSpanVec = ResolveStrToSpan(m_YQLeftSpanText);
	m_ZQSpanVec = ResolveStrToSpan(m_ZQSpanText);
	m_YQRightSpanVec = ResolveStrToSpan(m_YQSpanRightText);
	double YQLeftAllLen = accumulate(m_YQLeftSpanVec.begin(), m_YQLeftSpanVec.end(), 0);
	double ZQAllLen = accumulate(m_ZQSpanVec.begin(), m_ZQSpanVec.end(), 0);
	double YQRightAllLen = accumulate(m_YQRightSpanVec.begin(), m_YQRightSpanVec.end(), 0);
	m_BridgeSpanArrange.clear();
	CYNPoint2D m_SpanPt(0, 0);
	m_SpanPt.m_CoordinateX = m_BridgeCenterStake - ZQAllLen / 2 + m_ZQSpanVec.at(0) / 2;
	m_SpanPt.m_CoordinateY = m_ZQSpanVec.at(0);
	m_BridgeSpanArrange.push_back(m_SpanPt);
	unsigned int num = m_ZQSpanVec.size();
	for (unsigned int i = 1; i < num; i++)
	{
		m_SpanPt.m_CoordinateX += m_ZQSpanVec.at(i - 1) / 2 + m_ZQSpanVec.at(i) / 2;
		m_SpanPt.m_CoordinateY = m_ZQSpanVec.at(i);
		m_BridgeSpanArrange.push_back(m_SpanPt);
	}
	m_YQLeft.m_CoordinateX = m_YQLeftSpanVec.at(0);
	m_YQLeft.m_CoordinateY = YQLeftAllLen / m_YQLeft.m_CoordinateX;
	m_YQRight.m_CoordinateX = m_YQRightSpanVec.at(0);
	m_YQRight.m_CoordinateY = YQRightAllLen / m_YQRight.m_CoordinateX;
}

/*�����׿粼���ַ���*/
vector<double> CRigidFrameBridge::ResolveStrToSpan(CString str)
{
	CString tmpSTr;
	int smallNum;
	double smallLen;
	vector<double> m_Span;
	while (true)
	{
		if (LSplit(str, '+') == _T("ERROR"))
		{
			if (LSplit(str, 'x') == _T("ERROR"))
			{
				smallNum = 1;
				smallLen = _wtof(str);
			}
			else
			{
				smallNum = _wtoi(LSplit(str, 'x'));
				smallLen = _wtof(RSplit(str, 'x'));
			}
			for (int i = 0; i < smallNum; i++)
			{
				m_Span.push_back(smallLen);
			}
			break;
		}
		else
		{
			tmpSTr = LSplit(str, '+');
			if (LSplit(tmpSTr, 'x') == _T("ERROR"))
			{
				smallNum = 1;
				smallLen = _wtof(tmpSTr);
			}
			else
			{
				smallNum = _wtoi(LSplit(tmpSTr, 'x'));
				smallLen = _wtof(RSplit(tmpSTr, 'x'));
			}
			for (int i = 0; i < smallNum; i++)
			{
				m_Span.push_back(smallLen);
			}
			str = RSplit(str, '+');
		}
	}
	return m_Span;
}

/*��ȡ�ָ�����ߵ��ַ���*/
CString CRigidFrameBridge::LSplit(CString str, TCHAR chr)
{
	int pos = str.Find(chr);
	if (pos != -1)
		return str.Left(pos);
	else
		return _T("ERROR");
}

/*��ȡ�ָ����ұߵ��ַ���*/
CString CRigidFrameBridge::RSplit(CString str, TCHAR chr)
{
	int pos = str.Find(chr);
	if (pos != -1)
		return str.Right(str.GetLength() - pos - 1);
	else
		return _T("ERROR");
}

/*��ȡ���Ų����ַ���*/
void CRigidFrameBridge::GetBridgeLayOutText()
{
	CString m_txt = _T("");
	CString txt = _T("");
	int num = (int)(m_BridgeSpanArrange.size()) - 1;
	for (int i = 0; i < num; i++)
	{
		txt.Format(_T("%.1f+"), m_BridgeSpanArrange.at(i).m_CoordinateY);
		m_txt += txt;
	}
	txt.Format(_T("%.1f"), m_BridgeSpanArrange.back().m_CoordinateY);
	m_txt += txt;
	m_ZQLayOutRes = m_txt;
	CString txtCenter = _T("");
	txtCenter.Format(_T("%.3f"), (m_BridgeSpanArrange.at(0).m_CoordinateX - m_BridgeSpanArrange.at(0).m_CoordinateY / 2 + m_BridgeSpanArrange.back().m_CoordinateX + m_BridgeSpanArrange.back().m_CoordinateY / 2) / 2);
	m_CenterStake = txtCenter;
}

/*�������������ⶨ*/
void CRigidFrameBridge::PreSetGirderLength(int m_TIndex, double m_TGirderLen)  //�ú�����ָ��ĳ��T��������������������������βα�T����ţ���1��ʼ����T��������
{
	unsigned int num_1 = m_MainGirders.size();
	int num_2 = (int)(m_BridgeSpanArrange.size());

	if (num_1 >= 2 * 2 + 3) //���缰���������չ���
	{
		/*T����������*/
		if (m_TIndex > 1 && m_TIndex < num_2 - 1) //��ָ���м�T������ʱ����������T������
		{
			m_MainGirders.at(m_TIndex * 2)->m_AllLen = m_TGirderLen;
			for (int i = 1; i < m_TIndex; i++)
			{
				m_MainGirders.at((m_TIndex - i) * 2)->m_AllLen = 2 * (m_BridgeSpanArrange.at(m_TIndex - i - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((m_TIndex - i + 1) * 2)->m_AllLen) / 2);
			}
			for (int i = m_TIndex + 1; i < num_2; i++)
			{
				m_MainGirders.at(i * 2)->m_AllLen = 2 * (m_BridgeSpanArrange.at(i - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((i - 1) * 2)->m_AllLen) / 2);
			}
		}
		if (m_TIndex == 1) //��ָ��С׮�Ų�T������ʱ����������T������
		{
			m_MainGirders.at(m_TIndex * 2)->m_AllLen = m_TGirderLen;
			for (int i = m_TIndex + 1; i < num_2; i++)
			{
				m_MainGirders.at(i * 2)->m_AllLen = 2 * (m_BridgeSpanArrange.at(i - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((i - 1) * 2)->m_AllLen) / 2);
			}
		}
		if (m_TIndex == num_2 - 1) //��ָ����׮�Ų�T������ʱ����������T������
		{
			m_MainGirders.at(m_TIndex * 2)->m_AllLen = m_TGirderLen;
			for (int i = 1; i < num_2 - 1; i++)
			{
				m_MainGirders.at((m_TIndex - i) * 2)->m_AllLen = 2 * (m_BridgeSpanArrange.at(m_TIndex - i - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((m_TIndex - i + 1) * 2)->m_AllLen) / 2);
			}
		}
		for (int i = 0; i < num_2; i++) //��£������
		{
			m_MainGirders.at(i * 2 + 1)->m_AllLen = m_DefaultParam.m_ClosureLen;
		}
		m_MainGirders.at(0)->m_AllLen = m_BridgeSpanArrange.at(0).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at(2)->m_AllLen) / 2; //С׮�Ų��ֽ�������
		m_MainGirders.at(num_2 * 2)->m_AllLen = m_BridgeSpanArrange.at(num_2 - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((num_2 - 1) * 2)->m_AllLen) / 2; //��׮�Ų��ֽ�������
	}
	else  //��T��
	{
		AfxMessageBox(_T("���棺�ſ�����С��3�����������չ������ſ׿粼�÷�����"));
		m_TIndex = 1;
		m_MainGirders.at(m_TIndex * 2)->m_AllLen = m_TGirderLen;
		m_MainGirders.at(m_TIndex * 2 - 1)->m_AllLen = m_DefaultParam.m_ClosureLen;
		m_MainGirders.at(m_TIndex * 2 - 2)->m_AllLen = m_BridgeSpanArrange.at(m_TIndex - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at(m_TIndex * 2)->m_AllLen) / 2;
		m_MainGirders.at(m_TIndex * 2 + 1)->m_AllLen = m_DefaultParam.m_ClosureLen;
		m_MainGirders.at(m_TIndex * 2 + 2)->m_AllLen = m_BridgeSpanArrange.at(m_TIndex).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at(m_TIndex * 2)->m_AllLen) / 2;
	}
	//����������㡢�е���յ�׮��
	m_MainGirders.at(0)->m_Left = m_BridgeSpanArrange.at(0).m_CoordinateX - m_BridgeSpanArrange.at(0).m_CoordinateY / 2;
	m_MainGirders.at(0)->m_Center = m_MainGirders.at(0)->m_Left + m_MainGirders.at(0)->m_AllLen / 2;
	m_MainGirders.at(0)->m_Right = m_MainGirders.at(0)->m_Left + m_MainGirders.at(0)->m_AllLen;
	for (unsigned int i = 1; i < num_1; i++)
	{
		m_MainGirders.at(i)->m_Left = m_MainGirders.at(i - 1)->m_Left + m_MainGirders.at(i - 1)->m_AllLen;
		m_MainGirders.at(i)->m_Center = m_MainGirders.at(i)->m_Left + m_MainGirders.at(i)->m_AllLen / 2;
		m_MainGirders.at(i)->m_Right = m_MainGirders.at(i)->m_Left + m_MainGirders.at(i)->m_AllLen;
	}
	//�����������߳���
	for (int i = 0; i < num_2; i++)
	{
		m_MainGirders.at((i + 1) * 2)->m_CurveLen = (m_MainGirders.at((i + 1) * 2)->m_AllLen - m_MainGirders.at((i + 1) * 2)->m_BottomLen) / 2;
	}
}

/*�����߶ȳ����ⶨ*/
void CRigidFrameBridge::PreSetGirderHeight()
{
	int num = (int)(m_MainGirders.size());
	if (num >= 2 * 2 + 3) //���缰���������չ��������߶ȳ���
	{
		double m_TolesHeightSum(0);
		for (int i = 2; i < num - 4; i += 2)
		{
			if (m_MainGirders.at(i)->m_AllLen != m_MainGirders.at(i + 2)->m_AllLen)
			{
				m_MainGirders.at(i)->m_TolesHeight = ((m_MainGirders.at(i)->m_AllLen + m_MainGirders.at(i + 2)->m_AllLen) / 2) / m_DefaultParam.m_TGirderEndHeightRatio.m_CoordinateZ; //
				if (m_MainGirders.at(i)->m_TolesHeight <= m_DefaultParam.m_ClosureHeightMin)
				{
					m_MainGirders.at(i)->m_TolesHeight = m_DefaultParam.m_ClosureHeightMin;
				}
			}
			else
			{
				m_MainGirders.at(i)->m_TolesHeight = m_MainGirders.at(i)->m_AllLen / ((m_DefaultParam.m_TGirderEndHeightRatio.m_CoordinateX + m_DefaultParam.m_TGirderEndHeightRatio.m_CoordinateY) / 2); //
				if (m_MainGirders.at(i)->m_TolesHeight <= m_DefaultParam.m_ClosureHeightMin)
				{
					m_MainGirders.at(i)->m_TolesHeight = m_DefaultParam.m_ClosureHeightMin;
				}
			}
			m_TolesHeightSum += m_MainGirders.at(i)->m_TolesHeight;
			m_MainGirders.at(i)->m_RootHeight = ceil(10 * (m_MainGirders.at(i)->m_AllLen / ((m_DefaultParam.m_TGirderRootHeightRatio.m_CoordinateX + m_MainGirders.at(i)->m_AllLen / m_DefaultParam.m_TGirderRootHeightRatio.m_CoordinateY) / 2))) / 10;
			m_MainGirders.at(i + 2)->m_RootHeight = ceil(10 * (m_MainGirders.at(i + 2)->m_AllLen / ((m_DefaultParam.m_TGirderRootHeightRatio.m_CoordinateX + m_MainGirders.at(i)->m_AllLen / m_DefaultParam.m_TGirderRootHeightRatio.m_CoordinateY) / 2))) / 10;
		}
		for (int i = 2; i < num - 2; i += 2)
		{
			m_MainGirders.at(i)->m_TolesHeight = ceil(10 * (m_TolesHeightSum / ((num - 3) / 2 - 1))) / 10;
			m_MainGirders.at(i - 1)->m_TolesHeight = m_MainGirders.at(i)->m_TolesHeight;
			m_MainGirders.at(i - 1)->m_RootHeight = m_MainGirders.at(i - 1)->m_TolesHeight;
		}
		m_MainGirders.at(0)->m_TolesHeight = m_MainGirders.at(1)->m_RootHeight;
		m_MainGirders.at(0)->m_RootHeight = m_MainGirders.at(0)->m_TolesHeight;
		m_MainGirders.back()->m_TolesHeight = m_MainGirders.at(num - 3)->m_TolesHeight;
		m_MainGirders.back()->m_RootHeight = m_MainGirders.back()->m_TolesHeight;
		m_MainGirders.at(num - 2)->m_TolesHeight = m_MainGirders.back()->m_TolesHeight;
		m_MainGirders.at(num - 2)->m_RootHeight = m_MainGirders.at(num - 2)->m_TolesHeight;
	}
	else
	{
		AfxMessageBox(_T("���棺�ſ�����С��3�����������չ������ſ׿粼�÷�����"));
		for (int i = 0; i < num; i++)
		{
			m_MainGirders.at(i)->m_TolesHeight = m_BridgeSpanArrange.at(0).m_CoordinateY / 33;
			m_MainGirders.at(i)->m_RootHeight = m_MainGirders.at(i)->m_TolesHeight;
			if (m_MainGirders.at(i)->m_TolesHeight <= m_DefaultParam.m_ClosureHeightMin)
			{
				m_MainGirders.at(i)->m_TolesHeight = m_DefaultParam.m_ClosureHeightMin;
			}
		}
		m_MainGirders.at(2)->m_RootHeight = m_BridgeSpanArrange.at(0).m_CoordinateY / 10;

	}
}

void CRigidFrameBridge::CreateMainGirder()
{
	double start = 0.0;         //������ʼ�����ʼֵ
	double tLen = 100;          //������ʼֵ
	double leftCastLen = 7.5;   //С׮�Ų�߿��ֽ���������ʼֵ
	double rightCastLen = 7.5;  //��׮�Ų�߿��ֽ���������ʼֵ
	double rootHeight = 10;     //���������߶ȳ�ʼֵ
	double tolesHeight = 2;     //�����˲��߶ȳ�ʼֵ
	double bottomLen = 10;      //����ֱ�߶γ���ʼֵ
	double remainLen = 0.5;     //���۸�����ʣ�೤�ȳ�ʼֵ
	double rootThick = 1.2;     //���������װ���ʼֵ
	double tolesThick = 0.3;    //�����˲��װ���ʼֵ
	double heightRatio = 1.6;   //�����������ݴη���ʼֵ
	double thickRatio = 1.6;    //���׺���ݴη���ʼֵ
	CString m_TGirderId = _T("");

	for (std::vector<CMainGirder*>::iterator it = m_MainGirders.begin(); it != m_MainGirders.end(); it++)
	{
		delete *it;
		*it = nullptr;
	}
	m_MainGirders.clear();

	start = m_BridgeSpanArrange[0].m_CoordinateX - 0.5 * m_BridgeSpanArrange[0].m_CoordinateY;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_SPAN, _T("��߿��ֽ���"), start, leftCastLen, tolesHeight, rootThick));

	start += leftCastLen;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_CLOSURE, _T("��߿��£��"), start, m_DefaultParam.m_ClosureLen, tolesHeight, rootThick));

	start += m_DefaultParam.m_ClosureLen;
	m_TGirderId = _T("1#T��");
	m_MainGirders.push_back(new CMainGirder(CMainGirder::T_CONSTRUCTION, m_TGirderId, start, tLen, rootHeight, remainLen, rootHeight, tolesHeight, rootThick, tolesThick, heightRatio, thickRatio));

	int num = (int)(m_BridgeSpanArrange.size());
	for (int i = 0; i < num - 2; i++)
	{
		start += tLen;
		m_TGirderId.Format(_T("%d#�п��£��"), i + 1);
		m_MainGirders.push_back(new CMainGirder(CMainGirder::MID_CLOSURE, m_TGirderId, start, m_DefaultParam.m_ClosureLen, tolesHeight, rootThick));

		start += m_DefaultParam.m_ClosureLen;
		m_TGirderId.Format(_T("%d#T��"), i + 2);
		m_MainGirders.push_back(new CMainGirder(CMainGirder::T_CONSTRUCTION, m_TGirderId, start, tLen, rootHeight, remainLen, rootHeight, tolesHeight, rootThick, tolesThick, heightRatio, thickRatio));
	}

	start += tLen;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_CLOSURE, _T("�ұ߿��£��"), start, m_DefaultParam.m_ClosureLen, tolesHeight, rootThick));

	start += m_DefaultParam.m_ClosureLen;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_SPAN, _T("�ұ߿��ֽ���"), start, rightCastLen, tolesHeight, rootThick));

	PreSetGirderLength(1, m_BridgeSpanArrange[1].m_CoordinateY - m_DefaultParam.m_ClosureLen);
	PreSetGirderHeight();

	unsigned int num_2 = m_MainGirders.size();
	for (unsigned int i = 0; i < num_2; i++)
	{
		m_MainGirders[i]->SetRoadData(&m_RoadData);
		m_MainGirders[i]->CalcOutline();
		m_MainGirders[i]->CreatePier();
		m_MainGirders[i]->SetSection(&m_StdSect);
	}
}

double CRigidFrameBridge::GetBrgAllLen()
{
	vector<CYNPoint2D>::iterator  m_iter_begin, m_iter_end; //���������
	m_iter_begin = m_BridgeSpanArrange.begin();
	m_iter_end = m_BridgeSpanArrange.end();
	double sum = 0;
	for (vector<CYNPoint2D>::iterator i = m_iter_begin; i != m_iter_end; i++)
	{
		sum += i->m_CoordinateY;
	}
	CString &r_hplx = m_StdSect.m_SlpType;
	return sum;
}

/*���������������*/
bool CRigidFrameBridge::GetSideMainGirder(CMainGirder* src, CMainGirder* &r_LeftOne, CMainGirder* &r_RightOne)
{
	double cent = 0;
	for (vector<CMainGirder*>::iterator i = m_MainGirders.begin(); i != m_MainGirders.end(); ++i)
	{
		cent = (*i)->m_Center;
		if (cent == src->m_Center)
		{
			if (i == m_MainGirders.end())
			{
				i--;
				r_LeftOne = *i;
				r_RightOne = nullptr;
				return true;
			}
			if (i == m_MainGirders.begin())
			{
				i++;
				r_LeftOne = nullptr;
				r_RightOne = *i;
				return true;
			}
			i--;
			r_LeftOne = *i;
			i++;
			i++;
			r_RightOne = *i;
			return true;
		}
	}
	return false;
}



void CRigidFrameBridgeCallback::DoUpdate()
{
	osg::ref_ptr<osg::Group> road = _brg->m_RoadData.getUpdateCallback()->getRoot();
	_root->removeChildren(0, _root->getNumChildren());

	_root->addChild(_brg->m_RoadData.getUpdateCallback()->getRoot());
	_brg->m_RoadData.getUpdateCallback()->setView(getView());

	for (std::vector<CMainGirder*>::iterator it = _brg->m_MainGirders.begin();
		it != _brg->m_MainGirders.end(); it++)
	{
		_root->addChild((*it)->getUpdateCallback()->getRoot());
		(*it)->getUpdateCallback()->setView(getView());
	}
}