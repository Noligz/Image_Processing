#pragma once


// CSigmaDlg �Ի���

class CSigmaDlg : public CDialog
{
	DECLARE_DYNAMIC(CSigmaDlg)

public:
	CSigmaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSigmaDlg();

// �Ի�������
	enum { IDD = IDD_SIGMA };
	float SigmaValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
