// MFC_OSG.cpp : implementation of the cOSG class
//
#include "stdafx.h"
#include "cOSG.h"
#include "CroodAxisCallBack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


cOSG::cOSG(CWnd* obj) :
m_object(obj),
mViewer(NULL)
{
	m_hWnd = obj->m_hWnd;
	mRoot = new osg::Group();
}

cOSG::~cOSG()
{
	if (mViewer)
	{
		mViewer->setDone(true);
		mViewer->stopThreading();
		delete mViewer;
	}
}


void cOSG::InitOSG()
{
	//��ʼ���������
	InitCameraConfig();

	//ΪHUD���붯̬Text
	osg::ref_ptr<osgText::Text> updateText = new osgText::Text;
	//����HUD
	mRoot->addChild(CreateTextHUD(updateText.get()));

	//�������������
	mRoot->addChild(CreateCoordCamera(mViewer->getCamera()));
	//ʵʱ��ȡ��̸߳�HUD
	mViewer->addEventHandler(new PickHandler(updateText.get(),mRoot));

	//�Ż�mRoot
	osgUtil::Optimizer opt;
	opt.optimize(mRoot.get());
	opt.reset();

	//�Զ��������
	//this->SetManipulator(new osgGA::TrackballManipulator);
	//this->SetManipulator(new Contrl3DManipulator(this->mRoot));
	//this->SetManipulator(new UnionManipulator(mRoot));
	mViewer->setSceneData(mRoot.get());
	//mViewer->realize();
}

void cOSG::InitCameraConfig(void)
{
	//�ֲ��������洰�ڳߴ�����
	RECT rect;
	//Ϊ���ڴ�����
	mViewer = new osgViewer::Viewer();
	// ������Ⱦ״̬�¼�
	mViewer->addEventHandler(new osgViewer::StatsHandler);
	// ��ȡ��ǰ���ڳߴ�
	::GetWindowRect(m_hWnd, &rect);
	// ��ʼ��ͼ������������
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	//��ʼ��Windata,��osgչʾ����Ӧ����Ĵ�����
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
	// ���û�ͼ����������
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;
	traits->samples = 8;//���ڳ�ʼ�������

	// ����ͼ��������
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	// Ϊview���������
	osg::ref_ptr<osg::Camera> camera1 = mViewer->getCamera();

	// Ϊ�������ͼ��������
	camera1->setGraphicsContext(gc);

	// ��������ӿ�
	//camera->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));
	camera1->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	//����ͶӰ����������һЩ����
	camera1->setProjectionMatrixAsPerspective(
		45.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0, 1000.0);
	//camera1->setProjectionMatrixAsOrtho(0.0f, 10000.0f, 0.0f, 1000.0f, 1.0f, 1000.0);
	camera1->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera1->setClearColor(osg::Vec4(0.0f, 0.1f, 0.2f, 1.0f));
	mViewer->setCamera(camera1.get());

	//mRoot�����
	osg::ref_ptr<osg::Multisample> pms = new osg::Multisample;
	pms->setSampleCoverage(1, true);
	mRoot->getOrCreateStateSet()->setAttributeAndModes(pms, osg::StateAttribute::ON);
	mRoot->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	mRoot->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	//mRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	CreateLight();
}

void cOSG::PreFrameUpdate()
{
	// Due any preframe updates in this routine

	//���ģ�建��
	unsigned int clearMask = this->GetViewer()->getCamera()->getClearMask();
	this->GetViewer()->getCamera()->setClearMask(clearMask | GL_STENCIL_BUFFER_BIT);
	this->GetViewer()->getCamera()->setClearStencil(0);
}

void cOSG::PostFrameUpdate()
{
	// Due any postframe updates in this routine
}

void cOSG::Render(void* ptr)
{
	cOSG* osg = (cOSG*)ptr;

	osgViewer::Viewer* viewer = osg->GetViewer();

	// You have two options for the main viewer loop
	viewer->run();  // or
	 //     while(!viewer->done()) { viewer->frame(); }
	//viewer->run();

	//      viewer->run();
	//while (!viewer->done())
	//{
	//	osg->PreFrameUpdate();
	//	viewer->frame();
	//	osg->PostFrameUpdate();
	//	Sleep(500);         // Use this command if you need to allow other processes to have cpu time
	//}

	// For some reason this has to be here to avoid issue: 
	// if you have multiple OSG windows up 
	// and you exit one then all stop rendering
	//AfxMessageBox("Exit Rendering Thread");

	_endthread();
}



