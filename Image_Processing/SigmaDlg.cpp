// SigmaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "SigmaDlg.h"


// CSigmaDlg �Ի���

IMPLEMENT_DYNAMIC(CSigmaDlg, CDialog)

CSigmaDlg::CSigmaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSigmaDlg::IDD, pParent)
{
}

CSigmaDlg::~CSigmaDlg()
{
}

void CSigmaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSigmaDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSigmaDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSigmaDlg ��Ϣ�������

void CSigmaDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_SIGMA_VALUE)->GetWindowText(str);
	SigmaValue = atof(str);
	OnOK();
}
