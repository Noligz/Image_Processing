// Image_Processing.h : Image_Processing Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "histogram.h"
#include "HistDlg.h"
#include "ImageDlg.h"
#include "SliderDlg.h"
#include "SigmaDlg.h"


// CImage_ProcessingApp:
// �йش����ʵ�֣������ Image_Processing.cpp
//

class CImage_ProcessingApp : public CWinApp
{
public:
	CImage_ProcessingApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImage_ProcessingApp theApp;