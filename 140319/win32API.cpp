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

	WNDCLASSEX wcex; //������Ŭ���� ������ ���� ����
	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex); //count byte. ��ü�� �� byte��? ���������� ����//���ʼ�
	wcex.cbClsExtra = 0; //Ŭ������ �޸� �⺻�����δ� 0
	wcex.cbWndExtra = 0; //������
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH); //��׶��带 ĥ�� ��(�귯��)
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW); //�����쿡�� �������� ���콺 Ŀ�� //IDC_��ɾ� : Ŀ�� ���
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION); //Ÿ��Ʋ �� ���� �׸��� �۾�ǥ���� �������� �ǹ�
	wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION); //Sm : ����. 
	wcex.hInstance = hInst; //HINSTANCE hInst�� ���ڰ����� ������. ���� ����ǰ� �ִ� ���μ����� �����´�. //���ʼ���
	wcex.lpfnWndProc = &MyWndProc; //Long pointer function. �Լ������͸� �ѱ� //��� �޽��� ó���� ���⼭ �϶�� ���� //���ʼ�
	wcex.lpszClassName = lpClassName; //���� ���� Ŭ���� //���ʼ�
	wcex.lpszMenuName = NULL; //�޴�
	wcex.style = CS_VREDRAW | CS_HREDRAW; //Ŭ���� ��Ÿ��

	if (!::RegisterClassEx(&wcex)) //���. �����ڵ��̳� �ƴϳĿ� ���� ���� W(�����ڵ�) �Ǵ� A�� ����
	{
		DWORD dwError = ::GetLastError(); //GetLastError : �����Ұ� �ִٸ� �߸������ ���� Ȯ���� ���� ���. ������ȣ�� �� �� �ִ�.
		return 2;
	}

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName, //lpClassName : ������ Ŭ������ �ʿ��� //CreateWindow�� ����Ͽ��� ���ο��� EX�� ȣ����
		_T("Hello World"),
		//���� : x��ư�� ������ �ּ�ȭ, �ִ�ȭ ��ư �� ������ ����
		WS_OVERLAPPEDWINDOW, //�����콺Ÿ��. Ÿ��Ʋ�� �ݱ� ��ư ����...
		CW_USEDEFAULT, //x��ǥ
		CW_USEDEFAULT, //y��ǥ
		CW_USEDEFAULT, //width
		CW_USEDEFAULT, //height
		0, //
		0, //�޴�
		hInst, //�׻� ���� â
		0); //�ΰ������� �� ���� ������ �ִ� ���

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	::ShowWindow(hMainWnd, nCmdShow); //������ �� ����
	::UpdateWindow(hMainWnd);

	MSG msg;

	//��Ƽ������� �ݺ� �߿��� �ٸ� �۾� ����
	while (true) //�޽���ť�� �������� ������ ����
	{
		//if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //�޽����� �ֵ� ���� ������ ��ȯ. PM_REMOVE : ���� ���鼭 peek�� �ִ°� ����°�
		if (::GetMessage(&msg, 0, 0, 0)) //������ true, ������ �н�. ���� ���鼭 �������� ����
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) //�޽����� ������ //�ݵ�� �ʼ�
		{
			break;
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) //��� �޽����� �޾� ó���Ѵ�.
{
	if (uMsg == WM_DESTROY)
	{
		//SendMessage
		::PostQuitMessage(0); //������ ����. WM_QUIT�� ������ �� //�ݵ�� �ʼ�
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
		::GetClientRect(hWnd, &rc); //���� �׸� �� �ִ� ������ ũ��(Ŭ���̾�Ʈ�� ũ��)�� ���� �´�(â�� ���θ� ���Ѵ�)

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::DrawText(hdc, lpszHello, -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE); //-1 : NULL���ڱ��� ��� /rc : �׸� �� �ִ� ���� /DT_SINGLELINE : �� ���� �Ǹ� ��Ƽ������ �� // ������ 1,2,4,8 ~~ �ΰ� ��Ʈ�� ��ġ�� �ʰԲ� �ϱ� ����


		::EndPaint(hWnd, &ps);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam); //�⺻������ ���ο��� ��Ʈ���� �۾��� ��
}
