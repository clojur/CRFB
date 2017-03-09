#include "stdafx.h"
#include "Contrl2DManipulator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Contrl2DManipulator::Contrl2DManipulator(osg::ref_ptr<osg::Group> & model) :
m_model(model),
first(false)
{
	initCamera();
	m_deltaTime = 0.0;
	m_lastFrame = 0.0;
	m_currentFrame = 0.0;
}

Contrl2DManipulator::~Contrl2DManipulator()
{
}

bool Contrl2DManipulator::handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
{
	osgViewer::Viewer* viewer = dynamic_cast <osgViewer::Viewer*>(&aa);
	osg::Vec3 DragWorld;
	if (!first)
	{
		osgViewer::Viewer::Windows ws;
		viewer->getWindows(ws);
		first = true;
		if (!ws.empty())
		{
			osgViewer::Viewer::Windows::iterator iter = ws.begin();
			for (; iter != ws.end(); ++iter)
			{
				(*iter)->setCursor(osgViewer::GraphicsWindow::CrosshairCursor);
			}
		}
	}
	switch (ea.getEventType())
	{
		
	case (osgGA::GUIEventAdapter::MOVE) :
	{
		MouseWorld = getMouseToWorld(viewer->getCamera(), ea.getX(), ea.getY());
		break;
	}
	case(osgGA::GUIEventAdapter::SCROLL) :
	{
		//每次鼠标滚动都获取实时鼠标停留的世界坐标
		MouseWorld = getMouseToWorld(viewer->getCamera(), ea.getX(), ea.getY());
		if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
		{
			//限制放大
			if (m_ViewerPosition[1] >= -0.5f)
				return true;
			else
			{
				//计算鼠标停留点与相机的法向量（相机与鼠标世界坐标的视线方向）
				m_viewdirection = MouseWorld - m_ViewerPosition;
				//归一化向量
				m_viewdirection.normalize();
				//实时改变相机位置与相机视点
				m_ViewerPosition += m_viewdirection*(-m_ViewerPosition[1] / 5.0f);
				//m_CenterPosition += m_viewdirection*(-m_ViewerPosition[1] /5.0f);
				//视点中心调优
				m_CenterPosition = m_ViewerPosition;
				m_CenterPosition[1] = 0.0f;
			}

		}
		else if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
		{
			//限制缩小
			if (m_ViewerPosition[1] <= -2.2*m_radius)
				return true;
			else
			{
				//计算鼠标停留点与相机的法向量（相机与鼠标世界坐标的视线方向）
				m_viewdirection = MouseWorld - m_ViewerPosition;
				//归一化向量
				m_viewdirection.normalize();
				//实时改变相机位置与相机视点
				m_ViewerPosition -= m_viewdirection*(-m_ViewerPosition[1] / 5.0f);
				//m_CenterPosition -= m_viewdirection*(-m_ViewerPosition[1] / 5.0f);
				//视点中心调优
				m_CenterPosition = m_ViewerPosition;
				m_CenterPosition[1] = 0.0f;
			}
		}
		m_distance = m_ViewerPosition[1];
		return true;
	}
	case(osgGA::GUIEventAdapter::FRAME) :
	{
		m_currentFrame=ea.getTime();
		m_deltaTime = m_currentFrame - m_lastFrame;
		m_lastFrame = m_currentFrame;
	}
		//拖动事件
	case(osgGA::GUIEventAdapter::DRAG) :
	{
			//获取当前鼠标拖动位置的世界坐标
			DragWorld = getMouseToWorld(viewer->getCamera(), ea.getX(), ea.getY());
			//拖动坐标与鼠标MOVE时的坐标求向量
			osg::Vec3 Dragnormal = DragWorld - MouseWorld;
			DragWorld.normalize();
		if (ea.getButtonMask() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)//判断是否为滚轮中建触发的拖动
		{
			//让相机的位置沿着向量反方向运动
			m_ViewerPosition -= Dragnormal*0.5/**m_deltaTime*/;
			//让相机注视中心点的左右上下为相机位置，前后位置为0
			m_CenterPosition -= Dragnormal*0.5;
			m_CenterPosition[1] = 0.0f;
		}
		return true;
	}
	case(osgGA::GUIEventAdapter::DOUBLECLICK) :
	{
		if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
		{
			initCamera();
		}
		break;
	}
	default:
		return false;
	}
	return false;
}

void Contrl2DManipulator::setByMatrix(const osg::Matrixd & matrix)
{
	return;
}

void Contrl2DManipulator::setByInverseMatrix(const osg::Matrixd & matrix)
{
	return;
}

osg::Matrixd Contrl2DManipulator::getMatrix() const
{
	osg::Matrix result;
	osg::Matrix mat;
	//mat.makeRotate(osg::Quat(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f)));
	//result=mat*osg::Matrix::translate(m_ViewerPosition);
	//return result;

	mat.makeLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);
	return osg::Matrix::inverse(mat);
}

osg::Matrixd Contrl2DManipulator::getInverseMatrix() const
{
	osg::Matrix result;
	osg::Matrix mat;
	mat.makeLookAt(m_ViewerPosition, m_CenterPosition, m_UpNormal);
	return mat;
}

osg::Vec3 Contrl2DManipulator::getMouseToWorld(osg::ref_ptr<osg::Camera> camera, float x, float y)
{
	osg::Matrix VPW = camera->getViewMatrix()*camera->getProjectionMatrix()*camera->getViewport()->computeWindowMatrix();
	osg::Vec3 window(x, y, 0.0f);
	osg::Vec3 world;
	world = window*osg::Matrix::inverse(VPW);

	return world;
}

void Contrl2DManipulator::initCamera()
{
	m_radius = 10.0f/*m_model->getBound().radius()*/;
	m_center = m_model->getBound().center();

	m_distance = m_radius *2.2;
	m_ViewerPosition = m_center + osg::Vec3(0.0f, -m_distance, 0.0f);
	m_CenterPosition = m_center;
	m_UpNormal = osg::Vec3(0.0f, 0.0f, 1.0f);
	m_viewdirection = m_CenterPosition - m_ViewerPosition;
	m_viewdirection.normalize();
}

