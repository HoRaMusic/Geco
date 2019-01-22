#include "onChange.h"

void onChange::setinput(float _input)
{
    if (input != _input)
    {
        output = 5;
    }
    else
    {
        output = 0;
    }
    input = _input;
}

float onChange::getoutput()
{
    return output;
}
