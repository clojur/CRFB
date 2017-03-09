#include "stdafx.h"
// #include "RigidFrameBridge.h"
// #include "../RigidFrameDoc.h"
#include "MainGirder.h"
#include <osg\ShapeDrawable>
#include <YjOSG/YjOSG.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// extern CRigidFrameBridge* Get_Brg();

class GirderCallBack :public osg::NodeCallback
{
public:
	GirderCallBack(osg::ref_ptr<osg::Node> node) :m_node(node)
	{

	}
	~GirderCallBack(){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<osg::Group> root = dynamic_cast<osg::Group*>(node);
		root->removeChildren(0, root->getNumChildren());

		root->addChild(m_node);
		//��������
		traverse(node, nv);
	}
private:
	osg::ref_ptr<osg::Node> m_node;
};

IMPLEMENT_SERIAL(CMainGirder, CObject, 0)
CMainGirder::CMainGirder()
{
	m_MainPier = nullptr;
	m_Section = nullptr;
	for (vector<CSegment*>::iterator it = segments.begin(); it != segments.end(); it++)
	{
		*it = nullptr;
	}

	//Ϊ������Ҫ�ݶ�3������
	m_BottomLen = 9;
	m_Remain = 0.5;

	_root = new osg::Group;
	_callback = new CMainGirderCallback(this);
}

CMainGirder::CMainGirder(int type, CString id, double start, double length, double height, double thick)
	: m_Type(type), m_Id(id), m_Left(start), m_AllLen(length), m_RootHeight(height), m_RootThick(thick)
	, m_MainPier(nullptr), m_Section(nullptr)
{
	m_Right = m_Left + length;
	m_Center = (m_Left + m_Right) / 2;

	_root = new osg::Group;
	_callback = new CMainGirderCallback(this);
}

CMainGirder::CMainGirder(int type, CString id, double start, double length, double bottomLen, double remainLen, double rootHeight, double tolesHeight,
	double rootThick, double tolesThick, double heightRatio, double thickRatio)
	: m_Type(type), m_Id(id), m_Left(start), m_AllLen(length), m_BottomLen(bottomLen), m_Remain(remainLen), m_RootHeight(rootHeight), m_TolesHeight(tolesHeight)
	, m_RootThick(rootThick), m_TolesThick(tolesThick), m_HeightRatio(heightRatio), m_ThickRatio(thickRatio)
	, m_MainPier(nullptr), m_Section(nullptr)
{
	m_Right = m_Left + length;
	m_Center = (m_Left + m_Right) / 2;
	m_CurveLen = (m_AllLen - m_BottomLen) / 2;

	_root = new osg::Group;
	_callback = new CMainGirderCallback(this);
}

CMainGirder::~CMainGirder()
{
	//ɾ���ڶ�ָ��vector��ֹ�ڴ�й¶
	for (vector<CSegment*>::iterator it = segments.begin(); it != segments.end(); it++)
		if (nullptr != *it)
		{
			delete *it;
			*it = nullptr;
		}
	segments.clear();

	//ɾ�����ն���ָ���ֹ�ڴ�й¶
	if (m_MainPier != nullptr)
	{
		delete m_MainPier;
		m_MainPier = nullptr;
	}

}

