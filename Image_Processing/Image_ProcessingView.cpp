// Image_ProcessingView.cpp : CImage_ProcessingView 类的实现
//

#include "stdafx.h"
#include "Image_Processing.h"

#include "Image_ProcessingDoc.h"
#include "Image_ProcessingView.h"

#include "CImageOperation.h"
#include "math.h"

#define MaxKernelSize 15

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImage_ProcessingView

IMPLEMENT_DYNCREATE(CImage_ProcessingView, CView)

BEGIN_MESSAGE_MAP(CImage_ProcessingView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_HIST_DRAW, &CImage_ProcessingView::OnHistDraw)
	ON_COMMAND(ID_HIST_AVE, &CImage_ProcessingView::OnHistAve)
	ON_COMMAND(ID_HIST_MATCH, &CImage_ProcessingView::OnHistMatch)
	ON_COMMAND(ID_Roberts, &CImage_ProcessingView::OnRoberts)
	ON_COMMAND(ID_Prewitt, &CImage_ProcessingView::OnPrewitt)
	ON_COMMAND(ID_Sobel, &CImage_ProcessingView::OnSobel)
	ON_COMMAND(ID_Guassion, &CImage_ProcessingView::OnGuassion)
	ON_COMMAND(ID_Mean, &CImage_ProcessingView::OnMean)
	ON_COMMAND(ID_Median, &CImage_ProcessingView::OnMedian)
	ON_COMMAND(ID_Custom_Kernel, &CImage_ProcessingView::OnCustomKernel)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_COMMAND(ID_NEAR_NEI, &CImage_ProcessingView::OnNearNei)
	ON_COMMAND(ID_BILINEAR, &CImage_ProcessingView::OnBilinear)
	ON_COMMAND(ID_BICUBIC, &CImage_ProcessingView::OnBicubic)
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PLUS, &CImage_ProcessingView::OnPlus)
	ON_COMMAND(ID_MINUS, &CImage_ProcessingView::OnMinus)
	ON_COMMAND(ID_MULTIPLY, &CImage_ProcessingView::OnMultiply)
	ON_COMMAND(ID_DIVIDE, &CImage_ProcessingView::OnDivide)
	ON_COMMAND(ID_BinaryDilation, &CImage_ProcessingView::OnBinarydilation)
	ON_COMMAND(ID_BinaryErosion, &CImage_ProcessingView::OnBinaryerosion)
	ON_COMMAND(ID_BinaryOpen, &CImage_ProcessingView::OnBinaryopen)
	ON_COMMAND(ID_BinaryClose, &CImage_ProcessingView::OnBinaryclose)
	ON_COMMAND(ID_DTEuclidean, &CImage_ProcessingView::OnDteuclidean)
	ON_COMMAND(ID_DTCityBlock, &CImage_ProcessingView::OnDtcityblock)
	ON_COMMAND(ID_DTChessboard, &CImage_ProcessingView::OnDtchessboard)
	ON_COMMAND(ID_Skeleton, &CImage_ProcessingView::OnSkeleton)
	ON_COMMAND(ID_SkeletonReconstruction, &CImage_ProcessingView::OnSkeletonreconstruction)
	ON_COMMAND(ID_GrayDilation, &CImage_ProcessingView::OnGraydilation)
	ON_COMMAND(ID_GrayErosion, &CImage_ProcessingView::OnGrayerosion)
	ON_COMMAND(ID_GrayOpen, &CImage_ProcessingView::OnGrayopen)
	ON_COMMAND(ID_GrayClose, &CImage_ProcessingView::OnGrayclose)
	ON_COMMAND(ID_HW_CS, &CImage_ProcessingView::OnHwCs)
	ON_COMMAND(ID_HW_Count, &CImage_ProcessingView::OnHwCount)
	ON_COMMAND(ID_HW_Phone, &CImage_ProcessingView::OnHwPhone)
	ON_COMMAND(ID_HW_Noise, &CImage_ProcessingView::OnHwNoise)
	ON_COMMAND(ID_HW_Finger, &CImage_ProcessingView::OnHwFinger)
	ON_COMMAND(ID_HSI_Hist, &CImage_ProcessingView::OnHsiHist)
	ON_COMMAND(ID_HSI_HistAve, &CImage_ProcessingView::OnHsiHistave)
	ON_COMMAND(ID_HSISmooth, &CImage_ProcessingView::OnHsismooth)
END_MESSAGE_MAP()

// CImage_ProcessingView 构造/析构

CImage_ProcessingView::CImage_ProcessingView()
{
	// TODO: 在此处添加构造代码
	m_TransformEnable = false;
	m_TrackCursor = false;
	m_TransOffsetY = 0;
	m_TransOffsetX = 0;
	m_RotateAngle = 0;
	m_InterType = 0;
	m_ZoomX = 0;
	m_ZoomY = 0;
}

CImage_ProcessingView::~CImage_ProcessingView()
{
}

BOOL CImage_ProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	//设为黑色背景
	//cs.lpszClass=AfxRegisterWndClass(CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW,AfxGetApp()->LoadCursor(IDC_ARROW),CreateSolidBrush(RGB(0,0,0)));

	return CView::PreCreateWindow(cs);
}


void CImage_ProcessingView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_TransformEnable = false;
	m_TrackCursor = false;
	m_TransOffsetY = 0;
	m_TransOffsetX = 0;
	m_RotateAngle = 0;
	m_InterType = 0;
	m_ZoomX = 0;
	m_ZoomY = 0;
	Invalidate(true);
	OnDraw(GetDC());
}

// CImage_ProcessingView 绘制

