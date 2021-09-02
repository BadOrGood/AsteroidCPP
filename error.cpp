
VOID ErrorProcedure(
  IN HRESULT hResult
  )
{
  WCHAR szText[32];
  
  _snwprintf_s(szText, ARRAYSIZE(szText), L"Error occured 0x%08lX", hResult);

  MessageBoxW(NULL, szText, L"Asteroids", MB_OK | MB_ICONERROR);
  ExitProcess(hResult);
}
