// Image_ProcessingDoc.h : CImage_ProcessingDoc ��Ľӿ�
//


#pragma once


class CImage_ProcessingDoc : public CDocument
{
protected: // �������л�����
	CImage_ProcessingDoc();
	DECLARE_DYNCREATE(CImage_ProcessingDoc)

// ����
public:
	CImage LoadedImage;		//���ص�ͼ���ļ�
	CImage GreyImg;				//�Ҷ�ͼ

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CImage_ProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};


