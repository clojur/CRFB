
#include "stdafx.h"
#include "VprestressBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_SERIAL(CVprestressBar, CBarBase, 0);
CVprestressBar::CVprestressBar()
{
}

CVprestressBar::~CVprestressBar()
{
}






void CVprestressBar::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}


