//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2016 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// RibbonListButton.cpp : implementation file
//

#include "stdafx.h"
#include "RibbonListButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nImageMargin		= 4;
const int nCheckMargin		= 2;
const int nTextOffsetX		= 11;
const int nTextOffsetY		= 2;

const int nBorderMarginX	= 1;
const int nBorderMarginY	= 3;

class CRibbonListIcon: public CBCGPRibbonPaletteIcon
{
	friend class CRibbonListButton;
};

/////////////////////////////////////////////////////////////////////////////
// CRibbonListButton

IMPLEMENT_DYNCREATE (CRibbonListButton, CBCGPRibbonPaletteButton)

CRibbonListButton::CRibbonListButton () :
	CBCGPRibbonPaletteButton (0, NULL, -1, -1, CSize (0, 0), 0, TRUE)
{
	SetButtonMode (TRUE);
	SetRightAlignedText(TRUE);

	m_sizeMargins = globalUtils.ScaleByDPI(CSize (2, 2));
	m_sizeMaxText = CSize (0, 0);
	m_bShowSelection = TRUE;
	m_nTextLines = 1;
}

CRibbonListButton::CRibbonListButton(
		UINT	nID,
		LPCTSTR	lpszText,
		int		nSmallImageIndex,
		int		nLargeImageIndex,
		BOOL    bRightAlignedText) :
	CBCGPRibbonPaletteButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex, CSize (0, 0), 0, TRUE)
{
	SetButtonMode (TRUE);
	SetRightAlignedText (bRightAlignedText);

	m_sizeMargins = globalUtils.ScaleByDPI(CSize(2, 2));
	m_sizeMaxText = CSize (0, 0);
	m_bShowSelection = TRUE;
	m_nTextLines = 1;
}

CRibbonListButton::CRibbonListButton (
		UINT	nID,
		LPCTSTR	lpszText,
		int		nSmallImageIndex,
		int		nLargeImageIndex,
		UINT	uiImagesPaletteResID,
		int		cxPaletteImage,
		const CStringArray& arLabels,
		BOOL    bRightAlignedText) : 
	CBCGPRibbonPaletteButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex, CSize (0, 0), 0, TRUE)
{
	SetButtonMode (TRUE);
	SetRightAlignedText (bRightAlignedText);

	m_listImages.Load (uiImagesPaletteResID);

	BITMAP bmp;
	GetObject (m_listImages.GetImageWell (), sizeof (BITMAP), &bmp);

	m_listImages.SetImageSize (
		CSize (cxPaletteImage, bmp.bmHeight), TRUE);
#ifndef _BCGSUITE_INC_
	m_listImages.SmoothResize (globalData.GetRibbonImageScale ());
#endif

	m_imagesPalette.SetImageSize (m_listImages.GetImageSize ());

	m_arLabels.Copy (arLabels);

	m_nIcons = (int) m_arLabels.GetSize ();

	UpdateToolTips();

	m_sizeMargins = globalUtils.ScaleByDPI(CSize(2, 2));
	m_sizeMaxText = CSize (0, 0);
	m_bShowSelection = TRUE;
	m_nTextLines = 1;
}

CRibbonListButton::~CRibbonListButton()
{
}

void CRibbonListButton::SetRightAlignedText (BOOL bSet)
{
	if (m_bRightAlignedText != bSet)
	{
		m_bRightAlignedText = bSet;

		if (m_bRightAlignedText)
		{
			m_nTextLines = 1;
			SetIconsInRow (1);
		}
	}
}

void CRibbonListButton::SetShowSelection(BOOL bSet)
{
	m_bShowSelection = bSet;
	SelectItem(-1);
}

void CRibbonListButton::SetTextLines(int nLines)
{
	if (m_bRightAlignedText)
	{
		return;
	}

	nLines = max(1, nLines);

	if (m_nTextLines != nLines)
	{
		m_nTextLines = nLines;
		RebuildIconLocations();
	}
}

