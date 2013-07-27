#include "stdafx.h"
#include "Morphology.h"
#include "histogram.h"
#include "CImageOperation.h"

void StructureElement::CreateSquareSE(int width)
{
	m_Width=width;
	m_Height=width;
	m_CenterX=width/2;
	m_CenterY=width/2;
	for(int i=0;i<width;i++)
		for(int j=0;j<width;j++)
			m_Data[i][j]=1;
}

void StructureElement::CreateCrossSE()
{
	m_Width=3;
	m_Height=3;
	m_CenterX=1;
	m_CenterY=1;
	m_Data[0][0]=1;m_Data[0][1]=0;m_Data[0][2]=1;
	m_Data[1][0]=0;m_Data[1][1]=1;m_Data[1][2]=0;
	m_Data[2][0]=1;m_Data[2][1]=0;m_Data[2][2]=1;
}

void StructureElement::CreateDiscSE()
{
	m_Width=3;
	m_Height=3;
	m_CenterX=1;
	m_CenterY=1;
	m_Data[0][0]=0;m_Data[0][1]=1;m_Data[0][2]=0;
	m_Data[1][0]=1;m_Data[1][1]=1;m_Data[1][2]=1;
	m_Data[2][0]=0;m_Data[2][1]=1;m_Data[2][2]=0;
}

void StructureElement::CreateCircleSE(int r)
{
	m_Width = m_Height = 2*r+1;
	m_CenterX = m_CenterY = r;
	for(int i=0;i<m_Height;i++)
		for(int j=0;j<m_Width;j++)
			if((i-m_CenterY)*(i-m_CenterY)+(j-m_CenterX)*(j-m_CenterX) <= r*r)
				m_Data[i][j]=1;
			else
				m_Data[i][j]=0;
}

void StructureElement::CreateLineSE(int size,float degree)
{
	float rad = degree/180*3.1415926;
	int halfwidth=1;
	if(degree>45 && degree<135)
	{
		float cot =cos(rad)/sin(rad);

		m_Height=size;
		m_Width=fabs(size*cot+1.5+halfwidth*2);

		m_CenterX = m_Width/2;
		m_CenterY = m_Height/2;

		for(int i=0;i<m_Height;i++)
			for(int j=0;j<m_Width;j++)
				m_Data[i][j]=0;

		int tmpx;
		
		m_Data[m_CenterY][m_CenterX]=1;
		for(int i=1;i<=m_CenterY;i++)
		{
			tmpx=(int)(i*cot+0.5);
			if(tmpx<=m_CenterX)
			{
				m_Data[m_CenterY+i][m_CenterX+tmpx]=1;
				m_Data[m_CenterY-i][m_CenterX-tmpx]=1;
				for(int j=1;j<=halfwidth;j++)
				{
					m_Data[m_CenterY+i][m_CenterX+tmpx+j]=1;
					m_Data[m_CenterY-i][m_CenterX-tmpx-j]=1;
				}
			}
		}
	}
	else
	{
		m_Width=size;
		m_Height=fabs(size*tan(rad)+1.5+halfwidth*2);

		m_CenterX = m_Width/2;
		m_CenterY = m_Height/2;

		for(int i=0;i<m_Height;i++)
			for(int j=0;j<m_Width;j++)
				m_Data[i][j]=0;

		int tmpy;
		float tg = tan(rad);
		m_Data[m_CenterY][m_CenterX]=1;
		for(int i=1;i<=m_CenterX;i++)
		{
			tmpy=(int)(i*tg+0.5);
			if(tmpy<=m_CenterY)
			{
				m_Data[m_CenterY+tmpy][m_CenterX+i]=1;
				m_Data[m_CenterY-tmpy][m_CenterX-i]=1;
				for(int j=1;j<=halfwidth;j++)
				{
					m_Data[m_CenterY+tmpy+j][m_CenterX+i]=1;
					m_Data[m_CenterY-tmpy-j][m_CenterX-i]=1;
				}
			}
		}
	}
}


