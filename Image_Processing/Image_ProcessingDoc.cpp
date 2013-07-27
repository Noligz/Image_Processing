// Image_ProcessingDoc.cpp : CImage_ProcessingDoc ���ʵ��
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


// CImage_ProcessingDoc ����/����

CImage_ProcessingDoc::CImage_ProcessingDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CImage_ProcessingDoc::~CImage_ProcessingDoc()
{
}

BOOL CImage_ProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CImage_ProcessingDoc ���л�

void CImage_ProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CImage_ProcessingDoc ���

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


// CImage_ProcessingDoc ����

void CImage_ProcessingDoc::OnFileOpen()
{
	// TODO: �ڴ���������������

	//����ѡ���ļ��Ի���
	CString strFileName;
	CFileDialog OpenDlg(TRUE,_T("*.bmp"),NULL,NULL,_T("Image Files(*.bmp;*.jpg;*.png*;*.gif)|*.bmp;*.jpg;*.png*;*.gif|All Files (*.*)|*.*||"));
	if(OpenDlg.DoModal()==IDOK)
	{
		strFileName=OpenDlg.GetPathName();
	}
	else
		return;

	if(!LoadedImage.IsNull())
		LoadedImage.Destroy();		//���ԭ��ͼ��

	HRESULT hResult;
	hResult= LoadedImage.Load(strFileName);		//����ͼ������
	if (FAILED(hResult))
	{
		MessageBox(NULL,_T("����ͼ��ʧ��"),_T("ERROR"),MB_OK | MB_ICONERROR);		//����ͼ��ʧ��
		return;
	}

	if(!GreyImg.IsNull())
		GreyImg.Destroy();
	if(LoadedImage.GetBPP() == 24)//��ͼ
	{
		//�һ�
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
				//pPixel= (BYTE*) GreyImg.GetPixelAddress(x,y);//ȡ�������ݴ�ŵ�ַ
				//*pPixel = grayVal;
				//address += 3;
			}
			//address += pit;
		}
	}
	else//�Ҷ�ͼ
		GreyImg = LoadedImage;

	UpdateAllViews(NULL);		//�����ػ���Ϣ������OnDraw()��������
}
