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
	bool		first;//��һ�δ򿪲���������һЩ��ʼ������
	double		m_distance;//��������ͣ����ʵʱ����
	osg::Vec3	m_viewdirection;//���ߵĳ���

	double		m_OldDistance;//�����ʼ����
	osg::Vec3	m_OldCenter;//�����ʼ���ĵ�
	osg::Quat	m_OldRotation;//�����ʼ��ת��



	osg::Vec3	m_CenterPosition;//����λ��
	osg::Vec3	m_ViewerPosition;//���λ��
	osg::Vec3	m_UpNormal;//�����������

	osg::ref_ptr<osg::Group>	m_model;//��ǰ������
	osg::ref_ptr<osg::Node> m_Newmodel;//�µĳ�����

	osg::Vec3		MouseWorld;//������������

	osg::Matrix		m_mat;//�������

	bool	m_ScaleSpeed;//���ż��ٿ���
	bool	m_LineMode;//�߿�ģʽ
	//��ק����
private:
	float mX;
	float mY;
	bool mEnableDragger;
	//ʰȡ����
	osg::Node* m_OldNode;
	osg::ref_ptr<osg::Group> m_OldParent;
	osg::ref_ptr<osg::Group> m_OldGroup;
	bool mDragCamera;
	//���³���
	ViewManager* _viewMgr;
	HWND _hWnd;
public:
	osg::Node* m_Oldmodel;//�ϵĳ�����
};