void CImage_ProcessingView::OnDraw(CDC* pDC)
{
	CImage_ProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CImage* pImg = &(pDoc->GreyImg);
	if(!pImg->IsNull())
	{
		if(m_TransformEnable)
		{
			CImage TransImg, tmpImg;
			ImageRotate(&TransImg, pImg, m_RotateAngle, m_InterType);
			ImageZoom(&tmpImg, &TransImg, m_ZoomX, m_ZoomY, m_InterType);
			ImageTrans(&TransImg, &tmpImg, m_TransOffsetX, m_TransOffsetY);
			DrawImage(pDC, &TransImg, 0, 0);
		}
		else
			DrawImage(pDC, &(pDoc->LoadedImage), 0, 0);
	}
}

void CImage_ProcessingView::DrawImage(CDC* pDC, CImage* image, int PosX, int PosY)
{
	int	nWidth,nHeight,nXSrc,nYSrc;

	if(!image->IsNull())
	{
		nXSrc=0;
		nYSrc=0;
		nWidth=image->GetWidth();
		nHeight=image->GetHeight();
		image->BitBlt(*pDC,PosX,PosY,nWidth,nHeight,nXSrc,nYSrc,SRCCOPY);
	}
}

// CImage_ProcessingView 打印

BOOL CImage_ProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImage_ProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImage_ProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CImage_ProcessingView 诊断

