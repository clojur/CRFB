
/* ######################################################################################

类名: 刚构桥
描述: 用来构建刚构桥对象
包含的下级对象:路线数据、桥幅信息、标准截面、主梁对象集合、大小桩号侧引桥对象

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
	/*基础路线数据*/
	CRoadData m_RoadData;

	/*默认参数*/
	CDefaultParam m_DefaultParam;

	/*主桥跨径布置*/
	vector<CYNPoint2D> m_BridgeSpanArrange; //【分跨中心桩号，跨径】

	/*主梁对象集合*/
	vector<CMainGirder*> m_MainGirders;//（T构、合拢段、边跨现浇段）

	/*引桥跨径布置*/
	CYNPoint2D m_YQLeft; //小桩号侧引桥【标准跨径,跨数】
	CYNPoint2D m_YQRight; //大桩号侧引桥【标准跨径,跨数】

	/*节段划分*/
	CStdSection m_StdSect;//全桥适用的标准截面类对象

	/*主桥属性参数*/
	int m_ZQID;             //主桥ID(1,2,3,...n，不能超过自动布孔方案集合最大值)
	CString m_ZQName;       //主桥名称
	double m_ZQWidth;       //桥宽
	double m_ZQBaseStake;   //主桥布孔基点桩号（中心桩号）

	/*人工指定布孔字符串*/
	CString m_YQLayOutResLeft, m_ZQLayOutRes, m_YQLayOutResRight, m_CenterStake;

private:

	CRect rc;
	osg::ref_ptr<osg::Group> _root;
	CRigidFrameBridgeCallback* _callback;

	/*主桥属性*/
	CYNPoint2D m_ZQStartPt;   //主桥起点
	CYNPoint2D m_ZQEndPt;     //主桥终点
	int m_ZQMainSpan;         //主跨跨径（主跨主墩中心线里程差,取整）
	int m_ZQMainSpanNum;      //主跨跨数
	int m_ZQSideSpanLeft;     //小桩号侧边跨跨径（伸缩缝中心线到边主墩中心线里程差,取整）
	int m_ZQSideSpanRight;    //大桩号侧边跨跨径（伸缩缝中心线到边主墩中心线里程差,取整）

	//自动布孔方案集合
	vector<vector<CYNPoint2D>> m_BridgeSpanArrangeAll;

public:

	CRigidFrameBridge();
	CRigidFrameBridge(CString, double);
	CRigidFrameBridge(CRigidFrameBridge& brg);
	~CRigidFrameBridge();

	CRigidFrameBridge& operator = (CRigidFrameBridge& src);
	CRigidFrameBridgeCallback* getUpdateCallback() { return _callback; }
	osg::Group* getRoot() const { return _root; }

	/*序列化*/
	virtual void Serialize(CArchive& ar);

	/*绘图*/
	osg::ref_ptr<osg::Group> Draw(bool isfirst);

	/*主桥初步自动布孔*/
	void PreAutoLayOut(int index);

	/*主桥接线引桥布孔*/
	void YQAutoSetSpan(int, int);

	/*人为指定布孔方案*/
	void SetBridgeSpan(CString m_YQLeftSpanText, CString m_ZQSpanText, CString m_YQSpanRightText, double m_BridgeCenterStake);

	/*主梁梁长初步拟定*/
	void PreSetGirderLength(int index, double m_TGirderLen);

	/*主梁高度初步拟定*/
	void PreSetGirderHeight();

	/*创建主梁对象*/
	void CreateMainGirder();

	/*获取布孔方案及中心桩号字符串*/
	void GetBridgeLayOutText();

	/*获得桥梁全长*/
	double GetBrgAllLen();

	/*获得相邻主梁对象*/
	bool GetSideMainGirder(CMainGirder* src, CMainGirder* &r_LeftOne, CMainGirder* &r_RightOne);

	/*获得自动布孔方案全集*/
	vector<vector<CYNPoint2D>> GetBridgeSpanArrangeAll(){ return m_BridgeSpanArrangeAll; };

	/*字符串解析函数*/
	vector<double> ResolveStrToSpan(CString str); //解析跨径布置字符串

private:

	CString LSplit(CString str, TCHAR chr); //获取分隔符左边字符串
	CString RSplit(CString str, TCHAR chr); //获取分隔符右边字符串

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
