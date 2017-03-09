// DlgSectInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "../RigidFrame.h"
#include "../RigidFrameDoc.h"
#include "DlgSectInfo.h"
#include "BCGPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class SectNodeCallBack :public osg::NodeCallback
{
public:
	SectNodeCallBack(osg::ref_ptr<osg::Node> node) :m_node(node)
	{

	}
	~SectNodeCallBack(){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<osg::Group> root = dynamic_cast<osg::Group*>(node);
		unsigned int num = root->getNumChildren();
		for (unsigned int _index = 0; _index < num; _index++)
		{
			std::string strname = root->getChild(_index)->getName();
			if (strname == "SectLight1")
			{
				;
			}
			else
			{
				root->removeChild(root->getChild(_index));
				num--;
				_index--;
			}

		}
		root->addChild(m_node);
		//继续遍历
		traverse(node, nv);
	}
private:
	osg::ref_ptr<osg::Node> m_node;
};
// CDlgSectInfo 对话框

IMPLEMENT_DYNAMIC(CDlgSectInfo, CBCGPDialog)

CDlgSectInfo::CDlgSectInfo(CRigidFrameDoc* pDoc, CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgSectInfo::IDD, pParent)
, mPicture(nullptr)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	myDoc = pDoc;

	_root = new osg::Group;
	_callback = new SectionCallback(this);
}

CDlgSectInfo::~CDlgSectInfo()
{
}

void CDlgSectInfo::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS20, m_B);
	DDV_MinMaxDouble(pDX, m_B, 0.1, 6);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS4, m_h);
	DDV_MinMaxDouble(pDX, m_h, 0, .5);
	DDX_Control(pDX, IDC_COMBO2, m_CtrlDesElv);
	DDX_Control(pDX, IDC_COMBO1, m_CtrlSlptype);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS21, m_Wt);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS22, m_Wb);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS3, m_A);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS6, m_A1);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS7, m_A3);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS8, m_A4);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS9, m_A5);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS10, m_B1);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS11, m_B2);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS12, m_B3);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS13, m_B4);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS14, m_B5);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS15, m_B6);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS16, m_B7);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS18, m_B8);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS19, m_B9);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS, m_SlpVal);
	DDX_Text(pDX, IDC_EDIT_CENTERPOS2, m_Guardrail);
}


BEGIN_MESSAGE_MAP(CDlgSectInfo, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CDlgSectInfo::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS2, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS21, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS10, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS11, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS12, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS13, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS14, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS15, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS16, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS18, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS19, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS20, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS22, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS3, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS4, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS6, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS7, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS8, &CDlgSectInfo::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CENTERPOS9, &CDlgSectInfo::OnChangeEdit)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgSectInfo 消息处理程序


