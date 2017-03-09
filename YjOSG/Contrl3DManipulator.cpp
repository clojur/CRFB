#include "stdafx.h"
#include "Contrl3DManipulator.h"
#include "cOSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Contrl3DManipulator::Contrl3DManipulator(ViewManager* mgr, HWND hWnd, osg::Group* root) :
first(true),
_viewMgr(mgr),
_hWnd(hWnd),
m_model(root),
m_ScaleSpeed(false),
mX(0.0f),
mY(0.0f),
mEnableDragger(true),
m_LineMode(false)
{
}


Contrl3DManipulator::~Contrl3DManipulator()
{
	//	delete mCosg;
}

osg::Matrixd Contrl3DManipulator::getMatrix() const
{
	return osg::Matrixd::translate(_center)*
		osg::Matrixd::rotate(_rotation) *
		osg::Matrixd::translate(0., 0., _distance) ;
}

osg::Matrixd Contrl3DManipulator::getInverseMatrix() const
{
	return osg::Matrixd::translate(-_center) *
		osg::Matrixd::rotate(_rotation.inverse()) *
		osg::Matrixd::translate(0.0, 0.0, -_distance);
}

void Contrl3DManipulator::setByMatrix(const osg::Matrixd& matrix)
{
	_center = osg::Vec3d(0., 0., -_distance) * matrix;
	_rotation = matrix.getRotate();

	// fix current rotation
	if (getVerticalAxisFixed())
		fixVerticalAxis(_center, _rotation, true);
}

void Contrl3DManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{
	setByMatrix(osg::Matrix::inverse(matrix));
}

bool Contrl3DManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	osgViewer::Viewer* viewer = dynamic_cast <osgViewer::Viewer*>(&aa);
	osgViewer::Viewer::Windows ws;
	if (first)
	{
		ChangeMouseCursor(viewer, ws, osgViewer::GraphicsWindow::CrosshairCursor);
		FirstInto();
		first = false;
	}

	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::DRAG) :
	{
		if (osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON == ea.getButtonMask() && osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL == ea.getModKeyMask())
		{
			//更改鼠标样式
			ChangeMouseCursor(viewer, ws, osgViewer::GraphicsWindow::CrosshairCursor);
			//旋转操作
			return RotateOperate(ea, aa);
		}
		else if (osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON == ea.getButtonMask())
		{
			//更改鼠标样式
			ChangeMouseCursor(viewer, ws, osgViewer::GraphicsWindow::SprayCursor);
			return handleMouseDrag(ea, aa);
		}
		else
			break;
	}
	case(osgGA::GUIEventAdapter::PUSH) :
	{
		mX = ea.getX();
		mY = ea.getY();
		if (osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON == ea.getButtonMask())
		{
			pickModelFrame(ea, aa);
		}
		flushMouseEventStack();
		return false;
	}
	case osgGA::GUIEventAdapter::RELEASE:
	{
		//更改鼠标样式
		ChangeMouseCursor(viewer, ws, osgViewer::GraphicsWindow::CrosshairCursor);
		break;
	}
	case osgGA::GUIEventAdapter::KEYDOWN:
	{
		if (ea.getKey() == 'x' || ea.getKey() == 'X')//按下拾取
		{
			pickModelDrag(ea, aa);
			mEnableDragger = !mEnableDragger;
		}
		if (ea.getKey() == 'm' || ea.getKey() == 'M')//导出导入模型
		{
			LoadInAndOutModel();
		}
		if (ea.getKey() == 'z' || ea.getKey() == 'Z')//还原场景
		{
			_viewMgr->SetSceneData(_hWnd, m_Oldmodel);
		}
		if (ea.getKey() == 'l' || ea.getKey() == 'L')//绘制线形模型
		{
			DrawLineModel();
		}
	}
	case osgGA::GUIEventAdapter::KEYUP:
	{
	}
	case(osgGA::GUIEventAdapter::DOUBLECLICK) :
	{
		if (osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON == ea.getButton())
		{
			home(ea, aa);
			/*_center = m_OldCenter;
			_distance = m_OldDistance;
			_rotation = m_OldRotation;*/
			m_ScaleSpeed = false;
		}
		return true;
	}
	case(osgGA::GUIEventAdapter::SCROLL) :
	{
		InitScroll(viewer, ea);
		//char str[125] = {0};
		//sprintf_s(str, 120, "m_distance: %f \n",m_distance);
		if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
		{
			if (m_distance < 0.115)
				return true;
			else if (m_distance < 5.0f)
				m_ScaleSpeed = true;
			else
				;
			ScrollUp();
			//OutputDebugStringA(str);
		}
		else if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
		{

			if (m_distance > m_model->getBound().radius2())
				return true;
			else if (m_distance > m_model->getBound().radius() / 4.0f)
				m_ScaleSpeed = false;
			else
				;
			ScrollDown();
		}
		return true;
	}
	default:
		return false;

	}
	return false;
}

