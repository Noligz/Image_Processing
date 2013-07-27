// SliderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "SliderDlg.h"


// CSliderDlg 对话框

IMPLEMENT_DYNAMIC(CSliderDlg, CDialog)

CSliderDlg::CSliderDlg(CWnd* pParent /*=NULL*/, int *Value, int MinValue, int MaxValue)
	: CDialog(CSliderDlg::IDD, pParent)
{
	SliderValue = Value;
	min = MinValue;
	max = MaxValue;
}

CSliderDlg::~CSliderDlg()
{
}

void CSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
}


BEGIN_MESSAGE_MAP(CSliderDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSliderDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CSliderDlg::OnNMCustomdrawSlider1)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSliderDlg 消息处理程序

void CSliderDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	*SliderValue = m_Slider.GetPos();
	OnOK();
}

BOOL CSliderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Slider.SetRange(min,max);
	m_Slider.SetPos(min);
	CString s;
	s.Format("当前值：%d",min);
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(s);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSliderDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CSliderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl   *pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	int v = pSlidCtrl->GetPos();
	CString s;
	s.Format("当前值：%d",v);
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(s);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
