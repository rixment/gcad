#include <windows.h>

BOOL 
APIENTRY 
DllMain( HINSTANCE  hInst,
         DWORD      reason,
         LPVOID )
{
    switch(reason)
    {
      case DLL_PROCESS_ATTACH:
        break;
      case DLL_PROCESS_DETACH:
        break;
      case DLL_THREAD_ATTACH:
        break;
      case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}