void Binarize(CImage* pImg, int threshold)
{
	int imgWidth = pImg->GetWidth();
	int imgHeight = pImg->GetHeight();
	BYTE* imgData = (BYTE*)pImg->GetBits();
	int sPit = pImg->GetPitch();
	int sBitCount = pImg->GetBPP()/8;

	BYTE* current;

	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			current = imgData+i*sPit+j*sBitCount;
			if(*current<threshold)
				*current=0;
			else
				*current=255;
		}
	}
}


BYTE* ArrayBinaryDilation(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* pResult = new BYTE[imgWidth*imgHeight];

	//�������
	int i,j,k,l;

	//��־����
	int judge;

	int SE_C2L = se.m_CenterX;						//�ṹԪ�����ĵ���˵ľ���
	int SE_C2R = se.m_Width -1 - SE_C2L;			//�ṹԪ�����ĵ��ɶ˵ľ���
	int SE_C2T = se.m_CenterY;						//�ṹԪ�����ĵ��϶˵ľ���
	int SE_C2B = se.m_Height -1 - SE_C2T;			//�ṹԪ�����ĵ��¶˵ľ���

	//�Ӻڱߣ��ӱߺ��ͼ�����buf
	int newWidth = imgWidth+se.m_Width;
	int newHeight = imgHeight+se.m_Height;
	BYTE *buf=new BYTE[newWidth*newHeight];
	for(i=0;i<newHeight;i++)
	{
		for(j=0;j<newWidth;j++)
		{
			if(i<SE_C2T||i>=imgHeight+SE_C2B||j<SE_C2L||j>=imgWidth+SE_C2R)
				*(buf+i*newWidth+j)=0;
			else
				*(buf+i*newWidth+j)=*(pSrc+(i-SE_C2T)*imgWidth+j-SE_C2L);
		}
	}

	for(i=SE_C2T;i<imgHeight+SE_C2T;i++)
	{
		for(j=SE_C2L;j<imgWidth+SE_C2L;j++)
		{			
			judge=1;
			for(k=-SE_C2T;k<=SE_C2B;k++)
			{
				for(l=-SE_C2L;l<=SE_C2R;l++)
				{
					if(se.m_Data[k+SE_C2T][l+SE_C2L] == 1)
					{
						if(*(buf+(i+k)*newWidth+(j+l)) == 255)
						{
							judge=0;	
							break;
						}
					}
				}
			}

			if(judge)
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=0;
			else
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=255;
		}
	}

	delete[] buf;
	return pResult;
}

CImage* BinaryDilation(CImage* pSrcImg, StructureElement se)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;

	Binarize(pSrcImg,128);
	
	BYTE* pSrc = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pSrc[i*imgWidth+j] = *(imgBufIn+i*lineByte+j*sBitCount);

	BYTE* pResult = ArrayBinaryDilation(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount)=pResult[i*imgWidth+j] ;

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}


