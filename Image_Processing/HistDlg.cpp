// HistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "HistDlg.h"



// CHistDlg �Ի���

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

//�Ի����ʼ��
BOOL CHistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pHist->computeHist(0);

	if(m_pHist->m_pImage->GetBPP() < 24)
		GetDlgItem(IDC_SELECT_CHANNEL)->ShowWindow(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// CHistDlg ��Ϣ�������
void CHistDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	//����̬��1����rectָ��λ�ã�������ֱ��ͼ
	CRect rect;
	rect=CRect(25,40,281,180);
	GetDlgItem(IDC_STATIC_HistRect)->MoveWindow(&rect);

	//����ɫ��ʾ������ָ��λ��
	rect=CRect(25,181,281,200);
	GetDlgItem(IDC_STATIC_BMPBAR)->MoveWindow(&rect);

	//��ȡֱ��ͼ���ƾ�̬��ľ�������
 	CRect clientRect;  
	GetDlgItem(IDC_STATIC_HistRect)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);

	//��ֱ��ͼ�Ҷ�Ƶ����ֱ��ͼ���ƾ������������ϵ����ֱ��ͼ
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

	// TODO: �ڴ˴������Ϣ����������
	//�ú����ڴ����Ի���ʱ���ã�ȷ��ֱ��ͼ�Ի���Ĵ�С���ͻ����е�λ��
	//CRect rect;
	//rect=CRect(405,261,717,545);
	//MoveWindow(&rect);
}

void CHistDlg::OnCbnSelchangeSelectChannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ChannelSelected = VariableSel.GetCurSel();
	m_pHist->computeHist(m_ChannelSelected);
	Invalidate();
}
