// SliderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "SliderDlg.h"


// CSliderDlg �Ի���

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


// CSliderDlg ��Ϣ�������

void CSliderDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*SliderValue = m_Slider.GetPos();
	OnOK();
}

BOOL CSliderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Slider.SetRange(min,max);
	m_Slider.SetPos(min);
	CString s;
	s.Format("��ǰֵ��%d",min);
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(s);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSliderDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CSliderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CSliderCtrl   *pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	int v = pSlidCtrl->GetPos();
	CString s;
	s.Format("��ǰֵ��%d",v);
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(s);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
