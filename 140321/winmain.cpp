#include <windows.h>
#include <tchar.h> // for unicode
#include <math.h> //�Ǵ� <cmath>

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

	//Ÿ��Ʋ��, �׵θ� �β� ���ϴ� �Լ�
	int cx = ::GetSystemMetrics(SM_CXSCREEN); //����� ũ�⸦ ��ȯ
	int cy = ::GetSystemMetrics(SM_CYSCREEN); //����� ũ�⸦ ��ȯ

	//����� ȭ���� ���ϱ�
	cx = (cx-500)/2;
	cy = (cy-500)/2;

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		cx, //â�� ��ġ
		cy, //â�� ��ġ
		500, //�׸��� �ִ� ����
		500, //�׸��� �ִ� ����
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	//WM_TIMER��� �õ�� �Ź� �����Ѵ�.

	//��SetTimer �Լ� ���� �����ε� Ÿ�̹��� ������. ���� ��� ��� �ٶ�
	/*
	::SetTimer(hMainWnd, //handle window
			0,			//timer id
			1000,		//millisecond time
			NULL);		//TIMEPROC //�Լ������͸� �Ѱܾ� �Ѵ�. �� �ѱ�� WM_TIMER��� �õ�� ��� �߻��� ��Ų��. 
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

	//������ũ����Ʈ�� �Ҷ� �߻��ϴ� �޽��� 
	if(uMsg == WM_CREATE)
	{
		//ũ����Ʈ�� ���ÿ� ��Ÿ�̸�
		::SetTimer(hWnd,0,1000,NULL);
	}

	if (uMsg == WM_DESTROY)
	{
		//��������� �����Ѵ�
		::KillTimer(hWnd, NULL);


		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT) //????�� �׿��� �ʸ��� ȣ��
	{
		static float theta = 90.f; //static���� �� ������....???

		LPCTSTR lpszHello = _T("Welcome to Hell World");
		RECT rc;
		::GetClientRect(hWnd, &rc);



		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps); //HDC : �� ���� �� ������ �ִ�.

		::InflateRect(&rc, -50, -50); //���� â������ ���� //�� �Լ��� ����� �����ϸ� L = 50, R = 50 ����� ���Ҷ� (R - L) /2 

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


		int cx = (rc.right + rc.left) / 2;
		int cy = (rc.right + rc.left) / 2;

		const int length = 100;

		POINT pt;
		::MoveToEx(hdc, cx, cy, &pt); //���׸��� ������
		::LineTo(hdc, cx, cy - length); //������ �׷���
		
		//float theta = 90.f; //�� ���ڸ� �ٲٸ� �ٴ� ��ġ�� �ٲ�
		float pi = 2.f*asin(1.f);
		float D2R = pi/180.f;

		int x = cx + length*cos(theta*D2R);
		int y = cy + length*cos(theta*D2R);

		//��
		//int x = cx + length*sin(theta*D2R);
		//int y = cy - length*cos(theta*D2R);

		//��
		//12�ð� : 360�� = 1�ð� : x, x = 360/12 = 30
		//60�� : 30�� = 1�� : x, x = 30/60 = 1/2
		//60�� : 1/2 = 1�� : x, x = 1/120

		//60�� : 360 = 1�� : x, x = 6��
		//60�� : 6�� = 1�� : x, x = 1/10

		//��� : theta�� ������ �ٲ��ָ� �ɵ�

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, x, y);
		
		
		/*
		::MoveToEx(hdc, cx, cy, &pt); //���׸��� ������
		::LineTo(hdc, cx + 50, cy + 50); //������ �׷���
		::MoveToEx(hdc, cx, cy, &pt); //���׸��� ������
		::LineTo(hdc, cx - 50, cy + 50); //������ �׷���
		*/

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


		::EndPaint(hWnd, &ps);
	}
	else if(uMsg == WM_TIMER) //�Ź� �ʸ��� ������ �ȴ�.....?
	{
		//�ð� �������
		::GetLocalTime(&st); //��ǻ�� �ð��� ������
		//::GetSystemTime(&st); //���� ���� �ð��� ������

		RECT rc;
		::GetClientRect(hWnd, &rc);

		//�ʸ��� �����̰� ��
		theta += 10.f;
		if(theta > 360.)) //���� ������ �߰� �����ÿ�

		//���� �� �ð��� ����ϱ� ���� ����
		::InvalidateRect(hWnd, &rc, TRUE); //(����, ����, ����� �׸������� �ƴ���) //true : ����� �ٽ� �׷��� false: ������ ���� �׷���??
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
