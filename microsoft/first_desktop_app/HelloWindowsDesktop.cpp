#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>

// Global Variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

// Stored instance handle for use in Win32 API calls
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void PrintLastError(const char* context) {
	DWORD errorCode = GetLastError();
	char buffer[256];

	// Format the message
	sprintf_s(buffer, "%s failed with error code %lu\n", context, errorCode);

	// Convert buffer (char*) to wide string (wchar_t*)
	wchar_t wbuffer[256];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wbuffer, 256, buffer, _TRUNCATE);

	// Send to debugger output
	OutputDebugStringW(wbuffer);
}


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow) {

	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	//wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex)) {
		// Format the message
		PrintLastError("RegisterClassEx failed");
		
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return 1;
	}

	// Store instance handle in our global variable
	hInst = hInstance;

	HWND hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,			// An optional extended window style.
		szWindowClass,					// The name of the window application
		szTitle,						// The text that appears in the title bar
		WS_OVERLAPPEDWINDOW,			// The type of window to create
		CW_USEDEFAULT, CW_USEDEFAULT,	// Initial position (x, y)
		500, 100,						// Initial size (width, length)
		NULL,							// The parent of this window
		NULL,							// The menu for this window - none
		hInstance,						// The instance handle, first parameter of WinMain
		NULL							// Additional application data - none
	);
	if (!hwnd) {
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Main message loop:
	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

// Processes messages for the main window.	
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop!");

	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		// Here your application is laid out.
		// For this introduction, we just print out "Hello, Windows desktop!"	
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));
		// End application-specific layout section.
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
