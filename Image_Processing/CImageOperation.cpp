#include "stdafx.h"
#include "CImageOperation.h"

#define MaxImgSize 1024

// 复制调色板
void CopyColorTables(CImage * pNewImage, CImage * pImage)
{
	int				nColorTableEntries;
	RGBQUAD	ColorTabs[256];

	nColorTableEntries=pImage->GetMaxColorTableEntries();
 	if (nColorTableEntries>0)
	{
		pImage->GetColorTable(0,nColorTableEntries,ColorTabs);    
		pNewImage->SetColorTable(0,nColorTableEntries,ColorTabs);   //  复制调色板
	}
}

int GetBytesPerPixel(CImage *pImage)
{
int		nBytesPerPixel;

	nBytesPerPixel= pImage->GetBPP()/8;

	return nBytesPerPixel;
}

int GetBytesPerLine(CImage *pImage)
{
int		nBytesPerLine;

	nBytesPerLine=(pImage->GetWidth()*pImage->GetBPP()+31)/32*4;

	return nBytesPerLine;
}

bool IsPntInImage(CImage* pImg, int x, int y)
{
	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();
	if(x<0 || x>imgWidth || y<0 || y>imgHeight)
		return false;
	return true;
}

// CImage类对象之间的复制
void ImageCopy(CImage * pImage, CImage * pNewImage)
{
	 CDC			*pDC,*pNewDC;
	int				nWidth,nHeight,nBitsPerPixel;

	nWidth=pImage->GetWidth();
	nHeight=pImage->GetHeight();
	nBitsPerPixel=pImage->GetBPP();

	if(!pNewImage->IsNull())//  目标位图非空
	 	pNewImage->Destroy();//  删除目标位图
	pNewImage->Create(nWidth,nHeight,nBitsPerPixel,0); //  建立CImage类对象新位图
	CopyColorTables(pNewImage,pImage);//复制调色板

 	pDC= CDC::FromHandle(pImage->GetDC());//  建立源位图CDC
 	pNewDC=CDC::FromHandle(pNewImage->GetDC());//  建立目标位图CDC

 	pNewDC->BitBlt(0,0,nWidth,nHeight,pDC,0,0,SRCCOPY);//复制像素数据

 	pImage->ReleaseDC();//  释放CDC指针
 	pNewImage->ReleaseDC();
}

//BYTE CImageGetPixel(const CImage* pImg, int i, int j)
//{
//	COLORREF pixel;
//	pixel = pImg->GetPixel(i,j);
//	return GetRValue(pixel);
//}

// 取单个像素数据
BYTE CImageGetPixel(CImage* pImage, int x, int y)
{
	if (!IsPntInImage(pImage,x,y)) //坐标超出范围退出
		return 0;

	//DWORD	dwPixelValue;
	//BYTE		*pPixel;

	//pPixel= (BYTE*) pImage->GetPixelAddress(x,y);//取像素数据存放地址

	//switch(pImage->GetBPP())
	//{
	//case 1:		memcpy(&dwPixelValue,pPixel,1);//传出像素数据
	//				dwPixelValue=dwPixelValue & 0x1;
	//				break;

	//case 4:		memcpy(&dwPixelValue,pPixel,1);//传出像素数据
	//				dwPixelValue=dwPixelValue & 0xf;
	//				break;

	//case 8:		memcpy(&dwPixelValue,pPixel,1);//传出像素数据
	//				break;

	//case 24:		memcpy(&dwPixelValue,pPixel,3);//传出像素数据
	//				dwPixelValue=dwPixelValue & 0xffffff;
	//				break;
	//}

	//return (BYTE)(dwPixelValue);

   byte* pRealData;
   pRealData=(byte*)pImage->GetBits();
   int pit=pImage->GetPitch();
   int bitCount=pImage->GetBPP()/8;
   BYTE pixel = *(pRealData + pit*y + x*bitCount);
   return pixel;
}