cRenderingThread::cRenderingThread(cOSG* ptr)
	: OpenThreads::Thread(), _ptr(ptr), _done(false)
{
}

cRenderingThread::~cRenderingThread()
{
	_done = true;
	while (isRunning())
		OpenThreads::Thread::YieldCurrentThread();
}

void cRenderingThread::run()
{
	if (!_ptr)
	{
		_done = true;
		return;
	}

	osgViewer::Viewer* viewer = _ptr->GetViewer();
	do
	{
		_ptr->PreFrameUpdate();
		viewer->frame();
		_ptr->PostFrameUpdate();
	}while (!testCancel() && !viewer->done() && !_done);
}


// ���ݵ㼯������
osg::ref_ptr<osg::Geode> cOSG::CreateContinuousLine(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();

	//���ü���ͼԪ����
	geometry->setVertexArray(coords);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	geometry->setNormalArray(normals);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, 0, coords->size()));
	//��Ӽ���ͼԪ��Ҷ�ڵ�
	geode->addDrawable(geometry.get());

	//ѹ�뷨��������
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	return geode;
}
// ���ݵ㼯��������������
osg::ref_ptr<osg::Geode> cOSG::CreateContinuousTriangles(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();

	//���ü���ͼԪ����
	geometry->setVertexArray(coords);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	geometry->setNormalArray(normals);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::TRIANGLE_STRIP, 0, coords->size()));
	//��Ӽ���ͼԪ��Ҷ�ڵ�
	geode->addDrawable(geometry.get());

	//ѹ�뷨��������
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	//�رչ���
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return geode;
}

// ���ݵ㼯���ɵ���������
osg::ref_ptr<osg::Geode> cOSG::CreateTriangularMesh(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec3Array> normals)
{

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	double r, g, b;
	//������ɫ����
	for (unsigned int i = 0; i < coords->size(); i++)
	{
		double z = coords->at(i).z();

		r = 0.16;
		g = z*0.00045;
		b = 0.3;
		colors->push_back(osg::Vec4(r, g, b, 0.8f));
	}
	//���ü���ͼԪ����
	geometry->setVertexArray(coords);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	//geometry->setNormalArray(normals);
	//geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::TRIANGLES, 0, coords->size()));
	geometry->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	geometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	osgUtil::SmoothingVisitor::smooth(*(geometry.get()));
	//��Ӽ���ͼԪ��Ҷ�ڵ�
	geode->addDrawable(geometry.get());
	
	return geode;
}
// ���ݵ㼯�����߶�
osg::ref_ptr<osg::Geode> cOSG::CreateLineSegments(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();

	//���ü���ͼԪ����
	geometry->setVertexArray(coords);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	geometry->setNormalArray(normals);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES, 0, coords->size()));

	//��Ӽ���ͼԪ��Ҷ�ڵ�
	geode->addDrawable(geometry.get());

	//ѹ�뷨��������
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	return geode;
}
		

	
// ����TextHUD
osg::ref_ptr<osg::Node> cOSG::CreateTextHUD(osg::ref_ptr<osgText::Text> updateText)
{
	osg::Vec3 position(10.0f, 585.0f, 0.0f);
	osg::Vec3 delta(0.0f, -12.0f, 0.0f);

	osg::ref_ptr<osg::Camera> hudCamera = new osg::Camera;
	hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	hudCamera->setProjectionMatrixAsOrtho2D(0, 800, 0, 600);
	hudCamera->setViewMatrix(osg::Matrix::identity());
	hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	hudCamera->setName("TextHUD");

	{ // this displays what has been selected
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		geode->setName("The text label");
		geode->addDrawable(updateText);
		hudCamera->addChild(geode);

		updateText->setCharacterSize(12.0f);
		updateText->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		//updateText->setText("");
		updateText->setPosition(position);
		updateText->setDataVariance(osg::Object::DYNAMIC);

		position += delta;
	}

	return hudCamera;

}

//�����ӽڵ㵽���ڵ�
void cOSG::addNode(osg::ref_ptr<osg::Node> node)
{

	mRoot->addChild(node);
	return;
}

