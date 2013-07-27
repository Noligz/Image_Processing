// Image_ProcessingDoc.h : CImage_ProcessingDoc 类的接口
//


#pragma once


class CImage_ProcessingDoc : public CDocument
{
protected: // 仅从序列化创建
	CImage_ProcessingDoc();
	DECLARE_DYNCREATE(CImage_ProcessingDoc)

// 属性
public:
	CImage LoadedImage;		//加载的图像文件
	CImage GreyImg;				//灰度图

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CImage_ProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};