void CImageSetPixel(CImage* pImg, int x, int y, BYTE value)
{
	//超出图像范围
	if(!IsPntInImage(pImg,x,y) )
		return;

	//限制灰度值
	if(value<0)
		value=0;
	if(value>255)
		value=255;

	//pImg->SetPixelRGB(x, y, value, value, value);

	//BYTE *pPixel= (BYTE*) pImg->GetPixelAddress(x,y);//取像素数据存放地址
	////memcpy(pPixel,&value,1);
	//*pPixel = value;

   byte* pRealData;
   pRealData=(byte*)pImg->GetBits();
   int pit=pImg->GetPitch();
   int bitCount=pImg->GetBPP()/8;
   *(pRealData + pit*y + x*bitCount) = value;
}

int RestrictNum(int &num, int min, int max)
{
	if(num<min)
		return min;
	if(num>max)
		return max;
	return num;
}

//加宽图像(图像加边),在上下方向各扩展一行，在左右方向各扩展一列
CImage* AddImageBorder(CImage *pImage)
{
	CImage *pNewImage = new CImage;
	BYTE	*pPixel,*pNewPixel;
	int		i,nWidth,nHeight,nBytesPerLine,nBytesPerPixel;
 
	nWidth=pImage->GetWidth();
	nHeight=pImage->GetHeight();
	nBytesPerLine=GetBytesPerLine(pImage);
	nBytesPerPixel=GetBytesPerPixel(pImage);

	pNewImage->Create(nWidth+2,nHeight+2,pImage->GetBPP());
	CopyColorTables(pNewImage,pImage);//复制调色板

	//向新图像逐行复制像素数据
	for (i=0;i<nHeight;i++) 
	{
		pPixel  = (BYTE*) pImage->GetPixelAddress(0,i);       
		pNewPixel = (BYTE*) pNewImage->GetPixelAddress(1,i+1);
  		memcpy(pNewPixel, pPixel,nBytesPerLine);                         
	}

	//扩展顶边一行
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,0); 
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(1,0);
  	memcpy(pNewPixel, pPixel,nBytesPerLine);

	//扩展底边一行
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,nHeight-1);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(1,nHeight+1);
  	memcpy(pNewPixel, pPixel,nBytesPerLine);

	//扩展左边一列
	for(i=0;i<nHeight;i++)
	{
		pPixel  = (BYTE*) pImage->GetPixelAddress(0,i);
		pNewPixel = (BYTE*) pNewImage->GetPixelAddress(0,i+1);
  		memcpy(pNewPixel,pPixel,nBytesPerPixel);
	}

	//扩展右边一列
	for(i=0;i<nHeight;i++)
	{
		pPixel  = (BYTE*) pImage->GetPixelAddress(nWidth-1,i);
		pNewPixel = (BYTE*) pNewImage->GetPixelAddress(nWidth+1,i+1);
  		memcpy(pNewPixel,pPixel,nBytesPerPixel);
	}

	//复制左上角点
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,0);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(0,0);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	//复制右上角点
	pPixel  = (BYTE*) pImage->GetPixelAddress(nWidth-1,0);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(nWidth+1,0);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	//复制左下角点
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,nHeight-1);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(0,nHeight+1);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	//复制右下角点
	pPixel  = (BYTE*) pImage->GetPixelAddress(nWidth-1,nHeight-1);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(nWidth+1,nHeight+1);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	return pNewImage;
}

//图像去边
CImage* SubtractImageBorder(CImage *pImage)
{
	CImage *pNewImage = new CImage;
	CDC		*pDC,*pNewDC;
	int	nWidth,nHeight,nBitsPerPixel;

	nWidth=pImage->GetWidth()-2;
	nHeight=pImage->GetHeight()-2;
	nBitsPerPixel=pImage->GetBPP();

	if(!pNewImage->IsNull())//  目标位图非空
	 	pNewImage->Destroy();//  删除目标位图
	pNewImage->Create(nWidth,nHeight,nBitsPerPixel,0); //  建立CImage类对象新位图
	CopyColorTables(pNewImage,pImage);//复制调色板

 	pDC= CDC::FromHandle(pImage->GetDC());//  建立源位图CDC
 	pNewDC=CDC::FromHandle(pNewImage->GetDC());//  建立目标位图CDC

 	pNewDC->BitBlt(0,0,nWidth,nHeight,pDC,1,1,SRCCOPY);//复制像素数据

 	pImage->ReleaseDC();//  释放CDC指针
 	pNewImage->ReleaseDC();

	return pNewImage;
}

