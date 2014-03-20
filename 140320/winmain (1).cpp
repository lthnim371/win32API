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

	//WM_TIMER라는 시동어를 매번 실행한다.
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

	//만들었으면 꺼야한다
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
		HDC hdc = ::BeginPaint(hWnd, &ps); //HDC : 펜 같은 걸 가지고 있다.

		::InflateRect(&rc, -50, -50); //내부 창사이즈 조절

		//기본적으로 그리기는 WM_PAINT 처리시에 한다.

		//펜 만들고, DC가 선택하도록 하자
		HPEN GreenPen = ::CreatePen(PS_SOLID, 5, RGB(100,200,100));
		HPEN oldPen = (HPEN)::SelectObject(hdc, GreenPen);
		
		//붓 만들고, DC가 선택하도록 하자
		HBRUSH RedBrush = ::CreateSolidBrush(RGB(200,100,100));
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, RedBrush); //c++이 아니라서 형변환을 해줘야 한다

		

		//무언가 그리는....
		//::FillRect(hdc, &rc, RedBrush); //상자를 브러쉬로 '채운다' ( , 사이즈 //테두리색이 없다

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); //테두리 색이 있다(테두리펜을 사용한다)

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);//상자를 정해놓고 가득 채우기


		POINT pt;
		::MoveToEx(hdc, 100, 100, &pt); //선그리기 시작점
		::LineTo(hdc, 200, 300);//어디까지 그려라
		::LineTo(hdc, 400, 400);
		//맨 처음 점과 연결하기
		//::MoveToEx(hdc, 400, 400, &pt);
		::LineTo(hdc, 100, 100);
	
		//붓
		::SelectObject(hdc, oldBrush);
		::DeleteObject(RedBrush);

		//펜
		::SelectObject(hdc, oldPen);
		::DeleteObject(GreenPen);

		//시스템 타이머를 콜 바이 래퍼런스로 받는다
		
		::GetLocalTime(&st);
	

		
		TCHAR lpszTime[10];

		//시간 출력
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

		::InvalidateRect(hWnd, &rc, TRUE); //(인자, 영역, 지우고 그릴것인지 아닌지)
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
