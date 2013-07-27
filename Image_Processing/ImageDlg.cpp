// ImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "ImageDlg.h"


// CImageDlg 对话框

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


// CImageDlg 消息处理程序

void CImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	m_pImg->BitBlt(dc, ImgOffset, ImgOffset, imgWidth, imgHeight, 0, 0, SRCCOPY);
}

BOOL CImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int WndWidth = imgWidth + 2 * ImgOffset;
	int WndHeight = imgHeight + 110;
	CRect rect;

	//设置窗口大小
	rect=CRect(300, 300, 300 + WndWidth, 300 + WndHeight);
	MoveWindow(&rect);

	//移动保存按钮
	rect = CRect(10, WndHeight - 65, 70, WndHeight - 35);
	GetDlgItem(IDC_SAVE)->MoveWindow(&rect);

	//移动关闭按钮
	rect = CRect(WndWidth - 75, WndHeight - 65, WndWidth - 15, WndHeight - 35);
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImageDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pImg->IsNull()) 
	{
		MessageBox(_T("读取图像文件失败！"),_T("ERROR"),MB_OK | MB_ICONERROR);
		return;
	}
	CString strFilter;
	strFilter ="位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";
	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	if ( IDOK != dlg.DoModal())
		return;

	//获取文件名
	CString strFileName;
	strFileName = dlg.m_ofn.lpstrFile;

	// 如果用户没有指定文件扩展名，则为其添加一个
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

	// 图像保存
	HRESULT hResult = m_pImg->Save(strFileName);
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"),_T("ERROR"),MB_OK | MB_ICONERROR);
	}
}
