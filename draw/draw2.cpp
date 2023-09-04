// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <string>
#include <algorithm>

#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;
ULONGLONG ktory_tick;
bool w_gore = 1;
bool stop = 1;
bool w_ruchu = 0;
bool raz_dodane_pietro = false;


// buttons
HWND hwndButton;

// sent data
std::vector<int> na_ktore;

RECT drawArea1 = { 145, 0, 305, 800 };

//do wsiadania
RECT drawArea2 = { 20, 40, 140, 95 };

//do wysiadania
RECT drawArea_pietro4 = { 20, 40, 295, 95 };
RECT drawArea_pietro1 = { 152, 340, 450, 395 };
RECT drawArea_pietro3 = { 152, 140, 450, 195 };
RECT drawArea_pietro0 = { 20, 440, 295, 495 };
RECT drawArea_pietro2 = { 20, 240, 295, 295 };


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

class Osoba {
	private:
		int wspx = -1;
		int wspy = -1;
		int skad = -1;
		int dokad = -1;
	public:
		
		void Spawn(HWND, HDC&, PAINTSTRUCT&, RECT*, int ile_na_pietrze);
		void Idz(HWND, HDC&, PAINTSTRUCT&, RECT*, int);

		bool dodane_pietro_pasazera = false;
		bool czy_w_windzie = false;
		bool czy_wysiada = false;
		bool czy_wsiada = false;

 		int getWspx() const {
			return wspx;
		}
		int getWspy() const {return wspy;}
		int getSkad() const {return skad;}
		int getDokad() const {return dokad;}
		void setWsp(int x, int y) {
			wspx = x;
			wspy = y;
		}
		void setSkad(int skad1) {skad = skad1;}
		void setDokad(int dokad1) {dokad = dokad1;}
};
void Osoba::Idz(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, int w_windzie) {

	InvalidateRect(hWnd, drawArea, TRUE); //tutaj zmieni�em z TRUE na FALSE �eby nie miga�o, ale musia�em doda� "gumki"
	
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen pen(Color(0, 0, 0), 2);
	Pen pen1(Color(255, 255, 255), 2);
	
	//rysuje pasazerow
	graphics.DrawRectangle(&pen, getWspx(), getWspy(), 13, 35);

	//"gumki" pasa�er�w przy wsiadaniu/wysiadaniu
	graphics.DrawLine(&pen1, getWspx() - 2, getWspy()-1, getWspx() - 2, getWspy()+40);
	graphics.DrawLine(&pen1, getWspx() + 11, getWspy() + 1, getWspx() + 11, getWspy() + 34);
	graphics.DrawLine(&pen1, getWspx() + 2, getWspy() + 1, getWspx() + 2, getWspy() + 34);
	graphics.DrawLine(&pen1, getWspx() + 15, getWspy() - 1, getWspx() + 15, getWspy() + 40);

	EndPaint(hWnd, &ps);

}

