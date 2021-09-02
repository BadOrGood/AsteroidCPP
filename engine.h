#ifndef __ENGINE_H
#define __ENGINE_H

namespace Engine {
  extern ID2D1Factory          * Direct2dFactory;
  extern ID2D1HwndRenderTarget * RenderTarget;
  extern ID2D1SolidColorBrush  * WhiteBrush;

  HRESULT Init(
    IN HWND hWnd
    );
}

#endif
