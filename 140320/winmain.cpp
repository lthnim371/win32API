#pragma  once

#include <windows.h>
#include <tchar.h>

void ErrorMessageBox(const DWORD& dwError)
{
	DWORD dwFlag = FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD dwLocale = makelandid(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	DWORD dwRet = ::FormatMessage(dwFlag, NULL, dwEror, dwLocale, lpszError, 0, NULL);

	if(dwRet && lpszError)
	{
		//에러메세지를 메세지 박스를 이용해서 표시
		::MessageBox(NULL, lpszError, _T("Error"), MB_OK | MB_ICONERROR );
	}

	::LocalFree(lpszError);
}