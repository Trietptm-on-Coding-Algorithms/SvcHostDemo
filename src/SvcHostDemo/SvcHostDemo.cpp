#include <windows.h>

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    return true;
}

SERVICE_STATUS_HANDLE g_serviceStatusHandle = nullptr;

SERVICE_STATUS g_serviceStatus = 
{
    SERVICE_WIN32_SHARE_PROCESS,
    SERVICE_START_PENDING,
    SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE
};

DWORD WINAPI HandlerEx(
    DWORD dwControl,
    DWORD dwEventType,
    LPVOID lpEventData,
    LPVOID lpContext
)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        g_serviceStatus.dwCurrentState = SERVICE_STOPPED;
        break;
    case SERVICE_CONTROL_PAUSE:
        g_serviceStatus.dwCurrentState = SERVICE_PAUSED;
        break;
    case SERVICE_CONTROL_CONTINUE:
        g_serviceStatus.dwCurrentState = SERVICE_RUNNING;
        break;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    default:
        break;
    };

    SetServiceStatus(g_serviceStatusHandle, &g_serviceStatus);

    return NO_ERROR;
}

extern "C" __declspec(dllexport) VOID WINAPI ServiceMain(DWORD dwArgc, LPCWSTR* lpszArgv)
{
    g_serviceStatusHandle = RegisterServiceCtrlHandlerExW(L"SvcHostDemo", HandlerEx, nullptr);
    if (!g_serviceStatusHandle)
    {
        return;
    }

    g_serviceStatus.dwCurrentState = SERVICE_RUNNING;

    SetServiceStatus(g_serviceStatusHandle, &g_serviceStatus);
}