#include "stringLeft.h"

void stringLeft::setstring(char *_input)
{
    strcpy(input, _input);
}

void stringLeft::setlength(int _numberOfCharacters)
{
    numberOfCharacters = _numberOfCharacters;
}

char * stringLeft::getoutput()
{
    //char* output = (char*)malloc(100);
    for (int i = 0; i < numberOfCharacters; i++)
    {
        output[i] = input[i];
    }
    output[numberOfCharacters + 1] = '\0';
    return output;
}
