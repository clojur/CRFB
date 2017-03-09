#include "stdafx.h"
#include "ViewerManager.h"
#include <osgViewer/Viewer>


class ViewerThread : public OpenThreads::Thread
{
public:
	ViewerThread(osgViewer::Viewer* viewer)
		: OpenThreads::Thread(), _viewer(viewer)
	{
	}

	virtual ~ViewerThread()
	{
		_viewer->setDone(true);
		if (isRunning())
		{
			cancel();
			join();
		}
	}

	void run() override
	{
		if (!_viewer)
		{
			_viewer->setDone(true);
			return;
		}

		_viewer->run();
	}

protected:
	osgViewer::Viewer* _viewer;
};


ViewerManager::ViewerManager()
{
}



ViewerManager::~ViewerManager()
{
}

void ViewerManager::AddView(HWND hWnd)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	InitView(hWnd, viewer);
	_viewMap[hWnd] = viewer;
}

void ViewerManager::RemoveView(HWND hWnd)
{
	delete _threads[hWnd];
	_threads.erase(hWnd);
	_viewMap.erase(hWnd);
}

void ViewerManager::Display(HWND hWnd)
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(_viewMap[hWnd].get());
	_threads[hWnd] = new ViewerThread(viewer);
	_threads[hWnd]->start();
}
