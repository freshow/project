#include "MultiThread.h"

const int THREAD_NUM = 2;
HANDLE g_handles[THREAD_NUM];
unsigned g_pids[THREAD_NUM];

unsigned int __stdcall threadFunc(PVOID pParam)
{
    const int* const pThreadNum = (int*)(pParam);
    printf("Thread: %d, addr: %d, PID: %d\n", (*pThreadNum) + 1, g_pids[*pThreadNum], GetCurrentThreadId());
    return 0;
}

void multiThread_2_main(void)
{
    int j = 1;
    g_handles[j] = (HANDLE)_beginthreadex(NULL, 0, threadFunc, &j, 0, &g_pids[j]);

    int i = 0;
    g_handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadFunc, &i, 0, &g_pids[i]);

    DWORD dwEvent = WaitForMultipleObjects(THREAD_NUM, g_handles, TRUE, INFINITE);
    switch (dwEvent)
    {
    case WAIT_OBJECT_0 + 0:
        printf("First event was signaled.\n");
    	break;
    case WAIT_OBJECT_0 + 1:
        printf("Second event was signaled.\n");
        break;
    case WAIT_TIMEOUT:
        printf("Wait timed out.\n");
        break;
    default: 
        printf("Wait error: %d\n", GetLastError()); 
        ExitProcess(0);
    }

    return;
}