#ifdef _DEBUG
void CImage_ProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CImage_ProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage_ProcessingDoc)));
	return (CImage_ProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImage_ProcessingView 消息处理程序

void CImage_ProcessingView::OnHistDraw()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	if(pDoc->GreyImg.IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CHistDlg histdlg(NULL, &(pDoc->LoadedImage));
	if(histdlg.DoModal()==-1)
		return;
}

void CImage_ProcessingView::OnHistAve()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	if(pDoc->GreyImg.IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	Histogram hist(&(pDoc->GreyImg));
	//DrawImage(GetDC(), hist.histAve(), pDoc->GreyImg.GetWidth() + 10, 0);

	CImage* ResultImg = hist.histAve();
	CImageDlg ImageDlg(NULL, ResultImg);
	//ImageDlg.Create(IDD_IMAGE);
	//ImageDlg.ShowWindow(SW_SHOW);
	//ImageDlg.SetWindowPos(&wndTopMost,   0,0,0,0,   SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW   );
	ImageDlg.DoModal();

	CHistDlg histdlg(NULL, ResultImg);
	histdlg.DoModal();
}

void CImage_ProcessingView::OnHistMatch()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	if(pDoc->GreyImg.IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	//加载模板图像
	//弹出选择文件对话框
	CString strFileName;
	CFileDialog OpenDlg(TRUE,_T("*.bmp"),NULL,NULL,_T("Image Files(*.bmp;*.jpg;*.png*;*.gif)|*.bmp;*.jpg;*.png*;*.gif|All Files (*.*)|*.*||"));
	if(OpenDlg.DoModal()==IDOK)
	{
		strFileName=OpenDlg.GetPathName();
	}
	else
		return;

	CImage TemplateImg;
	HRESULT hResult;
	hResult= TemplateImg.Load(strFileName);		//加载图像数据
	if (FAILED(hResult))
	{
		MessageBox(_T("载入图像失败"),_T("ERROR"),MB_OK | MB_ICONERROR);		//载入图像失败
		return;
	}

	//计算模板直方图
	Histogram HistTemplate(&TemplateImg);
	HistTemplate.computeHist(0);

	//直方图匹配
	Histogram Hist(&(pDoc->GreyImg));		//原图像直方图
	CImage* ResultImg = Hist.HistMatch(HistTemplate);

	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();

	//匹配后图像的直方图
	CHistDlg histdlg_r(NULL, ResultImg);
	histdlg_r.DoModal();

	//匹配模板的直方图
	CHistDlg histdlg_t(NULL, &TemplateImg);
	histdlg_t.DoModal();
}

CImage* CImage_ProcessingView::Convolution(CImage* pImg, double kernel[15][15], int KernelSize)
{
	if(pImg->IsNull())
		return NULL;

	int HalfSize = KernelSize / 2;
	CImage* tmpImg;

	//图像加边
	CImage *Img_tmp1 = AddImageBorder(pImg);
	for(int i=1;i<HalfSize;i++)
	{
		tmpImg = Img_tmp1;
		Img_tmp1 = AddImageBorder(Img_tmp1);
		tmpImg->Destroy();
	}

	CImage* Img_tmp2 = new CImage;
	ImageCopy(Img_tmp1, Img_tmp2);

	int imgWidth = Img_tmp1->GetWidth();
	int imgHeight = Img_tmp1->GetHeight();

	BYTE pixel;
	float tmp;
	int i,j,row,col;
	for( i=HalfSize; i<imgHeight - HalfSize; i++)
	{
		for( j=HalfSize; j<imgWidth - HalfSize ;j++)
		{
			tmp = 0;
			for(col=-HalfSize;col<=HalfSize;col++)
			{
				for(row=-HalfSize;row<=HalfSize;row++)
				{
					tmp += kernel[row+HalfSize][col+HalfSize] * CImageGetPixel(Img_tmp1, j+col, i+row);
				}
			}
			pixel = (BYTE)(fabs(tmp)+0.5);
			CImageSetPixel(Img_tmp2, j, i, pixel);
		}
	}

	//图像去边
	for(int i=0;i<HalfSize;i++)
	{
		tmpImg = Img_tmp2;
		Img_tmp2 = SubtractImageBorder(Img_tmp2);
		tmpImg->Destroy();
	}

	return Img_tmp2;
}

void CImage_ProcessingView::OnRoberts()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	if(pDoc->GreyImg.IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage *Img_tmp1 = AddImageBorder(&(pDoc->GreyImg));		//图像加边

	CImage* Img_tmp2 = new CImage;
	ImageCopy(Img_tmp1, Img_tmp2);

	int	 i,j,NewPixel;
	BYTE cPixel[3][3];
	int Scale = 2;
	int imgWidth = Img_tmp1->GetWidth();
	int imgHeight = Img_tmp1->GetHeight();
	for (i=1;i<imgHeight-1;i++) 
	{
		for (j=1;j<imgWidth-1;j++)
		{
			cPixel[1][1]=CImageGetPixel(Img_tmp1, j, i);
			cPixel[1][2]=CImageGetPixel(Img_tmp1, j+1, i);
			cPixel[2][1]=CImageGetPixel(Img_tmp1, j, i+1);
			cPixel[2][2]=CImageGetPixel(Img_tmp1, j+1, i+1);

			//交叉差分叠加
			//NewPixel = abs(cPixel[2][2] - cPixel[1][1]) + abs(cPixel[2][1] - cPixel[1][2]);
			float tmp1 = cPixel[2][2] - cPixel[1][1];
			float tmp2 = cPixel[2][1] - cPixel[1][2];
			NewPixel = (BYTE)(sqrt(tmp1*tmp1 + tmp2*tmp2)+0.5);
			//NewPixel *= Scale;//因信号较弱,按比例增大
			if (NewPixel>255)//超限处理
				NewPixel=255;

			CImageSetPixel(Img_tmp2, j, i, NewPixel);//结果存入输出图像
		}
	}

	CImage *ResultImg = SubtractImageBorder(Img_tmp2);

	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnPrewitt()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	if(pDoc->GreyImg.IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage* Img_tmp = AddImageBorder(&(pDoc->GreyImg));		//图像加边
	
	int imgWidth = Img_tmp->GetWidth();
	int imgHeight = Img_tmp->GetHeight();

	CImage* Img_Tmp_2 = new CImage;
	ImageCopy(Img_tmp, Img_Tmp_2);

	BYTE pixel;
	int pixelx,pixely;

	////X方向
	//float kernelx[3][3] = {-1, 0, 1
	//							 ,-1, 0, 1
	//							 ,-1, 0, 1};
	////Y方向
	//float kernely[3][3] = {-1, -1, -1
	//								,0, 0, 0
	//								,1, 1, 1};

	//
	//for(i=1;i<imgHeight-1;i++)
	//{
	//	for(j=1;j<imgWidth-1;j++)
	//	{
	//		pixelx = Convolution_ij(Img_tmp,i,j,kernelx);
	//		pixely = Convolution_ij(Img_tmp,i,j,kernely);
	//		pixel = sqrt((double)(pixelx*pixelx + pixely*pixely));
	//		pixel /= 2;
	//		CImageSetPixel(Img_Tmp_2,i,j,pixel);
	//	}
	//}

	int	cPixel[3][3];

	for (int i=1;i<imgHeight-1;i++) 
	{
		for (int j=1;j<imgWidth-1;j++)
		{
			cPixel[0][0]=CImageGetPixel(Img_tmp,j-1,i-1);
			cPixel[0][1]=CImageGetPixel(Img_tmp,j,i-1);
			cPixel[0][2]=CImageGetPixel(Img_tmp,j+1,i-1);

			cPixel[1][0]=CImageGetPixel(Img_tmp,j-1,i);
			//cPixel[1][1]=CImageGetPixel(Img_tmp,j,i);
			cPixel[1][2]=CImageGetPixel(Img_tmp,j+1,i);

			cPixel[2][0]=CImageGetPixel(Img_tmp,j-1,i+1);
			cPixel[2][1]=CImageGetPixel(Img_tmp,j,i+1);
			cPixel[2][2]=CImageGetPixel(Img_tmp,j+1,i+1);

			pixelx=(cPixel[0][2]+cPixel[1][2]+cPixel[2][2])-(cPixel[0][0]+cPixel[1][0]+cPixel[2][0]);
			pixely=(cPixel[2][0]+cPixel[2][1]+cPixel[2][2])-(cPixel[0][0]+cPixel[0][1]+cPixel[0][2]);
			pixel = sqrt((double)(pixelx*pixelx + pixely*pixely));
			//pixel /= 4;
			CImageSetPixel(Img_Tmp_2,j,i,pixel);
		}
	}

	CImage* ResultImg = SubtractImageBorder(Img_Tmp_2);
	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnSobel()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	if(pDoc->GreyImg.IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage* Img_tmp = AddImageBorder(&(pDoc->GreyImg));		//图像加边

	int imgWidth = Img_tmp->GetWidth();
	int imgHeight = Img_tmp->GetHeight();

	CImage* Img_Tmp_2 = new CImage;
	ImageCopy(Img_tmp, Img_Tmp_2);

	BYTE	pixel,pixelx,pixely;
	BYTE	cPixel[3][3];
	for (int i=1;i<imgHeight-1;i++) 
	{
		for (int j=1;j<imgWidth-1;j++)
		{
			cPixel[0][0]=CImageGetPixel(Img_tmp,j-1,i-1);
			cPixel[0][1]=CImageGetPixel(Img_tmp,j,i-1);
			cPixel[0][2]=CImageGetPixel(Img_tmp,j+1,i-1);

			cPixel[1][0]=CImageGetPixel(Img_tmp,j-1,i);
			//cPixel[1][1]=CImageGetPixel(Img_tmp,j,i);
			cPixel[1][2]=CImageGetPixel(Img_tmp,j+1,i);

			cPixel[2][0]=CImageGetPixel(Img_tmp,j-1,i+1);
			cPixel[2][1]=CImageGetPixel(Img_tmp,j,i+1);
			cPixel[2][2]=CImageGetPixel(Img_tmp,j+1,i+1);

			pixelx=abs((cPixel[0][2]+2*(int)cPixel[1][2]+(int)cPixel[2][2])-(cPixel[0][0]+2*(int)cPixel[1][0]+cPixel[2][0]));
			pixely=abs((cPixel[2][0]+2*(int)cPixel[2][1]+(int)cPixel[2][2])-(cPixel[0][0]+2*(int)cPixel[0][1]+cPixel[0][2]));
			pixel = pixelx>pixely?pixelx:pixely;
			//pixel = sqrt((double)(pixelx*pixelx + pixely*pixely));
			//pixel /= 4;
			CImageSetPixel(Img_Tmp_2,j,i,pixel);
		}
	}

	CImage* ResultImg = SubtractImageBorder(Img_Tmp_2);
	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnGuassion()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	//get sigma
	float sigma;
	CSigmaDlg sigmadlg;
	if(sigmadlg.DoModal()==IDCANCEL)
		return;
	sigma = sigmadlg.SigmaValue;

	//set kernel
	int KernelSize=3;
	CSliderDlg sDlg(NULL,&KernelSize, 3, 15);
	if(sDlg.DoModal()==IDCANCEL)
		return;
	if(KernelSize % 2 == 0)
		KernelSize++;

	double kernel[MaxKernelSize][MaxKernelSize];
	double radius = 3;
	double delta =  2 * radius / (KernelSize - 1);
	double dx = -radius, dy;
	double sum=0;
	for(int i=0;i<KernelSize;i++)
	{
		dy = -radius;
		for(int j=0;j<KernelSize;j++)
		{
			kernel[i][j] = exp(-(dx*dx + dy*dy)/(2*sigma*sigma));
			sum += kernel[i][j];
			dy += delta;
		}
		dx += delta;
	}

	//归一化
	for(int i=0;i<KernelSize;i++)
		for(int j=0;j<KernelSize;j++)
			kernel[i][j] /= sum;

	CImage* ResultImg = Convolution(pImg,kernel,KernelSize);

	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnMean()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	//set kernel
	int KernelSize=3;
	CSliderDlg sDlg(NULL,&KernelSize, 3, 15);
	sDlg.DoModal();
	if(KernelSize % 2 == 0)
		KernelSize++;
	double kernel[MaxKernelSize][MaxKernelSize];
	for(int i=0;i<KernelSize;i++)
		for(int j=0;j<KernelSize;j++)
			kernel[i][j] = (double)1/KernelSize/KernelSize;
	CImage* ResultImg = Convolution(pImg,kernel,KernelSize);

	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}

//排序取中值，BubbleSort
int  CImage_ProcessingView::GetMiddleBySort(BYTE *Window, int nSize)
{
	int i,j,nTemp;
	int nMiddleLocation = nSize / 2;

	for (i=0;i<nSize;i++) //冒泡排序
	{
		for (j=nSize-2;j>i;j--) 
		{
			if (Window[j]>Window[j+1]) 
			{	                           
				nTemp=Window[j];    
				Window[j]=Window[j+1];  
				Window[j+1]=nTemp;    
			}
		}
	}
	return(Window[nMiddleLocation]);	                             
}

void CImage_ProcessingView::OnMedian()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	//Get Kernel Size
	int KernelSize=3;
	CSliderDlg sDlg(NULL,&KernelSize, 3, 15);
	sDlg.DoModal();
	if(KernelSize % 2 == 0)
		KernelSize++;

	BYTE		kernel[MaxKernelSize*MaxKernelSize];

	int HalfSize = KernelSize / 2;
	CImage* tmpImg;

	//图像加边
	CImage *Img_tmp1 = AddImageBorder(pImg);
	for(int i=1;i<HalfSize;i++)
	{
		tmpImg = Img_tmp1;
		Img_tmp1 = AddImageBorder(Img_tmp1);
		tmpImg->Destroy();
	}

	CImage* Img_tmp2 = new CImage;
	ImageCopy(Img_tmp1, Img_tmp2);

	int imgWidth = Img_tmp1->GetWidth();
	int imgHeight = Img_tmp1->GetHeight();

	BYTE pixel;
	int tmp;
	int i,j,row,col;
	for( i=HalfSize; i<imgHeight - HalfSize; i++)
	{
		for( j=HalfSize; j<imgWidth - HalfSize ;j++)
		{
			tmp=0;
			for(col=-HalfSize;col<=HalfSize;col++)
			{
				for(row=-HalfSize;row<=HalfSize;row++)
				{
					kernel[tmp++] = CImageGetPixel(Img_tmp1, j+col, i+row);
				}
			}
			pixel = GetMiddleBySort(kernel, KernelSize*KernelSize);
			CImageSetPixel(Img_tmp2, j, i, pixel);
		}
	}

	//图像去边
	for(int i=0;i<HalfSize;i++)
	{
		tmpImg = Img_tmp2;
		Img_tmp2 = SubtractImageBorder(Img_tmp2);
		tmpImg->Destroy();
	}

	CImageDlg ImageDlg(NULL, Img_tmp2);
	ImageDlg.DoModal();
}

//判断字符的的内容是否全是是数字 
bool IsAllDigital(const CString &str)
{  
    CString newString;  
    newString = str.SpanIncluding(_T("0123456789.-"));  
    if(newString == str)  
    {  
        return true;
    }  
    else  
    {  
        return false; 
    }
    return false;
}

void CImage_ProcessingView::OnCustomKernel()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	//弹出选择文件对话框
	CString filename;
	CString FilePathName;
	CFileDialog dlg(TRUE,_T("*.txt"),_T("CustomKernel"),NULL,_T("Text Files|*.txt|All Files (*.*)|*.*||"));
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		filename=dlg.GetFileTitle();
	}
	else
		return;

	//对文件进行读操作     
	CStdioFile   mFile;  
	if(!mFile.Open(FilePathName,CFile::modeRead))
	{
		//MessageBox(_T("文件不存在！"),_T("Error"),MB_OK|MB_ICONSTOP);
		return;
	}

	CString strTemp;

	//读取数据
	double kernel[MaxKernelSize][MaxKernelSize];
	int KernelSize;
	mFile.ReadString(strTemp);
	KernelSize=atof(strTemp);
	mFile.ReadString(strTemp);
	for(int i=0;i<KernelSize;i++)
	{
		CString temp[MaxKernelSize];
		for(int j=0;j<KernelSize;j++)
		{
			AfxExtractSubString( temp[j], (LPCTSTR)strTemp, j,'\t');
			if(!IsAllDigital(temp[j]))
			{
				//MessageBox(_T("内容书写格式不正确，请按照Sample.txt的格式书写。"),_T("Error"),MB_OK|MB_ICONSTOP);
				return;
			}
		}
		for(int j=0;j<KernelSize;j++)
			kernel[i][j]=atof(temp[j]);
		mFile.ReadString (strTemp);
	}
	mFile.Close();

	CImage* ResultImg = Convolution(pImg,kernel,KernelSize);

	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_TrackCursor)
	{
		CPoint pnt = point - m_CurrPnt;
		switch(nFlags)
		{
		case MK_LBUTTON:
			m_ZoomX += pnt.x;
			m_ZoomY += pnt.y;
			break;
		case MK_MBUTTON:
			m_TransOffsetX += pnt.x;
			m_TransOffsetY += pnt.y;
			break;
		case MK_RBUTTON:
			m_RotateAngle += (pnt.x + pnt.y) / 2;
			break;
		default:
			break;
		}
		m_CurrPnt = point;
		Invalidate(true);
	}

	CView::OnMouseMove(nFlags, point);
}

