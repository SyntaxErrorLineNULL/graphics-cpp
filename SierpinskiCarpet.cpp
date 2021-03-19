/**
 * Author: SyntaxErrorLineNULL
 */

#include <windows.h>
#include <cmath>

HWND _hwnd;
HDC _wdc;
int _ord;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawSierpinskiCarpet(int posX, int posY, int width, int height);

void Draw(HDC hdc, int Width, int Height, int ord){
    _wdc = hdc;
    _ord = Width / static_cast<int>(pow(3.0, ord) - 1);
    DrawSierpinskiCarpet(0, 0, Width, Height);
}

void DrawSierpinskiCarpet(int posX, int posY, int width, int height) {

    if(width < _ord || height < _ord) return;
    int Width = width / 3, Height = height / 3;
    RECT rc;
    SetRect(&rc, posX + Width, posY + Height, posX + Width + Width, posY + Height + Height);
    FillRect(_wdc, &rc, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(i == 1 && j == 1) continue;
            DrawSierpinskiCarpet(posX + j * Width, posY + i * Height, Width, Height);
        }
    }

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
    WNDCLASS wndclass;
    MSG msg;
    HWND hwnd;

    wndclass.lpszClassName = TEXT("Win32 Sierpinski carpet drawing");
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszMenuName = nullptr;
    wndclass.hInstance = hInstance;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wndclass)){
        MessageBox(nullptr, TEXT("Window class is not registered"), TEXT("Error"), MB_OK);
        return 0;
    }


    hwnd = CreateWindow(TEXT("Win32 Sierpinski carpet drawing"),
                        TEXT("Win32 Sierpinski carpet drawing window name"),
                        WS_OVERLAPPEDWINDOW,
                        200, 200, 700, 700, nullptr, nullptr, hInstance, nullptr
    );

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, hwnd, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

    HDC hdc;
    PAINTSTRUCT ps;
    int width, height;

    switch (message){
        case WM_CREATE:
            return 0;
        case WM_SIZE:
            width = LOWORD(lParam);
            height = HIWORD(lParam);

        case WM_PAINT :
            hdc = BeginPaint(hwnd, &ps);
            Draw(hdc, 400, 400, 5);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            if (MessageBox(hwnd, reinterpret_cast<LPCSTR>((LPCWSTR) "Really quit?"),
                           reinterpret_cast<LPCSTR>((LPCWSTR) "My application"), MB_OKCANCEL) == IDOK) {
                DestroyWindow(hwnd);
            }
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}