BYTE* ArrayBinaryErosion(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* pResult = new BYTE[imgWidth*imgHeight];

	//�������
	int i,j,k,l;

	//��־����
	int judge;

	int SE_C2L = se.m_CenterX;						//�ṹԪ�����ĵ���˵ľ���
	int SE_C2R = se.m_Width -1 - SE_C2L;			//�ṹԪ�����ĵ��ɶ˵ľ���
	int SE_C2T = se.m_CenterY;						//�ṹԪ�����ĵ��϶˵ľ���
	int SE_C2B = se.m_Height -1 - SE_C2T;			//�ṹԪ�����ĵ��¶˵ľ���

	//�Ӻڱߣ��ӱߺ��ͼ�����buf
	int newWidth = imgWidth+se.m_Width;
	int newHeight = imgHeight+se.m_Height;
	BYTE *buf=new BYTE[newWidth*newHeight];
	for(i=0;i<newHeight;i++)
	{
		for(j=0;j<newWidth;j++)
		{
			if(i<SE_C2T||i>=imgHeight+SE_C2B||j<SE_C2L||j>=imgWidth+SE_C2R)
				*(buf+i*newWidth+j)=0;
			else
				*(buf+i*newWidth+j)=*(pSrc+(i-SE_C2T)*imgWidth+j-SE_C2L);
		}
	}

	//���ͼ����ÿһ������λ�ã��ж��Ƿ�ṹԪ��������Ŀ���ڲ�
	for(i=SE_C2T;i<imgHeight+SE_C2T;i++)
	{
		for(j=SE_C2L;j<imgWidth+SE_C2L;j++)
		{			
			//�жϽṹԪ���Ƿ�����ڵ�ǰ������Ŀ���ڲ�,1Ϊ����,0Ϊ������
			judge=1;
			for(k=-SE_C2T;k<=SE_C2B;k++)
			{
				for(l=-SE_C2L;l<=SE_C2R;l++)
				{
					//�����ǰ�ṹԪ��λ��Ϊ1,�ж����Ӧͼ���ϵ����ص��Ƿ�Ϊ��0
					if(se.m_Data[k+SE_C2T][l+SE_C2L] == 1)
					{
						//���ͼ��ǰ����Ϊ0,��û�л��иõ�,���Ǹ�ʴ�����
						if(*(buf+(i+k)*newWidth+(j+l)) == 0)
						{
							judge=0;	
							break;
						}
					}
				}
			}

			if(judge)
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=255;
			else
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=0;
		}
	}

	delete[] buf;
	return pResult;
}

CImage* BinaryErosion(CImage* pSrcImg, StructureElement se)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;

	Binarize(pSrcImg,128);
	
	BYTE* pSrc = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pSrc[i*imgWidth+j] = *(imgBufIn+i*lineByte+j*sBitCount);

	BYTE* pResult = ArrayBinaryErosion(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount)=pResult[i*imgWidth+j] ;

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}

BYTE* ArrayBinaryOpen(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* tmp = ArrayBinaryErosion(pSrc,imgWidth,imgHeight,se);
	BYTE* pResult = ArrayBinaryDilation(tmp,imgWidth,imgHeight,se);
	delete[] tmp;
	return pResult;
}

CImage* BinaryOpen(CImage* pSrcImg, StructureElement se)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;

	Binarize(pSrcImg,128);

	BYTE* pSrc = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pSrc[i*imgWidth+j] = *(imgBufIn+i*lineByte+j*sBitCount);

	BYTE* pResult = ArrayBinaryOpen(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount)=pResult[i*imgWidth+j] ;

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}

BYTE* ArrayBinaryClose(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* tmp = ArrayBinaryDilation(pSrc,imgWidth,imgHeight,se);
	BYTE* pResult = ArrayBinaryErosion(tmp,imgWidth,imgHeight,se);
	delete[] tmp;
	return pResult;
}

CImage* BinaryClose(CImage* pSrcImg, StructureElement se)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();

	Binarize(pSrcImg,128);

	BYTE* pSrc = ReadCImage(pSrcImg);

	BYTE* pResult = ArrayBinaryClose(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=CreateGrayImg(pResult,imgWidth,imgHeight);

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}

