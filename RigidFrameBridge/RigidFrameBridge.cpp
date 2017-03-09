
#include "stdafx.h"
#include "RigidFrameBridge.h"
#include <osg\ShapeDrawable>
#include <numeric> //accumulate函数用

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
	//删除主梁指针vector防止内存泄露
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

/*序列化*/
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


/*主桥自动布孔函数*/
void CRigidFrameBridge::PreAutoLayOut(int index)
{
	m_BridgeSpanArrangeAll.clear();

	double m_ZQMainSpanDouble(0);
	double m_ZQSpanRatio = (m_DefaultParam.m_ZQSpanRatioLimit.m_CoordinateX + m_DefaultParam.m_ZQSpanRatioLimit.m_CoordinateY) / 2; //以最大和最小边中跨比平均值布孔
	double m_TwBoundry = m_RoadData.m_TwBoundryRight - m_RoadData.m_TwBoundryLeft;//填挖桩号区间
	double m_RigidFrameBridgeBoundry = (m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX)*(m_DefaultParam.m_ZQSpanRatioLimit.m_CoordinateX * 2 + 1);//主桥极限最小全长

	if (m_TwBoundry < m_RigidFrameBridgeBoundry)
	{
		AfxMessageBox(_T("警告：桥址所处填方路段区间小于连续刚构桥梁全长允许最小值，请检查自动布孔控制原则参数或者采用其他桥型方案！"));
	}
	else
	{
		double m_YQPierHeght = m_DefaultParam.m_YQPierHeightLimit.m_CoordinateY;//指定引桥最大墩高
		vector<double> m_Stake = m_RoadData.GetStakeByTwHeight(m_YQPierHeght);
		if (!m_Stake.empty())
		{
			vector<double>::iterator m_iter_begin, m_iter_end; //随机迭代器
			m_iter_begin = m_Stake.begin(); //指向开始元素的迭代器
			m_iter_end = m_Stake.end() - 1; //指向最后元素的迭代器
			m_YQLeft.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX;  //出于减小主桥规模考虑，宜用引桥标准跨径允许最大值；但为了满足引桥最大墩高的限制，此处应取最小值为宜，对于引桥常见范围(20~40)，该值对最大主跨的影响范围为2*(40-20)/(1+2*0.58)=18.5m，综和考虑，取最小值；
			m_YQRight.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX; //出于减小主桥规模考虑，宜用引桥标准跨径允许最大值；但为了满足引桥最大墩高的限制，此处应取最小值为宜，对于引桥常见范围(20~40)，该值对最大主跨的影响范围为(40-20)/(1+2*0.58)=18.5m，综和考虑，取最小值；
			m_ZQStartPt.m_CoordinateX = *m_iter_begin + m_YQLeft.m_CoordinateX;
			m_ZQEndPt.m_CoordinateX = *m_iter_end - m_YQRight.m_CoordinateX;
			double m_ZQBoundary = m_ZQEndPt.m_CoordinateX - m_ZQStartPt.m_CoordinateX; //主桥全长
			double m_YQBoundaryLeft = m_ZQStartPt.m_CoordinateX - m_RoadData.m_TwBoundryLeft; //左引桥全长
			double m_YQBoundaryRight = m_RoadData.m_TwBoundryRight - m_ZQEndPt.m_CoordinateX; //右引桥全长
			//开始布孔！//
			m_ZQMainSpanNum = 1;
			m_ZQMainSpanDouble = m_ZQBoundary / (m_ZQMainSpanNum + m_ZQSpanRatio * 2);
			if (m_ZQMainSpanDouble < m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX)
			{
				//以下只有一个主跨
				//合并主桥两侧首孔引桥,进行布孔
				m_ZQBoundary += m_YQLeft.m_CoordinateX + m_YQRight.m_CoordinateX;
				m_YQBoundaryLeft -= m_YQLeft.m_CoordinateX;
				m_YQBoundaryRight -= m_YQRight.m_CoordinateX;
				m_ZQStartPt.m_CoordinateX -= m_YQLeft.m_CoordinateX;
				m_ZQEndPt.m_CoordinateX += m_YQRight.m_CoordinateX;
				m_ZQMainSpanNum = 1;
				m_ZQMainSpanDouble = m_ZQBoundary / (m_ZQMainSpanNum + m_ZQSpanRatio * 2);
				if (m_ZQMainSpanDouble < m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX) //若合并主桥两侧首孔引桥后，主桥主跨还是小于允许最小值
				{
					if (m_ZQMainSpanDouble <= m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateY) //当主跨跨径小于引桥跨径范围最大值
					{
						AfxMessageBox(_T("警告：主桥主跨跨径已适用于引桥，建议主桥采用同引桥的桥型布孔！"));
					}
					else //当主跨跨径处于引桥跨径允许最大值与主桥主跨跨径允许最小值之间时
					{
						if (m_YQBoundaryLeft <= m_YQLeft.m_CoordinateX && m_YQBoundaryRight <= m_YQRight.m_CoordinateX) //当两侧引桥跨径均小于标准跨径时,可断定不设置引桥,而直接接桥台
						{
							m_ZQBoundary = m_RoadData.m_TwBoundryRight - m_RoadData.m_TwBoundryLeft; //主桥全长
							m_ZQMainSpanDouble = m_ZQBoundary / (m_ZQMainSpanNum + m_ZQSpanRatio * 2);
						}
						else //当两侧引桥跨径至少有一侧大于标准跨径时
						{
							m_ZQStartPt.m_CoordinateX -= (m_RigidFrameBridgeBoundry - m_ZQBoundary) / 2; //采用极限最小跨径布孔,修改主桥起点坐标
							m_ZQEndPt.m_CoordinateX += (m_RigidFrameBridgeBoundry - m_ZQBoundary) / 2; //采用极限最小跨径布孔,修改主桥终点坐标
							m_ZQBoundary = m_RigidFrameBridgeBoundry; //采用极限最小跨径布孔
							if (m_ZQStartPt.m_CoordinateX <= m_RoadData.m_TwBoundryLeft && m_ZQEndPt.m_CoordinateX < m_RoadData.m_TwBoundryRight) //当且仅当主桥起点超过填挖交界点范围
							{
								m_ZQStartPt.m_CoordinateX = m_RoadData.m_TwBoundryLeft;
								m_ZQMainSpanDouble = m_DefaultParam.m_ZQMainSpanLimit.m_CoordinateX;
								m_ZQEndPt.m_CoordinateX = m_ZQStartPt.m_CoordinateX + m_ZQBoundary;
							}
							else if (m_ZQStartPt.m_CoordinateX > m_RoadData.m_TwBoundryLeft && m_ZQEndPt.m_CoordinateX >= m_RoadData.m_TwBoundryRight) //当且仅当主桥终点超过填挖交界点范围
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
				m_ZQMainSpan = (int)ceil(m_ZQMainSpanDouble); //主桥主跨跨径向上取整
				m_ZQMainSpanNum = m_ZQMainSpanNum; //主跨跨数
				m_ZQSideSpanLeft = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //小桩号侧边跨跨径向上取整
				m_ZQSideSpanRight = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //大桩号侧边跨跨径向上取整
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
						m_ZQMainSpan = (int)ceil(m_ZQMainSpanDouble); //主桥主跨跨径向上取整
						m_ZQMainSpanNum = m_ZQMainSpanNum; //主跨跨数
						m_ZQSideSpanLeft = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //小桩号侧边跨跨径向上取整
						m_ZQSideSpanRight = (int)ceil(m_ZQMainSpan * m_ZQSpanRatio); //大桩号侧边跨跨径向上取整
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
			AfxMessageBox(_T("错误：指定引桥最大墩高超出桥址所在填方路段范围内最大填方高度！"));
		}
	}
	m_BridgeSpanArrange = m_BridgeSpanArrangeAll[index];
	GetBridgeLayOutText();
	m_ZQID = index;
}


/*主桥接线引桥布孔函数*/
void CRigidFrameBridge::YQAutoSetSpan(int m_ExceedSpanLimit, int m_StandardSpanInterval)
{
	m_ZQStartPt.m_CoordinateX = m_BridgeSpanArrange[0].m_CoordinateX - 0.5 * m_BridgeSpanArrange[0].m_CoordinateY;
	m_ZQEndPt.m_CoordinateX = m_BridgeSpanArrange.back().m_CoordinateX + 0.5 * m_BridgeSpanArrange.back().m_CoordinateY;
	double m_ExceedSpanLeft(0), m_ExceedSpanRight(0);
	if (m_ZQStartPt.m_CoordinateX > m_RoadData.m_TwBoundryLeft)
	{
		double m_YQBoundaryLeft = m_ZQStartPt.m_CoordinateX - m_RoadData.m_TwBoundryLeft;
		m_YQLeft.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateY; //左引桥标准跨径
		m_ExceedSpanLeft = m_YQLeft.m_CoordinateX * (int)ceil(m_YQBoundaryLeft / m_YQLeft.m_CoordinateX) - m_YQBoundaryLeft;
		while (m_ExceedSpanLeft > m_ExceedSpanLimit && (m_YQLeft.m_CoordinateX - m_StandardSpanInterval) >= m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX)
		{
			m_YQLeft.m_CoordinateX = m_YQLeft.m_CoordinateX - m_StandardSpanInterval;
			m_ExceedSpanLeft = m_YQLeft.m_CoordinateX * (int)ceil(m_YQBoundaryLeft / m_YQLeft.m_CoordinateX) - m_YQBoundaryLeft;
		}
		m_YQLeft.m_CoordinateY = (int)ceil(m_YQBoundaryLeft / m_YQLeft.m_CoordinateX); //左引桥标准跨数
	}
	if (m_ZQEndPt.m_CoordinateX < m_RoadData.m_TwBoundryRight)
	{
		double m_YQBoundaryRight = m_RoadData.m_TwBoundryRight - m_ZQEndPt.m_CoordinateX;
		m_YQRight.m_CoordinateX = m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateY; //右引桥标准跨径
		m_ExceedSpanRight = m_YQLeft.m_CoordinateX * (int)ceil(m_YQBoundaryRight / m_YQRight.m_CoordinateX) - m_YQBoundaryRight;
		while (m_ExceedSpanRight > m_ExceedSpanLimit && (m_YQRight.m_CoordinateX - m_StandardSpanInterval) >= m_DefaultParam.m_YQStandardSpanLimit.m_CoordinateX)
		{
			m_YQRight.m_CoordinateX = m_YQRight.m_CoordinateX - m_StandardSpanInterval;
			m_ExceedSpanRight = m_YQRight.m_CoordinateX * (int)ceil(m_YQBoundaryRight / m_YQRight.m_CoordinateX) - m_YQBoundaryRight;
		}
		m_YQRight.m_CoordinateY = (int)ceil(m_YQBoundaryRight / m_YQRight.m_CoordinateX); //右引桥标准跨数
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

/*人为指定布孔方案*/
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

/*解析孔跨布置字符串*/
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

/*获取分隔符左边的字符串*/
CString CRigidFrameBridge::LSplit(CString str, TCHAR chr)
{
	int pos = str.Find(chr);
	if (pos != -1)
		return str.Left(pos);
	else
		return _T("ERROR");
}

/*获取分隔符右边的字符串*/
CString CRigidFrameBridge::RSplit(CString str, TCHAR chr)
{
	int pos = str.Find(chr);
	if (pos != -1)
		return str.Right(str.GetLength() - pos - 1);
	else
		return _T("ERROR");
}

/*获取主桥布孔字符串*/
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

/*主梁梁长初步拟定*/
void CRigidFrameBridge::PreSetGirderLength(int m_TIndex, double m_TGirderLen)  //该函数需指定某个T构的梁长来推算出其他梁长，形参表【T构编号（从1开始），T构梁长】
{
	unsigned int num_1 = m_MainGirders.size();
	int num_2 = (int)(m_BridgeSpanArrange.size());

	if (num_1 >= 2 * 2 + 3) //三跨及以上连续刚构桥
	{
		/*T构梁长计算*/
		if (m_TIndex > 1 && m_TIndex < num_2 - 1) //当指定中间T构梁长时，计算其余T构梁长
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
		if (m_TIndex == 1) //当指定小桩号侧T构梁长时，计算其余T构梁长
		{
			m_MainGirders.at(m_TIndex * 2)->m_AllLen = m_TGirderLen;
			for (int i = m_TIndex + 1; i < num_2; i++)
			{
				m_MainGirders.at(i * 2)->m_AllLen = 2 * (m_BridgeSpanArrange.at(i - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((i - 1) * 2)->m_AllLen) / 2);
			}
		}
		if (m_TIndex == num_2 - 1) //当指定大桩号侧T构梁长时，计算其余T构梁长
		{
			m_MainGirders.at(m_TIndex * 2)->m_AllLen = m_TGirderLen;
			for (int i = 1; i < num_2 - 1; i++)
			{
				m_MainGirders.at((m_TIndex - i) * 2)->m_AllLen = 2 * (m_BridgeSpanArrange.at(m_TIndex - i - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((m_TIndex - i + 1) * 2)->m_AllLen) / 2);
			}
		}
		for (int i = 0; i < num_2; i++) //合拢段梁长
		{
			m_MainGirders.at(i * 2 + 1)->m_AllLen = m_DefaultParam.m_ClosureLen;
		}
		m_MainGirders.at(0)->m_AllLen = m_BridgeSpanArrange.at(0).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at(2)->m_AllLen) / 2; //小桩号侧现浇段梁长
		m_MainGirders.at(num_2 * 2)->m_AllLen = m_BridgeSpanArrange.at(num_2 - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at((num_2 - 1) * 2)->m_AllLen) / 2; //大桩号侧现浇段梁长
	}
	else  //单T构
	{
		AfxMessageBox(_T("警告：桥跨总数小于3，请检查连续刚构桥主桥孔跨布置方案！"));
		m_TIndex = 1;
		m_MainGirders.at(m_TIndex * 2)->m_AllLen = m_TGirderLen;
		m_MainGirders.at(m_TIndex * 2 - 1)->m_AllLen = m_DefaultParam.m_ClosureLen;
		m_MainGirders.at(m_TIndex * 2 - 2)->m_AllLen = m_BridgeSpanArrange.at(m_TIndex - 1).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at(m_TIndex * 2)->m_AllLen) / 2;
		m_MainGirders.at(m_TIndex * 2 + 1)->m_AllLen = m_DefaultParam.m_ClosureLen;
		m_MainGirders.at(m_TIndex * 2 + 2)->m_AllLen = m_BridgeSpanArrange.at(m_TIndex).m_CoordinateY - m_DefaultParam.m_ClosureLen - (m_MainGirders.at(m_TIndex * 2)->m_AllLen) / 2;
	}
	//更新主梁起点、中点和终点桩号
	m_MainGirders.at(0)->m_Left = m_BridgeSpanArrange.at(0).m_CoordinateX - m_BridgeSpanArrange.at(0).m_CoordinateY / 2;
	m_MainGirders.at(0)->m_Center = m_MainGirders.at(0)->m_Left + m_MainGirders.at(0)->m_AllLen / 2;
	m_MainGirders.at(0)->m_Right = m_MainGirders.at(0)->m_Left + m_MainGirders.at(0)->m_AllLen;
	for (unsigned int i = 1; i < num_1; i++)
	{
		m_MainGirders.at(i)->m_Left = m_MainGirders.at(i - 1)->m_Left + m_MainGirders.at(i - 1)->m_AllLen;
		m_MainGirders.at(i)->m_Center = m_MainGirders.at(i)->m_Left + m_MainGirders.at(i)->m_AllLen / 2;
		m_MainGirders.at(i)->m_Right = m_MainGirders.at(i)->m_Left + m_MainGirders.at(i)->m_AllLen;
	}
	//更新梁底曲线长度
	for (int i = 0; i < num_2; i++)
	{
		m_MainGirders.at((i + 1) * 2)->m_CurveLen = (m_MainGirders.at((i + 1) * 2)->m_AllLen - m_MainGirders.at((i + 1) * 2)->m_BottomLen) / 2;
	}
}

/*主梁高度初步拟定*/
void CRigidFrameBridge::PreSetGirderHeight()
{
	int num = (int)(m_MainGirders.size());
	if (num >= 2 * 2 + 3) //三跨及以上连续刚构桥主梁高度初拟
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
		AfxMessageBox(_T("警告：桥跨总数小于3，请检查连续刚构桥主桥孔跨布置方案！"));
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
	double start = 0.0;         //主梁起始坐标初始值
	double tLen = 100;          //梁长初始值
	double leftCastLen = 7.5;   //小桩号侧边跨现浇段梁长初始值
	double rightCastLen = 7.5;  //大桩号侧边跨现浇段梁长初始值
	double rootHeight = 10;     //主梁根部高度初始值
	double tolesHeight = 2;     //主梁端部高度初始值
	double bottomLen = 10;      //梁底直线段长初始值
	double remainLen = 0.5;     //悬臂根部的剩余长度初始值
	double rootThick = 1.2;     //主梁根部底板厚初始值
	double tolesThick = 0.3;    //主梁端部底板厚初始值
	double heightRatio = 1.6;   //梁底抛物线幂次方初始值
	double thickRatio = 1.6;    //梁底厚度幂次方初始值
	CString m_TGirderId = _T("");

	for (std::vector<CMainGirder*>::iterator it = m_MainGirders.begin(); it != m_MainGirders.end(); it++)
	{
		delete *it;
		*it = nullptr;
	}
	m_MainGirders.clear();

	start = m_BridgeSpanArrange[0].m_CoordinateX - 0.5 * m_BridgeSpanArrange[0].m_CoordinateY;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_SPAN, _T("左边跨现浇段"), start, leftCastLen, tolesHeight, rootThick));

	start += leftCastLen;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_CLOSURE, _T("左边跨合拢段"), start, m_DefaultParam.m_ClosureLen, tolesHeight, rootThick));

	start += m_DefaultParam.m_ClosureLen;
	m_TGirderId = _T("1#T构");
	m_MainGirders.push_back(new CMainGirder(CMainGirder::T_CONSTRUCTION, m_TGirderId, start, tLen, rootHeight, remainLen, rootHeight, tolesHeight, rootThick, tolesThick, heightRatio, thickRatio));

	int num = (int)(m_BridgeSpanArrange.size());
	for (int i = 0; i < num - 2; i++)
	{
		start += tLen;
		m_TGirderId.Format(_T("%d#中跨合拢段"), i + 1);
		m_MainGirders.push_back(new CMainGirder(CMainGirder::MID_CLOSURE, m_TGirderId, start, m_DefaultParam.m_ClosureLen, tolesHeight, rootThick));

		start += m_DefaultParam.m_ClosureLen;
		m_TGirderId.Format(_T("%d#T构"), i + 2);
		m_MainGirders.push_back(new CMainGirder(CMainGirder::T_CONSTRUCTION, m_TGirderId, start, tLen, rootHeight, remainLen, rootHeight, tolesHeight, rootThick, tolesThick, heightRatio, thickRatio));
	}

	start += tLen;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_CLOSURE, _T("右边跨合拢段"), start, m_DefaultParam.m_ClosureLen, tolesHeight, rootThick));

	start += m_DefaultParam.m_ClosureLen;
	m_MainGirders.push_back(new CMainGirder(CMainGirder::SIDE_SPAN, _T("右边跨现浇段"), start, rightCastLen, tolesHeight, rootThick));

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
	vector<CYNPoint2D>::iterator  m_iter_begin, m_iter_end; //随机迭代器
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

/*获得相邻主梁对象*/
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