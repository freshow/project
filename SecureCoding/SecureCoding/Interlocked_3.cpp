#include "MultiThread.h"

static volatile long g_nLoginCount;
static const int THREAD_NUM = 100;// 50

static unsigned int __stdcall threadFunc(PVOID pParam)
{
    Sleep(100);
    InterlockedIncrement(&g_nLoginCount);
    Sleep(50);
    return 0;
}

void interlocked_main(void)
{
    FUNC_CTRL(__FUNCTION__, true);

    int num = 20;
    while (num--)
    {
        //g_nLoginCount = 0;
        HANDLE handles[THREAD_NUM];
        for (int i = 0; i < THREAD_NUM; i++)
        {
            handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadFunc, NULL, 0, NULL);
        }
        WaitForMultipleObjects(THREAD_NUM, handles, TRUE, INFINITE);
        printf("there are %d users and result %d\n", THREAD_NUM, g_nLoginCount);
    }

    return;
}