void CRibbonListButton::AddGroup (
	LPCTSTR				lpszGroupName,
	UINT				uiImagesPaletteResID,
	int					cxPaletteImage,
	const CStringArray& arLabels)
{
	CBCGPRibbonPaletteButton::AddGroup (lpszGroupName, (int) arLabels.GetSize ());

	if (uiImagesPaletteResID != 0)
	{
		if (m_listImages.GetCount () == 0)
		{
			m_listImages.Load (uiImagesPaletteResID);

			BITMAP bmp;
			GetObject (m_listImages.GetImageWell (), sizeof (BITMAP), &bmp);

			m_listImages.SetImageSize (
				CSize (cxPaletteImage, bmp.bmHeight), TRUE);
#ifndef _BCGSUITE_INC_
			m_listImages.SmoothResize (globalData.GetRibbonImageScale ());
#endif

			m_imagesPalette.SetImageSize (m_listImages.GetImageSize ());
		}
		else
		{
#ifndef _BCGSUITE_INC_
			ASSERT (globalUtils.ScaleByDPI(cxPaletteImage) == m_listImages.GetImageSize ().cx);
#else
			ASSERT (cxPaletteImage == m_listImages.GetImageSize ().cx);
#endif

			m_listImages.Load (uiImagesPaletteResID, NULL, TRUE);
		}
	}

	if (m_arLabels.GetSize () == 0)
	{
		m_arLabels.Copy (arLabels);
	}
	else
	{
		m_arLabels.Append (arLabels);
	}

	m_nIcons = (int) m_arLabels.GetSize ();

	UpdateToolTips();

	RemoveAll ();
	m_sizeMaxText = CSize (0, 0);
}

void CRibbonListButton::Clear ()
{
	CBCGPRibbonPaletteButton::Clear ();

	m_arLabels.RemoveAll ();
	m_sizeMaxText = CSize (0, 0);
}

void CRibbonListButton::OnShowPopupMenu ()
{
	ASSERT_VALID (this);

#ifndef _BCGSUITE_INC_
	if (!m_bShowSelection)
	{
		ClearLastSelectedItem(m_nPaletteID == 0 ? m_nID : m_nPaletteID);
	}
#endif

	CBCGPBaseRibbonElement::OnShowPopupMenu ();

	if (m_sizeMaxText == CSize (0, 0))
	{
		CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
		ASSERT_VALID (pRibbonBar);

		CClientDC dc (pRibbonBar);

		CFont* pOldFont = dc.SelectObject (pRibbonBar->GetFont ());
		ASSERT (pOldFont != NULL);

		int i = 0;

		for (i = 0; i < m_arLabels.GetSize (); i++)
		{
			CSize szText = dc.GetTextExtent (m_arLabels [i]);

			m_sizeMaxText.cx = max (m_sizeMaxText.cx, szText.cx);
			m_sizeMaxText.cy = max (m_sizeMaxText.cy, szText.cy * m_nTextLines);
		}

		const int cxImage = m_listImages.GetImageSize ().cx;

		for (i = 0; i < m_arSubItems.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pButton = m_arSubItems [i];
			ASSERT_VALID (pButton);

			CString strText = pButton->GetText ();
			CSize szText = dc.GetTextExtent (strText);

			m_sizeMaxText.cx = max (m_sizeMaxText.cx, szText.cx - cxImage);
		}

		dc.SelectObject (pOldFont);
	}

	CBCGPRibbonPaletteButton::OnShowPopupMenu ();
}