void ImageTrans(CImage* pDestImg, CImage* pSrcImg, int &offsetX, int &offsetY)
{
	if(offsetX<0)
		offsetX = 0;
	if(offsetY<0)
		offsetY = 0;

	int nBitsPerPixel=pSrcImg->GetBPP();

	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();

	int newWidth = imgWidth+offsetX;
	int newHeight = imgHeight+offsetY;

	if(!pDestImg->IsNull())//  目标位图非空
	 	pDestImg->Destroy();//  删除目标位图
	pDestImg->Create(newWidth,newHeight,nBitsPerPixel,0); //  建立CImage类对象新位图
	CopyColorTables(pDestImg,pSrcImg);//复制调色板

	//设置新图像为纯白图像
	BYTE* pDestData;
	pDestData=(BYTE*)pDestImg->GetBits();
	int dPit=pDestImg->GetPitch();
	int dBitCount=pDestImg->GetBPP()/8;
	for(int i=0;i<newHeight;i++)
		for(int j=0;j<newWidth;j++)
			*(pDestData + dPit*i + j*dBitCount) = 255;

	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(pDestData + dPit*(i+offsetY) + (j+offsetX)*dBitCount) = *(pSrcData + sPit*i + sBitCount*j);

	//CDC		*pDC,*pNewDC;
 	//pDC= CDC::FromHandle(pSrcImg->GetDC());//  建立源位图CDC
 	//pNewDC=CDC::FromHandle(pDestImg->GetDC());//  建立目标位图CDC
 	//pNewDC->BitBlt(offsetX,offsetY,imgWidth,imgHeight,pDC,0,0,SRCCOPY);//复制像素数据
 	//pSrcImg->ReleaseDC();//  释放CDC指针
 	//pDestImg->ReleaseDC();
}

void ImageZoom(CImage* pDestImg, CImage* pSrcImg, int &ZoomX, int &ZoomY, int InterType)
{
	int widthIn = pSrcImg->GetWidth();
	int heightIn = pSrcImg->GetHeight();

	//确保缩放比例大于0
	if(ZoomX <= -widthIn)
		ZoomX = -widthIn + 1;
	if(ZoomY <= -heightIn)
		ZoomY = -heightIn + 1;

	//缩放比例
	float ratioX = 1 + (float)ZoomX/widthIn;
	float ratioY = 1 + (float)ZoomY/heightIn;

	int widthOut = widthIn + ZoomX;
	int heightOut = heightIn + ZoomY;

	if(!pDestImg->IsNull())//  目标位图非空
	 	pDestImg->Destroy();//  删除目标位图
	int nBitsPerPixel = pSrcImg->GetBPP();
	pDestImg->Create(widthOut,heightOut,nBitsPerPixel,0); //  建立CImage类对象新位图
	CopyColorTables(pDestImg,pSrcImg);//复制调色板

	switch(InterType)
	{
	case 0:
		ZoomNeighbor(pDestImg, pSrcImg, ratioX, ratioY, widthOut, heightOut, widthIn, heightIn);
		break;
	case 1:
		ZoomBilinear(pDestImg, pSrcImg, ratioX, ratioY, widthOut, heightOut, widthIn, heightIn);
		break;
	case 2:
		ZoomBicubic(pDestImg, pSrcImg, ratioX, ratioY, widthOut, heightOut, widthIn, heightIn);
		break;
	default:
		break;
	}
}

