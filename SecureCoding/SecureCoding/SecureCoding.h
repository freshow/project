#ifndef SECURE_CODING_H_
#define SECURE_CODING_H_

#include <iostream>
using namespace std;

#define BUF_SIZE    (5)

class funcCtrl
{
public:
    static void funcNameGet(char* funcNamePtr)
    {
        printf("%s\n", funcNamePtr);
    }
};

#define IS_RUN(isRun)   if (!(isRun)){return;}
#define FUNC_CTRL(funcName, isRun)   {IS_RUN(isRun); funcCtrl::funcNameGet(funcName);}

// chapter 2
void chapter2_2_String_Error_Operation(int argc, char* argv[]);
void chapter2_3_String_Vulnerability(int argc, char* argv[]);

#endif  // SECURE_CODING_H_
