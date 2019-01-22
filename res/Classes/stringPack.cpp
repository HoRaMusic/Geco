#include "stringPack.h"

void stringPack::setstr1(char *_input)
{
    strcpy(input1, _input);
}
void stringPack::setstr2(char *_input)
{
    strcpy(input2, _input);
}
void stringPack::setstr3(char *_input)
{
    strcpy(input3, _input);
}
void stringPack::setstr4(char *_input)
{
    strcpy(input4, _input);
}
void stringPack::setstr5(char *_input)
{
    strcpy(input5, _input);
}

char * stringPack::getoutput()
{
    //char* output = (char*)malloc(100);
    sprintf(output, "%s %s %s %s %s", input1, input2, input3, input4, input5);
    return output;
}
