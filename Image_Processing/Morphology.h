
#define MaxSESize 100
class StructureElement
{
public:
	BYTE m_Data[MaxSESize][MaxSESize];
	int m_Width, m_Height;
	int m_CenterX, m_CenterY;
public:
	void CreateSquareSE(int width);
	void CreateCrossSE();
	void CreateDiscSE();
	void CreateCircleSE(int r);
	void CreateLineSE(int size,float degree);
};

void Binarize(CImage* pImg, int threshold);
BYTE* ArrayBinaryDilation(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* BinaryDilation(CImage* pSrcImg, StructureElement se);
BYTE* ArrayBinaryErosion(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* BinaryErosion(CImage* pSrcImg, StructureElement se);
BYTE* ArrayBinaryOpen(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* BinaryOpen(CImage* pSrcImg, StructureElement se);
BYTE* ArrayBinaryClose(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* BinaryClose(CImage* pSrcImg, StructureElement se);

bool IsImgAllBlack(BYTE* pSrc,int imgWidth, int imgHeight);
CImage* DistanceTransform(CImage* pSrcImg, int type);
void BinaryUnion(BYTE* a, BYTE* b, int imgWidth, int imgHeight);
void BinaryMinus(BYTE* a, BYTE* b, int imgWidth, int imgHeight);
CImage* Skeleton(CImage* pSrcImg);
CImage* SkeletonReconstruct(CImage* pSrcImg);

BYTE* ArrayGrayDilation(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* GrayDilation(CImage* pSrcImg, StructureElement se);
BYTE* ArrayGrayErosion(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* GrayErosion(CImage* pSrcImg, StructureElement se);
BYTE* ArrayGrayOpen(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* GrayOpen(CImage* pSrcImg, StructureElement se);
BYTE* ArrayGrayClose(BYTE* pSrc, int imgWidth, int imgHeight, StructureElement se);
CImage* GrayClose(CImage* pSrcImg, StructureElement se);

void FillDomain(BYTE* pSrc, int imgWidth, int imgHeight, int x, int y, int &MinX, int &MaxX, int &MinY, int &MaxY);
int CountDomain(BYTE* pSrc, int imgWidth, int imgHeight,BYTE* &cm);
int CountDomain_Len(BYTE* pSrc, int imgWidth, int imgHeight,int* &len,int lindex);
