#include <atlimage.h>

class Histogram
{
public:
	int m_histArray[256];		//ֱ��ͼ����
	CImage* m_pImage;
private:
public:
	Histogram(CImage* pImage);		//���캯��
	void computeHist(int channel);		//ͳ��ֱ��ͼ
	CImage* histAve();		//ֱ��ͼ����
	CImage* HistMatch(Histogram HistTemplate);		//ֱ��ͼƥ��
};