BYTE BilinearInterpolation(BYTE array[2][2], float xpos, float ypos)
//xpos，ypos为相对于数组四个像素的插值位置坐标，取小数
{
	//两个中间变量
	int t1, t2;

	//先垂直方向线性插值
	t1 = (1-ypos)*array[0][0] + ypos*array[1][0];
	t2 = (1-ypos)*array[0][1] + ypos*array[1][1];

	//再水平方向线性插值
	int t=(int)((1-xpos)*t1 + xpos*t2);

	//若插值结果小于0，则输出0
	if(t<0)
		t=0;

	//若插值结果大于255，则输出255
	if(t>255) 
		t=255;

	//插值结果返回
	return (BYTE)t;
}

BYTE BicubicInterpolation(BYTE array[4][4], float xpos, float ypos)
{
	//申请数组，计算插值所需要的系数
	float col[4], row[4];

	//准备插值的x方向数据源
	col[0]=xpos+1;
	col[1]=xpos;
	col[2]=1-xpos;
	col[3]=2-xpos;

	//准备插值的y方向数据源
	row[0]=ypos+1;
	row[1]=ypos;
	row[2]=1-ypos;
	row[3]=2-ypos;
	
	//循环变量
	int i;

	//临时变量
	float t;

	//对水平方向系数数组进行计算
	for(i=0;i<4;i++){
		t=fabs(col[i]);
		if (t>=0&&t<1)
			col[i]=pow(t,3)-2*pow(t,2)+1;
		else if (t>=1&&t<2)
			col[i]=-pow(t,3)+5*pow(t,2)-8*t+4;
		else
			col[i]=0;
	}

	//对垂直方向系数数组进行计算
	for(i=0;i<4;i++){
		t=fabs(row[i]);
		if (t>=0&&t<1)
			row[i]=pow(t,3)-2*pow(t,2)+1;
		else if (t>=1&&t<2)
			row[i]=-pow(t,3)+5*pow(t,2)-8*t+4;
		else
			row[i]=0;
	}
	
	//将计算好的系数与对应图像数据数组作卷积
	float tempArray[4], temp;
	//先x方向卷积
	for(i=0;i<4;i++)
		tempArray[i]=row[0]*array[0][i]+row[1]*array[1][i]+row[2]*array[2][i]+row[3]*array[3][i];
	
	//再y方向卷积
	temp=0;
	for (i=0;i<4;i++)
		temp+=tempArray[i]*col[i];
		
	//将插值结果在图像灰度级范围内输出
	if (temp>255)
		temp=255;
	if (temp<0)
		temp=0;

	//函数返回值，插值结果
	return (BYTE)temp;
}

