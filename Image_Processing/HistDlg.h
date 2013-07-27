#pragma once



// CHistDlg �Ի���

class CHistDlg : public CDialog
{
	DECLARE_DYNAMIC(CHistDlg)

public:
	CHistDlg(CWnd* pParent = NULL , CImage* pImage = NULL);   // ��׼���캯��
	virtual ~CHistDlg();

// �Ի�������
	enum { IDD = IDD_HIST };
	Histogram* m_pHist;
	int m_ChannelSelected;
	CComboBox VariableSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSelectChannel();
};
