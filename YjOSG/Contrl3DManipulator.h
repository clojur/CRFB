#pragma once
#include "common.h"
#include "PickModel.h"
#include "ViewManager.h"
#include "PickObject.h"
class  cOSG;

class YJOSG_API Contrl3DManipulator :
	public osgGA::OrbitManipulator
{
public:
	Contrl3DManipulator(ViewManager *mgr, HWND hWnd, osg::Group* root);
	~Contrl3DManipulator();
private:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
	void pickModelDrag(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
	void pickModelFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;
	void InitScroll(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);
	void ScrollUp();
	void ScrollDown();
	void FirstInto();
	void ChangeMouseCursor(osgViewer::Viewer* viewer, osgViewer::Viewer::Windows ws, osgViewer::GraphicsWindow::MouseCursor MC);
	bool RotateOperate(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);
	void LoadInAndOutModel();
	void DrawLineModel();

	osg::Vec3 getCurrentMouseWorldCoord(osgViewer::Viewer* viewer, float x, float y, float z);
	osg::Vec3 getInsectionPoint(osgViewer::Viewer* viewer, float x, float y);
private:
	cOSG* mCosg;
	bool		first;//第一次打开操作器控制一些初始化参数
	double		m_distance;//相机距鼠标停留的实时距离
	osg::Vec3	m_viewdirection;//视线的朝向

	double		m_OldDistance;//保存初始距离
	osg::Vec3	m_OldCenter;//保存初始中心点
	osg::Quat	m_OldRotation;//保存初始旋转量



	osg::Vec3	m_CenterPosition;//中心位置
	osg::Vec3	m_ViewerPosition;//相机位置
	osg::Vec3	m_UpNormal;//相机向上向量

	osg::ref_ptr<osg::Group>	m_model;//当前场景树
	osg::ref_ptr<osg::Node> m_Newmodel;//新的场景树

	osg::Vec3		MouseWorld;//鼠标的世界坐标

	osg::Matrix		m_mat;//相机矩阵

	bool	m_ScaleSpeed;//缩放加速开关
	bool	m_LineMode;//线框模式
	//拖拽控制
private:
	float mX;
	float mY;
	bool mEnableDragger;
	//拾取控制
	osg::Node* m_OldNode;
	osg::ref_ptr<osg::Group> m_OldParent;
	osg::ref_ptr<osg::Group> m_OldGroup;
	bool mDragCamera;
	//更新场景
	ViewManager* _viewMgr;
	HWND _hWnd;
public:
	osg::Node* m_Oldmodel;//老的场景树
};