void CMainGirder::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_Id;            //����ID(��߿��ֽ���,��߿��£��,1#T��,1#�п��£��,2#T��,2#�п��£��.....,�ұ߿��£��,�ұ߿��ֽ��� )
		ar << m_Type;          //��������enum
		ar << m_Left;
		ar << m_Right;
		ar << m_Center;        //���������������
		ar << m_CurveLen;
		ar << m_AllLen;        //����
		ar << m_BottomLen;     //����ֱ�߶γ�
		ar << m_Remain;        //���۸�����ʣ�೤�ȣ�����ֱ�߶αȶ�˳����ߴ����ĳ��ȣ�
		ar << m_HeightRatio;   //���������߱仯��
		ar << m_ThickRatio;    //���׺�ȱ仯��
		ar << m_TolesHeight;   //���˲���
		ar << m_RootHeight;    //��������
		ar << m_TolesThick;    //���˲��װ��
		ar << m_RootThick;     //�������װ��
		ar << m_ResSplit;      //�������ֽ��(�ַ���)

		ar << segments.size();
		for (vector<CSegment*>::iterator iter = segments.begin(); iter != segments.end(); ++iter)
		{
			ar << *iter;
		}
		m_DefaultParam.Serialize(ar);
	}
	else
	{
		ar >> m_Id;            //����ID(��߿��ֽ���,��߿��£��,1#T��,1#�п��£��,2#T��,2#�п��£��.....,�ұ߿��£��,�ұ߿��ֽ��� )
		ar >> m_Type;          //��������enum
		ar >> m_Left;
		ar >> m_Right;
		ar >> m_Center;        //���������������
		ar >> m_CurveLen;
		ar >> m_AllLen;        //����
		ar >> m_BottomLen;     //����ֱ�߶γ�
		ar >> m_Remain;        //���۸�����ʣ�೤�ȣ�����ֱ�߶αȶ�˳����ߴ����ĳ��ȣ�
		ar >> m_HeightRatio;   //���������߱仯��
		ar >> m_ThickRatio;    //���׺�ȱ仯��
		ar >> m_TolesHeight;   //���˲���
		ar >> m_RootHeight;    //��������
		ar >> m_TolesThick;    //���˲��װ��
		ar >> m_RootThick;     //�������װ��
		ar >> m_ResSplit;      //�������ֽ��(�ַ���)

		int count;
		ar >> count;
		CSegment* newSeg = nullptr;
		for (int i = 0; i < count; i++)
		{
			ar >> newSeg;
			segments.push_back(newSeg);
		}
		m_DefaultParam.Serialize(ar);
	}
}


/////////// ��������ȡ�ָ�����ߵ��ַ���
CString CMainGirder::LSplit(CString str, TCHAR chr)
{
	int pos = str.Find(chr);
	if (pos != -1)
		return str.Left(pos);
	else
		return _T("ERROR");
}


/////////// ��������ȡ�ָ����ұߵ��ַ���
CString CMainGirder::RSplit(CString str, TCHAR chr)
{
	int pos = str.Find(chr);
	if (pos != -1)
		return str.Right(str.GetLength() - pos - 1);
	else
		return _T("ERROR");
}


//���ܻ��ֽڶθ���ͨ�ú���
bool AddsplitStr(int n, int nB, CString all, CString part, CString& resTmp, CString& resTotal)
{
	if (n > 0)
	{
		if (n >= nB)
		{
			if (n != 1)
				resTmp.Format(all, n);
			else
				resTmp.Format(_T("%s"), part);
			resTotal += resTmp;
		}
		else
			return false;
	}
	return true;
}


//���ܻ��ֽڶ�(�����ַ�����)
vector<CString>& CMainGirder::AutoCellsplit()
{
	CString resTmp(_T("")), resTotal(_T(""));
	double  LenCantilever = (m_AllLen - m_BottomLen - 2 * m_Remain) / 2;//T���������۳���(�����������γ���)

	// 	CRigidFrameBridge* brg = Get_Brg();

	//�����resSplit
	resSplit.clear();

	for (int n1 = 0; n1 <= m_DefaultParam.n1E; n1++) {
		for (int n2 = 0; n2 <= m_DefaultParam.n2E; n2++) {
			for (int n3 = 0; n3 <= m_DefaultParam.n3E; n3++) {
				for (int n4 = 0; n4 <= m_DefaultParam.n4E; n4++) {
					for (int n5 = 0; n5 <= m_DefaultParam.n5E; n5++) {
						for (int n6 = 0; n6 <= m_DefaultParam.n6E; n6++) {
							for (int n7 = 0; n7 <= m_DefaultParam.n7E; n7++) {

								resTotal = _T("");

								if (n1 * 2.5 + n2 * 3 + n3 * 3.5 + n4 * 4 + n5 * 4.5 + n6 * 5 + n7 * 5.5 == LenCantilever)
								{
									if (!AddsplitStr(n1, m_DefaultParam.n1B, _T("+%dx2.5"), _T("+2.5"), resTmp, resTotal)) goto Cancellation;
									if (!AddsplitStr(n2, m_DefaultParam.n2B, _T("+%dx3"), _T("+3"), resTmp, resTotal)) goto Cancellation;
									if (!AddsplitStr(n3, m_DefaultParam.n3B, _T("+%dx3.5"), _T("+3.5"), resTmp, resTotal)) goto Cancellation;
									if (!AddsplitStr(n4, m_DefaultParam.n4B, _T("+%dx4"), _T("+4"), resTmp, resTotal)) goto Cancellation;
									if (!AddsplitStr(n5, m_DefaultParam.n5B, _T("+%dx4.5"), _T("+4.5"), resTmp, resTotal)) goto Cancellation;
									if (!AddsplitStr(n6, m_DefaultParam.n6B, _T("+%dx5"), _T("+5"), resTmp, resTotal)) goto Cancellation;
									if (!AddsplitStr(n7, m_DefaultParam.n7B, _T("+%dx5.5"), _T("+5.5"), resTmp, resTotal)) goto Cancellation;

									//�˴�ȥ�������еĵ�һ��"+"��
									resTotal = resTotal.Right(resTotal.GetLength() - 1);
									resSplit.push_back(resTotal);
								}
							Cancellation:;
							}
						}
					}
				}
			}
		}
	}

	if (resSplit.size() == 0)
		AfxMessageBox(L"û�к��ʵĻ��ַ���,���ʵ��ı仮��ԭ�������!");
	else
		m_ResSplit = resSplit[0];//Ĭ�ϲ��õ�һ����
	return resSplit;
}

