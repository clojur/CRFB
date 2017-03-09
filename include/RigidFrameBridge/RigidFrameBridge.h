
/* ######################################################################################

����: �չ���
����: ���������չ��Ŷ���
�������¼�����:·�����ݡ��ŷ���Ϣ����׼���桢�������󼯺ϡ���С׮�Ų����Ŷ���

########################################################################################*/

#pragma once

#include <YjOSG/YjOSG.h>
#include <RoadData/RoadData.h>
#include <DefaultParam/DefaultParam.h>
#include <MainGirder/MainGirder.h>
#include <YjMath/YjMath.h>
#include <StdSection/StdSection.h>
#include "macro.h"

class CRigidFrameBridgeCallback;

class RIGIDFRAMEBRIDGE_API CRigidFrameBridge :public CObject
{
	_DECLARE_DYNCREATE(CRigidFrameBridge) \
		AFX_EXT_API friend CArchive& AFXAPI \
		operator>>(CArchive& ar, CRigidFrameBridge* &pOb);
public:
	/*����·������*/
	CRoadData m_RoadData;

	/*Ĭ�ϲ���*/
	CDefaultParam m_DefaultParam;

	/*���ſ羶����*/
	vector<CYNPoint2D> m_BridgeSpanArrange; //���ֿ�����׮�ţ��羶��

	/*�������󼯺�*/
	vector<CMainGirder*> m_MainGirders;//��T������£�Ρ��߿��ֽ��Σ�

	/*���ſ羶����*/
	CYNPoint2D m_YQLeft; //С׮�Ų����š���׼�羶,������
	CYNPoint2D m_YQRight; //��׮�Ų����š���׼�羶,������

	/*�ڶλ���*/
	CStdSection m_StdSect;//ȫ�����õı�׼���������

	/*�������Բ���*/
	int m_ZQID;             //����ID(1,2,3,...n�����ܳ����Զ����׷����������ֵ)
	CString m_ZQName;       //��������
	double m_ZQWidth;       //�ſ�
	double m_ZQBaseStake;   //���Ų��׻���׮�ţ�����׮�ţ�

	/*�˹�ָ�������ַ���*/
	CString m_YQLayOutResLeft, m_ZQLayOutRes, m_YQLayOutResRight, m_CenterStake;

private:

	CRect rc;
	osg::ref_ptr<osg::Group> _root;
	CRigidFrameBridgeCallback* _callback;

	/*��������*/
	CYNPoint2D m_ZQStartPt;   //�������
	CYNPoint2D m_ZQEndPt;     //�����յ�
	int m_ZQMainSpan;         //����羶������������������̲�,ȡ����
	int m_ZQMainSpanNum;      //�������
	int m_ZQSideSpanLeft;     //С׮�Ų�߿�羶�������������ߵ���������������̲�,ȡ����
	int m_ZQSideSpanRight;    //��׮�Ų�߿�羶�������������ߵ���������������̲�,ȡ����

	//�Զ����׷�������
	vector<vector<CYNPoint2D>> m_BridgeSpanArrangeAll;

public:

	CRigidFrameBridge();
	CRigidFrameBridge(CString, double);
	CRigidFrameBridge(CRigidFrameBridge& brg);
	~CRigidFrameBridge();

	CRigidFrameBridge& operator = (CRigidFrameBridge& src);
	CRigidFrameBridgeCallback* getUpdateCallback() { return _callback; }
	osg::Group* getRoot() const { return _root; }

	/*���л�*/
	virtual void Serialize(CArchive& ar);

	/*��ͼ*/
	osg::ref_ptr<osg::Group> Draw(bool isfirst);

	/*���ų����Զ�����*/
	void PreAutoLayOut(int index);

	/*���Ž������Ų���*/
	void YQAutoSetSpan(int, int);

	/*��Ϊָ�����׷���*/
	void SetBridgeSpan(CString m_YQLeftSpanText, CString m_ZQSpanText, CString m_YQSpanRightText, double m_BridgeCenterStake);

	/*�������������ⶨ*/
	void PreSetGirderLength(int index, double m_TGirderLen);

	/*�����߶ȳ����ⶨ*/
	void PreSetGirderHeight();

	/*������������*/
	void CreateMainGirder();

	/*��ȡ���׷���������׮���ַ���*/
	void GetBridgeLayOutText();

	/*�������ȫ��*/
	double GetBrgAllLen();

	/*���������������*/
	bool GetSideMainGirder(CMainGirder* src, CMainGirder* &r_LeftOne, CMainGirder* &r_RightOne);

	/*����Զ����׷���ȫ��*/
	vector<vector<CYNPoint2D>> GetBridgeSpanArrangeAll(){ return m_BridgeSpanArrangeAll; };

	/*�ַ�����������*/
	vector<double> ResolveStrToSpan(CString str); //�����羶�����ַ���

private:

	CString LSplit(CString str, TCHAR chr); //��ȡ�ָ�������ַ���
	CString RSplit(CString str, TCHAR chr); //��ȡ�ָ����ұ��ַ���

};

class CRigidFrameBridgeCallback : public ComponentCallback
{
public:
	CRigidFrameBridgeCallback(CRigidFrameBridge* brg)
		: ComponentCallback(brg->getRoot())
	{
		_brg = brg;
		_root->setName("CRigidFrameBridge");
	}

	void DoUpdate() override;

private:
	CRigidFrameBridge* _brg;
};
