// HistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "HistDlg.h"



// CHistDlg 对话框

IMPLEMENT_DYNAMIC(CHistDlg, CDialog)

CHistDlg::CHistDlg(CWnd* pParent /*=NULL*/, CImage* pImage)
	: CDialog(CHistDlg::IDD, pParent)
{
	m_pHist = new Histogram(pImage);
	m_ChannelSelected = 0;
}

CHistDlg::~CHistDlg()
{
}

void CHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_SELECT_CHANNEL, m_ChannelSelected);
	DDX_Control(pDX, IDC_SELECT_CHANNEL, VariableSel);
}


BEGIN_MESSAGE_MAP(CHistDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_SELECT_CHANNEL, &CHistDlg::OnCbnSelchangeSelectChannel)
END_MESSAGE_MAP()

//对话框初始化
BOOL CHistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pHist->computeHist(0);

	if(m_pHist->m_pImage->GetBPP() < 24)
		GetDlgItem(IDC_SELECT_CHANNEL)->ShowWindow(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CHistDlg 消息处理程序
void CHistDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	//将静态框1移至rect指定位置，用来画直方图
	CRect rect;
	rect=CRect(25,40,281,180);
	GetDlgItem(IDC_STATIC_HistRect)->MoveWindow(&rect);

	//将颜色提示条移至指定位置
	rect=CRect(25,181,281,200);
	GetDlgItem(IDC_STATIC_BMPBAR)->MoveWindow(&rect);

	//获取直方图绘制静态框的矩形区域
 	CRect clientRect;  
	GetDlgItem(IDC_STATIC_HistRect)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);

	//画直方图灰度频率与直方图绘制矩形区域比例关系，画直方图
	unsigned int max=0;
	int i;
	for(i=0;i<256;i++){
		if(m_pHist->m_histArray[i] > max)
			max=m_pHist->m_histArray[i];
	}
	DWORD bufTemp[256];
	double x=clientRect.Height();
	for(i=0;i<256;i++){
		bufTemp[i]=(unsigned long)((float)m_pHist->m_histArray[i]*x/max);
	}
	for(i=clientRect.left;i<=clientRect.right;i++){
		dc.MoveTo(i,clientRect.bottom-1);
		dc.LineTo(i,clientRect.bottom-bufTemp[i-clientRect.left]);
	}   
}

void CHistDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//该函数在创建对话框时调用，确定直方图对话框的大小及客户区中的位置
	//CRect rect;
	//rect=CRect(405,261,717,545);
	//MoveWindow(&rect);
}

void CHistDlg::OnCbnSelchangeSelectChannel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ChannelSelected = VariableSel.GetCurSel();
	m_pHist->computeHist(m_ChannelSelected);
	Invalidate();
}
