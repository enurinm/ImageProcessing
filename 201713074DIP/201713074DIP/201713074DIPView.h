
// 201713074DIPView.h : interface of the CMy201713074DIPView class
//

#pragma once
#include "JpegFile.h"
#include "opencv2\opencv.hpp"
using namespace cv;

class CMy201713074DIPView : public CView
{
protected: // create from serialization only
	CMy201713074DIPView();
	DECLARE_DYNCREATE(CMy201713074DIPView)

// Attributes
public:
	CMy201713074DIPDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMy201713074DIPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImgloadBmp();
	afx_msg void OnImgloadJpg();
	afx_msg void OnConversionRgbtohsi();
	afx_msg void OnGreylevelNegative();
	afx_msg void OnGreylevelGreyscale();
	afx_msg void OnGreylevelThresholding();
	afx_msg void OnGreylevelPowerlaw();
	afx_msg void OnGreylevelHistogramequalization();
	afx_msg void OnGreylevelPrinthistogram();
	afx_msg void OnFilteringGaussian();
	afx_msg void OnFilteringMedian();
	afx_msg void OnFilteringAverage();
	afx_msg void OnFilteringColormedian();
	afx_msg void OnFilteringHighboost();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeSobel();
	afx_msg void OnMenu32797();
	afx_msg void OnEdgeLog();
	afx_msg void OnVideoAvi();
	afx_msg void OnVideoMp4();
	afx_msg void OnVideoLucaskanade();
};

#ifndef _DEBUG  // debug version in 201713074DIPView.cpp
inline CMy201713074DIPDoc* CMy201713074DIPView::GetDocument() const
   { return reinterpret_cast<CMy201713074DIPDoc*>(m_pDocument); }
#endif