CImage* DistanceTransform(CImage* pSrcImg, int type)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;
	Binarize(pSrcImg,128);

	StructureElement se;
	switch(type)
	{
	default:
	case 0://Euclidean Distance
		se.CreateCircleSE(3);
		break;
	case 1://City Block Distance
		se.CreateDiscSE();
		break;
	case 2://Chessboard Distance
		se.CreateSquareSE(3);
		break;
	}

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	//�������
	int i,j,k,l;

	int* Data=new int[imgWidth*imgHeight];

	//ԭͼ�Ҷ�ֵΪ0����Ϊ0��Ϊ255����Ϊ1
	for(i=0;i<imgHeight;i++)
		for(j=0;j<imgWidth;j++)
			if(*(imgBufIn+i*lineByte+j*sBitCount)==0)
				*(Data+i*imgWidth+j)=0;
			else
				*(Data+i*imgWidth+j)=1;

	//��־����
	int judge;

	//�Ƿ������ʴ
	bool flag=true;

	int CurrDist=1;

	int SE_C2L = se.m_CenterX;						//�ṹԪ�����ĵ���˵ľ���
	int SE_C2R = se.m_Width -1 - SE_C2L;			//�ṹԪ�����ĵ��ɶ˵ľ���
	int SE_C2T = se.m_CenterY;						//�ṹԪ�����ĵ��϶˵ľ���
	int SE_C2B = se.m_Height -1 - SE_C2T;			//�ṹԪ�����ĵ��¶˵ľ���

	while(flag)
	{
		flag = false;
		//���ͼ����ÿһ������λ�ã��ж��Ƿ�ṹԪ��������Ŀ���ڲ�
		for(i=SE_C2T;i<imgHeight-SE_C2B;i++)
		{
			for(j=SE_C2L;j<imgWidth-SE_C2R;j++)
			{			
				//�жϽṹԪ���Ƿ�����ڵ�ǰ������Ŀ���ڲ�,1Ϊ����,0Ϊ������
				judge=1;
				for(k=-SE_C2T;k<=SE_C2B;k++)
				{
					for(l=-SE_C2L;l<=SE_C2R;l++)
					{
						//�����ǰ�ṹԪ��λ��Ϊ1,�ж����Ӧͼ���ϵ����ص��Ƿ�Ϊ��0
						if(se.m_Data[k+SE_C2T][l+SE_C2L] == 1)
						{
							//���ͼ��ǰ����Ϊ0,��û�л��иõ�,���Ǹ�ʴ�����
							if(*(Data+(i+k)*imgWidth+(j+l)) < CurrDist)
							{
								judge=0;	
								break;
							}
						}
					}
				}

				if(judge)
				{
					(*(Data+i*imgWidth+j))++;
					flag=true;
				}
			}
		}
		CurrDist++;
	}

	//��һ��
	CurrDist--;
	for(i=0;i<imgHeight;i++)
		for(j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount) = *(Data+i*imgWidth+j)*255/(float)CurrDist ;

	delete[] Data;

	return pDestImg;
}


bool IsImgAllBlack(BYTE* pSrc,int imgWidth, int imgHeight)
{
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			if(*(pSrc+i*imgWidth+j) == 255)
				return false;
	return true;
}

void BinaryUnion(BYTE* a, BYTE* b, int imgWidth, int imgHeight)
{
	int pixel;
	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			pixel = a[i*imgWidth+j] + b[i*imgWidth+j];
			if(pixel > 255)
				pixel = 255;
			a[i*imgWidth+j] =pixel;
		}
	}
}

void BinaryMinus(BYTE* a, BYTE* b, int imgWidth, int imgHeight)
{
	int pixel;
	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			pixel = a[i*imgWidth+j] - b[i*imgWidth+j];
			if(pixel < 0)
				pixel = 0;
			a[i*imgWidth+j] =pixel;
		}
	}
}

CImage* Skeleton(CImage* pSrcImg)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;

	Binarize(pSrcImg,128);
	
	BYTE* pSrc = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pSrc[i*imgWidth+j] = *(imgBufIn+i*lineByte+j*sBitCount);

	StructureElement se;
	se.CreateDiscSE();

	BYTE* pResult = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pResult[i*imgWidth+j] = 0;

	BYTE *tmp1,*tmp2,*t;
	tmp1 = pSrc;
	while(!IsImgAllBlack(tmp1,imgWidth,imgHeight))
	{
		tmp2 = ArrayBinaryErosion(tmp1,imgWidth,imgHeight,se);
		t = ArrayBinaryDilation(tmp2,imgWidth,imgHeight,se);
		BinaryMinus(tmp1,t,imgWidth,imgHeight);
		BinaryUnion(pResult,tmp1,imgWidth,imgHeight);
		delete[] t;
		delete[] tmp1;
		tmp1=tmp2;
	}

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount)=pResult[i*imgWidth+j] ;

	return pDestImg;
}

