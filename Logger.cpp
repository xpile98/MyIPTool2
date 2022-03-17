#include "StdAfx.h"
#include "Logger.h"
#include <process.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <CONIO.H>
//#include "AniUtil.h"

#undef PATH_SLASH 
#define PATH_SLASH           _T('\\') 
#define enter_cs(k)          EnterCriticalSection(k) 
#define leave_cs(k)          LeaveCriticalSection(k)

#if _MSC_VER > 1200 // VS6.0 �̻�
#define init_cs(k)           InitializeCriticalSectionAndSpinCount(k,2000)
#else 
#define init_cs(k)          InitializeCriticalSection(k)
#endif

#define delete_cs(k)         DeleteCriticalSection(k) 

#ifdef LOG_THREAD_ANSYNC 
#undef  enter_cs 
#undef  leave_cs 
#undef  init_cs 
#undef  delete_cs 
#define enter_cs(k) 
#define leave_cs(k) 
#define init_cs(k) 
#define delete_cs(k) 
#endif

#ifdef DIRECT_LOG_MODE
	#define	LOGGING(level,fmt,ap)	Log(_T(#level), fmt, ap)
	#define	LOGGING2(level,msg)	Log2(_T(#level), msg)
#else
	#define LOGGING(level,fmt,ap)	AddInfo(_T(#level), fmt, ap)
	#define LOGGING2(level,msg)	AddInfo2(_T(#level), msg)
#endif

#define LOG_SUB_DIR  _T("\\Util")


CLogger::CLogger(LPCTSTR szLabel, LPCTSTR szLogFile, BOOL bDbgView)
: m_hFile(INVALID_HANDLE_VALUE), m_strLabel(szLabel), m_strLogFile(szLogFile), m_bDbgView(bDbgView)
, m_bLogging(TRUE)
{
	init_cs(&m_csLog);

	ReadLogConfig();

	m_hThread = m_hThreadDel = NULL;
	
	UINT uiThreadId = 0;

#ifndef DIRECT_LOG_MODE
#if _MSC_VER > 1200 // VS6.0 �̻�
	InitializeCriticalSectionAndSpinCount(&m_csAdd, 2000);
#else 
    InitializeCriticalSection(&m_csAdd);
#endif

	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hExitDel = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hThread = (HANDLE)_beginthreadex(NULL,  // Security attributes
										0,    // stack
										LogThreadProc,   // Thread proc
										this,   // Thread param
										CREATE_SUSPENDED,   // creation mode
										&uiThreadId);   // Thread ID

	if(NULL != m_hThread)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread(m_hThread);
	}
#endif
	if(m_bAutoDelete)
	{
		m_hThreadDel = (HANDLE)_beginthreadex(NULL,  // Security attributes
												0,    // stack
												DelThreadProc,   // Thread proc
												this,   // Thread param
												CREATE_SUSPENDED,   // creation mode
												&uiThreadId);   // Thread ID

		if(NULL != m_hThreadDel)
		{
			//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(m_hThreadDel);
		}
	}

}

CLogger::~CLogger(void)
{
#ifndef DIRECT_LOG_MODE
    if(m_hThread != NULL)
    {
		SetEvent(m_hExit);
        SleepEx(100L, TRUE);
        if (WaitForSingleObject(m_hThread, 1000L) == WAIT_TIMEOUT)
            TerminateThread(m_hThread, 1L);
        CloseHandle(m_hThread);
        m_hThread = NULL;
    }

	CloseHandle(m_hExit);	
	m_hExit = NULL;
	DeleteCriticalSection(&m_csAdd);
#endif
	if(m_hThreadDel != NULL)
    {
		SetEvent(m_hExitDel);
        SleepEx(100L, TRUE);
        if (WaitForSingleObject(m_hThreadDel, 2000L) == WAIT_TIMEOUT)
            TerminateThread(m_hThreadDel, 1L);
        CloseHandle(m_hThreadDel);
        m_hThreadDel = NULL;
    }
	CloseHandle(m_hExitDel);	
	m_hExit = m_hExitDel = NULL;

	enter_cs(&m_csLog);
    // Kill Thread
	if(m_hFile != INVALID_HANDLE_VALUE) 
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	leave_cs(&m_csLog);
	delete_cs(&m_csLog);

	m_lsLogInfo.clear();
}

