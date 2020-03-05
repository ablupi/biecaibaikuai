#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <cstring>
#include <atlstr.h>
#include "Resource.h"
//#include <qt.h>

#define BLOCK 100
//#define MAKEINTRESOURCE MAKEINTRESOURCEW

//main是控制台应用程序
//WinMain是GUI窗口应用程序
//WINAPI：函数的调用约定
//HWND：窗口句柄类型	ID
//UINT：unsinged int

//窗口处理函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	//一、做一个窗口
	//	1、设计窗口类
	TCHAR szAPPClassName[] = TEXT("DunKaiGuiFoundation");

	WNDCLASSEX wc;
	wc.cbClsExtra = 0;	//窗口类扩展空间大小
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;	//窗口的扩展空间大小
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//窗口颜色，这里为白色
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//加载光标？意义不明
	//wc.hIcon = LoadIcon(NULL, IDI_BieCaiBaikuai);
	wc.hIcon = NULL;
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BIECAIBAIKUAI));
	//wc.hIconSm = LoadIcon(hInstance, IDI_BieCaiBaikuai);
	//wc.hIconSm = LoadIcon(NULL, IDI_BIECAIBAIKUAI);
	wc.hInstance = hInstance;	//应用程序实例句柄
	wc.lpfnWndProc = WindowProc;	//窗口处理函数，翻页
	wc.lpszClassName = szAPPClassName;	//窗口类型号
	wc.lpszMenuName = NULL;	//菜单名
	wc.style = CS_HREDRAW | CS_VREDRAW;	//窗口类的风格
	

	//	2、注册窗口类
	RegisterClassEx(&wc);

	//	3、创建窗口
	HWND hWnd = CreateWindow(
		szAPPClassName,	//窗口类型名
		TEXT("别踩白块"),	//窗口的标题
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	//窗口的风格
		500, 200,	//窗口左上角横坐标
		4 * BLOCK + 17, 4 * BLOCK + 40,	//窗口的宽和高
		NULL,	//父窗口句柄
		NULL,	//菜单句柄
		hInstance,	//应用程序实例句柄
		NULL	//参数
	);

	//	4、显示窗口
	ShowWindow(hWnd, SW_SHOW);

	//	5、更新窗口
	UpdateWindow(hWnd);
	
	//	6、消息循环
	MSG msg;	//消息机制驱动运行
	while (GetMessage(&msg,NULL,0,0)) {
		//将虚拟键消息转换成字符消息
		TranslateMessage(&msg);
		//将消息分发给窗口处理函数
		DispatchMessage(&msg);
	}

	return 0;
}

//窗口处理函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC;	//绘图句柄，DC、GDI
	static int bw[4];	//每屏有四个黑块，从上往下数
	RECT rect;	//矩形区域
	HBRUSH hBrush;	//画刷句柄
	POINT point;	//鼠标点击的坐标
	static int n = 0;	//结束后踩的格子数量
	static int tm = 0;	//所耗时间
	CString str;	//提示框显示变量
	//QIcon icon;

	switch (uMsg) {
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));
		for (int i = 0; i < 4; i++) {
			bw[i] = rand() % 4;
		}
		//开启定时器
		SetTimer(hWnd, 1, 10, NULL);
	}
	break;
	case WM_PAINT:	//窗口绘图消息
		hDC = BeginPaint(hWnd, &ps);
		//画图
		//Rectangle(hDc, 0, 0, 100, 100);

		for (int i = 0; i < 5; i++) {
			//水平线
			MoveToEx(hDC, 0, i * BLOCK, NULL);
			LineTo(hDC, BLOCK * 4, i * BLOCK);

			//竖直
			MoveToEx(hDC, i * BLOCK, 0, NULL);
			LineTo(hDC, i * BLOCK, BLOCK * 4);

		}
		//画黑块
		for (int i = 0; i < 4; i++) {
			SetRect(&rect, bw[i] * BLOCK, i * BLOCK, bw[i] * BLOCK + BLOCK, i * BLOCK + BLOCK);
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC, hBrush);
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			//释放资源
			DeleteObject(hBrush);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:	//鼠标左键
	{
		str.Format(_T("游戏结束，您的分数为:%d\n"), n);
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		if (point.x / BLOCK != bw[3] || point.y <= BLOCK * 3) {
			MessageBox(hWnd, str, L"提示", MB_OK);
			exit(0);	//退出程序
			//getchar();
			//break;
		}

		//随机数
		for (int i = 3; i >= 1; i--) {
			bw[i] = bw[i - 1];
		}
		bw[0] = rand() % 4;
		n++;
		//滚动窗口
		ScrollWindow(hWnd, 0, BLOCK, NULL, NULL);
	}

	break;
	//case WM_RBUTTONDOWN:	//右键
	//	break;
	//case WM_KEYDOWN:	//按键asdf
	//	break;
	case WM_CLOSE:	//窗口关闭消息，关闭只是把窗口最小化，窗口可以恢复
		DestroyWindow(hWnd);	//销毁窗口，不可以恢复
		break;
	case WM_DESTROY:	//窗口销毁消息
		PostQuitMessage(0);		//退出进程
		break;
	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}