void CImage_ProcessingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_TransformEnable)
	{
		m_CurrPnt = point;
		m_TrackCursor = true;
	}

	CView::OnLButtonDown(nFlags, point);
}

void CImage_ProcessingView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_TransformEnable)
	{
		m_CurrPnt = point;
		m_TrackCursor = true;
	}

	CView::OnMButtonDown(nFlags, point);
}

void CImage_ProcessingView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_TransformEnable)
	{
		m_CurrPnt = point;
		m_TrackCursor = true;
	}

	CView::OnRButtonDown(nFlags, point);
}

void CImage_ProcessingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_TrackCursor = false;

	CView::OnLButtonUp(nFlags, point);
}

void CImage_ProcessingView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_TrackCursor = false;

	CView::OnMButtonUp(nFlags, point);
}

void CImage_ProcessingView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_TrackCursor = false;

	CView::OnRButtonUp(nFlags, point);
}

void CImage_ProcessingView::OnNearNei()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = true;
	m_InterType = 0;
}

void CImage_ProcessingView::OnBilinear()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = true;
	m_InterType = 1;
}

void CImage_ProcessingView::OnBicubic()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = true;
	m_InterType = 2;
}


void CImage_ProcessingView::OnPlus()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	//弹出选择文件对话框
	CString strFileName;
	CFileDialog OpenDlg(TRUE,_T("*.bmp"),NULL,NULL,_T("Image Files(*.bmp;*.jpg;*.png*;*.gif)|*.bmp;*.jpg;*.png*;*.gif|All Files (*.*)|*.*||"));
	if(OpenDlg.DoModal()==IDOK)
	{
		strFileName=OpenDlg.GetPathName();
	}
	else
		return;

	CImage pNewImg;
	HRESULT hResult;
	hResult= pNewImg.Load(strFileName);		//加载图像数据
	if (FAILED(hResult))
	{
		MessageBox(_T("载入图像失败"),_T("ERROR"),MB_OK | MB_ICONERROR);		//载入图像失败
		return;
	}

	CImage CloneImg;
	ImageCopy(pImg, &CloneImg);

	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();
	int NewImgWidth = pNewImg.GetWidth();
	int NewImgHeight = pNewImg.GetHeight();

	int width = imgWidth > NewImgWidth ? NewImgWidth : imgWidth;
	int height = imgHeight > NewImgHeight ? NewImgHeight : imgHeight;

	BYTE pixel;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			pixel = CImageGetPixel(pImg, j, i);
			pixel += CImageGetPixel(&pNewImg, j, i);
			CImageSetPixel(&CloneImg, j, i, pixel / 2);
		}
	}

	CImageDlg ImageDlg(NULL, &CloneImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnMinus()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	CImage CloneImg;
	ImageCopy(pImg, &CloneImg);

	m_TransformEnable = false;

	//弹出选择文件对话框
	CString strFileName;
	CFileDialog OpenDlg(TRUE,_T("*.bmp"),NULL,NULL,_T("Image Files(*.bmp;*.jpg;*.png*;*.gif)|*.bmp;*.jpg;*.png*;*.gif|All Files (*.*)|*.*||"));
	if(OpenDlg.DoModal()==IDOK)
	{
		strFileName=OpenDlg.GetPathName();
	}
	else
		return;

	CImage pNewImg;
	HRESULT hResult;
	hResult= pNewImg.Load(strFileName);		//加载图像数据
	if (FAILED(hResult))
	{
		MessageBox(_T("载入图像失败"),_T("ERROR"),MB_OK | MB_ICONERROR);		//载入图像失败
		return;
	}

	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();
	int NewImgWidth = pNewImg.GetWidth();
	int NewImgHeight = pNewImg.GetHeight();

	int width = imgWidth > NewImgWidth ? NewImgWidth : imgWidth;
	int height = imgHeight > NewImgHeight ? NewImgHeight : imgHeight;

	BYTE pixel;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			pixel = CImageGetPixel(pImg, j, i);
			pixel -= CImageGetPixel(&pNewImg, j, i);
			CImageSetPixel(&CloneImg, j, i, abs(pixel));
		}
	}

	CImageDlg ImageDlg(NULL, &CloneImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnMultiply()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	//弹出选择文件对话框
	CString strFileName;
	CFileDialog OpenDlg(TRUE,_T("*.bmp"),NULL,NULL,_T("Image Files(*.bmp;*.jpg;*.png*;*.gif)|*.bmp;*.jpg;*.png*;*.gif|All Files (*.*)|*.*||"));
	if(OpenDlg.DoModal()==IDOK)
	{
		strFileName=OpenDlg.GetPathName();
	}
	else
		return;

	CImage pNewImg;
	HRESULT hResult;
	hResult= pNewImg.Load(strFileName);		//加载图像数据
	if (FAILED(hResult))
	{
		MessageBox(_T("载入图像失败"),_T("ERROR"),MB_OK | MB_ICONERROR);		//载入图像失败
		return;
	}

	CImage CloneImg;
	ImageCopy(pImg, &CloneImg);

	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();
	int NewImgWidth = pNewImg.GetWidth();
	int NewImgHeight = pNewImg.GetHeight();

	int width = imgWidth > NewImgWidth ? NewImgWidth : imgWidth;
	int height = imgHeight > NewImgHeight ? NewImgHeight : imgHeight;

	float imgData[MaxImageSize][MaxImageSize];
	float tmp,max=0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			tmp = CImageGetPixel(pImg, j, i);
			tmp *= CImageGetPixel(&pNewImg, j, i);
			imgData[i][j] = tmp;
			if(tmp>max)
				max = tmp;
		}
	}

	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			CImageSetPixel(&CloneImg, j, i, (BYTE)(imgData[i][j] / max * 255));

	CImageDlg ImageDlg(NULL, &CloneImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnDivide()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	CImage CloneImg;
	ImageCopy(pImg, &CloneImg);

	m_TransformEnable = false;

	//弹出选择文件对话框
	CString strFileName;
	CFileDialog OpenDlg(TRUE,_T("*.bmp"),NULL,NULL,_T("Image Files(*.bmp;*.jpg;*.png*;*.gif)|*.bmp;*.jpg;*.png*;*.gif|All Files (*.*)|*.*||"));
	if(OpenDlg.DoModal()==IDOK)
	{
		strFileName=OpenDlg.GetPathName();
	}
	else
		return;

	CImage pNewImg;
	HRESULT hResult;
	hResult= pNewImg.Load(strFileName);		//加载图像数据
	if (FAILED(hResult))
	{
		MessageBox(_T("载入图像失败"),_T("ERROR"),MB_OK | MB_ICONERROR);		//载入图像失败
		return;
	}

	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();
	int NewImgWidth = pNewImg.GetWidth();
	int NewImgHeight = pNewImg.GetHeight();

	int width = imgWidth > NewImgWidth ? NewImgWidth : imgWidth;
	int height = imgHeight > NewImgHeight ? NewImgHeight : imgHeight;

	float imgData[MaxImageSize][MaxImageSize];
	float tmp,max=0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			tmp = CImageGetPixel(&pNewImg, j, i);
			//除数不为0
			if(tmp==0)
				tmp=1;
			tmp /= CImageGetPixel(pImg, j, i);
			imgData[i][j] = tmp;
			if(tmp>max)
				max = tmp;
		}
	}

	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			CImageSetPixel(&CloneImg, j, i, (BYTE)(imgData[i][j] / max * 255));

	CImageDlg ImageDlg(NULL, &CloneImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::ReadSEFromFile(StructureElement &se)
{
	//弹出选择文件对话框
	CString filename;
	CString FilePathName;
	CFileDialog dlg(TRUE,_T("*.txt"),_T("CustomSE"),NULL,_T("Text Files|*.txt|All Files (*.*)|*.*||"));
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		filename=dlg.GetFileTitle();
	}
	else
		return;

	//对文件进行读操作     
	CStdioFile   mFile;  
	if(!mFile.Open(FilePathName,CFile::modeRead))
	{
		//MessageBox(_T("文件不存在！"),_T("Error"),MB_OK|MB_ICONSTOP);
		return;
	}

	CString strTemp;

	//读取数据
	CString temp[MaxSESize];

	//读取se的Size
	mFile.ReadString(strTemp);
	AfxExtractSubString( temp[0], (LPCTSTR)strTemp, 0,'\t');
	se.m_Width = atof(temp[0]);
	AfxExtractSubString( temp[1], (LPCTSTR)strTemp, 1,'\t');
	se.m_Height = atof(temp[1]);

	//读取SE中心
	mFile.ReadString(strTemp);
	AfxExtractSubString( temp[0], (LPCTSTR)strTemp, 0,'\t');
	se.m_CenterX = atof(temp[0]);
	AfxExtractSubString( temp[1], (LPCTSTR)strTemp, 1,'\t');
	se.m_CenterY = atof(temp[1]);
	
	//读取SE数据
	mFile.ReadString(strTemp);
	for(int i=0;i<se.m_Height;i++)
	{
		for(int j=0;j<se.m_Width;j++)
		{
			AfxExtractSubString( temp[j], (LPCTSTR)strTemp, j,'\t');
			if(!IsAllDigital(temp[j]))
			{
				//MessageBox(_T("内容书写格式不正确，请按照Sample.txt的格式书写。"),_T("Error"),MB_OK|MB_ICONSTOP);
				return;
			}
			se.m_Data[i][j] = atof(temp[j]);
		}
		mFile.ReadString (strTemp);
	}
	mFile.Close();
}

