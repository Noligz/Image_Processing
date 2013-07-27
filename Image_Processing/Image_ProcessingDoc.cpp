// Image_ProcessingDoc.cpp : CImage_ProcessingDoc 类的实现
//

#include "stdafx.h"
#include "Image_Processing.h"

#include "Image_ProcessingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImage_ProcessingDoc

IMPLEMENT_DYNCREATE(CImage_ProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImage_ProcessingDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CImage_ProcessingDoc::OnFileOpen)
END_MESSAGE_MAP()


// CImage_ProcessingDoc 构造/析构

CImage_ProcessingDoc::CImage_ProcessingDoc()
{
	// TODO: 在此添加一次性构造代码

}

CImage_ProcessingDoc::~CImage_ProcessingDoc()
{
}

BOOL CImage_ProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CImage_ProcessingDoc 序列化

void CImage_ProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CImage_ProcessingDoc 诊断

#ifdef _DEBUG
void CImage_ProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImage_ProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImage_ProcessingDoc 命令

void CImage_ProcessingDoc::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码

	//弹出选择文件对话框
	CString strFileName;
	CFileDialog OpenDlg(TRUE,_T("*.bmp"),NULL,NULL,_T("Image Files(*.bmp;*.jpg;*.png*;*.gif)|*.bmp;*.jpg;*.png*;*.gif|All Files (*.*)|*.*||"));
	if(OpenDlg.DoModal()==IDOK)
	{
		strFileName=OpenDlg.GetPathName();
	}
	else
		return;

	if(!LoadedImage.IsNull())
		LoadedImage.Destroy();		//清除原有图像

	HRESULT hResult;
	hResult= LoadedImage.Load(strFileName);		//加载图像数据
	if (FAILED(hResult))
	{
		MessageBox(NULL,_T("载入图像失败"),_T("ERROR"),MB_OK | MB_ICONERROR);		//载入图像失败
		return;
	}

	if(!GreyImg.IsNull())
		GreyImg.Destroy();
	if(LoadedImage.GetBPP() == 24)//彩图
	{
		//灰化
		BYTE* pSrcData;
		pSrcData=(BYTE*)LoadedImage.GetBits();
		int pit=LoadedImage.GetPitch();
		int bitCount=LoadedImage.GetBPP()/8;

		BYTE *pPixel, pixelR, pixelB, pixelG, grayVal;
		int imgWidth = LoadedImage.GetWidth();
		int imgHeight = LoadedImage.GetHeight();

		GreyImg.Create(imgWidth, imgHeight, 8, 0);
		RGBQUAD ColorTable[256];
		for(int i=0;i<256;i++)
			ColorTable[i].rgbBlue=ColorTable[i].rgbGreen=ColorTable[i].rgbRed=i;
		GreyImg.SetColorTable(0,256,ColorTable);
		BYTE* pNewData = (BYTE*)GreyImg.GetBits();
		int NewPit = GreyImg.GetPitch();

		//int address=0;
		for (int y=0; y<imgHeight; y++) 
		{
			for (int x=0; x<imgWidth; x++) 
			{
				pixelR = (BYTE)(*(pSrcData + pit*y + x*bitCount));
				pixelG = (BYTE)(*(pSrcData + pit*y + x*bitCount+1));
				pixelB = (BYTE)(*(pSrcData + pit*y + x*bitCount+2));

				grayVal=(BYTE)(pixelR*0.299 + pixelG*0.587  + pixelB*0.114 + 0.5);

				*(pNewData + NewPit*y + x) = grayVal;
				//pPixel= (BYTE*) GreyImg.GetPixelAddress(x,y);//取像素数据存放地址
				//*pPixel = grayVal;
				//address += 3;
			}
			//address += pit;
		}
	}
	else//灰度图
		GreyImg = LoadedImage;

	UpdateAllViews(NULL);		//发出重绘消息，调用OnDraw()函数绘制
}