CSize CRibbonListButton::GetIconSize () const
{
	CSize size(globalUtils.ScaleByDPI(CSize(16, 16)));
	if (m_listImages.IsValid ())
	{
		size = m_listImages.GetImageSize ();
	}

	if (m_bRightAlignedText)
	{
		int nImageWidth = 0;

		if (size.cx > 0)
		{
			nImageWidth = size.cx + 2 * (m_sizeMargins.cx + nCheckMargin) + nImageMargin + nTextOffsetX + m_sizeMaxText.cx;
		}

		size.cx = max(nImageWidth, m_sizeMaxText.cx + 2 * (m_sizeMargins.cy + nCheckMargin + nTextOffsetX));

		int nImageHeight = 0;

		if (size.cy > 0)
		{
			nImageHeight = size.cy + 2 * (m_sizeMargins.cy - nImageMargin + nCheckMargin);
		}

		size.cy = max(nImageHeight, m_sizeMaxText.cy + 2 * m_sizeMargins.cy);
	}
	else
	{
		size.cy += nTextOffsetY + m_sizeMaxText.cy;
	}

	return size;
}

void CRibbonListButton::OnDrawPaletteIcon (	CDC* pDC, CRect rectIcon, 
											int nIconIndex, CBCGPRibbonPaletteIcon* pIcon,
											COLORREF clrText)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (pIcon);
	ASSERT (nIconIndex >= 0);
	ASSERT (nIconIndex < m_nIcons);

	BOOL bChecked     = pIcon->IsChecked () && m_bShowSelection;
	BOOL bHighlighted = pIcon->IsHighlighted ();

	CRect rectText (0, 0, 0, 0);

	if (m_bRightAlignedText)
	{
		if (!m_bSmallIcons)
		{
			rectIcon.DeflateRect (nCheckMargin, nCheckMargin, nImageMargin, nCheckMargin);
		}

		CRect rect (rectIcon);
		CSize size(globalUtils.ScaleByDPI(CSize(16, 16)));
		if (m_listImages.IsValid ())
		{
			size = m_listImages.GetImageSize ();
		}

		rect.right = rect.left + size.cx + m_sizeMargins.cx * 2;

		if (bChecked)
		{
#ifdef _BCGSUITE_INC_
			CMFCVisualManager::GetInstance ()->OnDrawRibbonMenuCheckFrame (pDC, this, rect);
#else
			CBCGPVisualManager::GetInstance ()->OnDrawRibbonMenuCheckFrame (pDC, this, rect);
#endif
		}

		if (0 <= nIconIndex && nIconIndex < m_listImages.GetCount ())
		{
			m_listImages.DrawEx (pDC, rect, nIconIndex, 
				CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
		}
		else if (bChecked)
		{
			CBCGPToolbarMenuButton dummy;

#ifdef _BCGSUITE_INC_
			CMFCVisualManager::GetInstance ()->OnDrawMenuCheck (pDC, &dummy, rect, bHighlighted, FALSE);
#else
			CBCGPVisualManager::GetInstance ()->OnDrawMenuCheck (pDC, &dummy, rect, bHighlighted, FALSE);
#endif
		}

		rectText = rect;
		rectText.left  = rectText.right + nTextOffsetX;
		rectText.right = rectIcon.right;
	}
	else
	{
		if (!m_bSmallIcons)
		{
			rectIcon.DeflateRect (nImageMargin, nImageMargin);
		}

		CRect rect (rectIcon);
		rect.bottom = rect.top + m_listImages.GetImageSize ().cy;

		if (0 <= nIconIndex && nIconIndex < m_listImages.GetCount ())
		{
			m_listImages.DrawEx (pDC, rect, nIconIndex, 
				CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertTop);
		}

		rectText = rect;
		rectText.top    = rectText.bottom + nTextOffsetY;
		rectText.bottom = rectIcon.bottom;
	}

	if (!rectText.IsRectEmpty ())
	{
		if (clrText == (COLORREF)-1)
		{
			CBCGPToolbarButton btn;

#ifdef _BCGSUITE_INC_
			clrText = CMFCVisualManager::GetInstance ()->GetToolbarButtonTextColor (&btn, CMFCVisualManager::ButtonsIsRegular);
#else
			clrText = CBCGPVisualManager::GetInstance ()->GetToolbarButtonTextColor (&btn, CBCGPVisualManager::ButtonsIsRegular);
#endif
		}

		COLORREF clrOld = clrText == (COLORREF)-1 ? clrText : pDC->SetTextColor (clrText);

		CString strText = m_arLabels[nIconIndex];

		DWORD dwDrawFlags = m_bRightAlignedText
				? DT_VCENTER | DT_SINGLELINE | DT_LEFT
				: DT_TOP | DT_SINGLELINE | DT_CENTER | DT_END_ELLIPSIS;

		if (m_nTextLines > 1)
		{
			dwDrawFlags &= ~DT_SINGLELINE;
			dwDrawFlags |= DT_WORDBREAK;
		}

		dwDrawFlags |= DT_NOPREFIX;

		pDC->DrawText (strText, rectText, dwDrawFlags);

		if (clrOld != (COLORREF)-1)
		{
			pDC->SetTextColor (clrOld);
		}
	}
}