void CImage_ProcessingView::OnBinarydilation()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);

	CImage* pNewImg = new CImage;
	pNewImg = BinaryDilation(pImg,se);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnBinaryerosion()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);

	CImage* pNewImg = new CImage;
	pNewImg = BinaryErosion(pImg,se);

	//CImage* tmp;
	//for(int i=0;i<30;i++)
	//{
	//	tmp=pNewImg;
	//	pNewImg = BinaryErosion(tmp,se);
	//}

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnBinaryopen()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);

	CImage* pNewImg = new CImage;
	pNewImg = BinaryErosion(pImg,se);
	CImage* pResultImg = new CImage;
	pResultImg = BinaryDilation(pNewImg,se);

	CImageDlg ImageDlg(NULL, pResultImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnBinaryclose()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);
	//se.CreateCircleSE(2);

	CImage* pNewImg = new CImage;
	pNewImg = BinaryDilation(pImg,se);
	CImage* pResultImg = new CImage;
	pResultImg = BinaryErosion(pNewImg,se);

	CImageDlg ImageDlg(NULL, pResultImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnDteuclidean()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage* pNewImg = new CImage;
	pNewImg = DistanceTransform(pImg,0);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnDtcityblock()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage* pNewImg = new CImage;
	pNewImg = DistanceTransform(pImg,1);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnDtchessboard()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage* pNewImg = new CImage;
	pNewImg = DistanceTransform(pImg,2);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnSkeleton()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage* pNewImg = new CImage;
	pNewImg = Skeleton(pImg);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnSkeletonreconstruction()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	CImage* pNewImg = new CImage;
	pNewImg = SkeletonReconstruct(pImg);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnGraydilation()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);

	CImage* pNewImg = new CImage;
	pNewImg = GrayDilation(pImg,se);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnGrayerosion()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);

	CImage* pNewImg = new CImage;
	pNewImg = GrayErosion(pImg,se);

	CImageDlg ImageDlg(NULL, pNewImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnGrayopen()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);

	CImage* pNewImg = new CImage;
	pNewImg = GrayErosion(pImg,se);
	CImage* pResultImg = new CImage;
	pResultImg = GrayDilation(pNewImg,se);

	CImageDlg ImageDlg(NULL, pResultImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnGrayclose()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	ReadSEFromFile(se);

	CImage* pNewImg = new CImage;
	pNewImg = GrayDilation(pImg,se);
	CImage* pResultImg = new CImage;
	pResultImg = GrayErosion(pNewImg,se);

	CImageDlg ImageDlg(NULL, pResultImg);
	ImageDlg.DoModal();
}

