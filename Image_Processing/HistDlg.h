#pragma once



// CHistDlg 对话框

class CHistDlg : public CDialog
{
	DECLARE_DYNAMIC(CHistDlg)

public:
	CHistDlg(CWnd* pParent = NULL , CImage* pImage = NULL);   // 标准构造函数
	virtual ~CHistDlg();

// 对话框数据
	enum { IDD = IDD_HIST };
	Histogram* m_pHist;
	int m_ChannelSelected;
	CComboBox VariableSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSelectChannel();
};
