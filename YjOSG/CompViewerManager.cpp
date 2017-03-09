#include "stdafx.h"
#include "CompViewerManager.h"
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <OpenThreads/Thread>

class CompViewerThread : public OpenThreads::Thread
{
public:
	CompViewerThread(CompViewerManager* viewMgr)
		: OpenThreads::Thread(), _viewMgr(viewMgr), _done(false)
	{
	}

	virtual ~CompViewerThread()
	{
		_done = true;
		if (isRunning())
		{
			cancel();
			join();
		}
	}

	virtual void run()
	{
		if (!_viewMgr)
		{
			_done = true;
			return;
		}

		osgViewer::CompositeViewer* viewer = _viewMgr->getViewer();

		do
		{
			_viewMgr->PreFrameUpdate();
			viewer->frame();
			_viewMgr->PostFrameUpdate();
		} while (!testCancel() && !viewer->done() && !_done);
	}

protected:
	CompViewerManager* _viewMgr;
	bool _done;
};





CompViewerManager::CompViewerManager()
	: _added(false), _removed(false)
	, _viewer(nullptr), _threadHandle(nullptr)
{
}

CompViewerManager::~CompViewerManager()
{
	//_viewer->setDone(true);
	//Sleep(1000);
	//_viewer->stopThreading();

	if (_threadHandle)
	{
		delete _threadHandle;
		_threadHandle = nullptr;
	}
	_addViewQueue.clear();
	_delViewQueue.clear();
}

void CompViewerManager::AddView(HWND hWnd)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	InitView(hWnd, viewer);

	if (_viewer == NULL)
		_viewer = new osgViewer::CompositeViewer();

	_viewMap[hWnd] = viewer;
	_rootIndex[hWnd] = 0;
	_addViewQueue.push_back(viewer);
	_added = true;
}

void CompViewerManager::RemoveView(HWND hWnd)
{
	_removed = true;
	_delViewQueue.push_back(_viewMap[hWnd]);
	_viewMap.erase(hWnd);
}



void CompViewerManager::Display(HWND hWnd)
{
	if (_viewer->getNumViews() == 0)
	{
		// Start the thread to do OSG Rendering
		//mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, _viewMgr); 
		if (_threadHandle == nullptr)
			_threadHandle = new CompViewerThread(this);
		else
			_viewer->setDone(false);

		_threadHandle->start();
	}
}

void CompViewerManager::PreFrameUpdate()
{
	if (_added)
	{
		osgViewer::View* view = _addViewQueue.front();
		if (!view->getCameraManipulator())
			view->setCameraManipulator(new osgGA::TrackballManipulator());
		//unsigned int clearMask = view->getCamera()->getClearMask();
		//view->getCamera()->setClearMask(clearMask | GL_STENCIL_BUFFER_BIT);
		//view->getCamera()->setClearStencil(0);
		_viewer->addView(view);
		_addViewQueue.pop_back();
		_added = false;

		// Realize the Viewer
		if (!_viewer->isRealized())
			_viewer->realize();
	}

	if (_removed)
	{
		osgViewer::View* view = _delViewQueue.front();
		_viewer->removeView(view);
		_delViewQueue.pop_back();
		_removed = false;

		if (_viewer->getNumViews() == 0)
		{
			_viewer->setDone(true);
		}
	}
}

void CompViewerManager::PostFrameUpdate()
{
}