//�����ڶλ����ַ���(������)��ֵ������
void CMainGirder::ResolveStrToCell(CString str)
{
	CString tmpSTr;
	int smallNum;
	double smallLen;

	cell.clear();
	short cellCT = 0;
	for (;;)
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
			for (int K = cellCT; K <= cellCT + smallNum - 1; K++)
				cell.push_back(smallLen);
			cellCT = cellCT + smallNum;
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

			for (int K = cellCT; K <= cellCT + smallNum - 1; K++)
				cell.push_back(smallLen);
			cellCT = cellCT + smallNum;

			str = RSplit(str, '+');
		}
	}
}




//���ɵ��ڶ���������ĵ�Ԫ
void CMainGirder::CreateStdSegment()
{
	segments.clear();
	CSegment* newSeg = new CSegment();
	newSeg->m_Id = 101;
	newSeg->SetSection(m_Section);

	newSeg->m_Length = m_AllLen;
	newSeg->m_AddLength = m_AllLen / 2;
	newSeg->m_LeftStake = m_Center - m_AllLen / 2;
	newSeg->m_RightStake = m_Center + m_AllLen / 2;
	segments.push_back(newSeg);
}


//����ѡ���Ļ����ַ������ɽڶμ���
void CMainGirder::CreateSegmentsByStr(CString curStr)
{
	//(0) ����Ԫ�����Ļ���
	if (curStr == _T("Standard"))
	{
		CreateStdSegment();
		UpdateCell();
		return;
	}

	//(1) ��ɾ���ڶα������е�Ԫ�����²��õ����ջ��ַ���
	for (std::vector<CSegment*>::iterator it = segments.begin();
		it != segments.end(); it++)
	{
		if ((*it) != nullptr)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}
	segments.clear();
	m_ResSplit = curStr;

	//(2) ����¶ն��ڶ�
	CSegment* newSeg = new CSegment();
	newSeg->m_Id = -99;
	newSeg->m_Length = m_BottomLen / 2;
	newSeg->m_AddLength = -m_BottomLen / 2;
	newSeg->m_LeftStake = m_Center - m_BottomLen / 2;
	newSeg->m_RightStake = m_Center ;

	newSeg->SetSection(m_Section);
	segments.push_back(newSeg);

	newSeg = new CSegment();
	newSeg->m_Id = -100;
	newSeg->m_Length = m_Remain;
	newSeg->m_AddLength = -m_BottomLen / 2 - m_Remain;
	newSeg->m_LeftStake = m_Center - m_BottomLen / 2 - m_Remain;
	newSeg->m_RightStake = m_Center + m_BottomLen / 2;

	newSeg->SetSection(m_Section);
	segments.push_back(newSeg);

	newSeg = new CSegment();
	newSeg->m_Id = 99;
	newSeg->m_Length = m_BottomLen / 2;
	newSeg->m_AddLength = m_BottomLen / 2;
	newSeg->m_LeftStake = m_Center ;
	newSeg->m_RightStake = m_Center + m_BottomLen / 2;
	newSeg->SetSection(m_Section);
	segments.push_back(newSeg);

	newSeg = new CSegment();
	newSeg->m_Id = 100;
	newSeg->m_Length = m_Remain;
	newSeg->m_AddLength = m_BottomLen / 2 + m_Remain;
	newSeg->m_LeftStake = m_Center + m_BottomLen / 2;
	newSeg->m_RightStake = m_Center + m_BottomLen / 2 + m_Remain;
	newSeg->SetSection(m_Section);
	segments.push_back(newSeg);

	//(3) ��������۽ڶ�
	CString newID;
	ResolveStrToCell(curStr);
	double addLen = m_BottomLen / 2 + m_Remain;

	for (unsigned int i = 1; i <= cell.size(); i++)
	{
		short sgn = -1;
		for (int j = 1; j <= 2; j++)
		{
			newSeg = new CSegment();
			newSeg->m_Id = (int)sgn * (int)(i+100);
			newSeg->m_Length = cell[i - 1];
			newSeg->m_AddLength = sgn*(addLen + cell[i - 1]);
			if (sgn == -1)
			{
				newSeg->m_LeftStake = m_Center + newSeg->m_AddLength;
				newSeg->m_RightStake = newSeg->m_LeftStake + newSeg->m_Length;
			}
			else
			{
				newSeg->m_RightStake = m_Center + newSeg->m_AddLength;
				newSeg->m_LeftStake = newSeg->m_RightStake - newSeg->m_Length;
			}
			newSeg->SetSection(m_Section);
			segments.push_back(newSeg);
			sgn *= -1;
		}
		addLen += cell[i - 1];
	}
	UpdateCell();
}