void ZoomNeighbor(CImage* pDestImg, CImage* pSrcImg, float ratioX, float ratioY, int widthOut, int heightOut, int widthIn, int heightIn)
{
	BYTE pixel;
	int coordinateX, coordinateY;

	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	BYTE* pDestData = (BYTE*)pDestImg->GetBits();
	int dPit = pDestImg->GetPitch();
	int dBitCount = pDestImg->GetBPP()/8;

    //近邻插值
	for(int i=0; i< heightOut; i++)
	{
		for(int j=0; j< widthOut ; j++)
		{  
			//输出图像坐标为(j,i)的像素映射到原图中的坐标值，即插值位置
			coordinateX = j / ratioX;// + 0.5;
			coordinateY = i / ratioY;// + 0.5;

			if(0<=coordinateX && coordinateX<widthIn && coordinateY>=0 && coordinateY<heightIn)
			{
				pixel = *(pSrcData + sPit*coordinateY + sBitCount*coordinateX);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else
				*(pDestData + dPit*i + j*dBitCount) = 255;
		}
	}
}

void ZoomBilinear(CImage* pDestImg, CImage* pSrcImg, float ratioX, float ratioY, int widthOut, int heightOut, int widthIn, int heightIn)
{
	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	BYTE* pDestData = (BYTE*)pDestImg->GetBits();
	int dPit = pDestImg->GetPitch();
	int dBitCount = pDestImg->GetBPP()/8;

	//输出图像在输入图像中待插值的位置坐标，必须浮点型
    float coordinateX, coordinateY;

	//临时变量，待插值位置向下取整的坐标
	int Iu, Iv;

	//数组，存放插值位置周围的4个像素
	BYTE array[2][2], pixel;

  //双线性插值
	for(int i=0; i< heightOut; i++)
	{
		for(int j=0; j< widthOut ; j++)
		{   
			//输出图像坐标为(j,i)的像素映射到原图中的坐标值，即插值位置
			coordinateX=j/ratioX;
			coordinateY=i/ratioY;

			//对插值位置坐标取整
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;
			
			//若插值位置在输入图像范围内，则双线性插值
			if(0<=coordinateX && coordinateX<widthIn-1 && coordinateY>=0 && coordinateY<heightIn-1)
			{
				//将四个像素数据拷贝至array数组中
				array[0][0] = *(pSrcData + sPit*Iv + sBitCount*Iu);
				array[0][1] = *(pSrcData + sPit*Iv + sBitCount*(Iu+1));
				array[1][0] = *(pSrcData + sPit*(Iv+1) + sBitCount*Iu);
				array[1][1] = *(pSrcData + sPit*(Iv+1) + sBitCount*(Iu+1));

				//调用双线性插值函数插值并输出
				pixel = BilinearInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else//边缘像素采用近邻插值
			{
				pixel = *(pSrcData + sPit*Iv + sBitCount*Iu);
				*(pDestData + dPit*i + j*dBitCount) = 255;//pixel;
			}
		}
	}
}

void ZoomBicubic(CImage* pDestImg, CImage* pSrcImg, float ratioX, float ratioY, int widthOut, int heightOut, int widthIn, int heightIn)
{
	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	BYTE* pDestData = (BYTE*)pDestImg->GetBits();
	int dPit = pDestImg->GetPitch();
	int dBitCount = pDestImg->GetBPP()/8;

	//输出图像在输入图像中待插值的位置坐标，必须浮点型
    float coordinateX, coordinateY;

	//临时变量，待插值位置向下取整的坐标
	int Iu, Iv;

	//数组，存放插值位置周围16个像素
	BYTE array[4][4], pixel;
 
	//循环变量，遍历待插值位置4x4的图像数据
	int  x, y;

	//立方卷积插值
	for(int i=0; i< heightOut; i++)
	{
		for(int j=0; j< widthOut ; j++)
		{   
			//输出图像坐标为(j,i)的像素映射到原图中的坐标值，即插值位置
			coordinateX=j/ratioX;
			coordinateY=i/ratioY;

			//对插值位置坐标取整
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;

			//若插值位置在输入图像范围内，则立方卷积插值
			if(1<=coordinateX && coordinateX<widthIn-2 && coordinateY>=1 && coordinateY<heightIn-2)
			{
				//4x4个像素数据拷贝至array数组中
				for(y=Iv-1;y<Iv+3;y++)
					for (x=Iu-1;x<Iu+3;x++)
						array[y-Iv+1][x-Iu+1] = *(pSrcData + sPit*y + sBitCount*x);

				//调用立方卷积插值函数插值并输出
				pixel = BicubicInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else
			{//边缘像素采用近邻插值
				pixel = *(pSrcData + sPit*Iv + sBitCount*Iu);
				*(pDestData + dPit*i + j*dBitCount) = 255;//pixel;
			}
		}
	}
}

void ImageRotate(CImage* pDestImg, CImage* pSrcImg, float RotateAngle, int InterType)
{
	int widthIn = pSrcImg->GetWidth();
	int heightIn = pSrcImg->GetHeight();

	// 旋转角度（弧度）, 将旋转角度从度转换到弧度
	float	fRotateAngle= 2*3.1415926*RotateAngle/360;
	
	// 输入图像四个角的坐标，以图像中心为坐标系原点
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// 旋转后四个角的坐标，以图像中心为坐标系原点
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// 计算旋转角度的正弦
	float fSina = (float) sin((double)fRotateAngle);
	
	// 计算旋转角度的余弦
	float fCosa = (float) cos((double)fRotateAngle);
	
	// 计算原图的四个角的坐标，以图像中心为坐标系原点
	fSrcX1 = - (float)widthIn  / 2;
	fSrcY1 =   (float)heightIn / 2;
	fSrcX2 =  (float)widthIn / 2;
	fSrcY2 =   (float)heightIn / 2;
	fSrcX3 =- (float)widthIn / 2;
	fSrcY3 = - (float)heightIn / 2;
	fSrcX4 =  (float)widthIn / 2;
	fSrcY4 = - (float)heightIn / 2;
	
	// 计算新图四个角的坐标，以图像中心为坐标系原点
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// 旋转后输出图像宽度
	long widthOut  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// 旋转后输出图像高度
	long heightOut = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) ) + 0.5);

	if(!pDestImg->IsNull())//  目标位图非空
	 	pDestImg->Destroy();//  删除目标位图
	int nBitsPerPixel = pSrcImg->GetBPP();
	pDestImg->Create(widthOut,heightOut,nBitsPerPixel,0); //  建立CImage类对象新位图
	CopyColorTables(pDestImg,pSrcImg);//复制调色板

	// 两个常数
	float f1 = (float) (-0.5 * (widthOut - 1) * fCosa + 0.5 * (heightOut - 1) * fSina + 0.5 * (widthIn  - 1));
	float f2 = (float) (-0.5 * (widthOut - 1) * fSina - 0.5 * (heightOut - 1) * fCosa + 0.5 * (heightIn - 1));

	switch(InterType)
	{
	case 0:
		RotateNeighbor(pDestImg, pSrcImg, fSina, fCosa, f1, f2, widthOut, heightOut, widthIn, heightIn);
		break;
	case 1:
		RotateBilinear(pDestImg, pSrcImg, fSina, fCosa, f1, f2, widthOut, heightOut, widthIn, heightIn);
		break;
	case 2:
		RotateBicubic(pDestImg, pSrcImg, fSina, fCosa, f1, f2, widthOut, heightOut, widthIn, heightIn);
		break;
	default:
		break;
	}
}