void Contrl3DManipulator::pickModelDrag(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*> (&aa);

	osgUtil::LineSegmentIntersector::Intersections hits;
	if (view->computeIntersections(ea.getX(), ea.getY(), hits))
	{
		osgUtil::LineSegmentIntersector::Intersection intersection = *hits.begin();
		osg::NodePath& nodePath = intersection.nodePath;
		int nNodeSize = static_cast<int> (nodePath.size());

		if (nNodeSize > 0)
		{
			osg::Node* node = nodePath[nNodeSize - 1];
			osg::Node* grandParent = node->getParent(0)->getParent(0);

			// This method maybe not right?
			PickModel* shape = dynamic_cast<PickModel*> (grandParent);
			if (shape)
			{
				if (mEnableDragger)
				{
					shape->EnableDragger();
				}
				else
				{ 
					shape->DisableDragger();
				}
			}

		}
	}

}

void Contrl3DManipulator::InitScroll(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea)
{
	//获取相机各个分量值
	m_mat = osg::Matrixd::translate(-_center) *
		osg::Matrixd::rotate(_rotation.inverse()) *
		osg::Matrixd::translate(0.0, 0.0, -_distance);
	m_mat.getLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);


	//每次鼠标滚动都获取实时鼠标停留的世界坐标
	MouseWorld = getCurrentMouseWorldCoord(viewer,ea.getX(),ea.getY(),0.0f);

	//计算鼠标停留点与相机的法向量（相机位置->停留点）
	m_viewdirection = MouseWorld - m_ViewerPosition;

	//归一化向量
	m_viewdirection.normalize();

	//获取相机与鼠标停留点的距离
	m_distance = (m_ViewerPosition - MouseWorld).length();
}

void Contrl3DManipulator::ScrollUp()
{
	if (!m_ScaleSpeed)
	{
		if (m_distance > 50.0f)
		{
			//实时改变相机位置与相机视点
			m_ViewerPosition += m_viewdirection*(m_distance / 10.0f);
			//视点中心调优
			m_CenterPosition += m_viewdirection*(m_distance / 10.0f);
			_distance -= (m_viewdirection*(m_distance / 10.0f)).length();

			osg::Matrix mat;
			mat.makeLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);
			setByInverseMatrix(mat);
		}
		else
		{
			//实时改变相机位置与相机视点
			m_ViewerPosition += m_viewdirection*(m_distance);
			//视点中心调优
			m_CenterPosition += m_viewdirection*(m_distance);
			_distance -= (m_viewdirection*(m_distance)).length();

			osg::Matrix mat;
			mat.makeLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);
			setByInverseMatrix(mat);
		}
		
	}
	else
	{
		//实时改变相机位置与相机视点
		m_ViewerPosition += m_viewdirection*3.0;
		//视点中心调优
		m_CenterPosition += m_viewdirection*3.0;
		_distance -= (m_viewdirection * 3).length();
		osg::Matrix mat;
		mat.makeLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);
		setByInverseMatrix(mat);
	}
}

void Contrl3DManipulator::ScrollDown()
{
	if (!m_ScaleSpeed)
	{
		//实时改变相机位置与相机视点
		m_ViewerPosition -= m_viewdirection*(m_distance / 10.0f);
		//视点中心调优
		m_CenterPosition -= m_viewdirection*(m_distance / 10.0f);
		_distance += (m_viewdirection*(m_distance / 10.0f)).length();

		osg::Matrix mat;
		mat.makeLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);
		setByInverseMatrix(mat);
	}
	else
	{
		//实时改变相机位置与相机视点
		m_ViewerPosition -= m_viewdirection*20.0f;
		//视点中心调优
		m_CenterPosition -= m_viewdirection* 20.0f;
		_distance += (m_viewdirection* 20.0f).length();
		osg::Matrix mat;
		mat.makeLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);
		setByInverseMatrix(mat);
	}
}

