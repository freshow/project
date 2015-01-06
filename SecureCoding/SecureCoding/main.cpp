
#include "SecureCoding.h"
#include "MultiThread.h"

int main(int argc, char* argv[])
{
    chapter2_2_String_Error_Operation(argc, argv);
    chapter2_3_String_Vulnerability(argc, argv);

    consumer_main();
    multiThread_2_main();
    interlocked_main();

    system("pause");
}