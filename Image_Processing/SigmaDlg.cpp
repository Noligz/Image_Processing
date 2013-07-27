// SigmaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "SigmaDlg.h"


// CSigmaDlg 对话框

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


// CSigmaDlg 消息处理程序

void CSigmaDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_SIGMA_VALUE)->GetWindowText(str);
	SigmaValue = atof(str);
	OnOK();
}