void CRibbonListButton::OnClickPaletteIcon (CBCGPRibbonPaletteIcon* pIcon)
{
	CBCGPRibbonPaletteButton::OnClickPaletteIcon (pIcon);

	CRibbonListIcon* pListIcon = (CRibbonListIcon*)pIcon;
	if (!m_bShowSelection && pListIcon != NULL && pListIcon->m_nIndex >= 0)
	{
		pListIcon->m_bIsChecked = FALSE;
	}
}

void CRibbonListButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonPaletteButton::CopyFrom (s);

	CRibbonListButton& src = (CRibbonListButton&) s;

	src.m_listImages.CopyTo (m_listImages);
	m_imagesPalette.SetImageSize (m_listImages.GetImageSize ());
	
	m_arLabels.RemoveAll ();
	m_arLabels.Copy (src.m_arLabels);

	m_sizeMargins = src.m_sizeMargins;
	m_bRightAlignedText = src.m_bRightAlignedText;
	m_sizeMaxText = src.m_sizeMaxText;
	m_bShowSelection = src.m_bShowSelection;
	m_nTextLines = src.m_nTextLines;
}

void CRibbonListButton::UpdateToolTips()
{
	m_arToolTips.RemoveAll ();

	for (int i = 0; i < m_arLabels.GetSize(); i++)
	{
		SetItemToolTip (i, m_arLabels[i]);
	}
}

IMPLEMENT_DYNCREATE (CRibbonListFontButton, CBCGPRibbonPaletteButton)

CRibbonListFontButton::CRibbonListFontButton () :
	CBCGPRibbonPaletteButton (0, NULL, -1, -1, CSize (0, 0), 0, TRUE)
{
	SetButtonMode (TRUE);
	SetIconsInRow (1);

	m_sizeMargins = globalUtils.ScaleByDPI(CSize(2, 2));
	m_sizeMaxText = CSize (0, 0);
}

CRibbonListFontButton::CRibbonListFontButton(
		UINT	nID,
		LPCTSTR	lpszText,
		int		nSmallImageIndex,
		int		nLargeImageIndex) :
	CBCGPRibbonPaletteButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex, CSize (0, 0), 0, TRUE)
{
	SetButtonMode (TRUE);
	SetIconsInRow (1);

	m_sizeMargins = globalUtils.ScaleByDPI(CSize(2, 2));
	m_sizeMaxText = CSize (0, 0);
}

