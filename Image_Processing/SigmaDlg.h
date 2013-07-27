#pragma once


// CSigmaDlg 对话框

class CSigmaDlg : public CDialog
{
	DECLARE_DYNAMIC(CSigmaDlg)

public:
	CSigmaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSigmaDlg();

// 对话框数据
	enum { IDD = IDD_SIGMA };
	float SigmaValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