BOOL CDlgSectInfo::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 改变对话框上所有控件字体
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->SetFont(&theApp.m_fontRegular);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	// 按照文档内容给对话框控件赋初值
	m_CtrlDesElv.AddString(_T("设计中心线"));
	m_CtrlDesElv.AddString(_T("加宽前路基边缘"));
	m_CtrlDesElv.AddString(_T("中央分隔带边缘"));

	if (myDoc->RigidFrameBridgeExample->m_StdSect.m_LocElevation == _T("设计中心线"))
		m_CtrlDesElv.SetCurSel(0);
	else if (myDoc->RigidFrameBridgeExample->m_StdSect.m_LocElevation == _T("加宽前路基边缘"))
		m_CtrlDesElv.SetCurSel(1);
	else
		m_CtrlDesElv.SetCurSel(2);

	m_CtrlSlptype.AddString(_T("单向坡"));
	m_CtrlSlptype.AddString(_T("双向坡"));

	if (myDoc->RigidFrameBridgeExample->m_StdSect.m_SlpType == _T("单向坡"))
		m_CtrlSlptype.SetCurSel(0);
	else
		m_CtrlSlptype.SetCurSel(1);

	m_Wt = myDoc->RigidFrameBridgeExample->m_StdSect.m_TboxWidth;
	m_Wb = myDoc->RigidFrameBridgeExample->m_StdSect.m_DboxWidth;
	m_SlpVal = myDoc->RigidFrameBridgeExample->m_StdSect.m_SlpVal;
	m_Guardrail = myDoc->RigidFrameBridgeExample->m_StdSect.m_Guardrail;

	m_h = myDoc->RigidFrameBridgeExample->m_StdSect.m_H;
	m_A = myDoc->RigidFrameBridgeExample->m_StdSect.m_A;
	m_A1 = myDoc->RigidFrameBridgeExample->m_StdSect.m_A1;
	m_A3 = myDoc->RigidFrameBridgeExample->m_StdSect.m_A3;
	m_A4 = myDoc->RigidFrameBridgeExample->m_StdSect.m_A4;
	m_A5 = myDoc->RigidFrameBridgeExample->m_StdSect.m_A5;

	m_B = myDoc->RigidFrameBridgeExample->m_StdSect.m_B;
	m_B1 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B1;
	m_B2 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B2;
	m_B3 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B3;
	m_B4 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B4;
	m_B5 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B5;
	m_B6 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B6;
	m_B7 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B7;
	m_B8 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B8;
	m_B9 = myDoc->RigidFrameBridgeExample->m_StdSect.m_B9;
	UpdateData(FALSE);

	//mPicture = (CWnd*)this->GetDlgItem(IDC_TYPELIST);
	mPicture = new CWnd();
	mPicture->Create(NULL,NULL,WS_CHILD| WS_VISIBLE | WS_BORDER, CRect(20, 20,510, 440), this, ID_SET_PICTURE);
	if (mPicture->m_hWnd == NULL)
		AfxMessageBox(_T("sdfsdf"));
	theApp.GetViewManager()->AddView(((CWnd*)mPicture)->m_hWnd);
	//删除全部默认节点
	theApp.GetViewManager()->RemoveAllNode(mPicture->m_hWnd);
	//删除全部事务
	osgViewer::Viewer::EventHandlers ehs= (theApp.GetViewManager()->getView(mPicture->m_hWnd)->getEventHandlers());
	for (osgViewer::Viewer::EventHandlers::iterator iter = ehs.begin(); iter != ehs.end();iter++)
		theApp.GetViewManager()->getView(mPicture->m_hWnd)->removeEventHandler(*iter);
	//设置场景数据
	theApp.GetViewManager()->SetSceneData(mPicture->m_hWnd, this->getUpdateCallback());
	//设置操作器
	theApp.GetViewManager()->getView(mPicture->m_hWnd)->setCameraManipulator(new Contrl2DManipulator(this->getUpdateCallback()->m_model));
	theApp.GetViewManager()->Display(mPicture->m_hWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgSectInfo::OnDestroy()
{
	theApp.GetViewManager()->RemoveView(mPicture->m_hWnd);
	CBCGPDialog::OnDestroy();
}

void CDlgSectInfo::OnBnClickedOk()
{
	UpdateData(TRUE);
	myDoc->RigidFrameBridgeExample->m_StdSect.m_TboxWidth = m_Wt;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_DboxWidth = m_Wb;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_SlpVal = m_SlpVal;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_Guardrail = m_Guardrail;

	myDoc->RigidFrameBridgeExample->m_StdSect.m_H = m_h;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_A = m_A;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_A1 = m_A1;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_A3 = m_A3;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_A4 = m_A4;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_A5 = m_A5;

	myDoc->RigidFrameBridgeExample->m_StdSect.m_B = m_B;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B1 = m_B1;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B2 = m_B2;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B3 = m_B3;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B4 = m_B4;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B5 = m_B5;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B6 = m_B6;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B7 = m_B7;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B8 = m_B8;
	myDoc->RigidFrameBridgeExample->m_StdSect.m_B9 = m_B9;
	m_CtrlDesElv.GetLBText(m_CtrlDesElv.GetCurSel(), myDoc->RigidFrameBridgeExample->m_StdSect.m_LocElevation);
	m_CtrlSlptype.GetLBText(m_CtrlSlptype.GetCurSel(), myDoc->RigidFrameBridgeExample->m_StdSect.m_SlpType);
	CBCGPDialog::OnOK();
}

void CDlgSectInfo::OnChangeEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);

	_callback->Lock();
	_callback->Update();
	_callback->Unlock();
}

