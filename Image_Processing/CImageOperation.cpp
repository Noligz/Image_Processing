#include "stdafx.h"
#include "CImageOperation.h"

#define MaxImgSize 1024

// ���Ƶ�ɫ��
void CopyColorTables(CImage * pNewImage, CImage * pImage)
{
	int				nColorTableEntries;
	RGBQUAD	ColorTabs[256];

	nColorTableEntries=pImage->GetMaxColorTableEntries();
 	if (nColorTableEntries>0)
	{
		pImage->GetColorTable(0,nColorTableEntries,ColorTabs);    
		pNewImage->SetColorTable(0,nColorTableEntries,ColorTabs);   //  ���Ƶ�ɫ��
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

// CImage�����֮��ĸ���
void ImageCopy(CImage * pImage, CImage * pNewImage)
{
	 CDC			*pDC,*pNewDC;
	int				nWidth,nHeight,nBitsPerPixel;

	nWidth=pImage->GetWidth();
	nHeight=pImage->GetHeight();
	nBitsPerPixel=pImage->GetBPP();

	if(!pNewImage->IsNull())//  Ŀ��λͼ�ǿ�
	 	pNewImage->Destroy();//  ɾ��Ŀ��λͼ
	pNewImage->Create(nWidth,nHeight,nBitsPerPixel,0); //  ����CImage�������λͼ
	CopyColorTables(pNewImage,pImage);//���Ƶ�ɫ��

 	pDC= CDC::FromHandle(pImage->GetDC());//  ����ԴλͼCDC
 	pNewDC=CDC::FromHandle(pNewImage->GetDC());//  ����Ŀ��λͼCDC

 	pNewDC->BitBlt(0,0,nWidth,nHeight,pDC,0,0,SRCCOPY);//������������

 	pImage->ReleaseDC();//  �ͷ�CDCָ��
 	pNewImage->ReleaseDC();
}

//BYTE CImageGetPixel(const CImage* pImg, int i, int j)
//{
//	COLORREF pixel;
//	pixel = pImg->GetPixel(i,j);
//	return GetRValue(pixel);
//}

// ȡ������������
BYTE CImageGetPixel(CImage* pImage, int x, int y)
{
	if (!IsPntInImage(pImage,x,y)) //���곬����Χ�˳�
		return 0;

	//DWORD	dwPixelValue;
	//BYTE		*pPixel;

	//pPixel= (BYTE*) pImage->GetPixelAddress(x,y);//ȡ�������ݴ�ŵ�ַ

	//switch(pImage->GetBPP())
	//{
	//case 1:		memcpy(&dwPixelValue,pPixel,1);//������������
	//				dwPixelValue=dwPixelValue & 0x1;
	//				break;

	//case 4:		memcpy(&dwPixelValue,pPixel,1);//������������
	//				dwPixelValue=dwPixelValue & 0xf;
	//				break;

	//case 8:		memcpy(&dwPixelValue,pPixel,1);//������������
	//				break;

	//case 24:		memcpy(&dwPixelValue,pPixel,3);//������������
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
	//����ͼ��Χ
	if(!IsPntInImage(pImg,x,y) )
		return;

	//���ƻҶ�ֵ
	if(value<0)
		value=0;
	if(value>255)
		value=255;

	//pImg->SetPixelRGB(x, y, value, value, value);

	//BYTE *pPixel= (BYTE*) pImg->GetPixelAddress(x,y);//ȡ�������ݴ�ŵ�ַ
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

//�ӿ�ͼ��(ͼ��ӱ�),�����·������չһ�У������ҷ������չһ��
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
	CopyColorTables(pNewImage,pImage);//���Ƶ�ɫ��

	//����ͼ�����и�����������
	for (i=0;i<nHeight;i++) 
	{
		pPixel  = (BYTE*) pImage->GetPixelAddress(0,i);       
		pNewPixel = (BYTE*) pNewImage->GetPixelAddress(1,i+1);
  		memcpy(pNewPixel, pPixel,nBytesPerLine);                         
	}

	//��չ����һ��
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,0); 
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(1,0);
  	memcpy(pNewPixel, pPixel,nBytesPerLine);

	//��չ�ױ�һ��
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,nHeight-1);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(1,nHeight+1);
  	memcpy(pNewPixel, pPixel,nBytesPerLine);

	//��չ���һ��
	for(i=0;i<nHeight;i++)
	{
		pPixel  = (BYTE*) pImage->GetPixelAddress(0,i);
		pNewPixel = (BYTE*) pNewImage->GetPixelAddress(0,i+1);
  		memcpy(pNewPixel,pPixel,nBytesPerPixel);
	}

	//��չ�ұ�һ��
	for(i=0;i<nHeight;i++)
	{
		pPixel  = (BYTE*) pImage->GetPixelAddress(nWidth-1,i);
		pNewPixel = (BYTE*) pNewImage->GetPixelAddress(nWidth+1,i+1);
  		memcpy(pNewPixel,pPixel,nBytesPerPixel);
	}

	//�������Ͻǵ�
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,0);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(0,0);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	//�������Ͻǵ�
	pPixel  = (BYTE*) pImage->GetPixelAddress(nWidth-1,0);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(nWidth+1,0);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	//�������½ǵ�
	pPixel  = (BYTE*) pImage->GetPixelAddress(0,nHeight-1);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(0,nHeight+1);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	//�������½ǵ�
	pPixel  = (BYTE*) pImage->GetPixelAddress(nWidth-1,nHeight-1);
	pNewPixel = (BYTE*) pNewImage->GetPixelAddress(nWidth+1,nHeight+1);
  	memcpy(pNewPixel,pPixel,nBytesPerPixel);

	return pNewImage;
}