BOOL CLogger::ReadLogConfig()
{
	TCHAR szAppDir[MAX_PATH];
	
	memset(szAppDir, NULL, sizeof(szAppDir));
	::GetModuleFileName(NULL, szAppDir, MAX_PATH);
	m_strAppPath = szAppDir;

	int index = (int)m_strAppPath.rfind('\\');
	m_strAppPath = m_strAppPath.substr(0,index);
	index = (int)m_strAppPath.rfind('\\');
	tstring strTemp = m_strAppPath.substr(index+1, m_strAppPath.size() - index-1);
	std::transform(strTemp.begin(), strTemp.end(), strTemp.begin(), tolower);
	if(strTemp == _T("debug") || strTemp == _T("release"))
		m_strAppPath = m_strAppPath.substr(0,index);
#ifdef _WIN64
	index = (int)m_strAppPath.rfind('\\');
	strTemp = m_strAppPath.substr(index+1, m_strAppPath.size() - index-1);
	std::transform(strTemp.begin(), strTemp.end(), strTemp.begin(), tolower);
	if(strTemp == _T("x64"))
		m_strAppPath = m_strAppPath.substr(0,index);
#endif
	tstring strFilePath = m_strAppPath;
//	strFilePath += LOG_SUB_DIR;
//	strFilePath += _T("\\Util\\Logger.cfg");
	strFilePath += _T("\\Logger.cfg");

	//20130710 lee ������ Logger���� �ٸ� Setting���� ���� 
	tstring strSection;

	strSection = m_strLabel + _T("_") + _T("Output");
	
	m_eLogPeriod = (ELogPeriod)GetPrivateProfileInt(strSection.c_str(), _T("Log Period"), eLogDay, strFilePath.c_str());
	m_eDelPeriod = (EDelPeriod)GetPrivateProfileInt(strSection.c_str(), _T("Delete Period"), eDelYear, strFilePath.c_str());

	strSection = m_strLabel + _T("_") + _T("Config");

	m_iMsgMaxSize = GetPrivateProfileInt(strSection.c_str(), _T("Log Message Max Size"), 1024, strFilePath.c_str());
	m_eLogLevel = (ELogLevel)GetPrivateProfileInt(strSection.c_str(), _T("Log Level"), eLogErr, strFilePath.c_str());
	m_bLogging = GetPrivateProfileInt(strSection.c_str(), _T("Log Flag "), TRUE, strFilePath.c_str());
	m_bLogTickCount = GetPrivateProfileInt(strSection.c_str(), _T("Log Tick Count Flag"), TRUE, strFilePath.c_str());
	m_bUseCache = GetPrivateProfileInt(strSection.c_str(), _T("Log Use Cache Flag"), TRUE, strFilePath.c_str());
	m_bAutoDelete = GetPrivateProfileInt(strSection.c_str(), _T("Log Auto Delete Flag"), TRUE, strFilePath.c_str());

// 	if(!SetOutput(m_strLogFile.c_str()))
// 	{
// 		AfxMessageBox(_T("Log File Creation Error!!  Set [Log Flag] to [FALSE]."));
// 		m_bLogging = FALSE;
// 	}//���

	return TRUE;
}

