#pragma once


// CImageDlg 对话框

class CImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg(CWnd* pParent = NULL, CImage* pImg = NULL);   // 标准构造函数
	virtual ~CImageDlg();

// 对话框数据
	enum { IDD = IDD_IMAGE };
	CImage* m_pImg;
	int imgWidth;
	int imgHeight;
	int ImgOffset;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSave();
};