/*���ݽڶα�Ų��ҽڶζ���*/
CSegment* CMainGirder::GetSegmentByID(int m_Id)
{
	for (vector<CSegment*>::iterator iter = segments.begin(); iter != segments.end(); ++iter)
	{
		if ((*iter)->m_Id == m_Id)
		{
			return *iter;
		}
	}
	return NULL;
}



//���½ڶμ��ϵľ������...
void CMainGirder::UpdateCell()
{
	vector<CSegment*>::iterator iter;
	double lDis, rDis;
	double hp = m_Section->m_SlpVal;
	double Tw = m_Section->m_TboxWidth;
	double Bw = m_Section->m_DboxWidth;
	double hlk = m_Section->m_Guardrail;
	double sjgL,sjgR;

	for (iter = segments.begin(); iter != segments.end(); ++iter)
	{
		lDis = ((*iter)->m_AddLength > 0 ? (*iter)->m_AddLength - (*iter)->m_Length : (*iter)->m_AddLength);
		rDis = ((*iter)->m_AddLength > 0 ? (*iter)->m_AddLength : (*iter)->m_AddLength + (*iter)->m_Length);

		/**** ������� ****/
		(*iter)->m_LeftStake = m_Center + lDis;
		(*iter)->m_RightStake = m_Center + rDis;

		/**** ���ҵװ�� ****/
		(*iter)->m_LplateThick = getTB_atX(lDis);
		(*iter)->m_RplateThick = getTB_atX(rDis);

		/**** ������Ƹ߳� ****/
		(*iter)->m_LdesignElev = sjgL = getE_atX(lDis);
		(*iter)->m_RdesignElev = sjgR = getE_atX(rDis);


		if (m_Section->m_SlpType == _T("˫����"))
		{
			/**** ���ҽ���� ****/
			(*iter)->m_LlHeight = (*iter)->m_LrHeight = getH_AtX(lDis);
			(*iter)->m_RlHeight = (*iter)->m_RrHeight = getH_AtX(rDis);

			/**** ���Ҹ�����ඥ��߳� ****/
			if (m_Section->m_LocElevation == _T("���������"))
			{
				(*iter)->m_LlwebElev = (*iter)->m_LrwebElev = sjgL - Bw / 2 * fabs(hp);
				(*iter)->m_RlwebElev = (*iter)->m_RrwebElev = sjgR - Bw / 2 * fabs(hp);
			}
			else if (m_Section->m_LocElevation == _T("�ӿ�ǰ·����Ե"))
			{
				(*iter)->m_LlwebElev = (*iter)->m_LrwebElev = sjgL + (Tw - Bw) / 2 * fabs(hp);
				(*iter)->m_RlwebElev = (*iter)->m_RrwebElev = sjgR + (Tw - Bw) / 2 * fabs(hp);
			}
		}
		else
		{
			/*���ҽ����*/
			if (hp > 0)
			{
				(*iter)->m_LlHeight = getH_AtX(lDis);
				(*iter)->m_LrHeight = (*iter)->m_LlHeight + Bw*hp;

				(*iter)->m_RlHeight = getH_AtX(rDis);
				(*iter)->m_RrHeight = (*iter)->m_RlHeight + Bw*hp;
			}
			else         
			{
				(*iter)->m_LrHeight = getH_AtX(lDis);
				(*iter)->m_LlHeight = (*iter)->m_LrHeight - Bw*hp;

				(*iter)->m_RrHeight = getH_AtX(rDis);
				(*iter)->m_RlHeight = (*iter)->m_RrHeight - Bw*hp;
			}

			/**** ���Ҹ�����ඥ��߳� ****///(Ŀǰ��Ϊ��ֱ����,������ֻ��������Ƹ�λ������ָ�����Ե���)
			if (hp > 0)
			{
				(*iter)->m_LlwebElev = sjgL - (Tw / 2 + Bw / 2 - hlk) * hp;
				(*iter)->m_LrwebElev = sjgL - (Tw / 2 - Bw / 2 - hlk) * hp;
				(*iter)->m_RlwebElev = sjgR - (Tw / 2 + Bw / 2 - hlk) * hp;
				(*iter)->m_RrwebElev = sjgR - (Tw / 2 - Bw / 2 - hlk) * hp;
			}
			else
			{
				(*iter)->m_LlwebElev = sjgL - (Tw / 2 - Bw / 2 - hlk) * fabs(hp);
				(*iter)->m_LrwebElev = sjgL - (Tw / 2 + Bw / 2 - hlk) * fabs(hp);
				(*iter)->m_RlwebElev = sjgR - (Tw / 2 - Bw / 2 - hlk) * fabs(hp);
				(*iter)->m_RrwebElev = sjgR - (Tw / 2 + Bw / 2 - hlk) * fabs(hp);
			}
		}
		/**** ���ҵװ�߳�1��2 ****/
		(*iter)->m_LbottomElev1 = (*iter)->m_LlwebElev - (*iter)->m_LlHeight + (*iter)->m_LplateThick;
		(*iter)->m_LbottomElev2 = (*iter)->m_LlwebElev - (*iter)->m_LlHeight;
		(*iter)->m_RbottomElev1 = (*iter)->m_RlwebElev - (*iter)->m_RlHeight + (*iter)->m_RplateThick;
		(*iter)->m_RbottomElev2 = (*iter)->m_RlwebElev - (*iter)->m_RlHeight;

	}

	_callback->Lock();
	_callback->Update();
	_callback->Unlock();
}




