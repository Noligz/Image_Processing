#pragma once


// CSliderDlg �Ի���

class CSliderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSliderDlg)

public:
	CSliderDlg(CWnd* pParent = NULL, int* Value = NULL, int MinValue = 0, int MaxValue = 255);   // ��׼���캯��
	virtual ~CSliderDlg();

// �Ի�������
	enum { IDD = IDD_SLIDER };
	int *SliderValue;
	int min, max;
	CSliderCtrl m_Slider;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
