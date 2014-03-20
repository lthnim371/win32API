#include <windows.h>
#include <tchar.h> // for unicode

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);

INT
WINAPI
_tWinMain(HINSTANCE hInst,
		  HINSTANCE hPrevInst,
		  LPTSTR lpCmdLine,
		  INT nCmdShow)
{
	LPCTSTR lpClassName = _T("MyMainWindow");

	WNDCLASSEX wcex; //윈도우클래스 정보를 갖고 있음
	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex); //count byte. 객체가 몇 byte냐? 내부적으로 구분//★필수
	wcex.cbClsExtra = 0; //클래스의 메모리 기본적으로는 0
	wcex.cbWndExtra = 0; //윈도우
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH); //백그라운드를 칠할 붓(브러쉬)
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW); //윈도우에서 보여지는 마우스 커서 //IDC_명령어 : 커서 모양
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION); //타이틀 맨 왼쪽 그리고 작업표시줄 아이콘을 의미
	wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION); //Sm : 스몰. 
	wcex.hInstance = hInst; //HINSTANCE hInst를 인자값으로 가져옴. 현재 실행되고 있는 프로세서를 가져온다. //★필수★
	wcex.lpfnWndProc = &MyWndProc; //Long pointer function. 함수포인터를 넘김 //모든 메시지 처리를 여기서 하라는 설정 //★필수
	wcex.lpszClassName = lpClassName; //위에 정한 클래스 //★필수
	wcex.lpszMenuName = NULL; //메뉴
	wcex.style = CS_VREDRAW | CS_HREDRAW; //클래스 스타일

	if (!::RegisterClassEx(&wcex)) //등록. 유니코드이냐 아니냐에 따라 끝에 W(유니코드) 또는 A가 붙음
	{
		DWORD dwError = ::GetLastError(); //GetLastError : 리턴할게 있다면 잘못됬는지 여부 확인을 위해 사용. 에러번호를 알 수 있다.
		return 2;
	}

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName, //lpClassName : 윈도우 클래스가 필요함 //CreateWindow로 사용하여도 내부에서 EX로 호출함
		_T("Hello World"),
		//숙제 : x버튼만 나오게 최소화, 최대화 버튼 안 나오게 설정
		WS_OVERLAPPEDWINDOW, //윈도우스타일. 타이틀의 닫기 버튼 같은...
		CW_USEDEFAULT, //x좌표
		CW_USEDEFAULT, //y좌표
		CW_USEDEFAULT, //width
		CW_USEDEFAULT, //height
		0, //
		0, //메뉴
		hInst, //항상 상위 창
		0); //부가적으로 줄 것이 있을때 주는 기능

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	::ShowWindow(hMainWnd, nCmdShow); //없으면 안 보임
	::UpdateWindow(hMainWnd);

	MSG msg;

	//멀티쓰레드로 반복 중에도 다른 작업 가능
	while (true) //메시지큐를 가져오고 보내는 루프
	{
		//if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //메시지가 있든 없든 무조건 반환. PM_REMOVE : 갖고 오면서 peek에 있는걸 지우는거
		if (::GetMessage(&msg, 0, 0, 0)) //있으면 true, 없으면 패스. 갖고 오면서 지우지는 않음
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) //메시지가 끝나면 //반드시 필수
		{
			break;
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) //모든 메시지를 받아 처리한다.
{
	if (uMsg == WM_DESTROY)
	{
		//SendMessage
		::PostQuitMessage(0); //던지고 간다. WM_QUIT에 전송이 됨 //반드시 필수
		return 0;
	}
	else if(uMsg == WM_CLOSE)
	{
		if (IDOK == ::MessageBox(NULL, _T("Really?"), _T("Confirm"), MB_OKCANCEL))
		{
			::DestroyWindow(hWnd);
		}
		return 0;
	}
	else if (uMsg == WM_PAINT) //UpdateWindow
	{
		LPCTSTR lpszHello = _T("Welcome to Hell World");
		RECT rc;
		::GetClientRect(hWnd, &rc); //현재 그릴 수 있는 영역의 크기(클라이언트의 크기)를 갖고 온다(창의 내부를 말한다)

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::DrawText(hdc, lpszHello, -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE); //-1 : NULL문자까지 출력 /rc : 그릴 수 있는 영역 /DT_SINGLELINE : 안 쓰게 되면 멀티라인이 됨 // 순서가 1,2,4,8 ~~ 인건 비트로 겹치지 않게끔 하기 위함


		::EndPaint(hWnd, &ps);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam); //기본적으로 내부에서 디스트로이 작업을 함
}
