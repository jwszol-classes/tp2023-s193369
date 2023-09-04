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
bool w_gore = 0;
bool stop = 0;
bool w_ruchu = 0;


// buttons
HWND hwndButton;

// sent data
std::vector<int> na_ktore;

RECT drawArea1 = { 145, 0, 305, 800 };

//do wsiadania
RECT drawArea2 = { 20, 40, 140, 95 };

//do wysiadania
RECT drawArea_pietro4 = { 20, 40, 295, 95 };
RECT drawArea_pietro1 = { 160, 340, 450, 395 };
RECT drawArea_pietro3 = { 160, 140, 450, 195 };
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
		int wspx;
		int wspy;
		int skad;
		int dokad;
	public:
		
		void Spawn(HWND, HDC&, PAINTSTRUCT&, RECT*, int);
		void Idz(HWND, HDC&, PAINTSTRUCT&, RECT*, int);

		int getWspx() const {
			return wspx;
		}

		int getWspy() const {
			return wspy;
		}

		int getSkad() const {
			return skad;
		}

		int getDokad() const {
			return dokad;
		}

		void setWsp(int x, int y) {
			wspx = x;
			wspy = y;
		}

		void setSkad(int skad1) {
			skad = skad1;
		}

		void setDokad(int dokad1) {
			dokad = dokad1;
		}
};

void Osoba::Idz(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, int w_windzie) {

	InvalidateRect(hWnd, drawArea, FALSE); //tutaj zmieni³em z TRUE na FALSE ¿eby nie miga³o, ale musia³em dodaæ "gumki"
	
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen pen(Color(0, 0, 0), 2);
	Pen pen1(Color(255, 255, 255), 2);
	
	//rysuje pasa¿erów
	graphics.DrawRectangle(&pen, getWspx(), getWspy(), 25, 35);

	//"gumki" pasa¿erów przy wsiadaniu/wysiadaniu
	graphics.DrawLine(&pen1, getWspx() - 2, getWspy()-1, getWspx() - 2, getWspy()+40);
	graphics.DrawLine(&pen1, getWspx() + 23, getWspy() + 1, getWspx() + 23, getWspy() + 34);
	graphics.DrawLine(&pen1, getWspx() + 2, getWspy() + 1, getWspx() + 2, getWspy() + 34);
	graphics.DrawLine(&pen1, getWspx() + 27, getWspy() - 1, getWspx() + 27, getWspy() + 40);

	EndPaint(hWnd, &ps);

}