void RotateNeighbor(CImage* pDestImg, CImage* pSrcImg, float fSina, float fCosa, float f1, float f2, int widthOut, int heightOut, int widthIn, int heightIn)
{
	BYTE pixel;

	//输出图像在输入图像中待插值的位置坐标
	int	coordinateX, coordinateY;

	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	BYTE* pDestData = (BYTE*)pDestImg->GetBits();
	int dPit = pDestImg->GetPitch();
	int dBitCount = pDestImg->GetBPP()/8;

	// 最近邻插值旋转
	for(int i = 0; i < heightOut; i++)
	{
		for(int j = 0; j < widthOut; j++)
		{
			// 输出图像像素(j,i)映射到输入图像的坐标，近邻插值取整数
			coordinateX = (int)(j * fCosa - i * fSina + f1 + 0.5);
			coordinateY = (int)(j * fSina + i * fCosa + f2 + 0.5);
			
			// 判断是否在输入图像范围内
			if(0<=coordinateX && coordinateX<widthIn && coordinateY>=0 && coordinateY<heightIn)
			{
				//插值
				pixel = *(pSrcData + sPit*coordinateY + sBitCount*coordinateX);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else// 对于不在原图中的像素，赋值为255
				*(pDestData + dPit*i + j*dBitCount) = 255;
		}
	}
}

void RotateBilinear(CImage* pDestImg, CImage* pSrcImg, float fSina, float fCosa, float f1, float f2, int widthOut, int heightOut, int widthIn, int heightIn)
{
	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	BYTE* pDestData = (BYTE*)pDestImg->GetBits();
	int dPit = pDestImg->GetPitch();
	int dBitCount = pDestImg->GetBPP()/8;

	BYTE pixel;

	//输出图像在输入图像中待插值的位置坐标，必须浮点型
	float	coordinateX, coordinateY;

	//存放待插值的4个像素的数组
	BYTE array[2][2];

	//两个中间变量
	int Iu, Iv;

	//双线性插值旋转
	for(int i = 0; i < heightOut; i++)
	{
		for(int j = 0; j < widthOut; j++)
		{		
			// 输出图像像素(j,i)映射到输入图像的坐标
			coordinateX = j * fCosa - i * fSina + f1;
			coordinateY = j * fSina + i * fCosa + f2;

			//对坐标取整
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;

			// 判断是否在原图范围内
			if(0<=coordinateX && coordinateX<widthIn-1 && coordinateY>=0 && coordinateY<heightIn-1)
			{
				//将四个像素数据拷贝至array数组中
				array[0][0] = *(pSrcData + sPit*Iv + sBitCount*Iu);
				array[0][1] = *(pSrcData + sPit*Iv + sBitCount*(Iu+1));
				array[1][0] = *(pSrcData + sPit*(Iv+1) + sBitCount*Iu);
				array[1][1] = *(pSrcData + sPit*(Iv+1) + sBitCount*(Iu+1));

				//调用双线性插值函数插值并输出
				pixel = BilinearInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else// 对于不在原图中的像素，赋值为255
		    	*(pDestData + dPit*i + j*dBitCount) = 255;
		}
	}
}

void RotateBicubic(CImage* pDestImg, CImage* pSrcImg, float fSina, float fCosa, float f1, float f2, int widthOut, int heightOut, int widthIn, int heightIn)
{
	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	BYTE* pDestData = (BYTE*)pDestImg->GetBits();
	int dPit = pDestImg->GetPitch();
	int dBitCount = pDestImg->GetBPP()/8;

	BYTE pixel;

	//输出图像在输入图像中待插值的位置坐标，必须浮点型
	float	coordinateX, coordinateY;

	//存放待插值的16个像素的数组
	BYTE array[4][4];

	//两个中间变量
	int Iu, Iv;

	//循环变量，对确定要插值的位置取4x4邻域用
	int x, y;

	// 立方卷积插值
	for(int i = 0; i < heightOut; i++)
	{
		for(int j = 0; j < widthOut; j++)
		{		
			// 输出图像像素(j,i)映射到输入图像的坐标
			coordinateX = j * fCosa - i * fSina + f1;
			coordinateY = j * fSina + i * fCosa + f2;

			//对坐标取整
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;

			//若插值位置在输入图像范围内，则立方卷积插值
			if(1<=coordinateX && coordinateX<widthIn-2 && coordinateY>=1 && coordinateY<heightIn-2)
			{
				//4x4个像素数据拷贝至array数组中
				for(y=Iv-1;y<Iv+3;y++)
					for (x=Iu-1;x<Iu+3;x++)
						array[y-Iv+1][x-Iu+1] = *(pSrcData + sPit*y + sBitCount*x);

				//调用立方卷积插值函数插值并输出
				pixel = BicubicInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else// 对于不在原图中的像素，赋值为0
		    	*(pDestData + dPit*i + j*dBitCount) = 255;
		}
	}
}

BYTE* ReadCImage(CImage* pSrcImg)
{	
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;

	BYTE* pSrc = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pSrc[i*imgWidth+j] = *(imgBufIn+i*lineByte+j*sBitCount);

	return pSrc;
}

CImage* CreateGrayImg(BYTE* pData, int imgWidth, int imgHeight)
{
	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,8,0); //  建立CImage类对象新位图

	//调色板
	RGBQUAD ColorTable[256];
	for(int i=0;i<256;i++)
		ColorTable[i].rgbBlue=ColorTable[i].rgbGreen=ColorTable[i].rgbRed=i;
	pDestImg->SetColorTable(0,256,ColorTable);

	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();
		int lineByte = pDestImg->GetPitch();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j)=pData[i*imgWidth+j] ;

	return pDestImg;
}

BYTE* ReadColorCImage(CImage* pSrcImg)
{	
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;

	BYTE* pSrc = new BYTE[imgWidth*imgHeight*3];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			for(int channel=0;channel<3;channel++)
				pSrc[(i*imgWidth+j)*3+channel] = *(imgBufIn+i*lineByte+j*sBitCount+channel);

	return pSrc;
}

CImage* CreateColorImg(BYTE* pData, int imgWidth, int imgHeight)
{
	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,24,0); //  建立CImage类对象新位图

	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();
	int lineByte = pDestImg->GetPitch();
	int sBitCount = pDestImg->GetBPP()/8;

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			for(int channel=0;channel<3;channel++)
				*(imgBufOut+i*lineByte+j*sBitCount+channel)=pData[(i*imgWidth+j)*3+channel];

	return pDestImg;
}

