#include "stdafx.h"
#include "histogram.h"
#include "CImageOperation.h"

//���캯��
Histogram::Histogram(CImage* pImage)
{
	//ֱ��ͼ������0
	for(int i=0; i < 256; i++)
    	m_histArray[i] = 0;

	m_pImage = pImage;
}

//ͳ��ֱ��ͼ
void Histogram::computeHist(int channel)
{
	//���ǲ�ͼ���ֳ��Լ����ͨ��ֱ��ͼ
	if(channel > 0 && m_pImage->GetBPP() < 24)
		return;

	//ֱ��ͼ������0
	for(int i=0; i < 256; i++)
    	m_histArray[i] = 0;

	BYTE		cPixel;

	int imgWidth = m_pImage->GetWidth();		//ͼ��ĳ�
	int imgHeight = m_pImage->GetHeight();		//ͼ��Ŀ�

	int pit = m_pImage->GetPitch();
	int BitCount = m_pImage->GetBPP()/8;
	BYTE* pImgData = (BYTE*)m_pImage->GetBits();

	switch(channel)
	{
	case 0:
	case 1:
	case 2:
		for(int i=0; i < imgHeight; i++) 
		{   
 			for (int j=0; j < imgWidth; j++)
			{
				cPixel = *(pImgData + i*pit + j*BitCount + channel);
				m_histArray[cPixel]++;		//���Ҷ�ֵ����Ӧ��Ԫ��������
			}
		}
		break;
	case 3:
		for(int i=0; i < imgHeight; i++) 
		{   
 			for (int j=0; j < imgWidth; j++)
			{
				cPixel = *(pImgData + i*pit + j*BitCount)*0.299 + *(pImgData + i*pit + j*BitCount + 1)*0.587 + *(pImgData + i*pit + j*BitCount + 2)*0.114;
				m_histArray[cPixel]++;		//���Ҷ�ֵ����Ӧ��Ԫ��������
			}
		}
		break;
	default:
		break;
	}
}

//ֱ��ͼ����
CImage* Histogram::histAve()
{
	CImage* pImage = new CImage;
	computeHist(0);

	double map[256]={0};		//ӳ���
	int i, j;

	int imgWidth = m_pImage->GetWidth();		//ͼ��ĳ�
	int imgHeight = m_pImage->GetHeight();		//ͼ��Ŀ�

	//����ӳ���
	int num_pixel =  imgWidth * imgHeight;
	int sum = 0;
	for(i=0; i < 256; i++)
	{
		sum+=m_histArray[i];
		map[i]=(double)sum * 255 / num_pixel + 0.5;
	}

	BYTE cPixel,cNewPixel;

	ImageCopy(m_pImage, pImage);		//��m_pImageָ���λͼ���Ƶ�image��

	for(i=0; i < imgHeight; i++)
	{
		for(j=0; j < imgWidth; j++)
		{
			cPixel = CImageGetPixel(m_pImage,j ,i);
			cNewPixel = (BYTE)map[cPixel];
			CImageSetPixel(pImage, j,i, cNewPixel);
		}
	}

	return pImage;
}


CImage* Histogram::HistMatch(Histogram HistTemplate)
{
	int i,j;

	double HistArray[256]={0};

	int imgWidth = m_pImage->GetWidth();		//ͼ��ĳ�
	int imgHeight = m_pImage->GetHeight();		//ͼ��Ŀ�

	computeHist(0);		//����ֱ��ͼ

	//ԭͼ��ֱ��ͼ�ۻ���һ��
	int num_pixel =  m_pImage->GetWidth() * m_pImage->GetHeight();
	int sum = 0;
	for(i=0; i < 256; i++)
	{
		sum+=m_histArray[i];
		HistArray[i]=(double)sum / num_pixel;
	}

	//ģ��ֱ��ͼ�ۻ���һ��
	double TemplateArray[256]={0};
	num_pixel = HistTemplate.m_pImage->GetWidth() * HistTemplate.m_pImage->GetHeight();
	sum = 0;
	for(i=0;i<256;i++)
	{
		sum+=HistTemplate.m_histArray[i];
		TemplateArray[i]=(double)sum / num_pixel;
	}


	//ֱ��ͼƥ��
	double min_diff,tmp;
	int k = 0;
	BYTE mapPixel[256];
	for (i = 0;i < 256;i++)
	{
		 min_diff = fabs(HistArray[i] - TemplateArray[k]);
		 for (j = k + 1; j < 256;j++)
		 {
			  tmp = fabs(HistArray[i] - TemplateArray[j]);
			  if (tmp < min_diff)
			  {
				   min_diff = tmp;
				   k = j;
			  }
		}
		 if (k == 255)
		 {
			  for (int l = i;l < 256;l++)
			  {
				   mapPixel[l] = (BYTE) k;
			  }
			  break;
		 }
		 mapPixel[i] = (BYTE) k;
	}

	CImage* pImage = new CImage;
	BYTE cPixel,cNewPixel;

	ImageCopy(m_pImage, pImage);		//��m_pImageָ���λͼ���Ƶ�image��

	for(i=0; i < imgHeight; i++)
	{
		for(j=0; j < imgWidth; j++)
		{
			cPixel = CImageGetPixel(m_pImage,j ,i);
			cNewPixel = (BYTE)mapPixel[cPixel];
			CImageSetPixel(pImage, j, i, cNewPixel);
		}
	}

	return pImage;
}
