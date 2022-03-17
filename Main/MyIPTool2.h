
// MyIPTool2.h : MyIPTool2 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "MyIPImage.h"
//#include "Logger.h"		//190430 YWCho Logger Header

// CMyIPTool2App:
// �� Ŭ������ ������ ���ؼ��� MyIPTool2.cpp�� �����Ͻʽÿ�.
//

class CMyIPTool2App : public CWinAppEx
{
public:
	CMyIPTool2App();


// �������Դϴ�.
public:
	MyIPImage* m_pImage;	//190227 ���� �� â���� ����
	Mat* m_pMat;			//190227 ���� �� â���� ����
	CString m_sTitle;
	CMultiDocTemplate* m_pImageDocTemplate;

// 	clock_t timestart;	//�̰� ����� �ϴµ�;... ��ũ�η� �׳� �Ҷ� r�׷��� �Ƚ�!~
// 	clock_t timeend;

	//190430 YWCho logger 
	//CLogger* m_pOpTimeLog;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnWindowCloseall();

	afx_msg void OnWebcamLivecam();
};

extern CMyIPTool2App theApp;

void AfxNewImage(MyIPImage& image);		//190227 ���� �� â���� ����. �����Լ���.
void AfxNewImage(Mat& mat);				//190227 ���� �� â���� ����. �����Լ���.
void AfxNewImage(CString title, MyIPImage& image);		//190311 ������ ����� �Բ� ���� 
void AfxNewImage(CString title, Mat& mat);				//190311 ������ ����� �Բ� ���� 

void AfxPrintInfo(CString message);			
void AfxPrintInfo(LPCTSTR IpszFormat, ...);


enum { 
	IOP_ADD = 0, 
	IOP_SUB, 
	IOP_AVR, 
	IOP_DIF, 
	IOP_AND, 
	IOP_OR, 
	IOP_MIN, 
	IOP_MAX, 
	IOP_XOR,
	IOP_MUL,
};

enum { 
	IOP_BORDER_CONSTANT=0,
	IOP_BORDER_REPLICATE,
	IOP_BORDER_REFLECT,
	IOP_BORDER_WRAP
};

enum { 
	FILTER_AVR = 0, 
	FILTER_GAUSSIAN, 
	FILTER_MAX, 
	FILTER_MIN, 
	FILTER_MED, 
	FILTER_MED_ADP 
};

enum { 
	SHP_HIGHBOOST = 0,
	SHP_LAPLACIAN
};

enum {
	EDGE_ROBERT=0,
	EDGE_PREWITT,
	EDGE_SOBEL,
	EDGE_LAPLACIAN4,
	EDGE_LAPLACIAN8,
	EDGE_CANNY,
	EDGE_LOG,
	EDGE_DOG
};

enum {
	SE_DISK = 0,
	SE_RING,
	SE_RECTANGLE,
	SE_FRAME,
	SE_CROSS,
	SE_DIAMOND,
};

#define TIMECHECK_START clock_t endtime,s = clock();
#define TIMECHECK_START_AGAIN s = clock();
#define TIMECHECK_END endtime = clock(); AfxPrintInfo(L"%04.3f\n",(double)(endtime-s) / CLOCKS_PER_SEC); printf("%04.3fs\n",(double)(endtime-s) / CLOCKS_PER_SEC);
