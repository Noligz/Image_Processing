// ImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "ImageDlg.h"


// CImageDlg �Ի���

IMPLEMENT_DYNAMIC(CImageDlg, CDialog)

CImageDlg::CImageDlg(CWnd* pParent /*=NULL*/, CImage* pImg)
	: CDialog(CImageDlg::IDD, pParent)
{
	m_pImg = pImg;
	imgWidth = m_pImg->GetWidth();
	imgHeight = m_pImg->GetHeight();
	ImgOffset = 30;
}

CImageDlg::~CImageDlg()
{
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SAVE, &CImageDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// CImageDlg ��Ϣ�������

void CImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

void CImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	m_pImg->BitBlt(dc, ImgOffset, ImgOffset, imgWidth, imgHeight, 0, 0, SRCCOPY);
}

BOOL CImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int WndWidth = imgWidth + 2 * ImgOffset;
	int WndHeight = imgHeight + 110;
	CRect rect;

	//���ô��ڴ�С
	rect=CRect(300, 300, 300 + WndWidth, 300 + WndHeight);
	MoveWindow(&rect);

	//�ƶ����水ť
	rect = CRect(10, WndHeight - 65, 70, WndHeight - 35);
	GetDlgItem(IDC_SAVE)->MoveWindow(&rect);

	//�ƶ��رհ�ť
	rect = CRect(WndWidth - 75, WndHeight - 65, WndWidth - 15, WndHeight - 35);
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CImageDlg::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pImg->IsNull()) 
	{
		MessageBox(_T("��ȡͼ���ļ�ʧ�ܣ�"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}
	CString strFilter;
	strFilter ="λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|GIF ͼ���ļ�|*.gif|PNG ͼ���ļ�|*.png||";
	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	if ( IDOK != dlg.DoModal())
		return;

	//��ȡ�ļ���
	CString strFileName;
	strFileName = dlg.m_ofn.lpstrFile;

	// ����û�û��ָ���ļ���չ������Ϊ�����һ��
	CString strExtension;
	switch (dlg.m_ofn.nFilterIndex)
	{
		case 1:
			strExtension = ".bmp"; break;
		case 2:
			strExtension = ".jpg"; break;
		case 3:
			strExtension = ".gif"; break;
		case 4:
			strExtension = ".png"; break;
		default:
			break;
	}
	if(strFileName.Right(4) != strExtension)
		strFileName += strExtension; 

	// ͼ�񱣴�
	HRESULT hResult = m_pImg->Save(strFileName);
	if (FAILED(hResult))
	{
		MessageBox(_T("����ͼ���ļ�ʧ�ܣ�"),_T("ERROR"),MB_OK | MB_ICONERROR);
	}
}