void CDlgSectInfo::CalcViewePosition()
{
}



void SectionCallback::DoUpdate()
{
	_root->removeChildren(0, _root->getNumChildren());
	osg::ref_ptr<osg::Group> group = new osg::Group;
	osg::ref_ptr<osg::MatrixTransform> mat = new osg::MatrixTransform;
	mat->setMatrix(osg::Matrix::rotate(osg::Quat(osg::PI, osg::Vec3(0.0f, 0.0f, 1.0f))));
	group->addChild(this->CreateSect());
	group->addChild(this->CreateNotes());
	mat->addChild(group);

	_root->addChild(this->CreateLight());
	_root->addChild(mat);

	osg::ref_ptr<osgGA::GUIEventAdapter> dummyEvent = _view->getEventQueue()->createEvent();
	_view->getCameraManipulator()->home(*dummyEvent, *_view);
}


osg::Geode* SectionCallback::CreateCoord()
{
	double Srange = _dlg->m_Wt / 1.8f;
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Geometry> gmCoord = new osg::Geometry;
	osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth;

	osg::ref_ptr<osg::Vec3Array> v_coord = new osg::Vec3Array;
	v_coord->push_back(osg::Vec3(-50.0f, 0.0f, 0.0f));
	v_coord->push_back(osg::Vec3(50.0f, 0.0f, 0.0f));
	v_coord->push_back(osg::Vec3(0.0f, 0.0f, -50.0f));
	v_coord->push_back(osg::Vec3(0.0f, 0.0f, 50.0f));

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(-Srange, 0.0f, -Srange));
	vertices->push_back(osg::Vec3(Srange, 0.0f, -Srange));
	vertices->push_back(osg::Vec3(Srange, 0.0f, Srange));
	vertices->push_back(osg::Vec3(-Srange, 0.0f, Srange));
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0f, 0.1f, 0.2f, 1.0f));
	osg::ref_ptr<osg::Vec4Array> c_coord = new osg::Vec4Array;
	c_coord->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));

	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4));
	gm->setVertexArray(vertices);
	gm->setColorArray(colors, osg::Array::BIND_OVERALL);


	gmCoord->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 4));
	gmCoord->setVertexArray(v_coord);
	gmCoord->setColorArray(c_coord, osg::Array::BIND_OVERALL);
	geode->addDrawable(gm);
	//geode->addDrawable(gmCoord);
	lineWidth->setWidth(0.05f);
	geode->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
	geode->setName("SectCoord");
	//geode->addChild(osgDB::readNodeFile("cow.osg"));
	return geode.release();
}

osg::Node* SectionCallback::CreateLight()
{
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(0);
	light->setPosition(osg::Vec4(0.0, -1.0, 0.0, 0.0f));
	light->setDirection(osg::Vec3(0.0f, 1.0f, 0.0f));
	//light->setDirection(osg::Vec3(-0.85f, 0.9f, -0.8f));
	light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//light->setSpecular(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light.get());
	lightSource->setName("SectLight1");
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	return lightSource.release();
}

osg::Node* SectionCallback::CreateSect()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	//创建外轮廓
	osg::ref_ptr<osg::Geode> out_gd = new osg::Geode;
	osg::ref_ptr<osg::Geometry> out_gm = new osg::Geometry;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.2f, 1.0f, 1.0f, 1.0f));

	out_gm->setVertexArray(this->CalcOuter(0.0f, 3.0f, 5.0f));
	out_gm->setColorArray(colors, osg::Array::BIND_OVERALL);
	out_gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 11));

	out_gd->addDrawable(out_gm);

	//创建内轮廓
	osg::ref_ptr<osg::Geode> inner_gd = new osg::Geode;
	osg::ref_ptr<osg::Geometry> inner_gm = new osg::Geometry;

	inner_gm->setVertexArray(this->CalcInner(0.0f, 3.0f, 5.0f));
	inner_gm->setColorArray(colors, osg::Array::BIND_OVERALL);
	inner_gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 11));

	inner_gd->addDrawable(inner_gm);

	//返回内外轮廓图形
	geode->addChild(out_gd);
	geode->addChild(inner_gd);
	geode->setName("Sect");
	return geode.release();
}