//���ݳ�������Ƹ�(����xΪ�������������ĵľ���,������֮��)
double CMainGirder::getE_atX(double x)
{
	double stake = x + m_Center;
	double elevation = -1.0;
	m_Road->GetDesignElevation(stake, elevation);

	return elevation;
}



//���ݳ�����װ��(����xΪ�������������ĵľ���,������֮��)
double	CMainGirder::getTB_atX(double x)
{
	if (m_Type != CMainGirder::T_CONSTRUCTION)//����T������������ͳһ���ء������װ��
		return m_RootThick;
	else
		return pow(m_AllLen / 2 - fabs(x), m_ThickRatio)* (m_RootThick - m_TolesThick) / pow((m_AllLen - m_BottomLen) / 2, m_ThickRatio) + m_TolesThick;
}



//���ݳ���������(����xΪ�������������ĵľ���,������֮��)
double CMainGirder::getH_AtX(double x)
{
	if (m_Type != CMainGirder::T_CONSTRUCTION)//����T������������ͳһ���ء��˲��ߡ�
		return m_TolesHeight;
	else
	{
		if (fabs(x) <= m_BottomLen / 2)
			return m_RootHeight;
		else
			return pow(m_AllLen / 2 - fabs(x), m_HeightRatio)* (m_RootHeight - m_TolesHeight) / pow((m_AllLen - m_BottomLen) / 2, m_ThickRatio) + m_TolesHeight;
	}
}




