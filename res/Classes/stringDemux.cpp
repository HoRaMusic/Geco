#include "stringDemux.h"

void stringDemux::setinput(char * _input)
{
    strcpy(input, _input);
}
void stringDemux::setindex(int _index)
{
     index = _index;
}
char * stringDemux::getoutput1()
{
    //char* output = (char*)malloc(100);
    if (index == 0)
    {
        strcpy(output1, input);
    }
    else
    {
        strcpy(output1, "");
    }
    return output1;
}
char * stringDemux::getoutput2()
{
    //char* output = (char*)malloc(100);
    if (index == 1)
    {
        strcpy(output2, input);
    }
    else
    {
        strcpy(output2, "");
    }
    return output2;
}
char * stringDemux::getoutput3()
{
    //char* output = (char*)malloc(100);
    if (index == 2)
    {
        strcpy(output3, input);
    }
    else
    {
        strcpy(output3, "");
    }
    return output3;
}
char * stringDemux::getoutput4()
{
    //char* output = (char*)malloc(100);
    if (index == 3)
    {
        strcpy(output4, input);
    }
    else
    {
        strcpy(output4, "");
    }
    return output4;
}
char * stringDemux::getoutput5()
{
    //char* output = (char*)malloc(100);
    if (index == 4)
    {
        strcpy(output5, input);
    }
    else
    {
        strcpy(output5, "");
    }
    return output5;
}
char * stringDemux::getoutput6()
{
    //char* output = (char*)malloc(100);
    if (index == 5)
    {
        strcpy(output6, input);
    }
    else
    {
        strcpy(output6, "");
    }
    return output6;
}
char * stringDemux::getoutput7()
{
    //char* output = (char*)malloc(100);
    if (index == 6)
    {
        strcpy(output7, input);
    }
    else
    {
        strcpy(output7, "");
    }
    return output7;
}
char * stringDemux::getoutput8()
{
    //char* output = (char*)malloc(100);
    if (index == 7)
    {
        strcpy(output8, input);
    }
    else
    {
        strcpy(output8, "");
    }
    return output8;
}