void Osoba::Spawn(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, int ile_czeka) {
	
	ile_czeka++; //tutaj zwiekszam, bo cos nie dzialalo jak bylo niezmienione

	InvalidateRect(hWnd, drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen pen(Color(0, 0, 0), 2);
	for (int i = 0; i < ile_czeka; i++)
		graphics.DrawRectangle(&pen, 110 - i*30 , 55, 25, 35); //110-i*30, rysuje na 4 pietrze odsuniete od siebie osoby (55 na tej wysokosc jest gora postaci na 4 pietrze)
	EndPaint(hWnd, &ps);									//trzeba bedzie dac tu jakies zmienne zamiast liczb, zeby nie tworzyc oddzielnych funkcji na kazde pietro

}

void MyOnPaint(HDC hdc, int w_windzie) 
{
	Graphics graphics(hdc);
	Pen pen(Color(0, 0, 255), 4);
	Pen pen1(Color(0, 0, 0), 2);
	Pen pen2(Color(255, 255, 255), 2);
	graphics.DrawRectangle(&pen, 150, value, 150, 100);

	for (int i = 0; i < w_windzie; i++) {
		//"gumki" pasa¿erów (nie wiem czy dzia³a jak jad¹ w górê)
		graphics.DrawRectangle(&pen1, 265 - i * 30, value + 55, 25, 35);
		graphics.DrawRectangle(&pen2, 267 - i * 30, value + 57, 21, 31);
		//graphics.DrawLine(&pen1, getWspx() + 27, getWspy() - 1, getWspx() + 27, getWspy() + 40);
		graphics.DrawLine(&pen2, 263 - i * 30, value + 53, 267 - i * 30 + 25, value + 53);
	}
	

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

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea, int w_windzie)
{
	
	TCHAR buffer[20];
	//int pietro = 4 - value / 100;
	int pietro;						
	if (na_ktore.size())
		pietro = na_ktore[0];
	else
		pietro = -1;
	_sntprintf_s(buffer, _countof(buffer), _T("%d"), na_ktore[0]);

	Pen pen1(Color(255, 255, 255), 2);

	InvalidateRect(hWnd, drawArea, FALSE); //tutaj zmieni³em z TRUE na FALSE ¿eby nie miga³o, ale musia³em dodaæ "gumki"
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc, w_windzie);
	Graphics graphics(hdc);
	//tutaj s¹ gumki windy
	graphics.DrawLine(&pen1, 152, value + 3, 298, value + 3);
	graphics.DrawLine(&pen1, 147, value + 103, 303, value + 103);
	graphics.DrawLine(&pen1, 147, value - 3, 303, value - 3);
	graphics.DrawLine(&pen1, 152, value + 97, 298, value + 97);
	TextOut(hdc, 150, 10, buffer, _tcslen(buffer));
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
		(HMENU)ID_BUTTON3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON7,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 20,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON8,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 40,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON9,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 60,
		20, 20,
		hWnd,
		(HMENU)ID_BUTTON10,
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
	int static w_windzie = 0, pietro4 = 0;
	PAINTSTRUCT ps;
	HDC hdc;
	std::vector<Osoba> static pasazer;
	Osoba pas;

	std::wstring outputString;


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
		/*case ID_BUTTON2:
			value = 0;
			w_ruchu = 0;
			stop = 1;
			SetTimer(hWnd, TMR_1, 10, 0);
			repaintWindow(hWnd, hdc, ps, &drawArea1, w_windzie);
			break;
		//ten przycisk u¿ywa³em do wyœwietlania w okienku debugowania
		case ID_BUTTON3:
			if (na_ktore.size())
				for (int i = 0;i < na_ktore.size();i++) {
					outputString = std::to_wstring(na_ktore[i]) + L" ";
					OutputDebugString(outputString.c_str());
				}
			
			outputString = L"Dlugosc " + std::to_wstring(na_ktore.size()) + L" ";
			OutputDebugString(outputString.c_str());
			outputString = L"\n";
			OutputDebugString(outputString.c_str());
			break;
		*/
		case ID_BUTTON4:
			w_gore = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea1, w_windzie);
			break;
		case ID_BUTTON5:
			stop = 1;
			break;
		case ID_BUTTON7:
			na_ktore.push_back(4); //dodajê 4 do wektora na_które, ¿eby winda pojecha³a na 4 piêtro
			pas.setWsp(110 - pietro4 * 30, 55); //tworzê pasa¿era, o wspó³rzêdnych na pierwszym miejscu na 4 piêtrze
			pas.setSkad(4);  //ustawiam z jakiego pietra jedzie - chyba nie bêdzie potrzebne
			pas.setDokad(0); //na jakie
			pasazer.push_back(pas); //dodajê utworzonego pasa¿era do wektora pasazer
			pasazer[pasazer.size() - 1].Spawn(hWnd, hdc, ps, &drawArea2, pietro4); //tutaj spawnuje tego pasa¿era na piêtrze 4 (drawArea2)
			pietro4++;//dodajê ile osób czeka na 4 pietrze
			na_ktore.push_back(pasazer[pasazer.size()-1].getDokad()); //dodajê 0 do wektora na_które, ¿eby winda pojecha³a na 0 piêtro, czyli te
			//które siê ustawi³o w pas.setDokad
			break;
		//na piêtro 1
		case ID_BUTTON8:
			na_ktore.push_back(4);
			pas.setWsp(110 - pietro4 * 30, 55);
			pas.setSkad(4);
			pas.setDokad(1);
			pasazer.push_back(pas);
			pasazer[pasazer.size() - 1].Spawn(hWnd, hdc, ps, &drawArea2, pietro4);
			pietro4++;
			na_ktore.push_back(pasazer[pasazer.size()-1].getDokad());
			break;
		//na piêtro 2
		case ID_BUTTON9:
			na_ktore.push_back(4);
			pas.setWsp(110 - pietro4 * 30, 55);
			pas.setSkad(4);  
			pas.setDokad(2); 
			pasazer.push_back(pas);
			pasazer[pasazer.size() - 1].Spawn(hWnd, hdc, ps, &drawArea2, pietro4); 
			pietro4++;
			na_ktore.push_back(pasazer[pasazer.size()-1].getDokad());
			break;
		//na piêtro 3
		case ID_BUTTON10:
			na_ktore.push_back(4);
			pas.setWsp(110 - pietro4*30, 55);
			pas.setSkad(4);
			pas.setDokad(3);
			pasazer.push_back(pas);
			pasazer[pasazer.size() - 1].Spawn(hWnd, hdc, ps, &drawArea2, pietro4);
			pietro4++;
			na_ktore.push_back(pasazer[pasazer.size()-1].getDokad());
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		RamyWindy(hdc);
		TextOut(hdc, 100, 10, L"Pietro", 6);
		EndPaint(hWnd, &ps);
		stop = 1;
		//przy uruchomieniu programu od razu odpala siê timer
		SetTimer(hWnd, TMR_1, 10, 0);
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			
			if (na_ktore.size()){
				//jesli winda dojedzie na docelowe miejsce, zatrzymuje sie
				if (400 - value == 100 * na_ktore[0] && w_ruchu) {
					stop = 1;
					w_ruchu = 0;
					ktory_tick = GetTickCount64();
					na_ktore.erase(na_ktore.begin());
				}
				//kiedy winda dojedzie na miejsce i pasa¿er z indeksem 0 dojdzie do x = 400 (jeœli wychodzi w prawo) lub 140 (wychodzi w lewo)
				//to usuwaj¹ siê wszyscy pasa¿erowie (trzeba zmieniæ) i liczba osób w windzie=0
				if (w_windzie && (pasazer[0].getWspx() >= 400 || pasazer[0].getWspx() <= 140)) {
					for (int i = 0; i < w_windzie; i++)
						pasazer.erase(pasazer.begin());
					w_windzie = 0;
				}
			}
			
			/*for (int i = 0;i < pasazer.size();i++) {
				outputString = std::to_wstring(pasazer[i].getWspx()) + L"\n";
				OutputDebugString(outputString.c_str());
			}*/

			outputString = std::to_wstring(w_windzie) + L"\n"; //to jest do wyœwietlania przy debugowaniu - klikasz lokalny debuger 
			OutputDebugString(outputString.c_str());           //i to wyœwietla siê z prawej na dole

			/*if (na_ktore.size()) {
				for (int i = 0;i < na_ktore.size();i++) {
					outputString = std::to_wstring(na_ktore[i]) + L" ";
					OutputDebugString(outputString.c_str());
				}
				outputString = L"\n";
				OutputDebugString(outputString.c_str());
			}*/

			/*outputString = L"Dlugosc " + std::to_wstring(na_ktore.size()) + L" ";
			OutputDebugString(outputString.c_str());*/
			
			


			//wsiadanie do windy (na 4 piêtrze)
			if (pasazer.size())
				//dopóki pasa¿er, który wchodzi pierwszy nie osi¹gnie x = 265 to jego x zwiêszka siê o 1 (setWsp) i wyœwietla siê zwiêkszony o 1 (Idz)
				if (value == 0 && stop == 1 && pasazer[0].getWspx() <= 265  && pietro4) {
					for (int i = 0; i < pietro4; i++) {
						pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
						pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro4, pietro4);
					}							
				}

			int static buf = 0; //do jednorazowego zaktualizowania pozycji
			
			//wysiadanie
			if (pasazer.size()) {
				//winda zatrzyma siê na piêtrze 3 jeœli pasa¿er z indeksem 0 chcia³ na to piêtro
				if (value == 100 && stop == 1 && pasazer[0].getDokad() == 3) { //pietro3
					//tutaj ta aktualizacja pozycji - musi byæ, bo funkcja wyœwietlaj¹ca windê z pasa¿erami nie aktualizuje pozycji pasa¿erów
					if (!buf) { 
						for (int i = 0; i < w_windzie; i++)
							pasazer[i].setWsp(265 - i * 30, value + 55);
						buf = 1;
					}
					//tutaj w³aœciwie zaczyna siê wysiadanie - wysiadaj¹ zawsze wszyscy (trzeba zmieniæ)
					//zmieniam pozycjê x o 1 fukncj¹ setWsp, a póŸniej wyœwietlam funkcj¹ Idz
					//robiê to dla ka¿dego pasa¿era w windzie
					for (int i = 0; i < w_windzie; i++) {
						pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
						pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro3, w_windzie);
					}
					//break;
				}
				//to samo tylko dla kolejnych piêter
				if (value == 300 && stop == 1 && pasazer[0].getDokad() == 1) { //pietro1
					if (!buf) {
						for (int i = 0; i < w_windzie; i++)
							pasazer[i].setWsp(265 - i*30, value + 55);
						buf = 1;
					}
					for (int i = 0; i < w_windzie; i++)
						pasazer[i].setWsp(pasazer[i].getWspx() + 1, pasazer[i].getWspy());
					for (int i = 0; i < w_windzie; i++)
						pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro1, w_windzie);
					//break;
				}
				if (value == 200 && stop == 1 && pasazer[0].getDokad() == 2) { //pietro2
					if (!buf) { 
						for (int i = 0; i < w_windzie; i++)
							pasazer[i].setWsp(265 - i*30, value + 55);
						buf = 1;
					}
					for (int i = 0; i < w_windzie; i++) {
						pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
						pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro2, w_windzie);
					}
					//break;
				}
				if (value == 400 && stop == 1 && pasazer[0].getDokad() == 0) { //pietro0
					if (!buf) { 
						for (int i = 0; i < w_windzie; i++)
							pasazer[i].setWsp(265 - i * 30, value + 55);
						buf = 1;
					}
					for (int i = 0; i < w_windzie; i++) {
						pasazer[i].setWsp(pasazer[i].getWspx() - 1, pasazer[i].getWspy());
						pasazer[i].Idz(hWnd, hdc, ps, &drawArea_pietro0, w_windzie);
					}
				}
			}

			//jeœli winda stoi, to ruszy po 3 sekundach jeœli bêdzie mia³a kolejne piêtro na które ma jechaæ
			if (GetTickCount64() - ktory_tick >= 3000 && !w_ruchu && na_ktore.size()) {
				if (na_ktore[0] * 100 > 400 - value)
					w_gore = 1;
				if (na_ktore[0] * 100 < 400 - value)
					w_gore = 0;
				stop = 0;
				w_ruchu = 1;
			}

			//jesli pasazer z 4 pietra dojdzie do wspolrzednej x = 265 i jest z tego pietra to zwieksza sie liczba osob w windzie
			//to nadal chyba nie dzia³a poprawnie, wiêc dajê tylko, ¿e liczba pasa¿erów ogólnie na mapie to liczba osób w windzie
			if(pasazer.size())
				if (pasazer[0].getWspx() == 265 && pasazer[0].getSkad() == 4 - value / 100 && value == 0) {
					w_windzie++;		//powinno dzialac, ale za duzo dodaje (bo za duzo sie odpala), trzeba dodac jakies warunki
					w_windzie = pasazer.size(); //dziala tylko jak wsiada wszyscy z 4 pietra
				}

			//jesli winda ma value = 0 (0 pikseli w dol), czyli jest na 4 pietrze i pasazerowie wejd¹ do windy to liczba osób na pietrze 4 = 0
			if (value == 0 && w_windzie) {
				pietro4 = 0;
				if (na_ktore.size())
					if(na_ktore[0] == 4) //tutaj usuwam piêtro 4 z kolejki, na które ma jechaæ winda, bo ju¿ tutaj jest
						na_ktore.erase(na_ktore.begin());

				//usuwam powtarzaj¹ce siê elementy w kolejce piêter na które ma jechaæ winda (bo jak kilka osób chce jechaæ na te same to
				//ka¿de klikniêcie dodaje te piêtro do kolejki, a wystraczy, ¿eby by³o tylko raz
				auto uniqueEnd = std::unique(na_ktore.begin(), na_ktore.end()); 
				na_ktore.erase(uniqueEnd, na_ktore.end());

				//sortujê malej¹ca, bo winda jedzie w dó³ (bêdzie trzeba to uzale¿niæ od tego czy jedzie w górê, wtedy trzeba inaczej sortowaæ)
				//a sortujê po to, ¿eby zje¿d¿aj¹c w dó³ odwiedzaæ piêtra po kolei, np. 3,1,0, zamiast 1,0,3, bo przed sortowaniem
				//kolejnoœæ jest ustalona przez klikniêcia przycisku, na które chce jechaæ
				std::sort(na_ktore.begin(), na_ktore.end(), [](int a, int b) {
					return a > b;
					});
			}


			//to jest zeby jak winda stoi, to nie aktualizowac okienka, zeby nie migalo
			if (stop)
				break;

			//tutaj to zmieniam, bo po tym jak winda ruszy, czyli stop = 0 (nie bedzie tego break wyzej)
			buf = 0;


			if (!w_gore)
				value++;
			else
				value--;
			
				
			repaintWindow(hWnd, hdc, ps, &drawArea1, w_windzie);
			
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	//return 0;
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