CImage* SkeletonReconstruct(CImage* pSrcImg)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();
	BYTE* imgBufIn = (BYTE*)pSrcImg->GetBits();
	int lineByte = pSrcImg->GetPitch();
	int sBitCount = pSrcImg->GetBPP()/8;

	Binarize(pSrcImg,128);
	
	BYTE* pSrc = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pSrc[i*imgWidth+j] = *(imgBufIn+i*lineByte+j*sBitCount);

	StructureElement se;
	se.CreateDiscSE();

	BYTE* pResult = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			pResult[i*imgWidth+j] = 0;

	BYTE *tmp1,*tmp2,*t;
	int k=0;
	tmp1 = pSrc;
	while(!IsImgAllBlack(tmp1,imgWidth,imgHeight))
	{
		tmp2 = ArrayBinaryErosion(tmp1,imgWidth,imgHeight,se);
		t = ArrayBinaryDilation(tmp2,imgWidth,imgHeight,se);
		BinaryMinus(tmp1,t,imgWidth,imgHeight);
		delete[] t;
		for(int i=0;i<k;i++)
		{
			t = tmp1;
			tmp1 = ArrayBinaryDilation(t,imgWidth,imgHeight,se);
			delete[] t;
		}
		BinaryUnion(pResult,tmp1,imgWidth,imgHeight);
		
		delete[] tmp1;
		tmp1=tmp2;
		k++;
	}

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount)=pResult[i*imgWidth+j] ;

	return pDestImg;
}


BYTE* ArrayGrayDilation(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* pResult = new BYTE[imgWidth*imgHeight];

	//ѭ������
	int i,j,k,l;

	int SE_C2L = se.m_CenterX;						//�ṹԪ�����ĵ���˵ľ���
	int SE_C2R = se.m_Width -1 - SE_C2L;			//�ṹԪ�����ĵ��ɶ˵ľ���
	int SE_C2T = se.m_CenterY;						//�ṹԪ�����ĵ��϶˵ľ���
	int SE_C2B = se.m_Height -1 - SE_C2T;			//�ṹԪ�����ĵ��¶˵ľ���

	int gray,tmp;

	//������ͼ��,���¼�m_maskH/2�����صĻҶ�Ϊ0�ĺڱߣ����Ҽ�m_maskW/2����
	//�صĻҶ�Ϊ0�ĺڱߣ��ӱߺ��ͼ�����buf
	int newWidth = imgWidth+se.m_Width;
	int newHeight = imgHeight+se.m_Height;
	BYTE *buf=new BYTE[newWidth*newHeight];
	for(i=0;i<newHeight;i++)
	{
		for(j=0;j<newWidth;j++)
		{
			if(i<SE_C2T||i>=imgHeight+SE_C2B||j<SE_C2L||j>=imgWidth+SE_C2R)
				*(buf+i*newWidth+j)=0;
			else
				*(buf+i*newWidth+j)=*(pSrc+(i-SE_C2T)*imgWidth+j-SE_C2L);
		}
	}


	//��������
	for(i=SE_C2T;i<imgHeight+SE_C2T;i++)
	{
		for(j=SE_C2L;j<imgWidth+SE_C2L;j++)
		{
			gray=0;
			for(k=-SE_C2T;k<=SE_C2B;k++)
			{
				for(l=-SE_C2L;l<=SE_C2R;l++)
				{
					if(se.m_Data[k+SE_C2T][l+SE_C2L]==1)
					{
						tmp=*(buf+(i+k)*newWidth+j+l)+se.m_Data[k+SE_C2T][l+SE_C2L];
						if(tmp>gray)		
							gray=tmp;
					}
				}
			}
			if(gray>255)
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=255;
			else
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=gray;
		}
	}

	//�ͷŻ�����
	delete []buf;

	return pResult;
}

CImage* GrayDilation(CImage* pSrcImg, StructureElement se)
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

	BYTE* pResult = ArrayGrayDilation(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount)=pResult[i*imgWidth+j] ;

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}



