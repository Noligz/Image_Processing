#include <atlimage.h>

void CopyColorTables(CImage * pNewImage, CImage * pImage);
int GetBytesPerPixel(CImage *pImage);
int GetBytesPerLine(CImage *pImage);

bool IsPntInImage(CImage* pImg, int x, int y);

void ImageCopy(CImage * pImage, CImage * pNewImage);

BYTE CImageGetPixel(CImage* pImg, int x, int y);
void CImageSetPixel(CImage* pImg, int x, int y, BYTE value);

int RestrictNum(int &num, int min, int max);

CImage* AddImageBorder(CImage *pImage);
CImage* SubtractImageBorder(CImage *pImage);

void ImageZoom(CImage* pDestImg, CImage* pSrcImg, int &ZoomX, int &ZoomY, int InterType);
void ImageTrans(CImage* pDestImg, CImage* pSrcImg, int &offsetX, int &offsetY);
void ImageRotate(CImage* pDestImg, CImage* pSrcImg, float RotateAngle, int InterType);

void ZoomNeighbor(CImage* pDestImg, CImage* pSrcImg, float ratioX, float ratioY, int widthOut, int heightOut, int widthIn, int heightIn);
void ZoomBilinear(CImage* pDestImg, CImage* pSrcImg, float ratioX, float ratioY, int widthOut, int heightOut, int widthIn, int heightIn);
void ZoomBicubic(CImage* pDestImg, CImage* pSrcImg, float ratioX, float ratioY, int widthOut, int heightOut, int widthIn, int heightIn);

void RotateNeighbor(CImage* pDestImg, CImage* pSrcImg, float fSina, float fCosa, float f1, float f2, int widthOut, int heightOut, int widthIn, int heightIn);
void RotateBilinear(CImage* pDestImg, CImage* pSrcImg, float fSina, float fCosa, float f1, float f2, int widthOut, int heightOut, int widthIn, int heightIn);
void RotateBicubic(CImage* pDestImg, CImage* pSrcImg, float fSina, float fCosa, float f1, float f2, int widthOut, int heightOut, int widthIn, int heightIn);

BYTE* ReadCImage(CImage* pSrcImg);
CImage* CreateGrayImg(BYTE* pData, int imgWidth, int imgHeight);
BYTE* ReadColorCImage(CImage* pSrcImg);
CImage* CreateColorImg(BYTE* pData, int imgWidth, int imgHeight);

void RGB2HSI(BYTE* data,int imgWidth,int imgHeight);
void HSI2RGB(BYTE* data,int imgWidth,int imgHeight);

BYTE* ExtractChannel(BYTE* pSrc,int channel,int imgWidth,int imgHeight);
void SetChannel(BYTE* pDest,BYTE* pChanel,int channel,int imgWidth,int imgHeight);