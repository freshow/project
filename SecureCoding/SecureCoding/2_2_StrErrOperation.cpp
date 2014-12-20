
#include "SecureCoding.h"

#define BUF_SIZE    (5)

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void unbounded_string_copy_figure_2_1(void)
{
    FUNC_CTRL(__FUNCTION__, false);
    char password[BUF_SIZE];
    puts("enter 5 character password:");
    gets(password);
    cout << "password: " << password << endl;
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void unbounded_string_copy_figure_2_2(int argc, char* argv[])
{
    FUNC_CTRL(__FUNCTION__, false);
    char name[BUF_SIZE];
    strcpy(name, argv[0]);
    strcat(name, " = ");
    cout << "name: " << name << endl;
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void unbounded_string_copy_figure_2_3_fix_for_figure_2_2(int argc, char* argv[])
{
    FUNC_CTRL(__FUNCTION__, false);

    int buffLen = strlen(argv[0]);
    char* buff = (char*)malloc(buffLen + 1);
    if (NULL != buff)
    {
        // strcpy will add "\0" auto in the end of the string
        strcpy(buff, argv[0]);
        cout << "buff len: " << strlen(buff) << endl;
        cout << "buff: " << buff << endl;
        strcpy(buff, "1");
        cout << "buff len: " << strlen(buff) << endl;
        cout << "buff: " << buff << endl;
    }
    else
    {
        cout << "malloc buff failed." << endl;
    }
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void unbounded_string_copy_strdup(void)
{
    FUNC_CTRL(__FUNCTION__, false);
    char* myString = "12345";
    //int buffLen = strlen(myString);
    //char* buffer = (char*)malloc(buffLen + 1);

    char* buffer = strdup(myString);
    cout << "buff len: " << strlen(buffer) << endl;
    cout << "buff: " << buffer << endl;
    free(buffer);
    buffer = NULL;
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void unbounded_string_copy_figure_2_4(void)
{
    FUNC_CTRL(__FUNCTION__, false);
    char buf[BUF_SIZE];
    cin >> buf;
    cout << "echo: " << buf << endl;
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void unbounded_string_copy_figure_2_5_fix_for_figure_2_4(void)
{
    FUNC_CTRL(__FUNCTION__, false);
    char buf[BUF_SIZE];
    cin.width(BUF_SIZE);
    cin >> buf;
    cout << "echo: " << buf << endl;
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void off_by_one_err_figure_2_6(void)
{
    FUNC_CTRL(__FUNCTION__, false);
    char source[BUF_SIZE];
    strcpy(source, "01234");   // 1
    char* dest = (char*)malloc(strlen(source));// 2

    int i = 0;
    for (i = 1; i <= BUF_SIZE; i++)// 3, 4
    {
        dest[i] = source[i];
    }

    dest[i] = '\0';// 5
    printf("dest = %s", dest);
}

/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
char a[16];
char b[16];
char c[32];
void null_termination_error_figure_2_7(void)
{
    FUNC_CTRL(__FUNCTION__, false);

    strcpy(a, "0123456789abcdef");
    strcpy(b, "0123456789abcdef");
    strcpy(c, a);
    // after this step the memory allocation info like below:
    /*
    a[16]:0x00419504
    b[16]:0x004194D4
    c[32]:0x004194E4

    0x004194D4  30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66  0123456789abcdef
    0x004194E4  30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66  0123456789abcdef
    0x004194F4  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
    0x00419504  30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66  0123456789abcdef
    0x00419514  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
    */

    strcat(c, b);
    cout << "a = " << a << endl;
}
/************************************************************************/
/*                                                                      */
/*                                                                      */
/************************************************************************/
void chapter2_figure_2_8(int argc, char* argv[])
{
    FUNC_CTRL(__FUNCTION__, false);
    /*
    &i:     0x0018FD6C
    buff:   0x0018FD5C
    0x0018FD6C - 0x0018FD5C = 16(dec)
    */
    int i = 0;
    char buff[BUF_SIZE];

    // "d:\Code\z_VS_Project\SecureCoding\debug\SecureCoding.exe"
    // the 17th character is 'j' and the 17th byte offset of buff is var i
    char* argv0 = argv[0];

    while ('\0' != argv0[i])
    {
        /*
        i will be over writed by character 'j' and i++ change to 'k'
        then i = 0x6b. it means buff[0x6b] = argv0[0x6b];
        */
        buff[i] = argv0[i];
        i++;
    }

    buff[i] = '\0';
    cout << "buff = " << buff << endl;
}

/**************************************************************************
*
*     
*
**************************************************************************/
void chapter2_2_String_Error_Operation(int argc, char* argv[])
{
    FUNC_CTRL(__FUNCTION__, false);
    unbounded_string_copy_figure_2_1();
    unbounded_string_copy_figure_2_2(argc, argv);
    unbounded_string_copy_figure_2_3_fix_for_figure_2_2(argc, argv);
    unbounded_string_copy_strdup();
    unbounded_string_copy_figure_2_4();
    unbounded_string_copy_figure_2_5_fix_for_figure_2_4();
    off_by_one_err_figure_2_6();
    null_termination_error_figure_2_7();
    chapter2_figure_2_8(argc, argv);

    return;
}