void Osoba::Spawn(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, int ile_na_pietrze) {
	
	//ile_na_pietrze++; //tutaj zwiekszam, bo cos nie dzialalo jak bylo niezmienione

	InvalidateRect(hWnd, drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen pen(Color(0, 0, 0), 2);
	graphics.DrawRectangle(&pen, getWspx(), getWspy(), 13, 35); //110-i*30, rysuje na 4 pietrze odsuniete od siebie osoby (55 na tej wysokosc jest gora postaci na 4 pietrze)
	EndPaint(hWnd, &ps);									//trzeba bedzie dac tu jakies zmienne zamiast liczb, zeby nie tworzyc oddzielnych funkcji na kazde pietro

}
bool czy_zgodny_z_winda(int dokad, int skad, int w_gore)
{
	if(w_gore)
	{
		if(dokad > skad) return true;
		else return false;
	}
	else
	{
		if(dokad < skad) return true;
		else return false;
	}
}
void MyOnPaint(HDC hdc, int w_windzie, int czy_zajete[]) 
{
	Graphics graphics(hdc);
	Pen pen(Color(0, 0, 255), 4);
	Pen pen1(Color(0, 0, 0), 2);
	Pen pen2(Color(255, 255, 255), 6);
	Pen pen3(Color(255, 255, 255), 6);
	graphics.DrawRectangle(&pen, 150, value, 150, 100);

	for (int i = 0; i < 8; i++) 
	{
		//"gumki" pasa�er�w (nie wiem czy dzia�a jak jad� w g�r�)
		if(!w_gore)
		{
			graphics.DrawLine(&pen2, 154 + i*18, value + 86, 172 +i*18, value + 86);
			graphics.DrawLine(&pen3, 152, value + 51, 298, value + 51);
		}
		else if(w_gore)
		{
			graphics.DrawLine(&pen2, 154 + i*18, value + 55, 172 +i*18, value + 55);
			graphics.DrawLine(&pen3, 152, value + 94, 298, value + 94);
		}
		if(czy_zajete[i]) graphics.DrawRectangle(&pen1, 154 + i*18, value + 55, 13, 35);
	}
}
static void Paint(HWND hWnd, LPPAINTSTRUCT lpPS, HDC hdc, int w_windzie, int czy_zajete[])
{

	RECT rc;
	HDC hdcMem;
	HBITMAP hbmMem, hbmOld;
	HBRUSH hbrBkGnd;
	// Get the size of the client rectangle.
	GetClientRect(hWnd, &rc);
	// Create a compatible DC.
	hdcMem = CreateCompatibleDC(lpPS->hdc);
	// Create a bitmap big enough for our client rectangle.
	hbmMem = CreateCompatibleBitmap(lpPS->hdc, rc.right - rc.left, rc.bottom - rc.top);
	// Select the bitmap into the off-screen DC.
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
	// Erase the background.
	hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	FillRect(hdcMem, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);
	MyOnPaint(hdc, w_windzie, czy_zajete);
	// Drawing goes here.
	// HPEN hPenOld;
	// HPEN hLinePen;
	// COLORREF qLineColor;
	// qLineColor = RGB(255, 0, 0);
	// hLinePen = CreatePen(PS_SOLID, 7, qLineColor);
	// hPenOld = (HPEN)SelectObject(hdcMem, hLinePen);
	// SelectObject(hdcMem, hPenOld);
	// DeleteObject(hLinePen);

	BitBlt(lpPS->hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);
	// Done with off-screen bitmap and DC.
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}

void RamyWindy(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(0, 0, 0), 4);
	Pen pen1(Color(0, 0, 255), 4);
	graphics.DrawLine(&pen, 0, 100, 150, 100);
	graphics.DrawLine(&pen, 300, 200, 450, 200);
	graphics.DrawLine(&pen, 0, 300, 150, 300);
	graphics.DrawLine(&pen, 300, 400, 450, 400);
	graphics.DrawLine(&pen, 0, 500, 150, 500);
	graphics.DrawRectangle(&pen1, 150, value, 150, 100);
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea, int w_windzie, int czy_zajete[])
{
	
	TCHAR buffer[20];
	TCHAR buffer2[20];
	//int pietro = 4 - value / 100;
	int pietro;						
	if (na_ktore.size())
		pietro = na_ktore[0];
	else
		pietro = -1;
	_sntprintf_s(buffer, _countof(buffer), _T("%d"), pietro);

	w_windzie = 0;

	for (int i = 0; i < 8; i++)
		if(czy_zajete[i])
			w_windzie++;

	_sntprintf_s(buffer2, _countof(buffer2), _T("%d"), w_windzie*70);
	Pen pen1(Color(255, 255, 255), 2);

	InvalidateRect(hWnd, drawArea, FALSE); //tutaj zmieni�em z TRUE na FALSE �eby nie miga�o, ale musia�em doda� "gumki"
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc, w_windzie, czy_zajete);
	Graphics graphics(hdc);
	//tutaj s� gumki windy
	graphics.DrawLine(&pen1, 152, value + 3, 298, value + 3);
	graphics.DrawLine(&pen1, 147, value + 103, 303, value + 103);
	graphics.DrawLine(&pen1, 147, value - 3, 303, value - 3);
	graphics.DrawLine(&pen1, 152, value + 97, 298, value + 97);
	TextOut(hdc, 150, 10, buffer, _tcslen(buffer));
	TextOut(hdc, 150, 25, buffer2, _tcslen(buffer2));
	EndPaint(hWnd, &ps);
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value = 400;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("info_debug"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		450, 120,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON21,
		hInstance,
		NULL);

	// parter
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 480,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON1,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 460,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 440,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 420,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON4,
		hInstance,
		NULL);
	//pietro 1
		hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 380,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON5,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 360,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON6,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 340,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON7,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 320,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON8,
		hInstance,
		NULL);      
	//pietro 2
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 280,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON9,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 260,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON10,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 240,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON11,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 220,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON12,
		hInstance,
		NULL);
	//pietro 3
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 180,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON13,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 160,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON14,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 140,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON15,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 120,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON16,
		hInstance,
		NULL);
		//pietro 4
		hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 80,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON17,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 60,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON18,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 40,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON19,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 20,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON20,
		hInstance,
		NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	int static w_windzie = 0, pietro0 = 0, pietro1 = 0, pietro2 = 0, pietro3 = 0, pietro4 = 0;
	PAINTSTRUCT ps;
	int static czy_zajete[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	HDC hdc;
	std::vector<Osoba> static pasazer;
	Osoba pas;
	std::wstring outputString;
	static int wartosc1 = -1, wartosc2 = -1;
	bool poszedl = 1, wsiada;


	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		//ten przycisk u�ywa�em do wy�wietlania w okienku debugowania
		// case ID_BUTTON21:
		// 	if (na_ktore.size())
		// 		for (int i = 0;i < na_ktore.size();i++) {
		// 			outputString = std::to_wstring(na_ktore[i]) + L" ";
		// 			OutputDebugString(outputString.c_str());
		// 		}
			
		// 	outputString = L"Dlugosc " + std::to_wstring(na_ktore.size()) + L" ";
		// 	OutputDebugString(outputString.c_str());
		// 	outputString = L"\n";
		// 	OutputDebugString(outputString.c_str());
		// 	break;
		
		case ID_BUTTON1:
			pas.setWsp(130 - 17*pietro0, 455);
			pas.setSkad(0);
			pas.setDokad(1);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
			pietro0++;

			break;
		case ID_BUTTON2:
			pas.setWsp(130 - 17*pietro0, 455);
			pas.setSkad(0);
			pas.setDokad(2);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
			pietro0++;
			break;
		case ID_BUTTON3:
			pas.setWsp(130 - 17*pietro0, 455);
			pas.setSkad(0);
			pas.setDokad(3);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
			pietro0++;
			break;
		case ID_BUTTON4:
			pas.setWsp(130 - 17*pietro0, 455);
			pas.setSkad(0);
			pas.setDokad(4);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
			pietro0++;
			break;
		case ID_BUTTON5:
			pas.setWsp(320 + 17*pietro1, 355);
			pas.setSkad(1);
			pas.setDokad(0);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
			pietro1++;
			break;
		case ID_BUTTON6:
			pas.setWsp(320 + 17*pietro1, 355);
			pas.setSkad(1);
			pas.setDokad(2);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
			pietro1++;
			break;
		case ID_BUTTON7:
			pas.setWsp(320 + 17*pietro1, 355);
			pas.setSkad(1);
			pas.setDokad(3);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
			pietro1++;
			break;
		case ID_BUTTON8:
			pas.setWsp(320 + 17*pietro1, 355);
			pas.setSkad(1);
			pas.setDokad(4);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
			pietro1++;
			break;
		case ID_BUTTON9:
			pas.setWsp(130 - 17*pietro2, 255);
			pas.setSkad(2);
			pas.setDokad(0);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
			pietro2++;
			break;
		case ID_BUTTON10:
			pas.setWsp(130 - 17*pietro2, 255);
			pas.setSkad(2);
			pas.setDokad(1);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
			pietro2++;
			break;
		case ID_BUTTON11:
			pas.setWsp(130 - 17*pietro2, 255);
			pas.setSkad(2);
			pas.setDokad(3);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
			pietro2++;
			break;
		case ID_BUTTON12:
			pas.setWsp(130 - 17*pietro2, 255);
			pas.setSkad(2);
			pas.setDokad(4);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
			pietro2++;
			break;
		case ID_BUTTON13:
			pas.setWsp(320 + 17*pietro3, 155);
			pas.setSkad(3);
			pas.setDokad(0);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
			pietro3++;
			break;
		case ID_BUTTON14:
			pas.setWsp(320 + 17*pietro3, 155);
			pas.setSkad(3);
			pas.setDokad(1);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
			pietro3++;
			break;
		case ID_BUTTON15:
			pas.setWsp(320 + 17*pietro3, 155);
			pas.setSkad(3);
			pas.setDokad(2);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
			pietro3++;
			break;
		case ID_BUTTON16:
			pas.setWsp(320 + 17*pietro3, 155);
			pas.setSkad(3);
			pas.setDokad(4);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
			pietro3++;
			break;
		case ID_BUTTON17:
			pas.setWsp(130 - 17*pietro4, 55);
			pas.setSkad(4);
			pas.setDokad(0);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
			pietro4++;

			break;
		case ID_BUTTON18:
			pas.setWsp(130 - 17*pietro4, 55);
			pas.setSkad(4);
			pas.setDokad(1);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
			pietro4++;

			break;
		case ID_BUTTON19:
			pas.setWsp(130 - 17*pietro4, 55);
			pas.setSkad(4);
			pas.setDokad(2);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
			pietro4++;
			break;
		case ID_BUTTON20:
			pas.setWsp(130 - 17*pietro4, 55);
			pas.setSkad(4);
			pas.setDokad(3);
			pasazer.push_back(pas);
			pasazer[pasazer.size()-1].Spawn(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
			pietro4++;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		wartosc1 = 0;
		wartosc2 = 0;


		for (unsigned int i = 0; i < na_ktore.size(); i++) {
			if (na_ktore[i] == pas.getSkad()) {
				wartosc1++;
			}
			if (na_ktore[i] == pas.getDokad()) {
				wartosc2++;
			}
		}

		if(!wartosc1 && value != 400 - pas.getSkad()*100)
			na_ktore.push_back(pas.getSkad());
		if (!wartosc2)
			na_ktore.push_back(pas.getDokad());

		if (stop && value == 400 - pas.getSkad() * 100)
			ktory_tick = GetTickCount64();

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Paint(hWnd, &ps, hdc, w_windzie, czy_zajete);
		RamyWindy(hdc);
		TextOut(hdc, 100, 10, L"Pietro", 6);
		TextOut(hdc, 100, 25, L"Waga", 4);
		EndPaint(hWnd, &ps);
		stop = 1;
		//przy uruchomieniu programu od razu odpala si� timer
		SetTimer(hWnd, TMR_1, 10, 0);
		na_ktore.push_back(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
	case WM_ERASEBKGND: return (LRESULT)1; // Say we handled it.

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			
			if (na_ktore.size()){
				//jesli winda dojedzie na docelowe miejsce, zatrzymuje sie
				if (400 - value == 100 * na_ktore[0]) {
					stop = 1;
					w_ruchu = 0;
					raz_dodane_pietro = false;
					ktory_tick = GetTickCount64();
					na_ktore.erase(na_ktore.begin());
				}
				if (w_ruchu)
					for (unsigned int i = 0; i < pasazer.size(); i++) {
						if (400 - 100 * pasazer[i].getDokad() == value && pasazer[i].czy_w_windzie)
						{
							stop = 1;
							w_ruchu = 0;
							ktory_tick = GetTickCount64();
							for (unsigned int i = 0; i < na_ktore.size(); i++)
								if (value == 400 - 100 * na_ktore.at(i))
									na_ktore.erase(na_ktore.begin() + i);
						}
						if (w_windzie < 8 && (400 - 100 * pasazer[i].getSkad() == value && value == 400 && pietro0
							|| 400 - 100 * pasazer[i].getSkad() == value && value == 300 && pietro1
							|| 400 - 100 * pasazer[i].getSkad() == value && value == 200 && pietro2
							|| 400 - 100 * pasazer[i].getSkad() == value && value == 100 && pietro3
							|| 400 - 100 * pasazer[i].getSkad() == value && value == 0 && pietro4))
						{
							stop = 1;
							w_ruchu = 0;
							ktory_tick = GetTickCount64();
							for (unsigned int i = 0; i < na_ktore.size(); i++)
								if (value == 400 - 100 * na_ktore.at(i))
									na_ktore.erase(na_ktore.begin() + i);
						}
					}

			}
			
			/*for (int i = 0;i < pasazer.size();i++) {
				outputString = std::to_wstring(pasazer[i].getWspx()) + L"\n";
				OutputDebugString(outputString.c_str());
			}*/

			//outputString = std::to_wstring(w_windzie) + L"\n"; //to jest do wy�wietlania przy debugowaniu - klikasz lokalny debuger 
			//OutputDebugString(outputString.c_str());           //i to wy�wietla si� z prawej na dole

			if (na_ktore.size()) {
				for (unsigned int i = 0; i < na_ktore.size();i++) {
					outputString = std::to_wstring(na_ktore[i]) + L" ";
					OutputDebugString(outputString.c_str());
				}
				outputString = L"\n";
				OutputDebugString(outputString.c_str());
			}

			/*for (int i = 0; i < 8; i++) {
				outputString = std::to_wstring(czy_zajete[i]) + L" ";
				OutputDebugString(outputString.c_str());
			}
			outputString = L"\n";
			OutputDebugString(outputString.c_str());*/

			/*outputString = L"Dlugosc " + std::to_wstring(pasazer.size()) + L" ";
			OutputDebugString(outputString.c_str());
			outputString = L"\n";
			OutputDebugString(outputString.c_str());*/
			wsiada = 0;
			//wsiadanie do windy (na 4 pi�trze)
			if (pasazer.size())
			{
				int ile_moze_wsiasc = 0, ile_wolnych = 0;
				for(int i = 0; i < 8; i++)
					if(!czy_zajete[i]) ile_wolnych++;
				
				//dop�ki pasa�er, kt�ry wchodzi pierwszy nie osi�gnie x = 265 to jego x zwi�szka si� o 1 (setWsp) i wy�wietla si� zwi�kszony o 1 (Idz)
				
				
				if (value == 0 && stop == 1 && pietro4) 
				{
					wsiada = 1;
					//dodałem jeden bo nie wiem czemu cos nie dzialalo 
					if(ile_wolnych >= pietro4) ile_moze_wsiasc = pietro4;
					else ile_moze_wsiasc = ile_wolnych;
					bool podchodzenie = true;
							for(unsigned int i = 0; i < pasazer.size(); i++)
							{
								if(!pasazer[i].czy_w_windzie && !pasazer[i].czy_wsiada)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
								if(pasazer[i].czy_w_windzie)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 4 && ile_moze_wsiasc != 0)
								{
									pasazer[i].czy_wsiada = true;
									pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
									ile_moze_wsiasc--;
									if(pasazer[i].getWspx() == 154 && !czy_zajete[0]) 
									{	
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[0] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
									else if(pasazer[i].getWspx() == 172 && !czy_zajete[1]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[1] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
									else if(pasazer[i].getWspx() == 190 && !czy_zajete[2]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[2] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
									else if(pasazer[i].getWspx() == 208 && !czy_zajete[3]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[3] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
									else if(pasazer[i].getWspx() == 226 && !czy_zajete[4]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[4] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
									else if(pasazer[i].getWspx() == 244 && !czy_zajete[5]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[5] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
									else if(pasazer[i].getWspx() == 262 && !czy_zajete[6]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[6] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
									else if(pasazer[i].getWspx() == 280 && !czy_zajete[7]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[7] = true;
										pasazer[i].czy_w_windzie = true;
										pietro4--;
									}
								}
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 4 && !pasazer[i].czy_wsiada)
								{
									
									if(pasazer[i].getWspx() == 130) podchodzenie = false;
									if(podchodzenie)
									{
										pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
										pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
									}
									
								}
							}		
				}
				ile_wolnych = 0;
				for(int i = 0; i < 8; i++)
					if(!czy_zajete[i]) ile_wolnych++;
				
				if (value == 100 && stop == 1 && pietro3) {
					wsiada = 1;
					if(ile_wolnych >= pietro3) ile_moze_wsiasc = pietro3;
					else ile_moze_wsiasc = ile_wolnych;
					bool podchodzenie = true;
							for(unsigned int i = 0; i < pasazer.size(); i++)
							{
								if(!pasazer[i].czy_w_windzie && !pasazer[i].czy_wsiada)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
								if(pasazer[i].czy_w_windzie)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 3 && ile_moze_wsiasc != 0)
								{
									pasazer[i].czy_wsiada = true;
									pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
									ile_moze_wsiasc--;
									if(pasazer[i].getWspx() == 154 && !czy_zajete[0]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[0] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
									else if(pasazer[i].getWspx() == 172 && !czy_zajete[1]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[1] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
									else if(pasazer[i].getWspx() == 190 && !czy_zajete[2]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[2] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
									else if(pasazer[i].getWspx() == 208 && !czy_zajete[3]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[3] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
									else if(pasazer[i].getWspx() == 226 && !czy_zajete[4]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[4] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
									else if(pasazer[i].getWspx() == 244 && !czy_zajete[5]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[5] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
									else if(pasazer[i].getWspx() == 262 && !czy_zajete[6]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[6] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
									else if(pasazer[i].getWspx() == 280 && !czy_zajete[7]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[7] = true;
										pasazer[i].czy_w_windzie = true;
										pietro3--;
									}
								}
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 3 && !pasazer[i].czy_wsiada)
								{
									if(pasazer[i].getWspx() == 320) podchodzenie = false;
									if(podchodzenie)
									{
										pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
										pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro3, pietro3);
									}
									
								}
							}						
				}
				ile_wolnych = 0;
				for(int i = 0; i < 8; i++)
					if(!czy_zajete[i]) ile_wolnych++;

				if (value == 200 && stop == 1 && pietro2) {
					wsiada = 1;
					if(ile_wolnych >= pietro2) ile_moze_wsiasc = pietro2;
					else ile_moze_wsiasc = ile_wolnych;
					bool podchodzenie = true;
							for(unsigned int i = 0; i < pasazer.size(); i++)
							{
								if(!pasazer[i].czy_w_windzie && !pasazer[i].czy_wsiada)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
								if(pasazer[i].czy_w_windzie)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 2 && ile_moze_wsiasc != 0)
								{
									pasazer[i].czy_wsiada = true;
									pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
									ile_moze_wsiasc--;
									if(pasazer[i].getWspx() == 154 && !czy_zajete[0]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[0] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
									else if(pasazer[i].getWspx() == 172 && !czy_zajete[1]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[1] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
									else if(pasazer[i].getWspx() == 190 && !czy_zajete[2]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[2] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
									else if(pasazer[i].getWspx() == 208 && !czy_zajete[3]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[3] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
									else if(pasazer[i].getWspx() == 226 && !czy_zajete[4]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[4] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
									else if(pasazer[i].getWspx() == 244 && !czy_zajete[5]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[5] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
									else if(pasazer[i].getWspx() == 262 && !czy_zajete[6]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[6] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
									else if(pasazer[i].getWspx() == 280 && !czy_zajete[7]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[7] = true;
										pasazer[i].czy_w_windzie = true;
										pietro2--;
									}
								}
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 2 && !pasazer[i].czy_wsiada)
								{
									if(pasazer[i].getWspx() == 130) podchodzenie = false;
									if(podchodzenie)
									{
										pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
										pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro2, pietro2);
									}
									
								}
							}							
				}
				ile_wolnych = 0;
				for(int i = 0; i < 8; i++)
					if(!czy_zajete[i]) ile_wolnych++;
				
				if (value == 300 && stop == 1 && pietro1) {
					wsiada = 1;
					if(ile_wolnych >= pietro1) ile_moze_wsiasc = pietro1;
					else ile_moze_wsiasc = ile_wolnych;
					bool podchodzenie = true;
							for(unsigned int i = 0; i < pasazer.size(); i++)
							{
								if(!pasazer[i].czy_w_windzie && !pasazer[i].czy_wsiada)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
								if(pasazer[i].czy_w_windzie)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 1 && ile_moze_wsiasc != 0)
								{
									pasazer[i].czy_wsiada = true;
									pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
									ile_moze_wsiasc--;
									if(pasazer[i].getWspx() == 154 && !czy_zajete[0]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[0] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
									else if(pasazer[i].getWspx() == 172 && !czy_zajete[1]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[1] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
									else if(pasazer[i].getWspx() == 190 && !czy_zajete[2]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[2] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
									else if(pasazer[i].getWspx() == 208 && !czy_zajete[3]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[3] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
									else if(pasazer[i].getWspx() == 226 && !czy_zajete[4]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[4] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
									else if(pasazer[i].getWspx() == 244 && !czy_zajete[5]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[5] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
									else if(pasazer[i].getWspx() == 262 && !czy_zajete[6]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[6] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
									else if(pasazer[i].getWspx() == 280 && !czy_zajete[7]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[7] = true;
										pasazer[i].czy_w_windzie = true;
										pietro1--;
									}
								}
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 1 && !pasazer[i].czy_wsiada)
								{
									if(pasazer[i].getWspx() == 320) podchodzenie = false;
									if(podchodzenie)
									{
										pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
										pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro1, pietro1);
									}
									
								}
							}														
				}
				ile_wolnych = 0;
				for(int i = 0; i < 8; i++)
					if(!czy_zajete[i]) ile_wolnych++;

				if (value == 400 && stop == 1 && pietro0) {
					wsiada = 1;
					if(ile_wolnych >= pietro0) ile_moze_wsiasc = pietro0;
					else ile_moze_wsiasc = ile_wolnych;
					bool podchodzenie = true;
							for(unsigned int i = 0; i < pasazer.size(); i++)
							{
								if(!pasazer[i].czy_w_windzie && !pasazer[i].czy_wsiada)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
								if(pasazer[i].czy_w_windzie)
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 0 && ile_moze_wsiasc != 0)
								{
									pasazer[i].czy_wsiada = true;
									pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
									pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
									ile_moze_wsiasc--;
									if(pasazer[i].getWspx() == 154 && !czy_zajete[0]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[0] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
									else if(pasazer[i].getWspx() == 172 && !czy_zajete[1]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[1] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
									else if(pasazer[i].getWspx() == 190 && !czy_zajete[2]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[2] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
									else if(pasazer[i].getWspx() == 208 && !czy_zajete[3]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[3] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
									else if(pasazer[i].getWspx() == 226 && !czy_zajete[4]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[4] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
									else if(pasazer[i].getWspx() == 244 && !czy_zajete[5]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[5] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
									else if(pasazer[i].getWspx() == 262 && !czy_zajete[6]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[6] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
									else if(pasazer[i].getWspx() == 280 && !czy_zajete[7]) 
									{
										if(pasazer[i].dodane_pietro_pasazera) na_ktore.push_back(pasazer[i].getDokad());
										czy_zajete[7] = true;
										pasazer[i].czy_w_windzie = true;
										pietro0--;
									}
								}
								if(!pasazer[i].czy_w_windzie && pasazer[i].getSkad() == 0 && !pasazer[i].czy_wsiada)
								{
									if(pasazer[i].getWspx() == 130) podchodzenie = false;
									if(podchodzenie)
									{
										pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
										pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro0, pietro0);
									}
									
								}
							}							
				}
			}
			if(stop && !raz_dodane_pietro)
			{
				if(value == 0)
				{
					for(int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].getSkad() == 4 && !pasazer[i].czy_wsiada && !raz_dodane_pietro)
						{
							raz_dodane_pietro = true;
							na_ktore.push_back(4);
							for(int j = 0; j < pasazer.size(); j++)
							{
								if(pasazer[i].getSkad() == 4 && !pasazer[i].czy_wsiada && !pasazer[i].dodane_pietro_pasazera)
								{
									pasazer[i].dodane_pietro_pasazera = true;
								}
							}
						}
					}
				}
				if(value == 100)
				{
					for(int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].getSkad() == 3 && !pasazer[i].czy_wsiada && !raz_dodane_pietro)
						{
							raz_dodane_pietro = true;
							na_ktore.push_back(3);
							for(int j = 0; j < pasazer.size(); j++)
							{
								if(pasazer[i].getSkad() == 3 && !pasazer[i].czy_wsiada && !pasazer[i].dodane_pietro_pasazera)
								{
									pasazer[i].dodane_pietro_pasazera = true;
								}
							}
						}
					}
				}
				if(value == 200)
				{
					for(int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].getSkad() == 2 && !pasazer[i].czy_wsiada && !raz_dodane_pietro)
						{
							raz_dodane_pietro = true;
							na_ktore.push_back(2);
							for(int j = 0; j < pasazer.size(); j++)
							{
								if(pasazer[i].getSkad() == 2 && !pasazer[i].czy_wsiada && !pasazer[i].dodane_pietro_pasazera)
								{
									pasazer[i].dodane_pietro_pasazera = true;
								}
							}
						}
					}
				}
				if(value == 300)
				{
					for(int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].getSkad() == 1 && !pasazer[i].czy_wsiada && !raz_dodane_pietro)
						{
							raz_dodane_pietro = true;
							na_ktore.push_back(1);
							for(int j = 0; j < pasazer.size(); j++)
							{
								if(pasazer[i].getSkad() == 1 && !pasazer[i].czy_wsiada && !pasazer[i].dodane_pietro_pasazera)
								{
									pasazer[i].dodane_pietro_pasazera = true;
								}
							}
						}
					}
				}
				if(value == 0)
				{
					for(int i = 400; i < pasazer.size(); i++)
					{
						if(pasazer[i].getSkad() == 0 && !pasazer[i].czy_wsiada && !raz_dodane_pietro)
						{
							raz_dodane_pietro = true;
							na_ktore.push_back(0);
							for(int j = 0; j < pasazer.size(); j++)
							{
								if(pasazer[i].getSkad() == 0 && !pasazer[i].czy_wsiada && !pasazer[i].dodane_pietro_pasazera)
								{
									pasazer[i].dodane_pietro_pasazera = true;
								}
							}
						}
					}
				}
			}
			w_windzie = 0;
			for(int i = 0; i < 8; i++)
				if(czy_zajete[i]) w_windzie++;
			if (w_windzie == 8)
				wsiada = 0;
			int static buf = 0; //do jednorazowego zaktualizowania pozycji
			
			//wysiadanie
			if (pasazer.size()) {
				if (value == 100 && stop == 1) 
				{ //pietro3
					//tutaj ta aktualizacja pozycji - musi by�, bo funkcja wy�wietlaj�ca wind� z pasa�erami nie aktualizuje pozycji pasa�er�w
					if (!buf) { 
						for (unsigned int i = 0; i < pasazer.size(); i++)
						{
							if(pasazer[i].czy_w_windzie)
								pasazer[i].setWsp(pasazer[i].getWspx(), value + 55);
							buf = 1;
						}
					}
					
					for (unsigned int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].czy_w_windzie) pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro3, w_windzie);
						if(pasazer[i].czy_w_windzie && pasazer[i].getDokad() == 3 && !pasazer[i].czy_wysiada)
						{
							if(pasazer[i].getWspx() == 154) 
							{ 
								czy_zajete[0] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 172) { 
								czy_zajete[1] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 190) { 
								czy_zajete[2] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 208) { 
								czy_zajete[3] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 226) { 
								czy_zajete[4] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 244) { 
								czy_zajete[5] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 262) { 
								czy_zajete[6] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 280) { 
								czy_zajete[7] = false;
								pasazer[i].czy_wysiada = true;
							}
						}
						if(pasazer[i].czy_wysiada){
							pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
							pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro3, w_windzie);
						}
						if(pasazer[i].getWspx() > 450 && pasazer[i].czy_wysiada){
							pasazer[i].czy_w_windzie = false;
							pasazer[i].czy_wysiada = false;
							pasazer.erase(pasazer.begin() + i);
						}
					}
					//break;
				}
				//to samo tylko dla kolejnych pi�ter
				if (value == 300 && stop == 1) { //pietro1
					if (!buf) {
						for (unsigned int i = 0; i < pasazer.size(); i++)
						{
							if(pasazer[i].czy_w_windzie)
								pasazer[i].setWsp(pasazer[i].getWspx(), value + 55);
							buf = 1;
						}
					}
					for (unsigned int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].czy_w_windzie) pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro1, w_windzie);
						if(pasazer[i].czy_w_windzie && pasazer[i].getDokad() == 1 && !pasazer[i].czy_wysiada)
						{
							if(pasazer[i].getWspx() == 154) 
							{ 
								czy_zajete[0] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 172) { 
								czy_zajete[1] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 190) { 
								czy_zajete[2] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 208) { 
								czy_zajete[3] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 226) { 
								czy_zajete[4] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 244) { 
								czy_zajete[5] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 262) { 
								czy_zajete[6] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 280) { 
								czy_zajete[7] = false;
								pasazer[i].czy_wysiada = true;
							}
						}
						if(pasazer[i].czy_wysiada){
							pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
							pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro1, w_windzie);
						}
						if(pasazer[i].getWspx() > 450 && pasazer[i].czy_wysiada){
							pasazer[i].czy_w_windzie = false;
							pasazer[i].czy_wysiada = false;
							pasazer.erase(pasazer.begin() + i);
						}
					}

					//break;
				}
				if (value == 200 && stop == 1) { //pietro2
					if (!buf) { 
						for (unsigned int i = 0; i < pasazer.size(); i++)
						{
							if(pasazer[i].czy_w_windzie)
								pasazer[i].setWsp(pasazer[i].getWspx(), value + 55);
							buf = 1;
						}
					}
					for (unsigned int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].czy_w_windzie) pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro2, w_windzie);
						if(pasazer[i].czy_w_windzie && pasazer[i].getDokad() == 2 && !pasazer[i].czy_wysiada)
						{
							if(pasazer[i].getWspx() == 154) 
							{ 
								czy_zajete[0] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 172) { 
								czy_zajete[1] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 190) { 
								czy_zajete[2] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 208) { 
								czy_zajete[3] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 226) { 
								czy_zajete[4] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 244) { 
								czy_zajete[5] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 262) { 
								czy_zajete[6] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 280) { 
								czy_zajete[7] = false;
								pasazer[i].czy_wysiada = true;
							}
						}
						if(pasazer[i].czy_wysiada){
							pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
							pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro2, w_windzie);
						}
						if(pasazer[i].getWspx() < 3 && pasazer[i].czy_wysiada){
							pasazer[i].czy_w_windzie = false;
							pasazer[i].czy_wysiada = false;
							pasazer.erase(pasazer.begin() + i);
						}
					}
					//break;
				}
				if (value == 400 && stop == 1) { //pietro0
					if (!buf) { 
						for (unsigned int i = 0; i < pasazer.size(); i++)
						{
							if(pasazer[i].czy_w_windzie)
								pasazer[i].setWsp(pasazer[i].getWspx(), value + 55);
							buf = 1;
						}
					}
					for (unsigned int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].czy_w_windzie) pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro0, w_windzie);
						if(pasazer[i].czy_w_windzie && pasazer[i].getDokad() == 0 && !pasazer[i].czy_wysiada)
						{
							if(pasazer[i].getWspx() == 154) 
							{ 
								czy_zajete[0] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 172) { 
								czy_zajete[1] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 190) { 
								czy_zajete[2] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 208) { 
								czy_zajete[3] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 226) { 
								czy_zajete[4] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 244) { 
								czy_zajete[5] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 262) { 
								czy_zajete[6] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 280) { 
								czy_zajete[7] = false;
								pasazer[i].czy_wysiada = true;
							}
						}
						if(pasazer[i].czy_wysiada){
							pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
							pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro0, w_windzie);
						}
						if(pasazer[i].getWspx() < 3 && pasazer[i].czy_wysiada){
							pasazer[i].czy_w_windzie = false;
							pasazer[i].czy_wysiada = false;
							pasazer.erase(pasazer.begin() + i);
						}
					}
				}
				if (value == 0 && stop == 1) { //pietro4
					if (!buf) { 
						for (unsigned int i = 0; i < pasazer.size(); i++)
						{
							if(pasazer[i].czy_w_windzie)
								pasazer[i].setWsp(pasazer[i].getWspx(), value + 55);
							buf = 1;
						}
					}
					for (unsigned int i = 0; i < pasazer.size(); i++)
					{
						if(pasazer[i].czy_w_windzie) pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro4, w_windzie);
						if(pasazer[i].czy_w_windzie && pasazer[i].getDokad() == 4 && !pasazer[i].czy_wysiada) 
						{
							if(pasazer[i].getWspx() == 154) 
							{ 
								czy_zajete[0] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 172) { 
								czy_zajete[1] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 190) { 
								czy_zajete[2] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 208) { 
								czy_zajete[3] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 226) { 
								czy_zajete[4] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 244) { 
								czy_zajete[5] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 262) { 
								czy_zajete[6] = false;
								pasazer[i].czy_wysiada = true;
							}
							else if(pasazer[i].getWspx() == 280) { 
								czy_zajete[7] = false;
								pasazer[i].czy_wysiada = true;
							}
						}
						if(pasazer[i].czy_wysiada){
								pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
								pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro4, w_windzie);
							}
							if(pasazer[i].getWspx() < 3 && pasazer[i].czy_wysiada)
							{
								pasazer[i].czy_w_windzie = false;
								pasazer[i].czy_wysiada = false;
								pasazer.erase(pasazer.begin() + i);
							}
					}
				}
			}
			
			poszedl = 1;

			for (unsigned int i = 0; i < pasazer.size(); i++)
				if (pasazer[i].czy_wysiada) {
					poszedl = 0;
					break;
				}
			if (wsiada)
				poszedl = 0;

			//je�li winda stoi, to ruszy po 3 sekundach je�li b�dzie mia�a kolejne pi�tro na kt�re ma jecha�
			if (GetTickCount64() - ktory_tick >= 3000 && !w_ruchu && na_ktore.size() && poszedl) {
				if (na_ktore[0] * 100 > 400 - value)
					w_gore = 1;
				if (na_ktore[0] * 100 < 400 - value)
					w_gore = 0;
				stop = 0;
				w_ruchu = 1;
			}
			bool czy_pusta;
			for(int i = 0; i < 8; i++)
			{
				if(!czy_zajete[i]) czy_pusta = true;
				else 
				{
					czy_pusta = false;
				}
			}

			bool dodaj = false;
			int kolejne_pietro = 0;

			int ile_wolnych = 0;
			for(int i = 0; i < 8; i++)
				if(!czy_zajete[i]) ile_wolnych++;

			//to jest zeby jak winda stoi, to nie aktualizowac okienka, zeby nie migalo
			if (stop)
				break;
			//tutaj to zmieniam, bo po tym jak winda ruszy, czyli stop = 0 (nie bedzie tego break wyzej)
			buf = 0;

			if (!w_gore)
				value++;
			else
				value--;
			
			repaintWindow(hWnd, hdc, ps, &drawArea1, w_windzie, czy_zajete);
			
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
