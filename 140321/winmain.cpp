#include <windows.h>
#include <tchar.h> // for unicode
#include <math.h> //또는 <cmath>

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

	//타이틀바, 테두리 두께 구하는 함수
	int cx = ::GetSystemMetrics(SM_CXSCREEN); //모니터 크기를 반환
	int cy = ::GetSystemMetrics(SM_CYSCREEN); //모니터 크기를 반환

	//정가운데 화면을 구하기
	cx = (cx-500)/2;
	cy = (cy-500)/2;

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		cx, //창의 위치
		cy, //창의 위치
		500, //그릴수 있는 영역
		500, //그릴수 있는 영역
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	//WM_TIMER라는 시동어를 매번 실행한다.

	//★SetTimer 함수 원형 설명인데 타이밍을 놓쳤음. 주의 깊게 듣기 바람
	/*
	::SetTimer(hMainWnd, //handle window
			0,			//timer id
			1000,		//millisecond time
			NULL);		//TIMEPROC //함수포인터를 넘겨야 한다. 안 넘기면 WM_TIMER라는 시동어를 계속 발생을 시킨다. 
	*/

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




	return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static SYSTEMTIME st;

	//윈도우크레이트를 할때 발생하는 메시지 
	if(uMsg == WM_CREATE)
	{
		//크레이트와 동시에 셋타이머
		::SetTimer(hWnd,0,1000,NULL);
	}

	if (uMsg == WM_DESTROY)
	{
		//만들었으면 꺼야한다
		::KillTimer(hWnd, NULL);


		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT) //????가 쌓여서 초마다 호출
	{
		static float theta = 90.f; //static으로 한 이유는....???

		LPCTSTR lpszHello = _T("Welcome to Hell World");
		RECT rc;
		::GetClientRect(hWnd, &rc);



		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps); //HDC : 펜 같은 걸 가지고 있다.

		::InflateRect(&rc, -50, -50); //내부 창사이즈 조절 //이 함수로 사이즈를 조절하면 L = 50, R = 50 정가운데 구할때 (R - L) /2 

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


		int cx = (rc.right + rc.left) / 2;
		int cy = (rc.right + rc.left) / 2;

		const int length = 100;

		POINT pt;
		::MoveToEx(hdc, cx, cy, &pt); //선그리기 시작점
		::LineTo(hdc, cx, cy - length); //어디까지 그려라
		
		//float theta = 90.f; //이 숫자를 바꾸면 바늘 위치가 바뀜
		float pi = 2.f*asin(1.f);
		float D2R = pi/180.f;

		int x = cx + length*cos(theta*D2R);
		int y = cy + length*cos(theta*D2R);

		//팁
		//int x = cx + length*sin(theta*D2R);
		//int y = cy - length*cos(theta*D2R);

		//팁
		//12시간 : 360도 = 1시간 : x, x = 360/12 = 30
		//60분 : 30도 = 1분 : x, x = 30/60 = 1/2
		//60초 : 1/2 = 1초 : x, x = 1/120

		//60분 : 360 = 1분 : x, x = 6도
		//60초 : 6도 = 1초 : x, x = 1/10

		//결론 : theta만 적절히 바꿔주면 될듯

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, x, y);
		
		
		/*
		::MoveToEx(hdc, cx, cy, &pt); //선그리기 시작점
		::LineTo(hdc, cx + 50, cy + 50); //어디까지 그려라
		::MoveToEx(hdc, cx, cy, &pt); //선그리기 시작점
		::LineTo(hdc, cx - 50, cy + 50); //어디까지 그려라
		*/

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


		::EndPaint(hWnd, &ps);
	}
	else if(uMsg == WM_TIMER) //매번 초마다 실행이 된다.....?
	{
		//시간 갖고오기
		::GetLocalTime(&st); //컴퓨터 시간을 가져옴
		//::GetSystemTime(&st); //국제 기준 시간을 가져옴

		RECT rc;
		::GetClientRect(hWnd, &rc);

		//초마다 움직이게 됨
		theta += 10.f;
		if(theta > 360.)) //적다 말았음 추가 적으시오

		//갖고 온 시간을 출력하기 위한 영역
		::InvalidateRect(hWnd, &rc, TRUE); //(인자, 영역, 지우고 그릴것인지 아닌지) //true : 지우고 다시 그려라 false: 지우지 말고 그려라??
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
