#pragma once


// CImageDlg �Ի���

class CImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg(CWnd* pParent = NULL, CImage* pImg = NULL);   // ��׼���캯��
	virtual ~CImageDlg();

// �Ի�������
	enum { IDD = IDD_IMAGE };
	CImage* m_pImg;
	int imgWidth;
	int imgHeight;
	int ImgOffset;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSave();
};