void cOSG::addText(const osg::Vec3 &position, const std::string &str, bool mark)
{
	osg::ref_ptr<osg::Geode> gnode = new osg::Geode;
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	//��������
	text->setFont("Fonts/msyh.ttc");
	text->setCharacterSize(100);
	text->setText(str);
	text->setPosition(position);
	if (mark)
	{
		text->setRotation(osg::Quat(osg::PI_2,osg::Vec3(1.0f,0.0f,0.0f)));
	}	
	else
	{
		osg::Quat quat;
		quat = osg::Quat(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f))*osg::Quat(osg::PI_2, osg::Vec3(0.0f, 1.0f, 0.0f));
		text->setRotation(quat);
	}
	gnode->addDrawable(text);
	gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	mRoot->addChild(gnode);
	return;
}

osg::ref_ptr<osg::Group> cOSG::GetRoot() const
{
	return this->mRoot;
}

void cOSG::removeAllNode(bool all)
{
	osg::ref_ptr<osg::Node> node;
	if (mRoot.valid())
	{
		unsigned int num = mRoot->getNumChildren();
		if (!all)
		{
			for (unsigned int _index = 0; _index < num; _index++)
			{
				node = mRoot->getChild(_index);
				std::string strname = node->getName();
				if (strname == "TextHUD" || strname == "CoordCamera" || strname == "light1")
				{
					;
				}
				else
				{
					mRoot->removeChild(node);
					num--;
					_index--;
				}

			}
		}
		else
		{
			for (unsigned int _index = 0; _index < num; _index++)
			{
				node = mRoot->getChild(0);
				mRoot->removeChild(node);
			}
		}
		
	}
}

osgViewer::Viewer* cOSG::GetViewer() const
{
	return this->mViewer;
}

void cOSG::SetManipulator(osgGA::CameraManipulator *manipulator)
{
	osgGA::CameraManipulator* cm = mViewer->getCameraManipulator();
	if (cm)
	{
		mViewer->removeEventHandler(cm);
		mViewer->setCameraManipulator(manipulator);
	}
	else
	{
		mViewer->setCameraManipulator(manipulator);
	}
}

void cOSG::SetRoot(osg::Group* root)
{
	this->removeAllNode();
	this->mRoot->addChild(root);
}
//������Դ
osg::Group*  cOSG::CreateLight()
{
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(1);
	light->setPosition(osg::Vec4(0.0, 1.0, 1.0, 0.0f));
	//light->setDirection(osg::Vec3(-0.0f, -1.0f, -1.0f));
	//light->setDirection(osg::Vec3(-0.85f, 0.9f, -0.8f));
	light->setAmbient(osg::Vec4(1.0f,1.0f,1.0f, 1.0f));
	light->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f, 1.0f));
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light.get());
	lightSource->setName("light1");
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	osg::StateSet* rootStateSet = new osg::StateSet;
	mRoot->setStateSet(rootStateSet);
	lightSource->setStateSetModes(*rootStateSet, osg::StateAttribute::ON);

	mRoot->addChild(lightSource);
	return lightSource;
	
}
//��ȡ���Ƿ�����
osg::Vec3 cOSG::GetTriangleNormal(osg::Vec3 &point1, osg::Vec3 &point2, osg::Vec3 &point3)
{
	osg::Vec3 nml = ((point2 - point1) ^ (point3 - point2));
	 nml.normalize();
	 //nml[1] *= -1;
	 //nml[2] *= -1;
	 return nml;
}