BYTE* ArrayGrayErosion(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* pResult = new BYTE[imgWidth*imgHeight];

	//ѭ������
	int i,j,k,l;

	int SE_C2L = se.m_CenterX;						//�ṹԪ�����ĵ���˵ľ���
	int SE_C2R = se.m_Width -1 - SE_C2L;			//�ṹԪ�����ĵ��ɶ˵ľ���
	int SE_C2T = se.m_CenterY;						//�ṹԪ�����ĵ��϶˵ľ���
	int SE_C2B = se.m_Height -1 - SE_C2T;			//�ṹԪ�����ĵ��¶˵ľ���

	//���뻺��������м���
	int newWidth = imgWidth+se.m_Width;
	int newHeight = imgHeight+se.m_Height;
	BYTE *buf=new BYTE[newWidth*newHeight];

	//������ͼ��,���¼�m_maskH/2�����صĻҶ�Ϊ255�����ߣ����Ҽ�m_maskW/2����
	//�صĻҶ�Ϊ255�����ߣ��ӱߺ��ͼ�����buf
	for(i=0;i<newHeight;i++)
	{
		for(j=0;j<newWidth;j++)
		{
			if(i<SE_C2T||i>=imgHeight+SE_C2B||j<SE_C2L||j>=imgWidth+SE_C2R)
				*(buf+i*newWidth+j)=255;
			else
				*(buf+i*newWidth+j)=*(pSrc+(i-SE_C2T)*imgWidth+j-SE_C2L);
		}
	}

	//��ʴ���㣬���������������ݼ���Ӧģ�����ݣ�������Сֵ������Сֵ���Ǹõ����
	int gray,tmp;
	for(i=SE_C2T;i<imgHeight+SE_C2T;i++)
	{
		for(j=SE_C2L;j<imgWidth+SE_C2L;j++)
		{
			gray=255;
			for(k=-SE_C2T;k<=SE_C2B;k++)
			{
				for(l=-SE_C2L;l<=SE_C2R;l++)
				{
					if(se.m_Data[k+SE_C2T][l+SE_C2L]==1)
					{
						tmp=*(buf+(i+k)*newWidth+j+l)-se.m_Data[k+SE_C2T][l+SE_C2L];
						if(tmp<gray)		
							gray=tmp;
					}
				}
			}
			if(gray<0)
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=0;
			else
				*(pResult+(i-SE_C2T)*imgWidth+j-SE_C2L)=gray;
		}
	}

	//�ͷŻ�����
  	delete []buf;

	return pResult;
}

CImage* GrayErosion(CImage* pSrcImg, StructureElement se)
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

	BYTE* pResult = ArrayGrayErosion(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=new CImage;
	pDestImg->Create(imgWidth,imgHeight,pSrcImg->GetBPP(),0); //  ����CImage�������λͼ
	CopyColorTables(pDestImg,pSrcImg);//���Ƶ�ɫ��
	BYTE* imgBufOut = (BYTE*)pDestImg->GetBits();

	for(int i=0;i<imgHeight;i++)
		for(int j=0;j<imgWidth;j++)
			*(imgBufOut+i*lineByte+j*sBitCount)=pResult[i*imgWidth+j] ;

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}

BYTE* ArrayGrayOpen(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* tmp = ArrayGrayErosion(pSrc,imgWidth,imgHeight,se);
	BYTE* pResult = ArrayGrayDilation(tmp,imgWidth,imgHeight,se);
	delete[] tmp;
	return pResult;
}

CImage* GrayOpen(CImage* pSrcImg, StructureElement se)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();

	BYTE* pSrc = ReadCImage(pSrcImg);

	BYTE* pResult = ArrayGrayOpen(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=CreateGrayImg(pResult,imgWidth,imgHeight);

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}

BYTE* ArrayGrayClose(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se)
{
	BYTE* tmp = ArrayGrayDilation(pSrc,imgWidth,imgHeight,se);
	BYTE* pResult = ArrayGrayErosion(tmp,imgWidth,imgHeight,se);
	delete[] tmp;
	return pResult;
}