void CImage_ProcessingView::OnHwCount()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	Binarize(pImg,220);
	StructureElement se;
	se.CreateDiscSE();
	//pImg=BinaryDilation(pImg,se);
	//pImg=BinaryErosion(pImg,se);
	//pImg=BinaryDilation(pImg,se);
	//pImg=BinaryErosion(pImg,se);

	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pImg->GetBits();
	int lineByte = pImg->GetPitch();
	int sBitCount = pImg->GetBPP()/8;

	int MinX,MaxX,MinY,MaxY;
	MaxX=MaxY=0;
	MinX=imgWidth;
	MinY=imgHeight;

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			if(*(imgBufIn+i*lineByte+j*sBitCount)==255)
			{
				if(i<MinY)
					MinY=i;
				if(i>MaxY)
					MaxY=i;
				if(j<MinX)
					MinX=j;
				if(j>MaxX)
					MaxX=j;
			}

	int r=(MaxX-MinX+MaxY-MinY)/4 - 9;
	int CenterX = (MaxX+MinX)/2;
	int CenterY = (MaxY+MinY)/2;

	int delta = 1;
	int Result=0;
	int CurrColor;
	int x,y;

	x=CenterX+r*cos((float)0);
	y=CenterY+r*sin((float)0);
	CurrColor=*(imgBufIn+y*lineByte+x*sBitCount);
	for(int d=0;d<360;d+=delta)
	{
		x=CenterX+r*cos((float)d/180*3.14159);
		y=CenterY+r*sin((float)d/180*3.14159);
		if(*(imgBufIn+y*lineByte+x*sBitCount)!=CurrColor)
		{
			CurrColor=*(imgBufIn+y*lineByte+x*sBitCount);
			Result++;
		}
		*(imgBufIn+y*lineByte+x*sBitCount)=128;
	}
	Result /= 2;

	CString str;
	str.Format("%d",Result);
	CImageDlg ImageDlg(NULL, pImg);
	ImageDlg.DoModal();
	MessageBox(_T("齿轮齿数为：") +str,_T("Result"),MB_OK);
}