BOOL CLogger::CreateFolders(LPCTSTR lpszFolderPath)
{
	tstring strPath(lpszFolderPath);
	tstring strDir;

	DWORD dwRet;
	int iRet;

	int index(0);

	index = (int)strPath.find(':');
	if(index < 0)
		index = 0;
	else
		index +=2;

	do
	{
		index = (int)strPath.find('\\', index);
		
		if(index < 0)
			break;
		
		strDir = strPath.substr(0, index);
		if(strDir == _T("") || strDir == _T("\\"))
		{
			index++;
			continue;
		}

		dwRet = GetFileAttributes(strDir.c_str());
		if(dwRet != FILE_ATTRIBUTE_DIRECTORY)		// ������ �������� �ʴ� ���.
		{
			iRet = CreateDirectory(strDir.c_str(), NULL);
			if(iRet == ERROR_PATH_NOT_FOUND)		// �������� ���� ���. 
				return FALSE;
		}
		index++;
		SleepEx(2,false);
	}while(1);
	//������ ����...
	dwRet = GetFileAttributes(strPath.c_str());
	if(dwRet != FILE_ATTRIBUTE_DIRECTORY)		// ������ �������� �ʴ� ���.
	{
		int iRet = CreateDirectory(strPath.c_str(), NULL);
		if(iRet == ERROR_PATH_NOT_FOUND)		// �������� ���� ���. 
			return FALSE;
	}

	return TRUE;
} 

