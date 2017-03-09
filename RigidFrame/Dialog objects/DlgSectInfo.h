#pragma once
#include "afxwin.h"
#include <YjOSG/YjOSG.h>

// CDlgSectInfo 对话框
class SectionCallback;
class CDlgSectInfo : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSectInfo)

public:
	CDlgSectInfo(CRigidFrameDoc* pDoc, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSectInfo();

	// 对话框数据
	enum { IDD = IDD_DLG_SECT };
	CRigidFrameDoc* myDoc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
private:
	CWnd* mPicture;
	osg::ref_ptr<osg::Group> _root;
	SectionCallback* _callback;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	double m_B;
	double m_h;
	CBCGPComboBox m_CtrlDesElv;
	CBCGPComboBox m_CtrlSlptype;
	double m_Wt;
	double m_Wb;
	double m_A;
	double m_A1;
	double m_A3;
	double m_A4;
	double m_A5;
	double m_B1;
	double m_B2;
	double m_B3;
	double m_B4;
	double m_B5;
	double m_B6;
	double m_B7;
	double m_B8;
	double m_B9;
	double m_SlpVal;
	double m_Guardrail;

	void CalcViewePosition();
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeEdit();

	osg::Group* getRoot() const { return _root; }
	SectionCallback* getUpdateCallback() const { return _callback; }
	afx_msg void OnDestroy();
};


class SectionCallback : public ComponentCallback
{
public:
	SectionCallback(CDlgSectInfo* dlg)
		: ComponentCallback(dlg->getRoot())
		, _dlg(dlg)
	{
		m_model = dynamic_cast<osg::Group*>(this->CreateSect());
	}

	void DoUpdate() override;
	osg::ref_ptr <osg::Group> m_model;

private:
	osg::Geode* CreateCoord();//创建坐标轴
	osg::Node* CreateLight();//创建光源
	osg::Node* CreateSect();//创建节段
	osg::Vec3Array* CalcOuter(double centerX, double centerZ, double height);//计算外框点集
	osg::Vec3Array* CalcInner(double centerX, double centerZ, double height);//计算内框点集
	osg::Group* CreateNotes();//创建注释
	osgText::Text* CreateText(const wchar_t* text, osg::Vec3 position);//创建字体
	osg::Geometry* CreateNoteLine(osg::ref_ptr<osg::Vec3Array> vertices, int vertexCount);//创建标注线

private:
	CDlgSectInfo* _dlg;
	osg::ref_ptr<osg::Vec3Array> m_OuterVertices;//保存外框点集
	osg::ref_ptr<osg::Vec3Array> m_InnerVertices;//保存内框点集
};