void CImage_ProcessingView::OnHwCs()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();

	BYTE* pSrc = ReadCImage(pImg);
	BYTE *Tmp;

	//分离圆盘
	StructureElement se;
	se.CreateCircleSE(6);
	BYTE* CenterMap;
	Tmp=ArrayBinaryOpen(pSrc,imgWidth,imgHeight,se);
	int CircleCount = CountDomain(Tmp,imgWidth,imgHeight,CenterMap);
	delete[] Tmp;
	BYTE* pResultC_Array= ArrayBinaryDilation(CenterMap,imgWidth,imgHeight,se);
	delete[] CenterMap;
	CImage* pResultC = CreateGrayImg(pResultC_Array,imgWidth,imgHeight);
	delete[] pResultC_Array;

	CImageDlg ImageDlg(NULL, pResultC);
	ImageDlg.DoModal();

	CString str;
	str.Format("%d",CircleCount);
	MessageBox(_T("圆盘个数为：") +str,_T("Result"),MB_OK);

	//分离棒子
	StructureElement ses;
	ses.CreateCircleSE(1);

	CImage *pTmpS, *pImg_e;
	pImg_e = BinaryErosion(pImg,ses);
	//CImageDlg ImageDlg1(NULL, pImg_e);
	//ImageDlg1.DoModal();

	//for(int degree=4;degree<180;degree+=11)
	//{
	//	se.CreateLineSE(35,degree);
	//	pTmpS = BinaryOpen(pImg,se);
	//	//pTmpS = BinaryDilation(pTmpS,ses);
	//	CImageDlg ImageDlg(NULL, pTmpS);
	//	ImageDlg.DoModal();
	//	
	//	delete pTmpS;
	//	delete ImageDlg;
	//}

	int StickCount=0;
	int* len = new int[20];

	//BYTE* pImgE = ArrayBinaryErosion(pSrc,imgWidth,imgHeight,ses);
	BYTE* pImgE = ReadCImage(pImg_e);
	//CImage* ttt = CreateGrayImg(pImgE,imgWidth,imgHeight);
	//CImageDlg ImageDlg1(NULL, ttt);
	//ImageDlg1.DoModal();

	BYTE *pTmpO, *pTmpD;
	BYTE* pResultB_Array = new BYTE[imgWidth*imgHeight];

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pResultB_Array[i*imgWidth+j] = 0;

	for(int degree=4;degree<170;degree+=10)
	{
		se.CreateLineSE(35,degree);
		pTmpO = ArrayBinaryOpen(pImgE,imgWidth,imgHeight,se);
		pTmpD = ArrayGrayDilation(pTmpO,imgWidth,imgHeight,ses);
		BinaryUnion(pResultB_Array,pTmpD,imgWidth,imgHeight);

		//CImage* ttt = CreateGrayImg(pTmpD,imgWidth,imgHeight);
		//CImageDlg ImageDlg2(NULL, ttt);
		//ImageDlg2.DoModal();

		StickCount += CountDomain_Len(pTmpD,imgWidth,imgHeight,len,StickCount);

		delete[] pTmpO;
		delete[] pTmpD;

		if(degree>80)
			degree += 2;
		if(degree>130)
			degree -= 2;
	}
	delete[] pImgE;

	CImage	* pResultB = CreateGrayImg(pResultB_Array,imgWidth,imgHeight);
	CImageDlg ImageDlg2(NULL, pResultB);
	ImageDlg2.DoModal();

	str.Format("%d",StickCount);
	CString lll = "\n长度分别为：";
	CString tmpstr;
	for(int i=0;i<StickCount;i++)
	{
		tmpstr.Format("%d,",len[i]);
		lll+=tmpstr;
	}
	MessageBox(_T("棒子个数为：") +str+lll,_T("Result"),MB_OK);
}