osg::Vec3Array* SectionCallback::CalcOuter(double centerX, double centerZ, double height)
{
	//m_Outbz = new osg::Vec3Array;
	m_OuterVertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> outbz = new osg::Geode;
	double x = 0.0, y = 0.0, z = 0.0;

	if (m_OuterVertices->size() != 11)
		m_OuterVertices->resize(11);


	m_OuterVertices->at(0) = osg::Vec3(x, centerX, centerZ);
	//m_Outbz->at(0) = osg::Vec3(x, centerX, centerZ);


	x = centerX + _dlg->m_Wt / 2;
	z = centerZ + _dlg->m_Wt / 2 * _dlg->m_SlpVal;
	m_OuterVertices->at(1) = osg::Vec3(x, y, z);

	z -= _dlg->m_A;
	m_OuterVertices->at(2) = osg::Vec3(x, y, z);

	x -= _dlg->m_A1;
	z -= _dlg->m_B1;
	m_OuterVertices->at(3) = osg::Vec3(x, y, z);
	//m_Outbz->at(1) = osg::Vec3(x, y, z);

	x = centerX + _dlg->m_Wb / 2;
	z -= _dlg->m_B2;
	m_OuterVertices->at(4) = osg::Vec3(x, y, z);

	x = centerX - _dlg->m_Wt / 2;
	z = centerZ - _dlg->m_Wt / 2 * _dlg->m_SlpVal;
	m_OuterVertices->at(10) = osg::Vec3(x, y, z);

	z -= _dlg->m_A;
	m_OuterVertices->at(9) = osg::Vec3(x, y, z);

	x += _dlg->m_A1;
	z -= _dlg->m_B8;
	m_OuterVertices->at(8) = osg::Vec3(x, y, z);

	x = centerX - _dlg->m_Wb / 2;
	z -= _dlg->m_B7;
	m_OuterVertices->at(7) = osg::Vec3(x, y, z);

	if (_dlg->m_SlpVal < 0)
	{
		z = centerZ + _dlg->m_Wb / 2 * _dlg->m_SlpVal - height;

	}
	else
	{
		z = centerZ - _dlg->m_Wb / 2 * _dlg->m_SlpVal - height;
	}
	m_OuterVertices->at(5) = osg::Vec3(centerX + _dlg->m_Wb / 2, y, z);
	m_OuterVertices->at(6) = osg::Vec3(centerX - _dlg->m_Wb / 2, y, z);

	return m_OuterVertices;
}

