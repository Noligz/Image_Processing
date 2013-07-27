#include "Morphology.h"

// Image_ProcessingView.h : CImage_ProcessingView 类的接口
//


#pragma once


class CImage_ProcessingView : public CView
{
protected: // 仅从序列化创建
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

// 属性
public:
	CImage_ProcessingDoc* GetDocument() const;
public:
	bool m_TransformEnable;
	bool m_TrackCursor;
	CPoint m_CurrPnt;
	int m_TransOffsetX;
	int m_TransOffsetY;
	float m_RotateAngle;
	int m_ZoomX;
	int m_ZoomY;
	int m_InterType;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImage_ProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	void DrawImage(CDC* pDC, CImage* image, int PosX, int PosY);		//绘制加载的图像
	CImage* Convolution(CImage* pImg, double kernel[15][15], int KernelSize);
	BYTE Convolution_ij(CImage* pImg, int i, int j, float kernel[3][3]);
	int  GetMiddleBySort(BYTE *Window, int nSize);
	void ReadSEFromFile(StructureElement &se);

public:
	afx_msg void OnHistDraw();
	afx_msg void OnHistAve();
	afx_msg void OnHistMatch();
	afx_msg void OnRoberts();
	afx_msg void OnPrewitt();
	afx_msg void OnSobel();
	afx_msg void OnGuassion();
	afx_msg void OnMean();
	afx_msg void OnMedian();
	afx_msg void OnCustomKernel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNearNei();
	afx_msg void OnBilinear();
	afx_msg void OnBicubic();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPlus();
	afx_msg void OnMinus();
	afx_msg void OnMultiply();
	afx_msg void OnDivide();
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnBinarydilation();
	afx_msg void OnBinaryerosion();
	afx_msg void OnBinaryopen();
	afx_msg void OnBinaryclose();
	afx_msg void OnDteuclidean();
	afx_msg void OnDtcityblock();
	afx_msg void OnDtchessboard();
	afx_msg void OnSkeleton();
	afx_msg void OnSkeletonreconstruction();
	afx_msg void OnGraydilation();
	afx_msg void OnGrayerosion();
	afx_msg void OnGrayopen();
	afx_msg void OnGrayclose();
	afx_msg void OnHwCs();
	afx_msg void OnHwCount();
	afx_msg void OnHwPhone();
	afx_msg void OnHwNoise();
	afx_msg void OnHwFinger();
	afx_msg void OnHsiHist();
	afx_msg void OnHsiHistave();
	afx_msg void OnHsismooth();
};

#ifndef _DEBUG  // Image_ProcessingView.cpp 中的调试版本
inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
   { return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument); }
#endif

