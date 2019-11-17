#include<Windows.h>
#include<time.h>
#include<stdlib.h>

#define BLOCK 100
//入口  区别 控制台程序main  窗口GUI WinMain
//WINAPI 函数调用约定
//hInstance 当前程序的应用实例
//hPreInstance  前一个应用程序实例
//IpCmdLine  命令行参数
//nCmdShow 窗口显示方式  最大化/最小化
//HWND 窗口句柄类型 UINT 无符号整型 
//窗口处理函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR IpCmdLine, int nCmdShow){
	//1、做一个窗口  设计窗口类；注册窗口类；创建窗口；显示窗口；更新窗口；消息循环
	//设计
	TCHAR szAppClassName[] = TEXT("ShuaiQiGui");


	WNDCLASS wc;
	wc.cbClsExtra = 0;										//窗口类扩展空间大小 默认给0字节
	wc.cbWndExtra = 0;										//窗口类扩展空间大小 默认给0字节
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//加载白色背景
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//加载光标
	wc.hIcon = NULL;										//加载图标
	wc.hInstance = hInstance;								//应用程序实例句柄
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = szAppClassName;						//窗口类型名
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;						//窗口类风格

	//注册
	RegisterClass(&wc);

	//创建窗口
	HWND hWnd = CreateWindow(
		szAppClassName,  //窗口类型名
		TEXT("别踩白块"),  //窗口标题
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,  //窗口风格
		500, 200, //窗口左上角坐标
		4 * BLOCK + 17, 4 * BLOCK + 40,  //窗口宽和高
		NULL,  //父窗口句柄
		NULL,  //菜单句柄
		hInstance,  //应用程序实例句柄
		NULL   //参数
		);

	//显示
	ShowWindow(hWnd, SW_SHOW);

	//更新
	UpdateWindow(hWnd);

	//消息循环
	MSG msg;//消息机制驱动运行
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//将虚拟键消息转换成字符消息
		TranslateMessage(&msg);
		//将消息分发给窗口处理函数
		DispatchMessage(&msg);
	}

	return 0;
}

//窗口处理函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	PAINTSTRUCT ps;
	HDC hDC;  //绘图句柄  DC 设备上下文
	static int bw[4];  //每一屏幕有四个黑块  从上到下  左到右
	RECT rect;   //矩形区域
	HBRUSH hBrush;//刷子
	POINT point;   //鼠标点击坐标
	static int count = 0;  //统计格子个数
	static int tm = 0;  //所耗时间
	TCHAR szMes[200];

	switch (uMsg)
	{
	case WM_CREATE://窗口创建时 生成随机位置  显示黑块
	{
		srand((unsigned)time(NULL));
		for (int i = 0; i < 4; i++){
			bw[i] = rand() % 4;
		}
		//开启定时器
		SetTimer(hWnd, 1, 10, NULL);
	}
		break;
	case WM_TIMER:
		tm++;
		break;
	case WM_PAINT://窗口绘制
		hDC = BeginPaint(hWnd, &ps);
		//画图Rectangle(hDC, 0, 0, 100, 100);
		//划线
		//水平
		for (int i = 0; i <= 4; i++){
			MoveToEx(hDC, 0, i*BLOCK, NULL);
			LineTo(hDC, 400, i*BLOCK);
		}
		//竖直
		for (int i = 0; i <= 4; i++){
			MoveToEx(hDC, i*BLOCK, 0, NULL);
			LineTo(hDC, i*BLOCK, 4 * BLOCK);
		}
		//画黑块
		for (int i = 0; i < 4; i++){
			SetRect(&rect, bw[i] * BLOCK, i*BLOCK, bw[i] * BLOCK + BLOCK, i*BLOCK + BLOCK);
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC, hBrush);
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			//释放资源
			DeleteObject(hBrush);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:  //左键按下
	{
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		//判断是否踩中位置
		if ((point.x / BLOCK) != bw[3] || point.y / BLOCK != 3){
			wsprintf(szMes, L"您输了！用时：%d.%d秒 格数%d个", tm / 100, tm - (tm / 100) * 100, count);
			MessageBox(hWnd, szMes, L"游戏结束", MB_OK);
			PostQuitMessage(0);
		}
		else if (point.y / BLOCK == 3){
			//随机数
			for (int i = 3; i >= 1; i--){
				bw[i] = bw[i - 1];
			}
			bw[0] = rand() % 4;
			count++;
			//滚动窗口  一次往下一百像素（也就是一行)
			ScrollWindow(hWnd, 0, BLOCK, NULL, NULL);
		}
	}

		break;
		//case WM_KEYDOWN:   //键盘按下
		//	break;
	case WM_CLOSE: //窗口关闭消息
		DestroyWindow(hWnd);  //销毁窗口  无法恢复
		break;
	case WM_DESTROY:  //窗口销毁消息
		PostQuitMessage(0); //退出进程
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}