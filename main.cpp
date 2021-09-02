#include "engine.h"
#include "player.h"
#include "asteroid.h"

#define SZ_CLASS L"{231501D7-B2AD-4E6A-A14F-36F46220D729}"
#define WND_WIDTH  600
#define WND_HEIGHT 460

static LRESULT WINAPI WindowProcedure(
  IN HWND   hWnd,
  IN UINT   nMessage,
  IN WPARAM wParam,
  IN LPARAM lParam
  )
{
  if (nMessage == WM_GETMINMAXINFO) {
    LPMINMAXINFO MinMaxInfo = (LPMINMAXINFO)lParam;

    MinMaxInfo->ptMaxSize.x = WND_WIDTH;
    MinMaxInfo->ptMaxSize.y = WND_HEIGHT;
    MinMaxInfo->ptMinTrackSize.x = WND_WIDTH;
    MinMaxInfo->ptMinTrackSize.y = WND_HEIGHT;
    MinMaxInfo->ptMaxTrackSize.x = WND_WIDTH;
    MinMaxInfo->ptMaxTrackSize.y = WND_HEIGHT;

    return ERROR_SUCCESS;
  } else if (nMessage == WM_CLOSE) {
    DestroyWindow(hWnd);

    return ERROR_SUCCESS;
  } else if (nMessage == WM_DESTROY) {
    PostQuitMessage(ERROR_SUCCESS);

    return ERROR_SUCCESS;
  }

  return DefWindowProcW(hWnd, nMessage, wParam, lParam);
}

#ifdef _DEBUG
INT wmain(VOID) {
  HINSTANCE hInstance = GetModuleHandleW(NULL);
#else
INT WINAPI wWinMain(
  IN     HISTANCE hInstance,
  IN OPT HISTANCE hPrevInstance,
  IN     PWSTR    szCommandLine,
  IN     UINT     nCommandShow
  )

  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(szCommandLine);
  UNREFERENCED_PARAMETER(nCommandShow);
#endif
  HWND          hWindow;
  HRESULT       hResult;
  WNDCLASSW     WndClassW;
  MSG           Message = { 0 };
  LARGE_INTEGER PerformanceFrequency;
  LARGE_INTEGER BeginTime = { 0 };
  LARGE_INTEGER EndTime = { 0 };
  FLOAT         fDeltaTime = 1;
  RECT          WindowRect = {
    0, 0,
    WND_WIDTH,
    WND_HEIGHT
  };

  ZeroMemory(&WndClassW, sizeof(WndClassW));

  WndClassW.lpfnWndProc = WindowProcedure;
  WndClassW.hInstance = hInstance;
  WndClassW.lpszClassName = SZ_CLASS;

  if (!RegisterClassW(&WndClassW)) {
    ErrorProcedure(GetLastError());
  }

  AdjustWindowRect(&WindowRect, WS_OVERLAPPED, FALSE);

  if (!(hWindow = CreateWindowExW(0, SZ_CLASS, L"Asteroids", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL))) {
    ErrorProcedure(GetLastError());
  }

  if (FAILED(hResult = Engine::Init(hWindow))) {
    ErrorProcedure(hResult);
  }

  UpdateWindow(hWindow);
  ShowWindow(hWindow, SW_SHOW);

  PlayerInit();
  AsteroidInit();

  QueryPerformanceFrequency(&PerformanceFrequency);

  while (Message.message != WM_QUIT) {
    if (PeekMessageW(&Message, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }

    if (Message.message == WM_KEYUP) {
      PlayerOnKeyUp(Message.wParam);
    } else if (Message.message == WM_KEYDOWN) {
      PlayerOnKeyDown(fDeltaTime, Message.wParam);
    }

    PlayerUpdate(fDeltaTime);
    AsteroidUpdate(fDeltaTime);

    QueryPerformanceCounter(&BeginTime);

    Engine::RenderTarget->BeginDraw();
    Engine::RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    Engine::RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    PlayerDraw();
    AsteroidDraw();

    Engine::RenderTarget->EndDraw();
    QueryPerformanceCounter(&EndTime);

    fDeltaTime = (FLOAT)(((long double)(EndTime.QuadPart - BeginTime.QuadPart)) / (long double)PerformanceFrequency.QuadPart);
  }
}