osg::Vec3Array* SectionCallback::CalcInner(double centerX, double centerZ, double height)
{
	m_InnerVertices = new osg::Vec3Array;
	if (m_InnerVertices->size() != 11)
		m_InnerVertices->resize(11);

	double x = 0.0, y = 0.0, z = 0.0;
	double fbh = 0.5;
	double dbh = 0.5;
	double bottom = centerZ - _dlg->m_Wb / 2 * fabs(_dlg->m_SlpVal) - height + dbh;

	m_InnerVertices->at(0) = osg::Vec3(x, y, centerZ - _dlg->m_h);

	x = centerX + _dlg->m_B / 2;
	z = centerZ - _dlg->m_h + _dlg->m_B / 2 * _dlg->m_SlpVal;
	m_InnerVertices->at(1) = osg::Vec3(x, y, z);

	x += _dlg->m_A4;
	z -= _dlg->m_B4;
	m_InnerVertices->at(2) = osg::Vec3(x, y, z);

	z = z - _dlg->m_B3 / _dlg->m_A3 * (centerX + _dlg->m_Wb / 2 - fbh - x);
	x = centerX + _dlg->m_Wb / 2 - fbh;
	m_InnerVertices->at(3) = osg::Vec3(x, y, z);

	z = bottom + _dlg->m_A5;
	m_InnerVertices->at(4) = osg::Vec3(x, y, z);
	m_InnerVertices->at(7) = osg::Vec3(centerX - _dlg->m_Wb / 2 + fbh, y, z);

	m_InnerVertices->at(5) = osg::Vec3(centerX + _dlg->m_Wb / 2 - fbh - _dlg->m_B9, y, bottom);
	m_InnerVertices->at(6) = osg::Vec3(centerX - _dlg->m_Wb / 2 + fbh + _dlg->m_B9, y, bottom);

	x = centerX - _dlg->m_B / 2;
	z = centerZ - _dlg->m_h - _dlg->m_B / 2 * _dlg->m_SlpVal;
	m_InnerVertices->at(10) = osg::Vec3(x, y, z);

	x -= _dlg->m_A4;
	z -= _dlg->m_B5;
	m_InnerVertices->at(9) = osg::Vec3(x, y, z);

	z = z - _dlg->m_B6 / _dlg->m_A3 * (x - centerX + _dlg->m_Wb / 2 - fbh);
	x = centerX - _dlg->m_Wb / 2 + fbh;
	m_InnerVertices->at(8) = osg::Vec3(x, y, z);

	return m_InnerVertices;
}