BOOL CLogger::SetOutput(LPCTSTR lpszFilePath) 
{     
	tstring strPath(lpszFilePath);
	int index = (int)strPath.rfind('\\');
	strPath = strPath.substr(0,index);

	if(!CreateFolders(strPath.c_str()))
		return FALSE;	

	GetLocalTime(&m_tmStart);

	if(m_bUseCache)
		m_hFile = CreateFile(lpszFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	else
		m_hFile = CreateFile(lpszFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
	
	if(INVALID_HANDLE_VALUE == m_hFile)
		return false;

	SetFilePointer(m_hFile,0,NULL,FILE_END);

//>> 20150320 kang
#ifdef _UNICODE
	 WORD wd=0xfeff;
	 DWORD dwBytes;

	 WriteFile(m_hFile, &wd, 2, &dwBytes, 0);
#endif
//<<


	return true;
}  


void CLogger::SetUserName(LPCTSTR lpszUserName) //20130705 lee
{
	m_strUser = lpszUserName;
}
CLogger* CLogger::SetSrcPos(LPCTSTR lpszFile, int line, DWORD ThreadId) 
{
	//// ȣ�� ������� list up�ǵ��� ����ȭ����.
	//// ������ AddInfo()���� ����.
	EnterCriticalSection(&m_csAdd);
	m_loginfo.m_strSrcFile = lpszFile; 
	m_loginfo.m_iSrcLine = line;
	m_loginfo.m_dwSrcThreadId = ThreadId;
	m_loginfo.m_bSrc = TRUE; // �ҽ� �������� üũ

	return this;
}

void CLogger::Log(LPCTSTR fmt, ...) 
{     
	va_list ap;
	va_start(ap, fmt);
	TCHAR* pBuf = NULL;
#if _MSC_VER > 1200 // VS6.0 �̻�
	int len = _vsctprintf(fmt, ap) +1; //Null Terminating('\0') +1
	pBuf = new TCHAR[len];
	_vstprintf_s(pBuf, len, fmt, ap);
#else
	pBuf = new TCHAR[m_iMsgMaxSize];
	int len = _vsntprintf(pBuf, m_iMsgMaxSize, fmt, ap); // Max Size������ ��
#endif
	va_end(ap);

	if(len <= 0)
	{
		if(pBuf) 
			delete [] pBuf;
		return;
	}
	enter_cs(&m_csLog);

	if(m_bDbgView)
		OutputDebugString(pBuf);

	if(m_hFile != INVALID_HANDLE_VALUE) 
	{
		DWORD dwWritten;
//>> 20150320 kang
#ifdef _UNICODE	
		WriteFile(m_hFile, pBuf, len*2, &dwWritten, NULL);
#else
		WriteFile(m_hFile, pBuf, len, &dwWritten, NULL);
#endif //<<
		if(!m_bUseCache)	FlushFileBuffers(m_hFile);
	} 

	if(pBuf)	
		delete [] pBuf;

	leave_cs(&m_csLog); 
}

void CLogger::AddInfo(LPCTSTR level, LPCTSTR fmt, va_list& ap)
{ 
	if(!m_bLogging) return;

	// ȣ�� ������� list up�ǵ��� ����ȭ����.
	// SetSrcPos�� ���� ���� AddInfo �� ��� 
	if(!m_loginfo.m_bSrc) 
		EnterCriticalSection(&m_csAdd);

	TCHAR* pBuf = NULL; 
#if _MSC_VER > 1200 // VS6.0 �̻�
	int len = _vsctprintf(fmt, ap) +1; //Null Terminating('\0') +1
	pBuf = new TCHAR[len+1];
	_vstprintf_s(pBuf, len, fmt, ap);
#else
	pBuf = new TCHAR[m_iMsgMaxSize];
	int len = _vsntprintf(pBuf, m_iMsgMaxSize, fmt, ap); // Max Size������ ��
#endif
	if(len <= 0)
	{
		if(pBuf) 
			delete [] pBuf;

		m_loginfo.m_bSrc = FALSE; //����
		LeaveCriticalSection(&m_csAdd);
		return;
	}

	m_loginfo.m_strLevel = level;
	m_loginfo.m_strMsg = pBuf;
	if(pBuf) 
		delete [] pBuf;
	
	m_lsLogInfo.push_back(m_loginfo);
	m_loginfo.m_bSrc = FALSE; //����
	
	LeaveCriticalSection(&m_csAdd);
}

void CLogger::AddInfo2(LPCTSTR level, LPCTSTR msg)
{
	if(!m_bLogging) 
		return;
	// ȣ�� ������� list up�ǵ��� ����ȭ����.
	// SetSrcPos�� ���� ���� AddInfo �� ���
	if(!m_loginfo.m_bSrc) 
		EnterCriticalSection(&m_csAdd);

	m_loginfo.m_strLevel = level;
	m_loginfo.m_strMsg = msg;

	m_lsLogInfo.push_back(m_loginfo);
	m_loginfo.m_bSrc = FALSE; // ����...

	LeaveCriticalSection(&m_csAdd);
}

void CLogger::Log(LPCTSTR szLevel, LPCTSTR fmt, va_list& ap) 
{
	if(!m_bLogging) 
		return;

	TCHAR* pBuf = NULL;
#if _MSC_VER > 1200 // VS6.0 �̻�
	int len = _vsctprintf(fmt, ap) +1; //Null Terminating('\0') +1
	pBuf = new TCHAR[len];
	_vstprintf_s(pBuf, len, fmt, ap);
#else
	pBuf = new TCHAR[m_iMsgMaxSize];
	int len = _vsntprintf(pBuf, m_iMsgMaxSize, fmt, ap); // Max Size������ ��
#endif
	if(len <= 0)
	{
		if(pBuf) 
			delete [] pBuf;
		m_loginfo.m_bSrc = FALSE; // ����
		return;
	}

	m_loginfo.m_strLevel = szLevel;
	m_loginfo.m_strMsg = pBuf;

	if(pBuf) 
		delete [] pBuf;

	Log(m_loginfo);
	m_loginfo.m_bSrc = FALSE; // ����
}

void CLogger::Log2(LPCTSTR szLevel, LPCTSTR szMsg) 
{
	if(!m_bLogging) 
		return;

	m_loginfo.m_strLevel = szLevel;
	m_loginfo.m_strMsg = szMsg;

	Log(m_loginfo);
	m_loginfo.m_bSrc = FALSE; // ����
}


void CLogger::Log(LogInfo& loginfo) 
{
	if(!m_bLogging) 
		return;

	enter_cs(&m_csLog);

 	SYSTEMTIME tmNow;
	GetLocalTime(&tmNow);

	try
	{
		tostringstream ossTime, ossSrc;
		//>>20130703 lee
	/*	if(loginfo.m_strLevel != _T("ALWS"))*/ //>>2015.03.21 KMS ALWS�� �ð� �׻� ���
		{
			ossTime << '[' <<  loginfo.m_strLevel.c_str() << _T("] ");
			ossTime << '[' <<  m_strUser << _T("] "); //20130705 lee
			
			ossTime << tmNow.wMonth << '/' << tmNow.wDay << '_' << tmNow.wHour << ':' << tmNow.wMinute << ':' << tmNow.wSecond<< ':' << tmNow.wMilliseconds<< ' '; //130421 JSPark	20160528
			
			if(m_bLogTickCount)
			{
				DWORD dwMilsec = GetCurrentTime();
				ossTime << '\t' << dwMilsec << '_\t';
			}
		}
		//<<
		if(loginfo.m_bSrc)
		{
			int index = (int)loginfo.m_strSrcFile.rfind(_T('\\'));
			loginfo.m_strSrcFile = loginfo.m_strSrcFile.substr(index+1);
		
			ossSrc << _T(" ->(") << m_strLabel.c_str() << ':' << loginfo.m_strSrcFile.c_str() << ':';
			ossSrc << loginfo.m_iSrcLine << _T(" ,Thread:") << loginfo.m_dwSrcThreadId << _T(")");
		}
		else
		{	//>>20130703 lee
			if(loginfo.m_strLevel != _T("ALWS"))
			{
				ossSrc << '\t' << _T(" (") << m_strLabel.c_str() << _T(") - ");
			}
			//<<
		}
//>> 130321 JSPark
		tstring str(ossTime.str());
		str += loginfo.m_strMsg;
		str += ossSrc.str();
		str += _T("\r\n");
//<<
		if(m_bDbgView)
			OutputDebugString(str.c_str());

		if(m_hFile != INVALID_HANDLE_VALUE) 
		{
			ChangeFile(tmNow);

			DWORD dwWritten;
//>>20150303 lee
#ifdef _UNICODE	
			WriteFile(m_hFile, str.c_str(), (DWORD)str.size() * 2 , &dwWritten, NULL);
#else
			WriteFile(m_hFile, str.c_str(), (DWORD)str.size(), &dwWritten, NULL);
#endif
//<<

			if(!m_bUseCache)	FlushFileBuffers(m_hFile);
		}
	}

	catch(exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
	}

	leave_cs(&m_csLog); 
}


BOOL CLogger::ChangeFile(SYSTEMTIME& tmNow)
{
	if(!m_bLogging) 
		return TRUE;

	BOOL bChangeFile(FALSE);

	switch(m_eLogPeriod)
	{		
		//20130710 lee
	case eLogMinute:
		if(m_tmStart.wMinute != tmNow.wMinute)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLogHour:
		if(m_tmStart.wHour != tmNow.wHour)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLog3H:
		if((tmNow.wHour-m_tmStart.wHour) > 2)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLog6H:
		if((tmNow.wHour-m_tmStart.wHour) > 5)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLog12H:
		if((tmNow.wHour-m_tmStart.wHour) > 11)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLogDay:
		if(m_tmStart.wDay != tmNow.wDay)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLogWeek:
		if((tmNow.wDay-m_tmStart.wDay) > 6)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLog15D:
		if((tmNow.wDay-m_tmStart.wDay) > 14)
		{
			bChangeFile = TRUE;
			break;
		}
	case eLogMonth:
		if(m_tmStart.wMonth != tmNow.wMonth)
			bChangeFile = TRUE;
		break;	
	}


	if(bChangeFile)
	{
		tostringstream ossNew;

		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;

		try 
		{
			int index = (int)m_strLogFile.rfind('.');
			tstring strName = m_strLogFile.substr(0, index);
			tstring strExt = m_strLogFile.substr(index);
			ossNew << strName << '_';
			ossNew <<  tmNow.wMonth << '_' << tmNow.wDay << '_' << tmNow.wHour << '_' << tmNow.wMinute;
			ossNew << strExt ;
		}

		catch(exception& e)
		{
			cerr << "exception caught: " << e.what() << endl;
			return TRUE;
		}

		MoveFile(m_strLogFile.c_str(), ossNew.str().c_str());
		SetOutput(m_strLogFile.c_str());
	}

	return TRUE;
}


void CLogger::Always(LPCTSTR fmt, ...) 
{   
	va_list ap; 
	va_start(ap, fmt);

	LOGGING(ALWS,fmt,ap);

	va_end(ap);
}

void CLogger::Always2(LPCTSTR msg) 
{   
	LOGGING2(ALWS,msg);
}

void  CLogger::Fatal(LPCTSTR fmt, ...) 
{         
	if(eLogFatal > m_eLogLevel)
		return;

	va_list ap; 
	va_start(ap, fmt);

	LOGGING(FATL,fmt,ap);

	va_end(ap);
}

void  CLogger::Fatal2(LPCTSTR msg) 
{         
	if(eLogFatal > m_eLogLevel) 
		return;

	LOGGING2(FATL,msg);
}


void  CLogger::Error(LPCTSTR fmt, ...) 
{         
	if(eLogErr > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}
	va_list ap; 
	va_start(ap, fmt);

	LOGGING(ERR,fmt,ap);

	va_end(ap);
}

void  CLogger::Error2(LPCTSTR msg) 
{         
	if(eLogErr > m_eLogLevel)  
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	LOGGING2(ERR,msg);
}

void  CLogger::Warn(LPCTSTR fmt, ...) 
{
	if(eLogWarn > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	va_list ap; 
	va_start(ap, fmt);

	LOGGING(WARN,fmt,ap);

	va_end(ap);
}

void  CLogger::Warn2(LPCTSTR msg) 
{
	if(eLogWarn > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	LOGGING2(WARN, msg);
}

void  CLogger::Info(LPCTSTR fmt, ...) 
{
	if(eLogInfo > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	va_list ap; 
	va_start(ap, fmt);

	LOGGING(INFO,fmt,ap);

	va_end(ap);
}

void  CLogger::Info2(LPCTSTR msg) 
{
	if(eLogInfo > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	LOGGING2(INFO,msg);
}

void  CLogger::Step(LPCTSTR fmt, ...) 
{
	if(eLogStep > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	va_list ap; 
	va_start(ap, fmt);

	LOGGING(STEP,fmt,ap);

	va_end(ap);
}

void  CLogger::Step2(LPCTSTR msg) 
{
	if(eLogStep > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	LOGGING2(STEP,msg);
}

void  CLogger::Debug(LPCTSTR fmt, ...) 
{
	if(eLogDebug > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	va_list ap; 
	va_start(ap, fmt);

	LOGGING(DBG,fmt,ap);

	va_end(ap);
} 

void  CLogger::Debug2(LPCTSTR msg) 
{
	if(eLogDebug > m_eLogLevel) 
	{
		//>> 20130915 kang
		if(m_loginfo.m_bSrc)
		{
			m_loginfo.m_bSrc = FALSE;
			LeaveCriticalSection(&m_csAdd);
		} //<<
		return;
	}

	LOGGING2(DBG,msg);
}


void CLogger::DeleteLogFiles(LPCTSTR lpszFolderPath)
{
	if(m_eDelPeriod == eDelNo)
		return;

	tstring strFileFound = lpszFolderPath;
	strFileFound += _T("\\*.*");		 
	
	SYSTEMTIME stCurTime,stFileTime;
	int iSaveDays;

	switch(m_eDelPeriod)
	{
	case eDelHour:
		iSaveDays = 1;  //20130718 lee
		break;
	case eDelDay:
		iSaveDays = 1;
		break;
	case eDelWeek:
		iSaveDays = 7;
		break;
	case eDel15D:
		iSaveDays = 15;
		break;
	case eDelMonth:
		iSaveDays = 30;
		break;
	case eDelQuart:
		iSaveDays = 90;
		break;
	case eDelHalf:
		iSaveDays = 180;
		break;
	case eDelYear:
	default:
		iSaveDays = 365;
		break;
	}

	GetLocalTime(&stCurTime);
	
 	WIN32_FIND_DATA info;

	HANDLE hp = FindFirstFile(strFileFound.c_str(), &info);   //���丮�� ������ �ִ��� ù��° ���ϸ�.
    
	do
    {
        if(!((_tcscmp(info.cFileName, _T("."))==0) || (_tcscmp(info.cFileName, _T(".."))==0))) 
		{
            if((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)  //Sub���丮�� �����ϴ°�� 
            {
                tstring strSubFolder = lpszFolderPath;
				strSubFolder += _T("\\");
				strSubFolder += info.cFileName;

				DeleteLogFiles(strSubFolder.c_str()); /// {return (_Ptr == 0 ? _Nullstr() : _Ptr); } 
				RemoveDirectory(strSubFolder.c_str()); //�Ϻο� �ִ� ������ �����Ϸ��� ���⼭ ȣ������...
             }
            else
			{
				strFileFound = lpszFolderPath;
				strFileFound += _T("\\");
				strFileFound += info.cFileName;

				if(m_strLogFile != strFileFound) // ���� ������� ���� ����...
				{
					FileTimeToSystemTime(&info.ftLastWriteTime, &stFileTime);

					if(m_eDelPeriod == eDelHour)
					{
						if(stCurTime.wHour - stFileTime.wHour > iSaveDays || 
							stCurTime.wDay - stFileTime.wDay > 0)
							BOOL retVal = DeleteFile(strFileFound.c_str());
					}
					else
					{
						if(stCurTime.wDay - stFileTime.wDay > iSaveDays)
							BOOL retVal = DeleteFile(strFileFound.c_str());
					}
				}
            }

		}
		SleepEx(2, false);
    }while(FindNextFile(hp, &info));
    
	FindClose(hp);

//	RemoveDirectory(strSubFolder.c_str()); //���� ���Ÿ� ���⼭ �ϸ� ���ڷ� �Ѿ�� ����� �ֻ��� ������ ���ŵ�.
}

#ifndef DIRECT_LOG_MODE
void CLogger::Run()
{
	ResetEvent(m_hExit);

	LogInfo loginfo;
	while(::WaitForSingleObject(m_hExit, 10) != WAIT_OBJECT_0)
	{
		if(!m_lsLogInfo.empty())
		{
			// Add���̸� ��ٸ�.
			EnterCriticalSection(&m_csAdd);
			loginfo = m_lsLogInfo.front();
			m_lsLogInfo.pop_front();
			LeaveCriticalSection(&m_csAdd);
			
			Log(loginfo);
		}
		SleepEx(2, false);
	}
}

UINT WINAPI CLogger::LogThreadProc(LPVOID pParam)
{
    CLogger* pThis = reinterpret_cast<CLogger*>( pParam );
    _ASSERTE( pThis != NULL );

    pThis->Run();

    return 1L;
}
#endif

void CLogger::RunDel()
{
	ResetEvent(m_hExitDel);

	SYSTEMTIME tmStart, tmEnd;
	GetLocalTime(&tmStart);
	
	int index = (int)m_strLogFile.rfind('\\');
	tstring strPath = m_strLogFile.substr(0,index);
	  
	while (::WaitForSingleObject(m_hExitDel, 1000) != WAIT_OBJECT_0)
	{
		GetLocalTime(&tmEnd);

//		if((tmEnd.wDay - tmStart.wDay) > 0) // �Ϸ������ ����... //�ð� ������ ���� 		
		if((tmEnd.wHour - tmStart.wHour) > 0) // �Ϸ������ ����... //�ð� ������ ���� 
		{
			DeleteLogFiles(strPath.c_str());
			tmStart = tmEnd;
		}
	}
}

UINT WINAPI CLogger::DelThreadProc(LPVOID pParam)
{
    CLogger* pThis = reinterpret_cast<CLogger*>( pParam );
    _ASSERTE( pThis != NULL );

    pThis->RunDel();

    return 1L;
}


