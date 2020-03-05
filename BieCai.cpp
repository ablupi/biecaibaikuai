#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <cstring>
#include <atlstr.h>
#include "Resource.h"
//#include <qt.h>

#define BLOCK 100
//#define MAKEINTRESOURCE MAKEINTRESOURCEW

//main�ǿ���̨Ӧ�ó���
//WinMain��GUI����Ӧ�ó���
//WINAPI�������ĵ���Լ��
//HWND�����ھ������	ID
//UINT��unsinged int

//���ڴ�����
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	//һ����һ������
	//	1����ƴ�����
	TCHAR szAPPClassName[] = TEXT("DunKaiGuiFoundation");

	WNDCLASSEX wc;
	wc.cbClsExtra = 0;	//��������չ�ռ��С
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;	//���ڵ���չ�ռ��С
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ɫ������Ϊ��ɫ
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//���ع�ꣿ���岻��
	//wc.hIcon = LoadIcon(NULL, IDI_BieCaiBaikuai);
	wc.hIcon = NULL;
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BIECAIBAIKUAI));
	//wc.hIconSm = LoadIcon(hInstance, IDI_BieCaiBaikuai);
	//wc.hIconSm = LoadIcon(NULL, IDI_BIECAIBAIKUAI);
	wc.hInstance = hInstance;	//Ӧ�ó���ʵ�����
	wc.lpfnWndProc = WindowProc;	//���ڴ���������ҳ
	wc.lpszClassName = szAPPClassName;	//�������ͺ�
	wc.lpszMenuName = NULL;	//�˵���
	wc.style = CS_HREDRAW | CS_VREDRAW;	//������ķ��
	

	//	2��ע�ᴰ����
	RegisterClassEx(&wc);

	//	3����������
	HWND hWnd = CreateWindow(
		szAPPClassName,	//����������
		TEXT("��Ȱ׿�"),	//���ڵı���
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	//���ڵķ��
		500, 200,	//�������ϽǺ�����
		4 * BLOCK + 17, 4 * BLOCK + 40,	//���ڵĿ�͸�
		NULL,	//�����ھ��
		NULL,	//�˵����
		hInstance,	//Ӧ�ó���ʵ�����
		NULL	//����
	);

	//	4����ʾ����
	ShowWindow(hWnd, SW_SHOW);

	//	5�����´���
	UpdateWindow(hWnd);
	
	//	6����Ϣѭ��
	MSG msg;	//��Ϣ������������
	while (GetMessage(&msg,NULL,0,0)) {
		//���������Ϣת�����ַ���Ϣ
		TranslateMessage(&msg);
		//����Ϣ�ַ������ڴ�����
		DispatchMessage(&msg);
	}

	return 0;
}

//���ڴ�����
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC;	//��ͼ�����DC��GDI
	static int bw[4];	//ÿ�����ĸ��ڿ飬����������
	RECT rect;	//��������
	HBRUSH hBrush;	//��ˢ���
	POINT point;	//�����������
	static int n = 0;	//������ȵĸ�������
	static int tm = 0;	//����ʱ��
	CString str;	//��ʾ����ʾ����
	//QIcon icon;

	switch (uMsg) {
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));
		for (int i = 0; i < 4; i++) {
			bw[i] = rand() % 4;
		}
		//������ʱ��
		SetTimer(hWnd, 1, 10, NULL);
	}
	break;
	case WM_PAINT:	//���ڻ�ͼ��Ϣ
		hDC = BeginPaint(hWnd, &ps);
		//��ͼ
		//Rectangle(hDc, 0, 0, 100, 100);

		for (int i = 0; i < 5; i++) {
			//ˮƽ��
			MoveToEx(hDC, 0, i * BLOCK, NULL);
			LineTo(hDC, BLOCK * 4, i * BLOCK);

			//��ֱ
			MoveToEx(hDC, i * BLOCK, 0, NULL);
			LineTo(hDC, i * BLOCK, BLOCK * 4);

		}
		//���ڿ�
		for (int i = 0; i < 4; i++) {
			SetRect(&rect, bw[i] * BLOCK, i * BLOCK, bw[i] * BLOCK + BLOCK, i * BLOCK + BLOCK);
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC, hBrush);
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			//�ͷ���Դ
			DeleteObject(hBrush);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:	//������
	{
		str.Format(_T("��Ϸ���������ķ���Ϊ:%d\n"), n);
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		if (point.x / BLOCK != bw[3] || point.y <= BLOCK * 3) {
			MessageBox(hWnd, str, L"��ʾ", MB_OK);
			exit(0);	//�˳�����
			//getchar();
			//break;
		}

		//�����
		for (int i = 3; i >= 1; i--) {
			bw[i] = bw[i - 1];
		}
		bw[0] = rand() % 4;
		n++;
		//��������
		ScrollWindow(hWnd, 0, BLOCK, NULL, NULL);
	}

	break;
	//case WM_RBUTTONDOWN:	//�Ҽ�
	//	break;
	//case WM_KEYDOWN:	//����asdf
	//	break;
	case WM_CLOSE:	//���ڹر���Ϣ���ر�ֻ�ǰѴ�����С�������ڿ��Իָ�
		DestroyWindow(hWnd);	//���ٴ��ڣ������Իָ�
		break;
	case WM_DESTROY:	//����������Ϣ
		PostQuitMessage(0);		//�˳�����
		break;
	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}