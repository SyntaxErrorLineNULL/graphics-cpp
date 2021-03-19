#include<Windows.h>
#include <cmath>

constexpr double Radian = M_PI / 180.0F;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawLine(HDC hdc, int posX, int posY, int tgtX, int tgtY){
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
    MoveToEx(hdc, posX, posY, nullptr);
    SelectObject(hdc, hPen);
    LineTo(hdc, tgtX, tgtY);
}

void DrawTree(HDC hdc, int posX, int posY, int Angle, int Depth){

    if(Depth > 0){
        auto X = posX + (cos(Angle * Radian)) * Depth * 10.0;
        auto Y = posY + (sin(Angle * Radian)) * Depth * 10.0;

        DrawLine(hdc, posX, posY, X, Y);

        DrawTree(hdc, X, Y, Angle - 20, Depth - 1);
        DrawTree(hdc, X, Y, Angle + 20, Depth - 1);

    }

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
    WNDCLASS wndclass;
    MSG msg;
    HWND hwnd;

    wndclass.lpszClassName = TEXT("Win32 tree drawing");
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


    hwnd = CreateWindow(TEXT("Win32 tree drawing"),
                        TEXT("Win32 tree drawing basic pointer"),
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
        case WM_CREATE :
            return 0;
        case WM_SIZE :
            width = LOWORD(lParam);
            height = HIWORD(lParam);

        case WM_PAINT :
            hdc = BeginPaint(hwnd, &ps);
            DrawTree(hdc, 350, 500, -90, 9);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE :
            if (MessageBox(hwnd, reinterpret_cast<LPCSTR>((LPCWSTR) "Really quit?"),
                           reinterpret_cast<LPCSTR>((LPCWSTR) "My application"), MB_OKCANCEL) == IDOK) {
                DestroyWindow(hwnd);
            }
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}