CRibbonListFontButton::CRibbonListFontButton (
		UINT	nID,
		LPCTSTR	lpszText,
		int		nSmallImageIndex,
		int		nLargeImageIndex,
		UINT	uiImagesPaletteResID,
		int		cxPaletteImage,
		const XItems& arItems) : 
	CBCGPRibbonPaletteButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex, CSize (0, 0), 0, TRUE)
{
	SetButtonMode (TRUE);
	SetIconsInRow (1);

	m_listImages.Load (uiImagesPaletteResID);

	BITMAP bmp;
	GetObject (m_listImages.GetImageWell (), sizeof (BITMAP), &bmp);

	m_listImages.SetImageSize (
		CSize (cxPaletteImage, bmp.bmHeight), TRUE);
#ifndef _BCGSUITE_INC_
	m_listImages.SmoothResize (globalData.GetRibbonImageScale ());
#endif

	m_imagesPalette.SetImageSize (m_listImages.GetImageSize ());

	m_arItems.Copy (arItems);

	m_nIcons = (int) m_arItems.GetSize ();

	m_sizeMargins = globalUtils.ScaleByDPI(CSize(2, 2));
	m_sizeMaxText = CSize (0, 0);

	UpdateToolTips();
}

CRibbonListFontButton::~CRibbonListFontButton()
{
}

void CRibbonListFontButton::AddGroup (
	LPCTSTR			lpszGroupName,
	UINT			uiImagesPaletteResID,
	int				cxPaletteImage,
	const XItems& arItems)
{
	CBCGPRibbonPaletteButton::AddGroup (lpszGroupName, (int) arItems.GetSize ());

	if (m_listImages.GetCount () == 0)
	{
		m_listImages.Load (uiImagesPaletteResID);

		BITMAP bmp;
		GetObject (m_listImages.GetImageWell (), sizeof (BITMAP), &bmp);

		m_listImages.SetImageSize (
			CSize (cxPaletteImage, bmp.bmHeight), TRUE);
#ifndef _BCGSUITE_INC_
		m_listImages.SmoothResize (globalData.GetRibbonImageScale ());
#endif

		m_imagesPalette.SetImageSize (m_listImages.GetImageSize ());
	}
	else
	{
#ifndef _BCGSUITE_INC_
		ASSERT (globalUtils.ScaleByDPI(cxPaletteImage) == m_listImages.GetImageSize ().cx);
#else
		ASSERT (cxPaletteImage == m_listImages.GetImageSize ().cx);
#endif

		m_listImages.Load (uiImagesPaletteResID, NULL, TRUE);
	}

	if (m_arItems.GetSize () == 0)
	{
		m_arItems.Copy (arItems);
	}
	else
	{
		m_arItems.Append (arItems);
	}

	m_nIcons = (int) m_arItems.GetSize ();

	RemoveAll ();
	m_sizeMaxText = CSize (0, 0);

	UpdateToolTips();
}

void CRibbonListFontButton::Clear ()
{
	CBCGPRibbonPaletteButton::Clear ();

	m_arItems.RemoveAll ();
	m_sizeMaxText = CSize (0, 0);
}

void CRibbonListFontButton::OnShowPopupMenu ()
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnShowPopupMenu ();

	if (m_sizeMaxText == CSize (0, 0))
	{
		CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
		ASSERT_VALID (pRibbonBar);

		CClientDC dc (pRibbonBar);
		CFont* pOldFont = dc.SelectObject (&globalData.fontBold);

		for (int i = 0; i < m_arItems.GetSize (); i++)
		{
			CSize szText = dc.GetTextExtent (m_arItems [i].m_Caption);

			for (int j = 0; j < 2; j++)
			{
				LOGFONT& lf = m_arItems [i].m_Font[j];

				CFont font;
				font.CreateFontIndirect (&lf);
				dc.SelectObject (&font);

				CSize szText2 = dc.GetTextExtent (lf.lfFaceName);

				szText.cx = max (szText.cx, szText2.cx);
				szText.cy += szText2.cy;
			}

			m_arItems [i].m_Height = szText.cy;

			m_sizeMaxText.cx = max (m_sizeMaxText.cx, szText.cx);
			m_sizeMaxText.cy = max (m_sizeMaxText.cy, szText.cy);

			dc.SelectObject(&globalData.fontBold);
		}

		dc.SelectObject (pOldFont);
	}

	CBCGPRibbonPaletteButton::OnShowPopupMenu ();
}

