#pragma once
#include "common.h"
#include "Contrl2DManipulator.h"
#include "PickHandler.h"
#include "Contrl3DManipulator.h"

class YJOSG_API cOSG
{
/*��ʼ��OSG*/
public:
	cOSG(CWnd* obj);
	~cOSG();
	virtual void InitOSG();
	virtual void InitCameraConfig(void);
	virtual void PreFrameUpdate();
	virtual void PostFrameUpdate(void);
	void Done(bool value) { mDone = value; }
	bool Done(void) { return mDone; }
	static void Render(void* ptr);

/*��������*/
public:
	//�����ӽڵ㵽���ڵ�
	void addNode(osg::ref_ptr<osg::Node> node);
	//����Text�����ڵ�
	void addText(const osg::Vec3 &position, const std::string &str,bool mark=true);
	//���ò�����
	void SetManipulator(osgGA::CameraManipulator *manipulator);
	//���ó�����
	void SetRoot(osg::Group* root);
	//��ȡroot
	osg::ref_ptr<osg::Group> GetRoot() const;
	//��ȡViewer
	osgViewer::Viewer* GetViewer() const;
	//ɾ��ȫ����HUD�ڵ�
	void removeAllNode(bool all=false);
	//ͨ�������Ƴ�Node
	void removeNodeByName(const char* name);

/*���Ʒ���*/
public:
	// ���ݵ㼯����������
	static osg::ref_ptr<osg::Geode> CreateContinuousLine(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors);
	// ���ݵ㼯�����߶�
	static osg::ref_ptr<osg::Geode> CreateLineSegments(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors);
	// ���ݵ㼯��������������
	static osg::ref_ptr<osg::Geode> CreateContinuousTriangles(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors);
	//���ݺ����㼯����������
	static osg::ref_ptr<osg::Geode> CreateTriangularMesh(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec3Array> normals);
	// ����TextHUD
	osg::ref_ptr<osg::Node> CreateTextHUD(osg::ref_ptr<osgText::Text> updateText);
	//�������������
	osg::ref_ptr<osg::Camera> CreateCoordCamera(osg::Camera* cm);
	//������Դ
	osg::Group*  CreateLight();
	//�õ����Ƿ�����
	static osg::Vec3 GetTriangleNormal(osg::Vec3 &point1,osg::Vec3 &point2,osg::Vec3 &point3);
private:
	
	bool		mDone;
	HWND		m_hWnd;
	CWnd*		m_object;
	
	osgViewer::Viewer*				mViewer;
public:
	osg::ref_ptr<osg::Group>		mRoot;
	osg::ref_ptr<osg::Node>			mNode;
};


class cRenderingThread : public OpenThreads::Thread
{
public:
	cRenderingThread(cOSG* ptr);
	virtual ~cRenderingThread();

	virtual void run();

protected:
	cOSG* _ptr;
	bool _done;
};

