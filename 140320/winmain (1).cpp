#include <windows.h>
#include <tchar.h> // for unicode
#include "utility.h"

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);

INT
WINAPI
_tWinMain(HINSTANCE hInst,
		  HINSTANCE hPrevInst,
		  LPTSTR lpCmdLine,
		  INT nCmdShow)
{
	LPCTSTR lpClassName = _T("MyMainWindow");

	WNDCLASSEX wcex;
	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInst;
	wcex.lpfnWndProc = &MyWndProc;
	wcex.lpszClassName = lpClassName;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!::RegisterClassEx(&wcex))
	{
		DWORD dwError = ::GetLastError();

		return 2;
	}

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		ErrorMessageBox(dwError);
		return 1;
	}

	//WM_TIMER��� �õ�� �Ź� �����Ѵ�.
	::SetTimer(hMainWnd, 0, 1000, NULL);

	::ShowWindow(hMainWnd, nCmdShow);
	::UpdateWindow(hMainWnd);

	MSG msg;

	while (true)
	{
		if (::GetMessage(&msg, 0, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			break;
		}
	}

	//��������� �����Ѵ�
	::KillTimer(hMainWnd, NULL);


	return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static SYSTEMTIME st;

	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		LPCTSTR lpszHello = _T("Welcome to Hell World");
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps); //HDC : �� ���� �� ������ �ִ�.

		::InflateRect(&rc, -50, -50); //���� â������ ����

		//�⺻������ �׸���� WM_PAINT ó���ÿ� �Ѵ�.

		//�� �����, DC�� �����ϵ��� ����
		HPEN GreenPen = ::CreatePen(PS_SOLID, 5, RGB(100,200,100));
		HPEN oldPen = (HPEN)::SelectObject(hdc, GreenPen);
		
		//�� �����, DC�� �����ϵ��� ����
		HBRUSH RedBrush = ::CreateSolidBrush(RGB(200,100,100));
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, RedBrush); //c++�� �ƴ϶� ����ȯ�� ����� �Ѵ�

		

		//���� �׸���....
		//::FillRect(hdc, &rc, RedBrush); //���ڸ� �귯���� 'ä���' ( , ������ //�׵θ����� ����

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); //�׵θ� ���� �ִ�(�׵θ����� ����Ѵ�)

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);//���ڸ� ���س��� ���� ä���


		POINT pt;
		::MoveToEx(hdc, 100, 100, &pt); //���׸��� ������
		::LineTo(hdc, 200, 300);//������ �׷���
		::LineTo(hdc, 400, 400);
		//�� ó�� ���� �����ϱ�
		//::MoveToEx(hdc, 400, 400, &pt);
		::LineTo(hdc, 100, 100);
	
		//��
		::SelectObject(hdc, oldBrush);
		::DeleteObject(RedBrush);

		//��
		::SelectObject(hdc, oldPen);
		::DeleteObject(GreenPen);

		//�ý��� Ÿ�̸Ӹ� �� ���� ���۷����� �޴´�
		
		::GetLocalTime(&st);
	

		
		TCHAR lpszTime[10];

		//�ð� ���
		_stprintf_s(lpszTime, _countof(lpszTime), _T("%02d:%02d:%02d"), st.wHour, st.wMinute,st.wSecond);
		::DrawText(hdc, lpszTime, -1, &rc, DT_CENTER);

		//::DrawText(hdc, lpszHello, -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);


		::EndPaint(hWnd, &ps);
	}
	else if(uMsg == WM_TIMER)
	{
		::GetLocalTime(&st);
		
		RECT rc;
		::GetClientRect(hWnd, &rc);

		::InvalidateRect(hWnd, &rc, TRUE); //(����, ����, ����� �׸������� �ƴ���)
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
