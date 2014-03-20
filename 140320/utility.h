#pragma  once

#include <windows.h>
#include <tchar.h>

void ErrorMessageBox(const DWORD& dwError)
{
	LPTSTR lpszError = NULL;
	DWORD dwFlag = FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD dwLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	DWORD dwRet = ::FormatMessage(dwFlag, NULL, dwError, dwLocale, lpszError, 0, NULL);

	if(dwRet && lpszError)
	{
		//�����޼����� �޼��� �ڽ��� �̿��ؼ� ǥ��
		::MessageBox(NULL, lpszError, _T("Error"), MB_OK | MB_ICONERROR );
	}

	::LocalFree(lpszError);
}