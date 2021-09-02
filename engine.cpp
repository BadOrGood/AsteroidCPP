#include "engine.h"

namespace Engine {
  ID2D1Factory          * Direct2dFactory;
  ID2D1HwndRenderTarget * RenderTarget;
  ID2D1SolidColorBrush  * WhiteBrush;

  HRESULT Init(
    IN HWND hWnd
    )
  {
    HRESULT     hResult;
    RECT        WindowRect;
    D2D1_SIZE_U WindowSize;

    GetClientRect(hWnd, &WindowRect);

    WindowSize = D2D1::SizeU(WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top);

    if (FAILED(hResult = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Engine::Direct2dFactory))) {
      return hResult;
    }

    if (FAILED(hResult = Engine::Direct2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, WindowSize), &Engine::RenderTarget))) {
      return hResult;
    }

    if (FAILED(hResult = Engine::RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &Engine::WhiteBrush))) {
      return hResult;
    }

    return ERROR_SUCCESS;
  }
}
