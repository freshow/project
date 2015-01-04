
#include "MultiThread.h"

BOOL set_console_color(WORD wAttributes)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (INVALID_HANDLE_VALUE == hConsole)
    {
        return FALSE;
    }

    return SetConsoleTextAttribute(hConsole, wAttributes);
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
static const int PRODUCE_NUMBER = 10;
static CRITICAL_SECTION g_cs;
static HANDLE g_hEventBufferEmpty;
static HANDLE g_hEventBufferFull;
static int g_Buffer;

unsigned int __stdcall producerThreadFunc(PVOID pParam)
{
    for (int i = 1; i <= PRODUCE_NUMBER; i++)
    {
        // waiting for buffer empty
        WaitForSingleObject(g_hEventBufferEmpty, INFINITE);

        EnterCriticalSection(&g_cs);
        g_Buffer = i;
        printf("\nput product %d into buffer", g_Buffer);
        LeaveCriticalSection(&g_cs);

        SetEvent(g_hEventBufferFull);
    }

    return 0;
}

unsigned int __stdcall consumerThreadFunc(PVOID pParam)
{
    volatile bool flag = true;
    
    while (flag)
    {
        // waiting for buffer full
        WaitForSingleObject(g_hEventBufferFull, INFINITE);

        EnterCriticalSection(&g_cs);
        set_console_color(FOREGROUND_GREEN); 
        printf("\nget product %d from buffer\n", g_Buffer);
        set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        if (PRODUCE_NUMBER == g_Buffer)
        {
            flag = false;
        }
        LeaveCriticalSection(&g_cs);

        SetEvent(g_hEventBufferEmpty);

        Sleep(10);
    }

    return 0;
}

void consumer_1_Producer_1_buffer_1_main(void)
{
    FUNC_CTRL(__FUNCTION__, false);
    InitializeCriticalSection(&g_cs);
    g_hEventBufferFull  = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_hEventBufferEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);
    
    const int THREADNUM = 2;
    HANDLE hThread[THREADNUM];
    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, producerThreadFunc, NULL, 0, NULL);
    hThread[1] = (HANDLE)_beginthreadex(NULL, 0, consumerThreadFunc, NULL, 0, NULL);
    WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);
    
    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    CloseHandle(g_hEventBufferEmpty);
    CloseHandle(g_hEventBufferFull);
    DeleteCriticalSection(&g_cs);
    return;
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
static const int END_PRODUCE_NUMBER = 8;
static const int BUFFER_SIZE = 4;
static int g_Buffer2[BUFFER_SIZE];
static int g_i = 0;
static int g_j = 0;
static HANDLE g_hSemaphoreBufferEmpty;
static HANDLE g_hSemaphoreBufferFull;
static bool g_IsProductOver = false;

unsigned int __stdcall producerThreadFunc2(PVOID pParam)
{
    for (int i = 1; i <= END_PRODUCE_NUMBER; i++)
    {
        // waiting for buffer empty
        WaitForSingleObject(g_hSemaphoreBufferEmpty, INFINITE);

        EnterCriticalSection(&g_cs);
        g_Buffer2[g_i] = i;
        printf("\nput product %d into buffer %d", g_Buffer2[g_i], g_i + 1);
        g_i = (g_i + 1) % BUFFER_SIZE;
        LeaveCriticalSection(&g_cs);

        ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);
    }
    printf("\nproducer Thread Func2 finished\n");
    return 0;
}

unsigned int __stdcall consumerThreadFunc2(PVOID pParam)
{
    while (true)
    {
        // waiting for buffer full
        WaitForSingleObject(g_hSemaphoreBufferFull, INFINITE);

        EnterCriticalSection(&g_cs);

        if (g_IsProductOver)
        {
            set_console_color(FOREGROUND_GREEN); 
            printf("\nPID: %d no more product", GetCurrentThreadId());
            set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        }

        set_console_color(FOREGROUND_GREEN); 
        printf("\nPID: %d get product %d from buffer %d", GetCurrentThreadId(), g_Buffer2[g_j], g_j + 1);
        set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        if (END_PRODUCE_NUMBER == g_Buffer2[g_j])
        {
            g_IsProductOver = true;
            LeaveCriticalSection(&g_cs);
            ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);
            break;
        }
        g_j = (g_j + 1) % BUFFER_SIZE;
        LeaveCriticalSection(&g_cs);
        /*Sleep(500);*/
        ReleaseSemaphore(g_hSemaphoreBufferEmpty, 1, NULL);
    }

    set_console_color(FOREGROUND_GREEN);
    printf("\nPID: %d consumer Thread Func2 finished\n", GetCurrentThreadId()); 
    set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return 0;
}

void consumer_2_Producer_1_buffer_4_main(void)
{
    FUNC_CTRL(__FUNCTION__, true);
    InitializeCriticalSection(&g_cs);
    g_hSemaphoreBufferEmpty = CreateSemaphore(NULL, 4, 4, NULL);
    g_hSemaphoreBufferFull  = CreateSemaphore(NULL, 0, 4, NULL);
    g_i = 0;
    g_j = 0;
    memset(g_Buffer2, 0, sizeof(g_Buffer2));

    const int THREADNUM = 3;
    HANDLE hThread[THREADNUM];
    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, producerThreadFunc2, NULL, 0, NULL);
    hThread[1] = (HANDLE)_beginthreadex(NULL, 0, consumerThreadFunc2, NULL, 0, NULL);
    hThread[2] = (HANDLE)_beginthreadex(NULL, 0, consumerThreadFunc2, NULL, 0, NULL);
    WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);

    for (int i = 0; i < THREADNUM; i++)
    {
        CloseHandle(hThread[i]);
    }
    
    CloseHandle(g_hSemaphoreBufferEmpty);
    CloseHandle(g_hSemaphoreBufferFull);
    DeleteCriticalSection(&g_cs);
    return;
}

void consumer_main(void)
{
    consumer_1_Producer_1_buffer_1_main();
    consumer_2_Producer_1_buffer_4_main();
}