void Contrl3DManipulator::pickModelFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	osg::Matrix ViewMatrix = viewer->getCamera()->getViewMatrix();
	osg::Matrix ProjectionMatrix = viewer->getCamera()->getProjectionMatrix();
	osg::Matrix WindowMatrix = viewer->getCamera()->getViewport()->computeWindowMatrix();

	osg::Matrix VPW = ViewMatrix*ProjectionMatrix*WindowMatrix;
	osg::Vec3 nearPoint = osg::Vec3(ea.getX(), ea.getY(), 0.0f)*osg::Matrix::inverse(VPW);
	osg::Vec3 farPoint = osg::Vec3(ea.getX(), ea.getY(), 1.0f)*osg::Matrix::inverse(VPW);



	osg::ref_ptr<osgUtil::LineSegmentIntersector> lineSegment = new osgUtil::LineSegmentIntersector(nearPoint, farPoint);
	osgUtil::IntersectionVisitor iv(lineSegment);


	//让CRoadData隐藏
	osg::ref_ptr<osg::Node> Road;
	osg::Group* model = dynamic_cast<osg::Group*>(_viewMgr->GetSceneData(_hWnd));
	unsigned int childCount = model->getNumChildren();
	for (unsigned int i = 0; i < childCount; ++i)
	{
		if (model->getChild(i)->getName() == "CRigidFrameBridge")
		{
			osg::Node* node = model->getChild(i);
			osg::ref_ptr<osg::Group> node_root = dynamic_cast<osg::Group*>(node);
			for (unsigned int j = 0; j < node_root->getNumChildren(); j++)
			{
				if (node_root->getChild(j)->getName() == "CRoadData")
				{
					Road = node_root->getChild(j);
					Road->setNodeMask(false);
				}
			}
		}
	}
	viewer->getSceneData()->accept(iv);
	//CRoadData展示
	if (Road != NULL)
		Road->setNodeMask(true);

	osgUtil::LineSegmentIntersector::Intersections intersections = lineSegment->getIntersections();

	unsigned int num = intersections.size();
	osg::Node* node=NULL;
	osg::ref_ptr<osg::Group> parent;
	if (num >= 2)
	{
		osg::NodePath nodePath = intersections.begin()->nodePath;
		if (nodePath.size() >= 2)
		{
			node = nodePath[nodePath.size() - 1];
			osg::ref_ptr<CClassGeode> n =dynamic_cast<CClassGeode*>(node);
			n->GetModelAttrib();
			parent = dynamic_cast<osg::Group*> (nodePath[nodePath.size() - 2]);
		}
	}
	//外边框模式
	/*osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline;
	outline->addChild(node);
	outline->setColor(osg::Vec4(1.0f, 0.6f, 0.0f, 1.0f));
	outline->setWidth(4.0f);
	parent->replaceChild(node, outline);*/

	//刻画线
	osg::ref_ptr<osgFX::Scribe> scribe;
	if (m_OldGroup != NULL)
	{
		m_OldParent->replaceChild(m_OldGroup, m_OldGroup->getChild(0));
	}
	if (node != NULL)
	{
		scribe = new osgFX::Scribe;
		scribe->setWireframeColor(osg::Vec4(1.0f, 0.0f, 0.0f, 0.0f));
		scribe->setWireframeLineWidth(2.0f);
		scribe->addChild(node);
		scribe->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		parent->replaceChild(node, scribe);
	}	
	//选中透明物体
	if (m_OldNode != NULL)
	{
		m_OldNode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::OFF);
		m_OldNode->getOrCreateStateSet()->setRenderBinDetails(-1, "RenderBin");
	}
	if (node != NULL)
	{
		osg::ref_ptr<osg::StateSet> state = node->getOrCreateStateSet();
		//打开混合融合模式  
		state->setMode(GL_BLEND, osg::StateAttribute::ON);
		state->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
		state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		//使用BlendFunc实现透明效果  
		osg::BlendColor* bc = new osg::BlendColor(osg::Vec4(1.0, 1.0, 1.0, 0.0));
		osg::BlendFunc*bf = new osg::BlendFunc();
		state->setAttributeAndModes(bf, osg::StateAttribute::ON);
		state->setAttributeAndModes(bc, osg::StateAttribute::ON);
		bf->setSource(osg::BlendFunc::CONSTANT_ALPHA);
		bf->setDestination(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
		bc->setConstantColor(osg::Vec4(0.0f, 0.0f, 1.0f, 0.5f));
	}
	//保存节点
	m_OldNode = node;
	m_OldParent = parent;
	m_OldGroup = scribe;

	//线框模式
	//if (m_OldNode != NULL)
	//{
	//	osg::ref_ptr<osg::StateSet> stateset = m_OldNode->getOrCreateStateSet();
	//	osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode();
	//	polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
	//	stateset->setAttribute(polygonMode, osg::StateAttribute::ON);
	//}
	//if (node != NULL)
	//{
	//	osg::ref_ptr<osg::StateSet> stateset = node->getOrCreateStateSet();
	//	osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode();
	//	polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	//	stateset->setAttribute(polygonMode, osg::StateAttribute::ON);
	//}
}