osg::Group* SectionCallback::CreateNotes()
{
	osg::ref_ptr<osg::Group> notes = new osg::Group;
	notes->setName("Notes");
	double x, y, z;
	//外圈

	//画标注H
	osg::ref_ptr<osg::Geode> h_geode = new osg::Geode;
	osg::ref_ptr<osg::Vec3Array> h_vertex = new osg::Vec3Array;
	h_vertex->push_back(m_OuterVertices->at(0));

	osg::Vec3 second(m_OuterVertices->at(0).x() + 1.0, m_OuterVertices->at(0).y(), m_OuterVertices->at(0).z() + 0.5);
	h_vertex->push_back(second);
	h_vertex->push_back(m_InnerVertices->at(0));
	osg::Vec3 four(m_InnerVertices->at(0).x() + 1.0, m_InnerVertices->at(0).y(), m_InnerVertices->at(0).z() + 0.5);
	h_vertex->push_back(four);
	h_geode->addDrawable(this->CreateNoteLine(h_vertex, 4));

	x = (second.x() + four.x()) / 2.0 + 0.2f;
	y = 0.0f;
	z = (second.z() + four.z()) / 2.0;
	h_geode->addDrawable(this->CreateText(L"H", osg::Vec3(x, y, z)));


	//画标注A
	osg::ref_ptr<osg::Geode> A_geode = new osg::Geode;
	osg::ref_ptr<osg::Vec3Array> A_vertex = new osg::Vec3Array;

	A_vertex->push_back(m_OuterVertices->at(1));
	x = m_OuterVertices->at(1).x() + 0.6f;
	y = 0.0f;
	z = m_OuterVertices->at(1).z() + 0.5f;
	osg::Vec3 ALineSecond(x, y, z);
	A_vertex->push_back(ALineSecond);
	A_vertex->push_back(m_OuterVertices->at(2));
	x = m_OuterVertices->at(2).x() + 0.6f;
	y = 0.0f;
	z = m_OuterVertices->at(2).z() + 0.5f;
	osg::Vec3 ALineFour(x, y, z);
	A_vertex->push_back(ALineFour);
	A_geode->addDrawable(this->CreateNoteLine(A_vertex, 4));

	x = (ALineSecond.x() + ALineFour.x()) / 2.0f + 0.2f;
	y = 0.0f;
	z = (ALineSecond.z() + ALineFour.z()) / 2.0f;
	A_geode->addDrawable(this->CreateText(L"A", osg::Vec3(x, y, z)));


	//画标注 A1XB1
	osg::ref_ptr<osg::Vec3Array> A1B1_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A1B1_geode = new osg::Geode;
	x = (m_OuterVertices->at(2).x() + m_OuterVertices->at(3).x()) / 2.0f;
	y = 0.0f;
	z = (m_OuterVertices->at(2).z() + m_OuterVertices->at(3).z()) / 2.0f;
	A1B1_vertex->push_back(osg::Vec3(x, y, z));
	x += 0.6f;
	z -= 0.5f;
	A1B1_vertex->push_back(osg::Vec3(x, y, z));
	A1B1_vertex->push_back(osg::Vec3(x, y, z));
	x += 1.5f;
	A1B1_vertex->push_back(osg::Vec3(x, y, z));
	A1B1_geode->addDrawable(this->CreateNoteLine(A1B1_vertex, 4));
	A1B1_geode->addDrawable(this->CreateText(L"A1×B1", osg::Vec3(x - 0.7, y, z + 0.25)));


	//画标注A2XB2
	osg::ref_ptr<osg::Vec3Array> A2B2_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A2B2_geode = new osg::Geode;
	x = (m_OuterVertices->at(3).x() + m_OuterVertices->at(4).x()) / 2.0f;
	y = 0.0f;
	z = (m_OuterVertices->at(3).z() + m_OuterVertices->at(4).z()) / 2.0f;
	A2B2_vertex->push_back(osg::Vec3(x, y, z));
	x += 0.2f;
	z -= 0.8f;
	A2B2_vertex->push_back(osg::Vec3(x, y, z));
	A2B2_vertex->push_back(osg::Vec3(x, y, z));
	x += 1.5f;
	A2B2_vertex->push_back(osg::Vec3(x, y, z));
	A2B2_geode->addDrawable(this->CreateNoteLine(A2B2_vertex, 4));
	A2B2_geode->addDrawable(this->CreateText(L"A2×B2", osg::Vec3(x - 0.7, y, z + 0.25)));


	//画标注A2XB7
	osg::ref_ptr<osg::Vec3Array> A2B7_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A2B7_geode = new osg::Geode;
	x = (m_OuterVertices->at(7).x() + m_OuterVertices->at(8).x()) / 2.0f;
	y = 0.0f;
	z = (m_OuterVertices->at(7).z() + m_OuterVertices->at(8).z()) / 2.0f;
	A2B7_vertex->push_back(osg::Vec3(x, y, z));
	x -= 0.2f;
	z -= 0.8f;
	A2B7_vertex->push_back(osg::Vec3(x, y, z));
	A2B7_vertex->push_back(osg::Vec3(x, y, z));
	x -= 1.5f;
	A2B7_vertex->push_back(osg::Vec3(x, y, z));
	A2B7_geode->addDrawable(this->CreateNoteLine(A2B7_vertex, 4));
	A2B7_geode->addDrawable(this->CreateText(L"A2×B7", osg::Vec3(x + 0.7, y, z + 0.25)));

	//画标注A1XB8
	osg::ref_ptr<osg::Vec3Array> A1B8_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A1B8_geode = new osg::Geode;
	x = (m_OuterVertices->at(8).x() + m_OuterVertices->at(9).x()) / 2.0f;
	y = 0.0f;
	z = (m_OuterVertices->at(8).z() + m_OuterVertices->at(9).z()) / 2.0f;
	A1B8_vertex->push_back(osg::Vec3(x, y, z));
	x -= 0.6f;
	z -= 0.8f;
	A1B8_vertex->push_back(osg::Vec3(x, y, z));
	A1B8_vertex->push_back(osg::Vec3(x, y, z));
	x -= 1.5f;
	A1B8_vertex->push_back(osg::Vec3(x, y, z));
	A1B8_geode->addDrawable(this->CreateNoteLine(A1B8_vertex, 4));
	A1B8_geode->addDrawable(this->CreateText(L"A1×B8", osg::Vec3(x + 0.7, y, z + 0.25)));

	//内圈
	//画标注B
	osg::ref_ptr<osg::Vec3Array> B_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> B_geode = new osg::Geode;
	B_vertex->push_back(m_InnerVertices->at(1));
	double x1, y1, z1;
	x1 = m_InnerVertices->at(1).x();
	y1 = 0.0f;
	z1 = m_InnerVertices->at(1).z() - 0.4f;
	B_vertex->push_back(osg::Vec3(x1, y1, z1));
	B_vertex->push_back(m_InnerVertices->at(10));
	x = m_InnerVertices->at(10).x();
	y = 0.0f;
	z = m_InnerVertices->at(10).z() - 0.4f;
	B_vertex->push_back(osg::Vec3(x, y, z));
	B_vertex->push_back(osg::Vec3(x, y, z));
	B_vertex->push_back(osg::Vec3(x1, y1, z1));
	B_geode->addDrawable(this->CreateNoteLine(B_vertex, 6));
	x = m_InnerVertices->at(0).x();
	y = 0.0f;
	z = m_InnerVertices->at(0).z() - 0.2f;
	B_geode->addDrawable(this->CreateText(L"B", osg::Vec3(x, y, z)));

	//画标注A4XB4
	osg::ref_ptr<osg::Vec3Array> A4B4_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A4B4_geode = new osg::Geode;
	x = (m_InnerVertices->at(1).x() + m_InnerVertices->at(2).x()) / 2.0f;
	y = 0.0f;
	z = (m_InnerVertices->at(1).z() + m_InnerVertices->at(2).z()) / 2.0f;
	A4B4_vertex->push_back(osg::Vec3(x, y, z));
	x += 1.5f;
	z += 0.8f;
	A4B4_vertex->push_back(osg::Vec3(x, y, z));
	A4B4_vertex->push_back(osg::Vec3(x, y, z));
	x += 1.5f;
	A4B4_vertex->push_back(osg::Vec3(x, y, z));
	A4B4_geode->addDrawable(this->CreateNoteLine(A4B4_vertex, 4));
	A4B4_geode->addDrawable(this->CreateText(L"A4×B4", osg::Vec3(x - 0.7, y, z + 0.25)));

	//画标注A3XB3
	osg::ref_ptr<osg::Vec3Array> A3B3_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A3B3_geode = new osg::Geode;
	x = (m_InnerVertices->at(2).x() + m_InnerVertices->at(3).x()) / 2.0f;
	y = 0.0f;
	z = (m_InnerVertices->at(2).z() + m_InnerVertices->at(3).z()) / 2.0f;
	A3B3_vertex->push_back(osg::Vec3(x, y, z));
	x += 0.3f;
	z -= 0.7f;
	A3B3_vertex->push_back(osg::Vec3(x, y, z));
	A3B3_vertex->push_back(osg::Vec3(x, y, z));
	x -= 1.5f;
	A3B3_vertex->push_back(osg::Vec3(x, y, z));
	A3B3_geode->addDrawable(this->CreateNoteLine(A3B3_vertex, 4));
	A3B3_geode->addDrawable(this->CreateText(L"A3×B3", osg::Vec3(x + 0.6, y, z + 0.25)));

	//画标注A5XB9
	osg::ref_ptr<osg::Vec3Array> A5B9_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A5B9_geode = new osg::Geode;
	x = (m_InnerVertices->at(4).x() + m_InnerVertices->at(5).x()) / 2.0f;
	y = 0.0f;
	z = (m_InnerVertices->at(4).z() + m_InnerVertices->at(5).z()) / 2.0f;
	A5B9_vertex->push_back(osg::Vec3(x, y, z));
	x -= 0.2f;
	z += 0.4f;
	A5B9_vertex->push_back(osg::Vec3(x, y, z));
	A5B9_vertex->push_back(osg::Vec3(x, y, z));
	x -= 1.5f;
	A5B9_vertex->push_back(osg::Vec3(x, y, z));
	A5B9_geode->addDrawable(this->CreateNoteLine(A5B9_vertex, 4));
	A5B9_geode->addDrawable(this->CreateText(L"A5×B9", osg::Vec3(x + 0.7, y, z + 0.25)));

	//画标注A3XB6
	osg::ref_ptr<osg::Vec3Array> A3B6_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A3B6_geode = new osg::Geode;
	x = (m_InnerVertices->at(8).x() + m_InnerVertices->at(9).x()) / 2.0f;
	y = 0.0f;
	z = (m_InnerVertices->at(8).z() + m_InnerVertices->at(9).z()) / 2.0f;
	A3B6_vertex->push_back(osg::Vec3(x, y, z));
	x -= 0.3f;
	z -= 0.7f;
	A3B6_vertex->push_back(osg::Vec3(x, y, z));
	A3B6_vertex->push_back(osg::Vec3(x, y, z));
	x += 1.5f;
	A3B6_vertex->push_back(osg::Vec3(x, y, z));
	A3B6_geode->addDrawable(this->CreateNoteLine(A3B6_vertex, 4));
	A3B6_geode->addDrawable(this->CreateText(L"A3×B6", osg::Vec3(x - 0.5, y, z + 0.25)));

	//画标注A4XB5
	osg::ref_ptr<osg::Vec3Array> A4B5_vertex = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> A4B5_geode = new osg::Geode;
	x = (m_InnerVertices->at(10).x() + m_InnerVertices->at(9).x()) / 2.0f;
	y = 0.0f;
	z = (m_InnerVertices->at(10).z() + m_InnerVertices->at(9).z()) / 2.0f;
	A4B5_vertex->push_back(osg::Vec3(x, y, z));
	x -= 1.5f;
	z += 0.8f;
	A4B5_vertex->push_back(osg::Vec3(x, y, z));
	A4B5_vertex->push_back(osg::Vec3(x, y, z));
	x -= 1.5f;
	A4B5_vertex->push_back(osg::Vec3(x, y, z));
	A4B5_geode->addDrawable(this->CreateNoteLine(A4B5_vertex, 4));
	A4B5_geode->addDrawable(this->CreateText(L"A4×B5", osg::Vec3(x + 0.7, y, z + 0.25)));

	//外圈标注节点
	notes->addChild(h_geode);
	notes->addChild(A_geode);
	notes->addChild(A1B1_geode);
	notes->addChild(A2B2_geode);
	notes->addChild(A2B7_geode);
	notes->addChild(A1B8_geode);
	//内圈标注节点
	notes->addChild(B_geode);
	notes->addChild(A4B4_geode);
	notes->addChild(A3B3_geode);
	notes->addChild(A5B9_geode);
	notes->addChild(A3B6_geode);
	notes->addChild(A4B5_geode);
	/*osg::ref_ptr<osg::MatrixTransform> mat = new osg::MatrixTransform;
	mat->setMatrix(osg::Matrix::rotate(osg::Quat(osg::PI, osg::Vec3(0.0f, 0.0f, 1.0f))));
	mat->addChild(notes);*/
	return notes.release();
}

osgText::Text* SectionCallback::CreateText(const wchar_t* text, osg::Vec3 position)
{
	osg::ref_ptr<osgText::Text> Th = new osgText::Text;
	Th->setText(text);
	Th->setFont("C:\\Windows\\Fonts\\simsun.ttc");
	Th->setCharacterSize(0.5f);
	Th->setColor(osg::Vec4(5.0f, 5.0f, 5.0f, 1.0f));
	Th->setPosition(position);
	Th->setRotation(osg::Quat(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f))*osg::Quat(osg::PI, osg::Vec3(0.0f, 0.0f, 1.0f)));
	//Th->setAxisAlignment(osgText::Text::XZ_PLANE);
	Th->setAlignment(osgText::Text::CENTER_CENTER);
	return Th.release();
}

osg::Geometry* SectionCallback::CreateNoteLine(osg::ref_ptr<osg::Vec3Array> vertices, int vertexCount)
{
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	//标注颜色，统一红色
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	gm->setColorArray(colors, osg::Array::BIND_OVERALL);
	gm->setVertexArray(vertices);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertexCount));
	return gm.release();
}