void CMainGirder::CalcOutline()
{
	double stake = 0;
	double elevation = 0;
	double height = 0;
	double len = 0;
	double OneSegment = 5; //T��������λ�ڶγ���
	int cnt = 0;

	if (m_Type != T_CONSTRUCTION)
	{
		for (int i = 0; i <= 1; i++)
		{
			stake = m_Left + m_AllLen*i;
			m_Road->GetDesignElevation(stake, elevation);
			m_BeamTop.push_back(CYNPoint2D(stake, elevation));
			height = m_RootHeight;
			m_BeamBottom.push_back(CYNPoint2D(stake, elevation - height));
		}
	}
	else
	{
		len = m_Center - m_BottomLen / 2 - m_Left;
		cnt = (int)floor(len / OneSegment); //����ȡ��
		stake = m_Left;
		m_Road->GetDesignElevation(stake, elevation);
		m_BeamTop.push_back(CYNPoint2D(stake, elevation));
		height = getH_AtX(stake - m_Center);
		m_BeamBottom.push_back(CYNPoint2D(stake, elevation - height));

		stake += len - cnt*OneSegment;
		for (int i = 0; i <= cnt; i++)
		{
			m_Road->GetDesignElevation(stake, elevation);
			m_BeamTop.push_back(CYNPoint2D(stake, elevation));
			height = getH_AtX(stake - m_Center);
			m_BeamBottom.push_back(CYNPoint2D(stake, elevation - height));
			stake += OneSegment;
		}

		stake = m_Center + m_BottomLen / 2;
		m_Road->GetDesignElevation(stake, elevation);
		m_BeamTop.push_back(CYNPoint2D(stake, elevation));
		height = getH_AtX(stake - m_Center);
		m_BeamBottom.push_back(CYNPoint2D(stake, elevation - height));

		for (int i = 1; i <= cnt; i++)
		{
			stake += OneSegment;
			m_Road->GetDesignElevation(stake, elevation);
			m_BeamTop.push_back(CYNPoint2D(stake, elevation));
			height = getH_AtX(stake - m_Center);
			m_BeamBottom.push_back(CYNPoint2D(stake, elevation - height));
		}

		stake = m_Center + m_AllLen / 2;
		m_Road->GetDesignElevation(stake, elevation);
		m_BeamTop.push_back(CYNPoint2D(stake, elevation));
		height = getH_AtX(stake - m_Center);
		m_BeamBottom.push_back(CYNPoint2D(stake, elevation - height));
	}
}



void CMainGirder::CreatePier()
{
	if (m_Type != T_CONSTRUCTION)
		return;

	double width = 6.0;     //˫���ڶպ�����
	double thickness = 2.0; //˫���ڶյ�֫���
	double capDepth = 4.0;  //��̨�߶�
	double padding = 2.0;	//��߳���

	double dia = 1.2; //׮��

	m_MainPier = new CMainPier(m_Road, m_Center, 0.0, m_BottomLen, width);
	m_MainPier->SetPierParam(thickness, m_RootHeight);
	m_MainPier->SetCapParam(capDepth, padding);
	m_MainPier->SetPileParam(dia, 40.0);
	m_MainPier->Create();
}