CImage* GrayClose(CImage* pSrcImg, StructureElement se)
{
	int imgWidth = pSrcImg->GetWidth();
	int imgHeight = pSrcImg->GetHeight();

	BYTE* pSrc = ReadCImage(pSrcImg);

	BYTE* pResult = ArrayGrayClose(pSrc,imgWidth,imgHeight,se);

	CImage* pDestImg=CreateGrayImg(pResult,imgWidth,imgHeight);

	delete[] pSrc;
	delete[] pResult;
	return pDestImg;
}

void FillDomain(BYTE* pSrc, int imgWidth, int imgHeight, int x, int y, int &MinX, int &MaxX, int &MinY, int &MaxY)
{
	if(x<0 || y<0 || x>=imgWidth || y>=imgHeight)
		return;
	if(pSrc[y*imgWidth+x]!=255)
		return;

	pSrc[y*imgWidth+x]=0;
	if(x<MinX)
		MinX=x;
	if(x>MaxX)
		MaxX=x;
	if(y<MinY)
		MinY=y;
	if(y>MaxY)
		MaxY=y;

	FillDomain(pSrc,imgWidth,imgHeight,x-1,y,MinX,MaxX,MinY,MaxY);
	FillDomain(pSrc,imgWidth,imgHeight,x,y-1,MinX,MaxX,MinY,MaxY);
	FillDomain(pSrc,imgWidth,imgHeight,x+1,y,MinX,MaxX,MinY,MaxY);
	FillDomain(pSrc,imgWidth,imgHeight,x,y+1,MinX,MaxX,MinY,MaxY);
}

int CountDomain(BYTE* pSrc, int imgWidth, int imgHeight,BYTE* &cm)
{
	//����ͼ��
	BYTE* data = new BYTE[imgWidth*imgHeight];
	BYTE* CenterMap = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			data[i*imgWidth+j]=pSrc[i*imgWidth+j];
			CenterMap[i*imgWidth+j]=0;
		}
	}

	int domaincount=0;
	int MinX,MaxX,MinY,MaxY;
	int tmpx,tmpy;
	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			if(data[i*imgWidth+j]==255)
			{
				domaincount++;
				MinX=MaxX=j;
				MinY=MaxY=i;
				FillDomain(data,imgWidth,imgHeight,j,i,MinX,MaxX,MinY,MaxY);
				tmpx=(MinX+MaxX)/2;
				tmpy=(MinY+MaxY)/2;
				CenterMap[tmpy*imgWidth+tmpx]=255;
			}
		}
	}

	if(cm==NULL)
		delete[] CenterMap;
	else
		cm=CenterMap;

	delete[] data;
	return domaincount;
}

int CountDomain_Len(BYTE* pSrc, int imgWidth, int imgHeight,int* &len,int lindex)
{
	//����ͼ��
	BYTE* data = new BYTE[imgWidth*imgHeight];
	BYTE* CenterMap = new BYTE[imgWidth*imgHeight];
	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			data[i*imgWidth+j]=pSrc[i*imgWidth+j];
			CenterMap[i*imgWidth+j]=0;
		}
	}

	int domaincount=0;
	int MinX,MaxX,MinY,MaxY;
	int tmpx,tmpy;
	//len=new int[10];
	for(int i=0;i<imgHeight;i++)
	{
		for(int j=0;j<imgWidth;j++)
		{
			if(data[i*imgWidth+j]==255)
			{
				MinX=MaxX=j;
				MinY=MaxY=i;
				FillDomain(data,imgWidth,imgHeight,j,i,MinX,MaxX,MinY,MaxY);
				tmpx = MaxX - MinX;
				tmpy = MaxY - MinY;
				len[domaincount+lindex] = sqrt((float)(tmpx*tmpx + tmpy*tmpy));
				domaincount++;
			}
		}
	}

	delete[] data;
	return domaincount;
}