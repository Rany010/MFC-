#include<Windows.h>
#include<time.h>
#include<stdlib.h>

#define BLOCK 100
//���  ���� ����̨����main  ����GUI WinMain
//WINAPI ��������Լ��
//hInstance ��ǰ�����Ӧ��ʵ��
//hPreInstance  ǰһ��Ӧ�ó���ʵ��
//IpCmdLine  �����в���
//nCmdShow ������ʾ��ʽ  ���/��С��
//HWND ���ھ������ UINT �޷������� 
//���ڴ�����
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR IpCmdLine, int nCmdShow){
	//1����һ������  ��ƴ����ࣻע�ᴰ���ࣻ�������ڣ���ʾ���ڣ����´��ڣ���Ϣѭ��
	//���
	TCHAR szAppClassName[] = TEXT("ShuaiQiGui");


	WNDCLASS wc;
	wc.cbClsExtra = 0;										//��������չ�ռ��С Ĭ�ϸ�0�ֽ�
	wc.cbWndExtra = 0;										//��������չ�ռ��С Ĭ�ϸ�0�ֽ�
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//���ذ�ɫ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//���ع��
	wc.hIcon = NULL;										//����ͼ��
	wc.hInstance = hInstance;								//Ӧ�ó���ʵ�����
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = szAppClassName;						//����������
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;						//��������

	//ע��
	RegisterClass(&wc);

	//��������
	HWND hWnd = CreateWindow(
		szAppClassName,  //����������
		TEXT("��Ȱ׿�"),  //���ڱ���
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,  //���ڷ��
		500, 200, //�������Ͻ�����
		4 * BLOCK + 17, 4 * BLOCK + 40,  //���ڿ�͸�
		NULL,  //�����ھ��
		NULL,  //�˵����
		hInstance,  //Ӧ�ó���ʵ�����
		NULL   //����
		);

	//��ʾ
	ShowWindow(hWnd, SW_SHOW);

	//����
	UpdateWindow(hWnd);

	//��Ϣѭ��
	MSG msg;//��Ϣ������������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//���������Ϣת�����ַ���Ϣ
		TranslateMessage(&msg);
		//����Ϣ�ַ������ڴ�����
		DispatchMessage(&msg);
	}

	return 0;
}

//���ڴ�����
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	PAINTSTRUCT ps;
	HDC hDC;  //��ͼ���  DC �豸������
	static int bw[4];  //ÿһ��Ļ���ĸ��ڿ�  ���ϵ���  ����
	RECT rect;   //��������
	HBRUSH hBrush;//ˢ��
	POINT point;   //���������
	static int count = 0;  //ͳ�Ƹ��Ӹ���
	static int tm = 0;  //����ʱ��
	TCHAR szMes[200];

	switch (uMsg)
	{
	case WM_CREATE://���ڴ���ʱ �������λ��  ��ʾ�ڿ�
	{
		srand((unsigned)time(NULL));
		for (int i = 0; i < 4; i++){
			bw[i] = rand() % 4;
		}
		//������ʱ��
		SetTimer(hWnd, 1, 10, NULL);
	}
		break;
	case WM_TIMER:
		tm++;
		break;
	case WM_PAINT://���ڻ���
		hDC = BeginPaint(hWnd, &ps);
		//��ͼRectangle(hDC, 0, 0, 100, 100);
		//����
		//ˮƽ
		for (int i = 0; i <= 4; i++){
			MoveToEx(hDC, 0, i*BLOCK, NULL);
			LineTo(hDC, 400, i*BLOCK);
		}
		//��ֱ
		for (int i = 0; i <= 4; i++){
			MoveToEx(hDC, i*BLOCK, 0, NULL);
			LineTo(hDC, i*BLOCK, 4 * BLOCK);
		}
		//���ڿ�
		for (int i = 0; i < 4; i++){
			SetRect(&rect, bw[i] * BLOCK, i*BLOCK, bw[i] * BLOCK + BLOCK, i*BLOCK + BLOCK);
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC, hBrush);
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			//�ͷ���Դ
			DeleteObject(hBrush);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:  //�������
	{
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		//�ж��Ƿ����λ��
		if ((point.x / BLOCK) != bw[3] || point.y / BLOCK != 3){
			wsprintf(szMes, L"�����ˣ���ʱ��%d.%d�� ����%d��", tm / 100, tm - (tm / 100) * 100, count);
			MessageBox(hWnd, szMes, L"��Ϸ����", MB_OK);
			PostQuitMessage(0);
		}
		else if (point.y / BLOCK == 3){
			//�����
			for (int i = 3; i >= 1; i--){
				bw[i] = bw[i - 1];
			}
			bw[0] = rand() % 4;
			count++;
			//��������  һ������һ�����أ�Ҳ����һ��)
			ScrollWindow(hWnd, 0, BLOCK, NULL, NULL);
		}
	}

		break;
		//case WM_KEYDOWN:   //���̰���
		//	break;
	case WM_CLOSE: //���ڹر���Ϣ
		DestroyWindow(hWnd);  //���ٴ���  �޷��ָ�
		break;
	case WM_DESTROY:  //����������Ϣ
		PostQuitMessage(0); //�˳�����
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}