void Contrl3DManipulator::FirstInto()
{
	_distance = m_model->getBound().radius()*1.8;
	if (_distance < 0)
	{
		osg::Matrix m;
		m.makeRotate(90.0f, osg::Vec3(1.0, 0.0, 0.0));
		setByMatrix(m);
	}
	m_OldCenter = _center;
	m_OldDistance = _distance;
	m_OldRotation = _rotation;
}

void Contrl3DManipulator::ChangeMouseCursor(osgViewer::Viewer* viewer, osgViewer::Viewer::Windows ws, osgViewer::GraphicsWindow::MouseCursor MC)
{
	viewer->getWindows(ws);
	if (!ws.empty())
	{
		osgViewer::Viewer::Windows::iterator iter = ws.begin();
		for (; iter != ws.end(); ++iter)
		{
			(*iter)->setCursor(MC);
		}
	}
}

bool Contrl3DManipulator::RotateOperate(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
{
	addMouseEvent(ea);
	// return if less then two events have been added
	if (_ga_t0.get() == NULL || _ga_t1.get() == NULL)
		return false;

	// get delta time
	double eventTimeDelta = _ga_t0->getTime() - _ga_t1->getTime();
	if (eventTimeDelta < 0.)
	{
		OSG_WARN << "Manipulator warning: eventTimeDelta = " << eventTimeDelta << std::endl;
		eventTimeDelta = 0.;
	}

	// get deltaX and deltaY
	float dx = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
	float dy = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();

	// return if there is no movement.
	if (dx == 0. && dy == 0.)
		return false;
	if (performMovementLeftMouseButton(eventTimeDelta, dx, dy))
		aa.requestRedraw();

	aa.requestContinuousUpdate(false);
	_thrown = false;

	return true;
}

void Contrl3DManipulator::LoadInAndOutModel()
{
	unsigned int num = m_model->getNumChildren();
	osg::ref_ptr<osg::Node> node = _viewMgr->GetSceneData(_hWnd);
	osgDB::Registry::instance()->writeNode(*node, "ground1.stl", osgDB::Registry::instance()->getOptions());
	osgDB::Registry::instance()->writeNode(*node, "ground1.osgt", osgDB::Registry::instance()->getOptions());


	m_Oldmodel = _viewMgr->GetSceneData(_hWnd);
	m_Newmodel = osgDB::readNodeFile("ground1.stl");
	_viewMgr->SetSceneData(_hWnd, m_Newmodel);
}

void Contrl3DManipulator::DrawLineModel()
{
	osg::ref_ptr<osg::Node> node = _viewMgr->GetSceneData(_hWnd);
	osg::ref_ptr<osg::StateSet> stateSet = node->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	if (!m_LineMode)
	{
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		m_LineMode = true;
	}

	else
	{
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		m_LineMode = false;
	}
	stateSet->setAttribute(pm, osg::PolygonMode::OVERRIDE || osg::PolygonMode::ON);
}

osg::Vec3 Contrl3DManipulator::getCurrentMouseWorldCoord(osgViewer::Viewer* viewer, float x, float y, float z)
{
	osg::Vec3 windowMouse(x, y, z);
	osg::Vec3 MouseWorldCoord;
	osg::Matrix VPW_Matrix = viewer->getCamera()->getViewMatrix()*viewer->getCamera()->getProjectionMatrix()*viewer->getCamera()->getViewport()->computeWindowMatrix();
	MouseWorldCoord = windowMouse*osg::Matrix::inverse(VPW_Matrix);
	return MouseWorldCoord;
}
osg::Vec3 Contrl3DManipulator::getInsectionPoint(osgViewer::Viewer* viewer, float x, float y)
{
	osg::Vec3d firstInsectionPoint;
	osg::Vec3 nearPoint = getCurrentMouseWorldCoord(viewer, x, y, 0.0f);
	osg::Vec3 farPoint = getCurrentMouseWorldCoord(viewer, x, y, 1.0f);
	osg::ref_ptr<osgUtil::LineSegmentIntersector> lineSegment = new osgUtil::LineSegmentIntersector(nearPoint, farPoint);
	osgUtil::IntersectionVisitor iv(lineSegment);
	viewer->getSceneData()->accept(iv);
	osgUtil::LineSegmentIntersector::Intersection intersection = lineSegment->getFirstIntersection();
	firstInsectionPoint = osg::Vec3d(intersection.getWorldIntersectPoint());
	return firstInsectionPoint;
}