float GetMin(float a,float b,float c)
{
	float tmp=a;
	if(b<tmp)
		tmp=b;
	if(c<tmp)
		tmp=c;
	return tmp;
}

void RGB2HSI(BYTE* data,int imgWidth,int imgHeight)
{
	float thita;
	float r,g,b;
	int addr;

	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			addr=(i*imgWidth+j)*3;
			r=(float)data[addr]/255;
			g=(float)data[addr+1]/255;
			b=(float)data[addr+2]/255;
			thita=acosf((r-g+r-b)/2/sqrt((r-g)*(r-g)+(r-b)*(g-b)));
			if(b<=g)
				data[addr]=thita/2/PI*255;//+0.5;
			else
				data[addr]=(1-thita/2/PI)*255;//+0.5;
			data[addr+1]=(1-3/(r+g+b)*GetMin(r,g,b))*255;//+0.5;
			data[addr+2]=(r+g+b)/3*255;//+0.5;
		}
	}
}

void HSI2RGB(BYTE* data,int imgWidth,int imgHeight)
{
	float thita;
	float h,s,ii;
	int tmp1,tmp2,tmp3;
	int addr;

	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			addr=(i*imgWidth+j)*3;
			h=(float)data[addr]/255*360;
			s=(float)data[addr+1]/255;
			ii=(float)data[addr+2]/255;

			tmp1=ii*(1-s)*255+0.5;

			if(h<120)
			{
				tmp2=ii*(1+s*cos(h/180*PI)/cos((60-h)/180*PI))*255+0.5;
				tmp3=ii*3*255-tmp1-tmp2+0.5;
				tmp1=RestrictNum(tmp1,0,255);
				tmp2=RestrictNum(tmp2,0,255);
				tmp3=RestrictNum(tmp3,0,255);
				data[addr+2]=tmp1;
				data[addr]=tmp2;
				data[addr+1]=tmp3;
			}
			else if(h<240)
			{
				h-=120;
				tmp2=ii*(1+s*cos(h/180*PI)/cos((60-h)/180*PI))*255+0.5;
				tmp3=ii*3*255-tmp1-tmp2+0.5;
				tmp1=RestrictNum(tmp1,0,255);
				tmp2=RestrictNum(tmp2,0,255);
				tmp3=RestrictNum(tmp3,0,255);
				data[addr]=tmp1;
				data[addr+1]=tmp2;
				data[addr+2]=tmp3;
			}
			else
			{
				h-=240;
				tmp2=ii*(1+s*cos(h/180*PI)/cos((60-h)/180*PI))*255+0.5;
				tmp3=ii*3*255-tmp1-tmp2+0.5;
				tmp1=RestrictNum(tmp1,0,255);
				tmp2=RestrictNum(tmp2,0,255);
				tmp3=RestrictNum(tmp3,0,255);
				data[addr+1]=tmp1;
				data[addr+2]=tmp2;
				data[addr]=tmp3;
			}
		}
	}
}

BYTE* ExtractChannel(BYTE* pSrc,int channel,int imgWidth,int imgHeight)
{
	BYTE* pDest=new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pDest[i*imgWidth+j]=pSrc[(i*imgWidth+j)*3+channel];
	return pDest;
}

void SetChannel(BYTE* pDest,BYTE* pChanel,int channel,int imgWidth,int imgHeight)
{
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pDest[(i*imgWidth+j)*3+channel]=pChanel[i*imgWidth+j];
}