//ͼ��ȥ��
CImage* SubtractImageBorder(CImage *pImage)
{
	CImage *pNewImage = new CImage;
	CDC		*pDC,*pNewDC;
	int	nWidth,nHeight,nBitsPerPixel;

	nWidth=pImage->GetWidth()-2;
	nHeight=pImage->GetHeight()-2;
	nBitsPerPixel=pImage->GetBPP();

	if(!pNewImage->IsNull())//  Ŀ��λͼ�ǿ�
	 	pNewImage->Destroy();//  ɾ��Ŀ��λͼ
	pNewImage->Create(nWidth,nHeight,nBitsPerPixel,0); //  ����CImage�������λͼ
	CopyColorTables(pNewImage,pImage);//���Ƶ�ɫ��

 	pDC= CDC::FromHandle(pImage->GetDC());//  ����ԴλͼCDC
 	pNewDC=CDC::FromHandle(pNewImage->GetDC());//  ����Ŀ��λͼCDC

 	pNewDC->BitBlt(0,0,nWidth,nHeight,pDC,1,1,SRCCOPY);//������������

 	pImage->ReleaseDC();//  �ͷ�CDCָ��
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

	if(!pDestImg->IsNull())//  Ŀ��λͼ�ǿ�
	 	pDestImg->Destroy();//  ɾ��Ŀ��λͼ
	pDestImg->Create(newWidth,newHeight,nBitsPerPixel,0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��

	//������ͼ��Ϊ����ͼ��
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
 	//pDC= CDC::FromHandle(pSrcImg->GetDC());//  ����ԴλͼCDC
 	//pNewDC=CDC::FromHandle(pDestImg->GetDC());//  ����Ŀ��λͼCDC
 	//pNewDC->BitBlt(offsetX,offsetY,imgWidth,imgHeight,pDC,0,0,SRCCOPY);//������������
 	//pSrcImg->ReleaseDC();//  �ͷ�CDCָ��
 	//pDestImg->ReleaseDC();
}

void ImageZoom(CImage* pDestImg, CImage* pSrcImg, int &ZoomX, int &ZoomY, int InterType)
{
	int widthIn = pSrcImg->GetWidth();
	int heightIn = pSrcImg->GetHeight();

	//ȷ�����ű�������0
	if(ZoomX <= -widthIn)
		ZoomX = -widthIn + 1;
	if(ZoomY <= -heightIn)
		ZoomY = -heightIn + 1;

	//���ű���
	float ratioX = 1 + (float)ZoomX/widthIn;
	float ratioY = 1 + (float)ZoomY/heightIn;

	int widthOut = widthIn + ZoomX;
	int heightOut = heightIn + ZoomY;

	if(!pDestImg->IsNull())//  Ŀ��λͼ�ǿ�
	 	pDestImg->Destroy();//  ɾ��Ŀ��λͼ
	int nBitsPerPixel = pSrcImg->GetBPP();
	pDestImg->Create(widthOut,heightOut,nBitsPerPixel,0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��

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
//xpos��yposΪ����������ĸ����صĲ�ֵλ�����꣬ȡС��
{
	//�����м����
	int t1, t2;

	//�ȴ�ֱ�������Բ�ֵ
	t1 = (1-ypos)*array[0][0] + ypos*array[1][0];
	t2 = (1-ypos)*array[0][1] + ypos*array[1][1];

	//��ˮƽ�������Բ�ֵ
	int t=(int)((1-xpos)*t1 + xpos*t2);

	//����ֵ���С��0�������0
	if(t<0)
		t=0;

	//����ֵ�������255�������255
	if(t>255) 
		t=255;

	//��ֵ�������
	return (BYTE)t;
}

BYTE BicubicInterpolation(BYTE array[4][4], float xpos, float ypos)
{
	//�������飬�����ֵ����Ҫ��ϵ��
	float col[4], row[4];

	//׼����ֵ��x��������Դ
	col[0]=xpos+1;
	col[1]=xpos;
	col[2]=1-xpos;
	col[3]=2-xpos;

	//׼����ֵ��y��������Դ
	row[0]=ypos+1;
	row[1]=ypos;
	row[2]=1-ypos;
	row[3]=2-ypos;
	
	//ѭ������
	int i;

	//��ʱ����
	float t;

	//��ˮƽ����ϵ��������м���
	for(i=0;i<4;i++){
		t=fabs(col[i]);
		if (t>=0&&t<1)
			col[i]=pow(t,3)-2*pow(t,2)+1;
		else if (t>=1&&t<2)
			col[i]=-pow(t,3)+5*pow(t,2)-8*t+4;
		else
			col[i]=0;
	}

	//�Դ�ֱ����ϵ��������м���
	for(i=0;i<4;i++){
		t=fabs(row[i]);
		if (t>=0&&t<1)
			row[i]=pow(t,3)-2*pow(t,2)+1;
		else if (t>=1&&t<2)
			row[i]=-pow(t,3)+5*pow(t,2)-8*t+4;
		else
			row[i]=0;
	}
	
	//������õ�ϵ�����Ӧͼ���������������
	float tempArray[4], temp;
	//��x������
	for(i=0;i<4;i++)
		tempArray[i]=row[0]*array[0][i]+row[1]*array[1][i]+row[2]*array[2][i]+row[3]*array[3][i];
	
	//��y������
	temp=0;
	for (i=0;i<4;i++)
		temp+=tempArray[i]*col[i];
		
	//����ֵ�����ͼ��Ҷȼ���Χ�����
	if (temp>255)
		temp=255;
	if (temp<0)
		temp=0;

	//��������ֵ����ֵ���
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

    //���ڲ�ֵ
	for(int i=0; i< heightOut; i++)
	{
		for(int j=0; j< widthOut ; j++)
		{  
			//���ͼ������Ϊ(j,i)������ӳ�䵽ԭͼ�е�����ֵ������ֵλ��
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

	//���ͼ��������ͼ���д���ֵ��λ�����꣬���븡����
    float coordinateX, coordinateY;

	//��ʱ����������ֵλ������ȡ��������
	int Iu, Iv;

	//���飬��Ų�ֵλ����Χ��4������
	BYTE array[2][2], pixel;

  //˫���Բ�ֵ
	for(int i=0; i< heightOut; i++)
	{
		for(int j=0; j< widthOut ; j++)
		{   
			//���ͼ������Ϊ(j,i)������ӳ�䵽ԭͼ�е�����ֵ������ֵλ��
			coordinateX=j/ratioX;
			coordinateY=i/ratioY;

			//�Բ�ֵλ������ȡ��
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;
			
			//����ֵλ��������ͼ��Χ�ڣ���˫���Բ�ֵ
			if(0<=coordinateX && coordinateX<widthIn-1 && coordinateY>=0 && coordinateY<heightIn-1)
			{
				//���ĸ��������ݿ�����array������
				array[0][0] = *(pSrcData + sPit*Iv + sBitCount*Iu);
				array[0][1] = *(pSrcData + sPit*Iv + sBitCount*(Iu+1));
				array[1][0] = *(pSrcData + sPit*(Iv+1) + sBitCount*Iu);
				array[1][1] = *(pSrcData + sPit*(Iv+1) + sBitCount*(Iu+1));

				//����˫���Բ�ֵ������ֵ�����
				pixel = BilinearInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else//��Ե���ز��ý��ڲ�ֵ
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

	//���ͼ��������ͼ���д���ֵ��λ�����꣬���븡����
    float coordinateX, coordinateY;

	//��ʱ����������ֵλ������ȡ��������
	int Iu, Iv;

	//���飬��Ų�ֵλ����Χ16������
	BYTE array[4][4], pixel;
 
	//ѭ����������������ֵλ��4x4��ͼ������
	int  x, y;

	//���������ֵ
	for(int i=0; i< heightOut; i++)
	{
		for(int j=0; j< widthOut ; j++)
		{   
			//���ͼ������Ϊ(j,i)������ӳ�䵽ԭͼ�е�����ֵ������ֵλ��
			coordinateX=j/ratioX;
			coordinateY=i/ratioY;

			//�Բ�ֵλ������ȡ��
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;

			//����ֵλ��������ͼ��Χ�ڣ������������ֵ
			if(1<=coordinateX && coordinateX<widthIn-2 && coordinateY>=1 && coordinateY<heightIn-2)
			{
				//4x4���������ݿ�����array������
				for(y=Iv-1;y<Iv+3;y++)
					for (x=Iu-1;x<Iu+3;x++)
						array[y-Iv+1][x-Iu+1] = *(pSrcData + sPit*y + sBitCount*x);

				//�������������ֵ������ֵ�����
				pixel = BicubicInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else
			{//��Ե���ز��ý��ڲ�ֵ
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

	// ��ת�Ƕȣ����ȣ�, ����ת�ǶȴӶ�ת��������
	float	fRotateAngle= 2*3.1415926*RotateAngle/360;
	
	// ����ͼ���ĸ��ǵ����꣬��ͼ������Ϊ����ϵԭ��
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// ��ת���ĸ��ǵ����꣬��ͼ������Ϊ����ϵԭ��
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// ������ת�Ƕȵ�����
	float fSina = (float) sin((double)fRotateAngle);
	
	// ������ת�Ƕȵ�����
	float fCosa = (float) cos((double)fRotateAngle);
	
	// ����ԭͼ���ĸ��ǵ����꣬��ͼ������Ϊ����ϵԭ��
	fSrcX1 = - (float)widthIn  / 2;
	fSrcY1 =   (float)heightIn / 2;
	fSrcX2 =  (float)widthIn / 2;
	fSrcY2 =   (float)heightIn / 2;
	fSrcX3 =- (float)widthIn / 2;
	fSrcY3 = - (float)heightIn / 2;
	fSrcX4 =  (float)widthIn / 2;
	fSrcY4 = - (float)heightIn / 2;
	
	// ������ͼ�ĸ��ǵ����꣬��ͼ������Ϊ����ϵԭ��
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// ��ת�����ͼ����
	long widthOut  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// ��ת�����ͼ��߶�
	long heightOut = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) ) + 0.5);

	if(!pDestImg->IsNull())//  Ŀ��λͼ�ǿ�
	 	pDestImg->Destroy();//  ɾ��Ŀ��λͼ
	int nBitsPerPixel = pSrcImg->GetBPP();
	pDestImg->Create(widthOut,heightOut,nBitsPerPixel,0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��

	// ��������
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

	//���ͼ��������ͼ���д���ֵ��λ������
	int	coordinateX, coordinateY;

	BYTE* pSrcData = (BYTE*)pSrcImg->GetBits();
	int sPit = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	BYTE* pDestData = (BYTE*)pDestImg->GetBits();
	int dPit = pDestImg->GetPitch();
	int dBitCount = pDestImg->GetBPP()/8;

	// ����ڲ�ֵ��ת
	for(int i = 0; i < heightOut; i++)
	{
		for(int j = 0; j < widthOut; j++)
		{
			// ���ͼ������(j,i)ӳ�䵽����ͼ������꣬���ڲ�ֵȡ����
			coordinateX = (int)(j * fCosa - i * fSina + f1 + 0.5);
			coordinateY = (int)(j * fSina + i * fCosa + f2 + 0.5);
			
			// �ж��Ƿ�������ͼ��Χ��
			if(0<=coordinateX && coordinateX<widthIn && coordinateY>=0 && coordinateY<heightIn)
			{
				//��ֵ
				pixel = *(pSrcData + sPit*coordinateY + sBitCount*coordinateX);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else// ���ڲ���ԭͼ�е����أ���ֵΪ255
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

	//���ͼ��������ͼ���д���ֵ��λ�����꣬���븡����
	float	coordinateX, coordinateY;

	//��Ŵ���ֵ��4�����ص�����
	BYTE array[2][2];

	//�����м����
	int Iu, Iv;

	//˫���Բ�ֵ��ת
	for(int i = 0; i < heightOut; i++)
	{
		for(int j = 0; j < widthOut; j++)
		{		
			// ���ͼ������(j,i)ӳ�䵽����ͼ�������
			coordinateX = j * fCosa - i * fSina + f1;
			coordinateY = j * fSina + i * fCosa + f2;

			//������ȡ��
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;

			// �ж��Ƿ���ԭͼ��Χ��
			if(0<=coordinateX && coordinateX<widthIn-1 && coordinateY>=0 && coordinateY<heightIn-1)
			{
				//���ĸ��������ݿ�����array������
				array[0][0] = *(pSrcData + sPit*Iv + sBitCount*Iu);
				array[0][1] = *(pSrcData + sPit*Iv + sBitCount*(Iu+1));
				array[1][0] = *(pSrcData + sPit*(Iv+1) + sBitCount*Iu);
				array[1][1] = *(pSrcData + sPit*(Iv+1) + sBitCount*(Iu+1));

				//����˫���Բ�ֵ������ֵ�����
				pixel = BilinearInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else// ���ڲ���ԭͼ�е����أ���ֵΪ255
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

	//���ͼ��������ͼ���д���ֵ��λ�����꣬���븡����
	float	coordinateX, coordinateY;

	//��Ŵ���ֵ��16�����ص�����
	BYTE array[4][4];

	//�����м����
	int Iu, Iv;

	//ѭ����������ȷ��Ҫ��ֵ��λ��ȡ4x4������
	int x, y;

	// ���������ֵ
	for(int i = 0; i < heightOut; i++)
	{
		for(int j = 0; j < widthOut; j++)
		{		
			// ���ͼ������(j,i)ӳ�䵽����ͼ�������
			coordinateX = j * fCosa - i * fSina + f1;
			coordinateY = j * fSina + i * fCosa + f2;

			//������ȡ��
			Iu=(int)coordinateX;
			Iv=(int)coordinateY;

			//����ֵλ��������ͼ��Χ�ڣ������������ֵ
			if(1<=coordinateX && coordinateX<widthIn-2 && coordinateY>=1 && coordinateY<heightIn-2)
			{
				//4x4���������ݿ�����array������
				for(y=Iv-1;y<Iv+3;y++)
					for (x=Iu-1;x<Iu+3;x++)
						array[y-Iv+1][x-Iu+1] = *(pSrcData + sPit*y + sBitCount*x);

				//�������������ֵ������ֵ�����
				pixel = BicubicInterpolation(array, coordinateX-Iu, coordinateY-Iv);
				*(pDestData + dPit*i + j*dBitCount) = pixel;
			}
			else// ���ڲ���ԭͼ�е����أ���ֵΪ0
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
	pDestImg->Create(imgWidth,imgHeight,8,0); //  ����CImage�������λͼ

	//��ɫ��
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
	pDestImg->Create(imgWidth,imgHeight,24,0); //  ����CImage�������λͼ

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