osg::ref_ptr<osg::Geometry> CMainGirder::CreatePrismoid(osg::ref_ptr<osg::Vec3Array> vertices)
{
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	geometry->setVertexArray(vertices);

	osg::ref_ptr<osg::DrawElementsUInt> left = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	left->push_back(0);
	left->push_back(1);
	left->push_back(2);
	left->push_back(3);
	geometry->addPrimitiveSet(left);

	osg::ref_ptr<osg::DrawElementsUInt> right = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	right->push_back(4);
	right->push_back(5);
	right->push_back(6);
	right->push_back(7);
	geometry->addPrimitiveSet(right);

	osg::ref_ptr<osg::DrawElementsUInt> up = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	up->push_back(0);
	up->push_back(3);
	up->push_back(7);
	up->push_back(4);
	geometry->addPrimitiveSet(up);

	osg::ref_ptr<osg::DrawElementsUInt> down = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	down->push_back(1);
	down->push_back(2);
	down->push_back(6);
	down->push_back(5);
	geometry->addPrimitiveSet(down);

	osg::ref_ptr<osg::DrawElementsUInt> front = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	front->push_back(3);
	front->push_back(2);
	front->push_back(6);
	front->push_back(7);
	geometry->addPrimitiveSet(front);

	osg::ref_ptr<osg::DrawElementsUInt> back = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	back->push_back(0);
	back->push_back(1);
	back->push_back(5);
	back->push_back(4);
	geometry->addPrimitiveSet(back);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	geometry->setColorArray(colors, osg::Array::BIND_OVERALL);

	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
	n->push_back(osg::Vec3(-1.0, 0.0, 0.0));
	n->push_back(osg::Vec3(1.0, 0.0, 0.0));
	n->push_back(osg::Vec3(0.0, 0.0, 1.0));
	n->push_back(osg::Vec3(0.0, 0.0, -1.0));
	n->push_back(osg::Vec3(0.0, -1.0, 0.0));
	n->push_back(osg::Vec3(0.0, 1.0, 0.0));
	geometry->setNormalArray(n, osg::Array::BIND_PER_PRIMITIVE_SET);

	return geometry.get();
}

void CMainGirder::GetModelAttrib()
{
	AfxMessageBox(_T("����"));
}




