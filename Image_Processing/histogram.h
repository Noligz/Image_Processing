#include <atlimage.h>

class Histogram
{
public:
	int m_histArray[256];		//直方图数组
	CImage* m_pImage;
private:
public:
	Histogram(CImage* pImage);		//构造函数
	void computeHist(int channel);		//统计直方图
	CImage* histAve();		//直方图均衡
	CImage* HistMatch(Histogram HistTemplate);		//直方图匹配
};