//�������������
osg::ref_ptr<osg::Camera> cOSG::CreateCoordCamera(osg::Camera* cm)
{
	osg::ref_ptr < osg::Camera > camera = new osg::Camera;
	osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0.5f, 0.5f, 0.5f), 1.0f);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	
	//����������
	osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(box);
	drawable->setColor(osg::Vec4(0.2f, 0.4f, 0.4f, 0.4f));
	drawable->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->addDrawable(drawable);
	geode->setName("box");
	//�������塮ǰ��
	osg::ref_ptr<osg::Texture2D> textureFront = new osg::Texture2D;
	osg::ref_ptr<osgText::Text> Tfront = new osgText::Text;
	Tfront->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tfront->setText(L"ǰ");
	Tfront->setPosition(osg::Vec3(0.5f, -0.05f, 0.5f));
	Tfront->setAxisAlignment(osgText::Text::XZ_PLANE);
	Tfront->setAlignment(osgText::Text::CENTER_CENTER);
	Tfront->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tfront->setCharacterSize(0.6f);
	geode->addDrawable(Tfront);

	//�������塮��
	osg::ref_ptr<osgText::Text> Tback = new osgText::Text;
	Tback->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tback->setText(L"��");
	Tback->setPosition(osg::Vec3(0.5f, 1.05f, 0.5f));
	Tback->setRotation(osg::Quat(osg::PI, osg::Vec3(0.0f, 0.0f, 1.0f))*osg::Quat(-osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f)));
	Tback->setAlignment(osgText::Text::CENTER_CENTER);
	Tback->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tback->setCharacterSize(0.6f);
	geode->addDrawable(Tback);
	//�������塮��
	osg::ref_ptr<osgText::Text> Tleft = new osgText::Text;
	Tleft->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tleft->setText(L"��");
	Tleft->setPosition(osg::Vec3(-0.05f, 0.5f, 0.5f));
	Tleft->setRotation(osg::Quat(-osg::PI_2, osg::Vec3(0.0f, 1.0f, 0.0f))*osg::Quat(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f)));
	Tleft->setAlignment(osgText::Text::CENTER_CENTER);
	Tleft->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tleft->setCharacterSize(0.6f);
	geode->addDrawable(Tleft);
	//�������塮�ҡ�
	osg::ref_ptr<osgText::Text> Tright = new osgText::Text;
	Tright->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tright->setText(L"��");
	Tright->setPosition(osg::Vec3(1.05f, 0.5f, 0.5f));
	Tright->setAxisAlignment(osgText::Text::YZ_PLANE);
	Tright->setAlignment(osgText::Text::CENTER_CENTER);
	Tright->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tright->setCharacterSize(0.6f);
	geode->addDrawable(Tright);
	//����������
	osg::ref_ptr<osgText::Text> Tup = new osgText::Text;
	Tup->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tup->setText(L"��");
	Tup->setPosition(osg::Vec3(0.5f, 0.5f, 1.05f));
	Tup->setAxisAlignment(osgText::Text::XY_PLANE);
	Tup->setAlignment(osgText::Text::CENTER_CENTER);
	Tup->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tup->setCharacterSize(0.6f);
	geode->addDrawable(Tup);
	//����������
	osg::ref_ptr<osgText::Text> Tdown = new osgText::Text;
	Tdown->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tdown->setText(L"��");
	Tdown->setPosition(osg::Vec3(0.5f, 0.5f, -0.05f));
	Tdown->setAxisAlignment(osgText::Text::XY_PLANE);
	Tdown->setAlignment(osgText::Text::CENTER_CENTER);
	Tdown->setRotation(osg::Quat(osg::PI, osg::Vec3(1.0f, 0.0f, 0.0f)));
	Tdown->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tdown->setCharacterSize(0.6f);
	geode->addDrawable(Tdown);

	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setName("CoordCamera");
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("axes.osgt");
	osg::BoundingSphere bs = node->getBound();
	osg::ref_ptr<osg::MatrixTransform> matf = new osg::MatrixTransform;
	matf->addChild(node);
	matf->addChild(geode);
	matf->setUpdateCallback(new CroodAxisCallBack(cm));

	double viewDistance = 2 * bs.radius();
	double znear = viewDistance - bs.radius();
	double zfar = viewDistance + bs.radius();
	float right = bs.radius();
	float top = bs.radius();
	camera->setProjectionMatrixAsOrtho(-right*1.4, right*1.4, -top*1.4, top*1.4, znear, zfar);
	camera->setViewport(5.0, -20.0f, 200, 200);
	osg::Vec3d upDirection(0.0, 0.0, 1.0);
	osg::Vec3d viewDirection(0.0, -1.0, 0.0);
	osg::Vec3d center = bs.center();
	osg::Vec3d eyePoint = center + viewDirection*viewDistance;
	camera->setViewMatrixAsLookAt(eyePoint, center, upDirection);
	camera->addChild(matf);
	camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return camera.release();
}

void cOSG::removeNodeByName(const char* name)
{
	if (mRoot->getNumChildren() != 0)
		for (unsigned int i = 0; i < mRoot->getNumChildren(); ++i)
		{
			osg::ref_ptr<osg::Node> node = mRoot->getChild(i);
			if (node->getName() == name)
			{
				mRoot->removeChild(node);
				break;
			}
		}
}
