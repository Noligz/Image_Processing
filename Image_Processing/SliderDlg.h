#pragma once


// CSliderDlg 对话框

class CSliderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSliderDlg)

public:
	CSliderDlg(CWnd* pParent = NULL, int* Value = NULL, int MinValue = 0, int MaxValue = 255);   // 标准构造函数
	virtual ~CSliderDlg();

// 对话框数据
	enum { IDD = IDD_SLIDER };
	int *SliderValue;
	int min, max;
	CSliderCtrl m_Slider;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