void CImage_ProcessingView::OnHwPhone()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	se.CreateCircleSE(4);
	CImage* pC = BinaryClose(pImg,se);
	CImage* pResult = Skeleton(pC);
	pC->Destroy();
	delete pC;

	CImageDlg ImageDlg(NULL, pResult);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnHwNoise()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	//ReadSEFromFile(se);
	se.CreateSquareSE(3);

	CImage* pNewImg = GrayClose(pImg,se);
	CImage* pResultImg = GrayOpen(pNewImg,se);

	pNewImg->Destroy();
	delete pNewImg;

	CImageDlg ImageDlg(NULL, pResultImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnHwFinger()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->GreyImg);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	m_TransformEnable = false;

	StructureElement se;
	//ReadSEFromFile(se);
	se.CreateSquareSE(3);

	CImage* pNewImg = BinaryOpen(pImg,se);
	CImage* pResultImg = BinaryClose(pNewImg,se);

	pNewImg->Destroy();
	delete pNewImg;

	CImageDlg ImageDlg(NULL, pResultImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnHsiHist()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->LoadedImage);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	int imgWidth=pImg->GetWidth();
	int imgHeight=pImg->GetHeight();
	BYTE* pData;
	pData=ReadColorCImage(pImg);
	RGB2HSI(pData,imgWidth,imgHeight);

	BYTE* ChannelI=ExtractChannel(pData,2,imgWidth,imgHeight);
	CImage* pImgChanelI=CreateGrayImg(ChannelI,imgWidth,imgHeight);

	CHistDlg histdlg(NULL, pImgChanelI);
	if(histdlg.DoModal()==-1)
		return;
}


void CImage_ProcessingView::OnHsiHistave()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->LoadedImage);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	int imgWidth=pImg->GetWidth();
	int imgHeight=pImg->GetHeight();
	BYTE* pData;
	pData=ReadColorCImage(pImg);
	RGB2HSI(pData,imgWidth,imgHeight);

	BYTE* ChannelI=ExtractChannel(pData,2,imgWidth,imgHeight);
	CImage* pImgChanelI=CreateGrayImg(ChannelI,imgWidth,imgHeight);

	Histogram hist(pImgChanelI);
	CImage* HistAveImg = hist.histAve();

	BYTE* HistAveChannelI=ReadCImage(HistAveImg);
	SetChannel(pData,HistAveChannelI,2,imgWidth,imgHeight);
	HSI2RGB(pData,imgWidth,imgHeight);
	CImage* ResultImg=CreateColorImg(pData,imgWidth,imgHeight);

	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}


void CImage_ProcessingView::OnHsismooth()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingDoc* pDoc = GetDocument();
	CImage *pImg = &(pDoc->LoadedImage);
	if(pImg->IsNull())
	{
		MessageBox(_T("请先载入图像"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}

	int imgWidth=pImg->GetWidth();
	int imgHeight=pImg->GetHeight();
	BYTE* pData;
	pData=ReadColorCImage(pImg);
	RGB2HSI(pData,imgWidth,imgHeight);

	HSI2RGB(pData,imgWidth,imgHeight);
	CImage* ResultImg=CreateColorImg(pData,imgWidth,imgHeight);

	CImageDlg ImageDlg(NULL, ResultImg);
	ImageDlg.DoModal();
}