CSize CRibbonListFontButton::GetIconSize () const
{
	CSize size = CBCGPRibbonPaletteButton::GetIconSize ();

	size.cx += 2 * (m_sizeMargins.cx + nCheckMargin) + nImageMargin + nTextOffsetX + m_sizeMaxText.cx;
	size.cy += 2 * (m_sizeMargins.cy - nImageMargin + nCheckMargin);

	return size;
}

void CRibbonListFontButton::OnDrawPaletteIcon (	CDC* pDC, CRect rectIcon, 
											int nIconIndex, CBCGPRibbonPaletteIcon* /*pIcon*/,
											COLORREF clrText)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT (nIconIndex >= 0);
	ASSERT (nIconIndex < m_nIcons);

	CRect rectText (0, 0, 0, 0);

	if (!m_bSmallIcons)
	{
		rectIcon.DeflateRect (nCheckMargin, nCheckMargin, nImageMargin, nCheckMargin);
	}

	CRect rect (rectIcon);
	rect.right  = rect.left + m_listImages.GetImageSize ().cx + m_sizeMargins.cx * 2;

	if (0 <= nIconIndex && nIconIndex < m_listImages.GetCount ())
	{
		m_listImages.DrawEx (pDC, rect, nIconIndex, 
			CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
	}

	rectText = rect;
	rectText.left  = rectText.right + nTextOffsetX;
	rectText.right = rectIcon.right;

	if (!rectText.IsRectEmpty ())
	{
		if (clrText == (COLORREF)-1)
		{
			CBCGPToolbarButton btn;

#ifdef _BCGSUITE_INC_
			clrText = CMFCVisualManager::GetInstance ()->GetToolbarButtonTextColor (&btn, CMFCVisualManager::ButtonsIsRegular);
#else
			clrText = CBCGPVisualManager::GetInstance ()->GetToolbarButtonTextColor (&btn, CBCGPVisualManager::ButtonsIsRegular);
#endif
		}

		COLORREF clrOld = clrText == (COLORREF)-1 ? clrText : pDC->SetTextColor (clrText);

		rectText.OffsetRect(0, (rectText.Height () - m_arItems [nIconIndex].m_Height) / 2);

		CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);

		int nHeight = pDC->DrawText (m_arItems [nIconIndex].m_Caption, rectText, DT_TOP | DT_SINGLELINE | DT_LEFT);
		rectText.OffsetRect (0, nHeight);

		for (int j = 0; j < 2; j++)
		{
			LOGFONT& lf = m_arItems [nIconIndex].m_Font[j];

			CFont font;
			font.CreateFontIndirect (&lf);
			pDC->SelectObject (&font);

			nHeight = pDC->DrawText (lf.lfFaceName, rectText, DT_TOP | DT_SINGLELINE | DT_LEFT);

			rectText.OffsetRect (0, nHeight);
		}

		pDC->SelectObject (pOldFont);

		if (clrOld != (COLORREF)-1)
		{
			pDC->SetTextColor (clrOld);
		}
	}
}

void CRibbonListFontButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonPaletteButton::CopyFrom (s);

	CRibbonListFontButton& src = (CRibbonListFontButton&) s;

	src.m_listImages.CopyTo (m_listImages);
	m_imagesPalette.SetImageSize (m_listImages.GetImageSize ());
	
	m_arItems.RemoveAll ();
	m_arItems.Copy (src.m_arItems);

	m_sizeMargins = src.m_sizeMargins;
	m_sizeMaxText = src.m_sizeMaxText;
}

void CRibbonListFontButton::UpdateToolTips()
{
	m_arToolTips.RemoveAll ();

	for (int i = 0; i < m_arItems.GetSize (); i++)
	{
		SetItemToolTip (i, m_arItems [i].m_Caption);
	}
}