void CMainGirderCallback::DoUpdate()
{
	_root->removeChildren(0, _root->getNumChildren());

	if (!_girder->segments.empty())
	{
		for (std::vector<CSegment*>::iterator it = _girder->segments.begin(); it != _girder->segments.end(); it++)
		{
			_root->addChild((*it)->getRoot());
		}

		//if (_girder->m_Type == CMainGirder::T_CONSTRUCTION)
		//	root->addChild(new PickModel(_girder->m_MainPier->Draw()));

		//_root->setUpdateCallback(new GirderCallBack(root));
	}
	else
	{
		//osg::Geode* geode = new osg::Geode();
		osg::ref_ptr<CClassGeode> geode = new CClassGeode(_girder);

		for (int i = 0; i < (int)(_girder->m_BeamBottom.size()) - 1; i++)
		{
			osg::ref_ptr<osg::Vec3Array> leftOuter = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec3Array> leftInner = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec3Array> rightOuter = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec3Array> rightInner = new osg::Vec3Array();

			double left = _girder->m_BeamBottom[i].m_CoordinateX;
			double right = _girder->m_BeamBottom[i + 1].m_CoordinateX;
			double leftTop = _girder->m_BeamTop[i].m_CoordinateY;
			double leftBottom = _girder->m_BeamBottom[i].m_CoordinateY;
			double rightTop = _girder->m_BeamTop[i + 1].m_CoordinateY;
			double rightBottom = _girder->m_BeamBottom[i + 1].m_CoordinateY;
			/*
			vertices->push_back(osg::Vec3(left, 3.0, leftTop));
			vertices->push_back(osg::Vec3(left, 3.0, leftBottom));
			vertices->push_back(osg::Vec3(left, -3.0, leftBottom));
			vertices->push_back(osg::Vec3(left, -3.0, leftTop));
			vertices->push_back(osg::Vec3(right, 3.0, rightTop));
			vertices->push_back(osg::Vec3(right, 3.0, rightBottom));
			vertices->push_back(osg::Vec3(right, -3.0, rightBottom));
			vertices->push_back(osg::Vec3(right, -3.0, rightTop));
			geode->addDrawable(CreatePrismoid(vertices));
			*/

			//�ڶ������
			osg::ref_ptr<osg::Geometry> leftGeometry = _girder->m_Section->Draw(left, 0.0, leftTop, leftTop - leftBottom, leftOuter, leftInner);
			osg::ref_ptr<osg::Vec3Array> leftNormal = new osg::Vec3Array();
			leftNormal->push_back(osg::Vec3(-1.0, 0.0, 0.0));
			leftGeometry->setNormalArray(leftNormal, osg::Array::BIND_OVERALL);
			osg::ref_ptr<osg::Vec4Array> leftColor = new osg::Vec4Array();
			leftColor->push_back(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
			leftGeometry->setColorArray(leftColor, osg::Array::BIND_OVERALL);
			geode->addDrawable(leftGeometry);

			//�ڶ��Ҷ���
			osg::ref_ptr<osg::Geometry> rightGeometry = _girder->m_Section->Draw(right, 0.0, rightTop, rightTop - rightBottom, rightOuter, rightInner);
			osg::ref_ptr<osg::Vec3Array> rightNormal = new osg::Vec3Array();
			rightNormal->push_back(osg::Vec3(1.0, 0.0, 0.0));
			rightGeometry->setNormalArray(rightNormal, osg::Array::BIND_OVERALL);
			osg::ref_ptr<osg::Vec4Array> rightColor = new osg::Vec4Array();
			rightColor->push_back(osg::Vec4(0.5, 0.5, 0.5, 1.0f));
			rightGeometry->setColorArray(rightColor, osg::Array::BIND_OVERALL);
			geode->addDrawable(rightGeometry);

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(*leftOuter);
			vertices->insert(vertices->end(), leftInner->begin(), leftInner->end());
			vertices->insert(vertices->end(), rightOuter->begin(), rightOuter->end());
			vertices->insert(vertices->end(), rightInner->begin(), rightInner->end());

			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(0.3, 0.5, 0.5, 1.0f));
			//colors->push_back(osg::Vec4(1.0, 0.0, 0.5, 1.0f));
			geometry->setVertexArray(vertices);
			geometry->setColorArray(colors, osg::Array::BIND_OVERALL);
			osg::ref_ptr<osg::DrawElementsUInt> out = NULL;
			for (int i = 0; i < 10; i++)
			{
				out = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
				out->push_back(i);
				out->push_back(22 + i);
				out->push_back(22 + i + 1);
				out->push_back(i + 1);
				geometry->addPrimitiveSet(out);
			}
			out = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
			out->push_back(10);
			out->push_back(32);
			out->push_back(22);
			out->push_back(0);
			geometry->addPrimitiveSet(out);

			osg::ref_ptr<osg::DrawElementsUInt> in = NULL;
			for (int i = 11; i < 21; i++)
			{
				in = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
				in->push_back(i + 1);
				in->push_back(22 + i + 1);
				in->push_back(22 + i);
				in->push_back(i);
				geometry->addPrimitiveSet(in);
			}
			in = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
			in->push_back(11);
			in->push_back(33);
			in->push_back(43);
			in->push_back(21);
			geometry->addPrimitiveSet(in);

			osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
			normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
			normal->push_back(osg::Vec3(0.0, 1.0, 0.0));
			normal->push_back(osg::Vec3(0.0, 0.5, -0.866));
			normal->push_back(osg::Vec3(0.0, 0.866, -0.5));
			normal->push_back(osg::Vec3(0.0, 1.0, 0.0));
			normal->push_back(osg::Vec3(0.0, 0.0, -1.0));
			normal->push_back(osg::Vec3(0.0, -1.0, 0.0));
			normal->push_back(osg::Vec3(0.0, -0.866, -0.5));
			normal->push_back(osg::Vec3(0.0, -0.5, -0.866));
			normal->push_back(osg::Vec3(0.0, -1.0, 0.0));
			normal->push_back(osg::Vec3(0.0, 0.0, 1.0));

			normal->push_back(osg::Vec3(0.0, 0.0, -1.0));
			normal->push_back(osg::Vec3(0.0, -0.5, -0.866));
			normal->push_back(osg::Vec3(0.0, -0.866, -0.5));
			normal->push_back(osg::Vec3(0.0, -1.0, 0.0));
			normal->push_back(osg::Vec3(0.0, -0.707, 0.707));
			normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
			normal->push_back(osg::Vec3(0.0, 0.707, 0.707));
			normal->push_back(osg::Vec3(0.0, 1.0, 0.0));
			normal->push_back(osg::Vec3(0.0, 0.866, -0.5));
			normal->push_back(osg::Vec3(0.0, 0.5, -0.866));
			normal->push_back(osg::Vec3(0.0, 0.0, -1.0));
			geometry->setNormalArray(normal, osg::Array::BIND_PER_PRIMITIVE_SET);

			geode->addDrawable(geometry);
		}

		_root->addChild(new PickModel(geode));
	}

	if (_girder->m_Type == CMainGirder::T_CONSTRUCTION)
	{
		_root->addChild(new PickModel(_girder->m_MainPier->getRoot()));
		_girder->m_MainPier->getUpdateCallback()->setView(getView());
	}
}

